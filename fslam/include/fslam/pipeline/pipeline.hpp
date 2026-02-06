#pragma once
#include "fslam/core/result.hpp"
#include "fslam/core/types.hpp"
#include "fslam/sensors/camera.hpp"

namespace fs {

class Pipeline {
public:
    class Builder;
    virtual ~Pipeline() = default;


    static Builder Create();
protected:
    Pipeline() = default;
};


class Pipeline::Builder {
public:
    Builder() = default;
    Builder& WithCamera(const ref<Camera>& camera);
    result<ref<Pipeline>> Build();

private:
    ref<Camera> mCamera;
};

} // namespace fs
