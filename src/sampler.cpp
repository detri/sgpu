#include "sgpu/sampler.hpp"

namespace sgpu
{
    sampler::sampler(SDL_GPUDevice* device, SDL_GPUSampler* sampler) : m_device(device),
                                                                       m_sampler(sampler) {}

    sampler::~sampler()
    {
        SDL_ReleaseGPUSampler(m_device, m_sampler);
    }

    SDL_GPUSampler* sampler::get_sampler() const noexcept
    {
        return m_sampler;
    }
}
