#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class graphics_pipeline : public resource
    {
    public:
        graphics_pipeline(SDL_GPUDevice* device, SDL_GPUGraphicsPipeline* graphics_pipeline);

        ~graphics_pipeline() override;

        [[nodiscard]] SDL_GPUGraphicsPipeline* get_graphics_pipeline() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUGraphicsPipeline* m_graphics_pipeline;
    };
}
