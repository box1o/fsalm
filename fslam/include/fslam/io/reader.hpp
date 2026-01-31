#pragma once
#include "fslam/core/types.hpp"
#include "fslam/sensors/camera.hpp"
#include <filesystem>

namespace fs {

struct ReaderInfo {
    std::filesystem::path path;
    ref<Camera> camera;
};

class Reader {
public:
    using FrameData = std::pair<cv::Mat, Timestamp>;
    class Iterator;

public:
    virtual ~Reader() = default;

    [[nodiscard]] virtual FrameData Next() = 0;
    [[nodiscard]] virtual bool HasNext() = 0;
    [[nodiscard]] virtual u64 Size() const = 0;

    virtual void Reset() = 0;

    [[nodiscard]] static ref<Reader> Create(const ReaderInfo& info);

    // For range-based for loop support
    Iterator begin();
    Iterator end();

protected:
    Reader() = default;
};

// NOTE: Iterator to iterate through frames
class Reader::Iterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = FrameData;
    using difference_type = std::ptrdiff_t;
    using pointer = const FrameData*;
    using reference = const FrameData&;

    Iterator() : mReader(nullptr), mDone(true) {}
    explicit Iterator(Reader* reader) : mReader(reader), mDone(false) {
        if (mReader && mReader->HasNext()) {
            mCurrent = mReader->Next();
        } else {
            mDone = true;
        }
    }

    reference operator*() const { return mCurrent; }
    pointer operator->() const { return &mCurrent; }

    Iterator& operator++() {
        if (mReader && mReader->HasNext()) {
            mCurrent = mReader->Next();
        } else {
            mDone = true;
        }
        return *this;
    }

    bool operator==(const Iterator& other) const { return mDone == other.mDone; }

    bool operator!=(const Iterator& other) const { return !(*this == other); }

private:
    Reader* mReader;
    FrameData mCurrent;
    bool mDone;
};

} // namespace fs
