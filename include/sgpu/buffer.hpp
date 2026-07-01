#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class buffer : public resource
    {
    public:
        buffer(SDL_GPUDevice* device, SDL_GPUBuffer* buffer);

        ~buffer() override;

        [[nodiscard]] SDL_GPUBuffer* get_buffer() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUBuffer* m_buffer;
    };
}
