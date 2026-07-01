#include "sgpu/texture.hpp"

namespace sgpu
{
    texture::texture(SDL_GPUDevice* device, SDL_GPUTexture* texture, bool owned) : m_device(device),
        m_texture(texture),
        m_owned(owned) {}

    texture::~texture()
    {
        if (m_owned && m_texture)
        {
            SDL_ReleaseGPUTexture(m_device, m_texture);
        }
    }

    SDL_GPUTexture* texture::get_texture() const noexcept
    {
        return m_texture;
    }
}
