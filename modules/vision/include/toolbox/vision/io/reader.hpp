#pragma once
#include <filesystem>
#include <opencv2/core/mat.hpp>

#include "toolbox/base/base.hpp"

namespace ct {

struct ReaderInfo {
    std::filesystem::path path;
};
using Timestamp = f64;

class Reader {
public:
    using FrameData = std::pair<cv::Mat, Timestamp>;
    class Iterator;

    virtual ~Reader() = default;

    [[nodiscard]] virtual result<FrameData> Next() = 0;
    [[nodiscard]] virtual bool HasNext() const = 0;
    [[nodiscard]] virtual u64 Size() const = 0;
    virtual void Reset() = 0;

    [[nodiscard]] static result<ref<Reader>> Create(const ReaderInfo& info);

    Iterator begin();
    Iterator end();

protected:
    Reader() = default;
};

// NOTE: Input iterator for range-based for loop support
class Reader::Iterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = FrameData;
    using difference_type = std::ptrdiff_t;
    using pointer = const FrameData*;
    using reference = const FrameData&;

    Iterator() : mReader(nullptr), mDone(true) {}

    explicit Iterator(Reader* reader) : mReader(reader), mDone(false) { advance(); }

    reference operator*() const { return mCurrent; }
    pointer operator->() const { return &mCurrent; }

    Iterator& operator++() {
        advance();
        return *this;
    }

    bool operator==(const Iterator& other) const { return mDone == other.mDone; }
    bool operator!=(const Iterator& other) const { return !(*this == other); }

private:
    void advance() {
        if (mReader && mReader->HasNext()) {
            auto res = mReader->Next();
            if (res.has_value()) {
                mCurrent = std::move(res.value());
            } else {
                mDone = true;
            }
        } else {
            mDone = true;
        }
    }

    Reader* mReader;
    FrameData mCurrent;
    bool mDone;
};

} // namespace ct
