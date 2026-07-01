#include "sgpu/transfer_buffer.hpp"

namespace sgpu
{
    transfer_buffer::transfer_buffer(SDL_GPUDevice* device, SDL_GPUTransferBuffer* transfer_buffer) : m_device(device),
        m_transfer_buffer(transfer_buffer) {}

    transfer_buffer::~transfer_buffer()
    {
        SDL_ReleaseGPUTransferBuffer(m_device, m_transfer_buffer);
    }

    SDL_GPUTransferBuffer* transfer_buffer::get_transfer_buffer() const noexcept
    {
        return m_transfer_buffer;
    }
}
