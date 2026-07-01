#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class transfer_buffer : public resource
    {
    public:
        transfer_buffer(SDL_GPUDevice* device, SDL_GPUTransferBuffer* transfer_buffer);

        ~transfer_buffer() override;

        [[nodiscard]] SDL_GPUTransferBuffer* get_transfer_buffer() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUTransferBuffer* m_transfer_buffer;
    };
}
