#pragma once

#include "SDL3/SDL_gpu.h"
#include "sgpu/pass.hpp"
#include "sgpu/graphics_pipeline.hpp"
#include "sgpu/buffer.hpp"
#include "sgpu/texture.hpp"
#include "sgpu/sampler.hpp"
#include <vector>
#include <span>
#include <utility>

#include "handle.hpp"

namespace sgpu
{
    struct buffer_binding
    {
        handle<buffer> buffer_handle;
        uint32_t offset;
    };

    struct texture_sampler
    {
        handle<texture> texture_handle;
        handle<sampler> sampler_handle;
    };

    class render_pass : public detail::pass
    {
    public:
        explicit render_pass(SDL_GPURenderPass* pass);

        ~render_pass() override;

        render_pass() = delete;
        render_pass(const render_pass&) = delete;
        render_pass& operator=(const render_pass&) = delete;
        render_pass(render_pass&&) = delete;
        render_pass& operator=(render_pass&&) = delete;

        void bind_graphics_pipeline(const handle<graphics_pipeline>& pipeline);

        void bind_vertex_buffers(uint32_t first_slot, std::span<buffer_binding> bindings);

        void bind_index_buffer(const buffer_binding& binding,
                               SDL_GPUIndexElementSize index_size = SDL_GPU_INDEXELEMENTSIZE_32BIT);

        void bind_fragment_samplers(uint32_t first_slot, std::span<texture_sampler> samplers);

        void bind_vertex_samplers(uint32_t first_slot, std::span<texture_sampler> samplers);

        void bind_vertex_storage_buffers(uint32_t first_slot, std::span<handle<buffer> > buffers);

        void bind_vertex_storage_textures(uint32_t first_slot, std::span<handle<texture> > textures);

        void bind_fragment_storage_buffers(uint32_t first_slot, std::span<handle<buffer> > buffers);

        void bind_fragment_storage_textures(uint32_t first_slot, std::span<handle<texture> > textures);

        void draw(uint32_t num_vertices, uint32_t num_instances, uint32_t first_vertex, uint32_t first_instance) const;

        void draw_indexed(uint32_t num_indices, uint32_t num_instances, uint32_t first_index, int vertex_offset,
                          uint32_t first_instance) const;

        void draw_indirect(const handle<buffer>& buffer, uint32_t offset, uint32_t draw_count);

        void draw_indexed_indirect(const handle<buffer>& buffer, uint32_t offset, uint32_t draw_count);

    private:
        SDL_GPURenderPass* m_pass;
    };
}
