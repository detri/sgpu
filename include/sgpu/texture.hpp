#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class texture : public resource
    {
    public:
        texture(SDL_GPUDevice* device, SDL_GPUTexture* texture, bool owned = true);

        ~texture() override;

        [[nodiscard]] SDL_GPUTexture* get_texture() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUTexture* m_texture;
        bool m_owned;
    };
}
