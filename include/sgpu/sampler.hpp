#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class sampler : public resource
    {
    public:
        sampler(SDL_GPUDevice* device, SDL_GPUSampler* sampler);

        ~sampler() override;

        [[nodiscard]] SDL_GPUSampler* get_sampler() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUSampler* m_sampler;
    };
}
