#pragma once

#include "SDL3/SDL_gpu.h"

#include "sgpu/resource.hpp"

namespace sgpu
{
    class shader : public resource
    {
    public:
        shader(SDL_GPUDevice* device, SDL_GPUShader* shader);

        ~shader() override;

        [[nodiscard]] SDL_GPUShader* get_shader() const noexcept;

    private:
        SDL_GPUDevice* m_device;
        SDL_GPUShader* m_shader;
    };
}
