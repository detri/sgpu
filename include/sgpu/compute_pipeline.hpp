#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class compute_pipeline : public resource
    {
    public:
        compute_pipeline(SDL_GPUDevice* device, SDL_GPUComputePipeline* compute_pipeline);

        ~compute_pipeline() override;

        [[nodiscard]] SDL_GPUComputePipeline* get_compute_pipeline() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUComputePipeline* m_compute_pipeline;
    };
}
