#include "sgpu/shader.hpp"

namespace sgpu
{
    shader::shader(SDL_GPUDevice* device, SDL_GPUShader* shader):
        m_device(device),
        m_shader(shader) {
    }

    shader::~shader() {
        SDL_ReleaseGPUShader(m_device, m_shader);
    }
    SDL_GPUShader* shader::get_shader() const noexcept {
        return m_shader;
    }
}
