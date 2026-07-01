#include "sgpu/render_pass.hpp"

namespace sgpu
{
    render_pass::render_pass(SDL_GPURenderPass* pass) : m_pass(pass) {}

    render_pass::~render_pass()
    {
        SDL_EndGPURenderPass(m_pass);
    }

    void render_pass::bind_graphics_pipeline(const handle<graphics_pipeline>& pipeline)
    {
        m_tracked_resources.emplace_back(pipeline);
        SDL_BindGPUGraphicsPipeline(m_pass, pipeline->get_graphics_pipeline());
    }

    void render_pass::bind_vertex_buffers(const uint32_t first_slot, const std::span<buffer_binding> bindings)
    {
        std::vector<SDL_GPUBufferBinding> sdl_bindings(bindings.size());
        for (size_t i = 0; i < bindings.size(); ++i)
        {
            m_tracked_resources.emplace_back(bindings[i].buffer_handle);
            sdl_bindings[i].buffer = bindings[i].buffer_handle->get_buffer();
            sdl_bindings[i].offset = bindings[i].offset;
        }
        SDL_BindGPUVertexBuffers(m_pass, first_slot, sdl_bindings.data(), sdl_bindings.size());
    }

    void render_pass::bind_index_buffer(const buffer_binding& binding, const SDL_GPUIndexElementSize index_size)
    {
        m_tracked_resources.emplace_back(binding.buffer_handle);
        SDL_GPUBufferBinding sdl_binding{};
        sdl_binding.buffer = binding.buffer_handle->get_buffer();
        sdl_binding.offset = binding.offset;
        SDL_BindGPUIndexBuffer(m_pass, &sdl_binding, index_size);
    }

    void render_pass::bind_fragment_samplers(const uint32_t first_slot, const std::span<texture_sampler> samplers)
    {
        std::vector<SDL_GPUTextureSamplerBinding> sdl_bindings(samplers.size());
        for (size_t i = 0; i < samplers.size(); ++i)
        {
            m_tracked_resources.emplace_back(samplers[i].texture_handle);
            m_tracked_resources.emplace_back(samplers[i].sampler_handle);
            sdl_bindings[i].texture = samplers[i].texture_handle->get_texture();
            sdl_bindings[i].sampler = samplers[i].sampler_handle->get_sampler();
        }
        SDL_BindGPUFragmentSamplers(m_pass, first_slot, sdl_bindings.data(), sdl_bindings.size());
    }

    void render_pass::bind_vertex_samplers(const uint32_t first_slot, const std::span<texture_sampler> samplers)
    {
        std::vector<SDL_GPUTextureSamplerBinding> sdl_bindings(samplers.size());
        for (size_t i = 0; i < samplers.size(); ++i)
        {
            m_tracked_resources.emplace_back(samplers[i].texture_handle);
            m_tracked_resources.emplace_back(samplers[i].sampler_handle);
            sdl_bindings[i].texture = samplers[i].texture_handle->get_texture();
            sdl_bindings[i].sampler = samplers[i].sampler_handle->get_sampler();
        }
        SDL_BindGPUVertexSamplers(m_pass, first_slot, sdl_bindings.data(), sdl_bindings.size());
    }

    void render_pass::bind_vertex_storage_buffers(const uint32_t first_slot, const std::span<handle<buffer> > buffers)
    {
        std::vector<SDL_GPUBuffer*> sdl_buffers(buffers.size());
        for (size_t i = 0; i < buffers.size(); ++i)
        {
            m_tracked_resources.emplace_back(buffers[i]);
            sdl_buffers[i] = buffers[i]->get_buffer();
        }
        SDL_BindGPUVertexStorageBuffers(m_pass, first_slot, sdl_buffers.data(), sdl_buffers.size());
    }

    void render_pass::
    bind_vertex_storage_textures(const uint32_t first_slot, const std::span<handle<texture> > textures)
    {
        std::vector<SDL_GPUTexture*> sdl_textures(textures.size());
        for (size_t i = 0; i < textures.size(); ++i)
        {
            m_tracked_resources.emplace_back(textures[i]);
            sdl_textures[i] = textures[i]->get_texture();
        }
        SDL_BindGPUVertexStorageTextures(m_pass, first_slot, sdl_textures.data(), sdl_textures.size());
    }

    void render_pass::
    bind_fragment_storage_buffers(const uint32_t first_slot, const std::span<handle<buffer> > buffers)
    {
        std::vector<SDL_GPUBuffer*> sdl_buffers(buffers.size());
        for (size_t i = 0; i < buffers.size(); ++i)
        {
            m_tracked_resources.emplace_back(buffers[i]);
            sdl_buffers[i] = buffers[i]->get_buffer();
        }
        SDL_BindGPUFragmentStorageBuffers(m_pass, first_slot, sdl_buffers.data(), sdl_buffers.size());
    }

    void render_pass::bind_fragment_storage_textures(const uint32_t first_slot,
                                                     const std::span<handle<texture> > textures)
    {
        std::vector<SDL_GPUTexture*> sdl_textures(textures.size());
        for (size_t i = 0; i < textures.size(); ++i)
        {
            m_tracked_resources.emplace_back(textures[i]);
            sdl_textures[i] = textures[i]->get_texture();
        }
        SDL_BindGPUFragmentStorageTextures(m_pass, first_slot, sdl_textures.data(), sdl_textures.size());
    }

    void render_pass::draw(const uint32_t num_vertices, const uint32_t num_instances, const uint32_t first_vertex,
                           const uint32_t first_instance) const
    {
        SDL_DrawGPUPrimitives(m_pass, num_vertices, num_instances, first_vertex, first_instance);
    }

    void render_pass::draw_indexed(const uint32_t num_indices, const uint32_t num_instances, const uint32_t first_index,
                                   const int vertex_offset, const uint32_t first_instance) const
    {
        SDL_DrawGPUIndexedPrimitives(m_pass, num_indices, num_instances, first_index, vertex_offset, first_instance);
    }

    void render_pass::draw_indirect(const handle<buffer>& buffer, const uint32_t offset, const uint32_t draw_count)
    {
        m_tracked_resources.emplace_back(buffer);
        SDL_DrawGPUPrimitivesIndirect(m_pass, buffer->get_buffer(), offset, draw_count);
    }

    void render_pass::draw_indexed_indirect(const handle<buffer>& buffer, const uint32_t offset,
                                            const uint32_t draw_count)
    {
        m_tracked_resources.emplace_back(buffer);
        SDL_DrawGPUIndexedPrimitivesIndirect(m_pass, buffer->get_buffer(), offset, draw_count);
    }
}
