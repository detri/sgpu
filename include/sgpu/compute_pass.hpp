#pragma once

#include "SDL3/SDL_gpu.h"
#include "sgpu/pass.hpp"

namespace sgpu
{
    class compute_pass : public detail::pass
    {
    public:
        explicit compute_pass(SDL_GPUComputePass* pass);

        ~compute_pass() override;

        compute_pass() = delete;
        compute_pass(const compute_pass&) = delete;
        compute_pass& operator=(const compute_pass&) = delete;
        compute_pass(compute_pass&&) = delete;
        compute_pass& operator=(compute_pass&&) = delete;

    private:
        SDL_GPUComputePass* m_pass;
    };
}