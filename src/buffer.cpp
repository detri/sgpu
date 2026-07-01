#include "sgpu/buffer.hpp"

namespace sgpu
{
    buffer::buffer(SDL_GPUDevice* device, SDL_GPUBuffer* buffer):
        m_device(device),
        m_buffer(buffer) {
    }

    buffer::~buffer() {
        SDL_ReleaseGPUBuffer(m_device, m_buffer);
    }
    SDL_GPUBuffer* buffer::get_buffer() const noexcept {
        return m_buffer;
    }
}
