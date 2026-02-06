#include "fslam/core/types.hpp"
#include <fslam/pipeline/pipeline.hpp>

namespace fs {

Pipeline::Builder Pipeline::Create() { return Builder(); }

Pipeline::Builder& Pipeline::Builder::WithCamera(const ref<Camera>& camera) {
    mCamera = camera;
    return *this;
}

result<ref<Pipeline>> Pipeline::Builder::Build() {
    if (!mCamera) {
        return err(
            ErrorCode::CREATE_PIPELINE_FAILED, "Pipeline Camera is not set");
    }



    // TODO: create a concrete derived Pipeline implementation:
    // return ok(make_ref<MonoPipeline>(mCamera));

    return err(ErrorCode::CREATE_PIPELINE_FAILED,
        "Pipeline::Builder::Build(): no concrete Pipeline implementation wired yet");
}

} // namespace fs
