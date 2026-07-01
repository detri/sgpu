#include "sgpu/compute_pipeline.hpp"

namespace sgpu
{
    compute_pipeline::compute_pipeline(SDL_GPUDevice* device, SDL_GPUComputePipeline* compute_pipeline):
        m_device(device),
        m_compute_pipeline(compute_pipeline) {
    }

    compute_pipeline::~compute_pipeline() {
        SDL_ReleaseGPUComputePipeline(m_device, m_compute_pipeline);
    }
    SDL_GPUComputePipeline* compute_pipeline::get_compute_pipeline() const noexcept {
        return m_compute_pipeline;
    }
}
