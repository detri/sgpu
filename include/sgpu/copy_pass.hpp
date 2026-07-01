#pragma once

#include "sgpu/pass.hpp"
#include "sgpu/buffer.hpp"
#include "sgpu/texture.hpp"
#include "sgpu/transfer_buffer.hpp"
#include "SDL3/SDL_gpu.h"

namespace sgpu
{
    class copy_pass : public detail::pass
    {
    public:
        explicit copy_pass(SDL_GPUCopyPass* pass);

        ~copy_pass() override;

        copy_pass() = delete;
        copy_pass(const copy_pass&) = delete;
        copy_pass& operator=(const copy_pass&) = delete;
        copy_pass(copy_pass&&) = delete;
        copy_pass& operator=(copy_pass&&) = delete;

        void upload_to_buffer(const handle<transfer_buffer>& source,
                              uint32_t source_offset,
                              const handle<buffer>& destination,
                              uint32_t destination_offset,
                              uint32_t size,
                              bool one_shot = false);

        void upload_to_texture(const handle<transfer_buffer>& source,
                               uint32_t source_offset,
                               const handle<texture>& texture,
                               uint32_t layer_width,
                               uint32_t layer_height,
                               bool one_shot = true
        );

        void download_from_buffer(const handle<transfer_buffer>& destination,
                                  uint32_t destination_offset,
                                  const handle<buffer>& source,
                                  uint32_t source_offset,
                                  uint32_t size);

        void download_from_texture(const handle<transfer_buffer>& destination,
                                   uint32_t destination_offset,
                                   const handle<texture>& source,
                                   uint32_t source_width,
                                   uint32_t source_height);

    private:
        SDL_GPUCopyPass* m_pass;
    };
}
