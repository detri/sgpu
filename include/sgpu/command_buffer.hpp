#pragma once

#include "SDL3/SDL_gpu.h"
#include "sgpu/render_pass.hpp"
#include "sgpu/copy_pass.hpp"
#include "sgpu/compute_pass.hpp"

#include <cassert>
#include <functional>

namespace sgpu
{
    struct storage_buffer
    {
        handle<buffer> buffer_handle;
        bool cycle;
    };

    struct storage_texture
    {
        handle<texture> texture_handle;
        uint32_t mip_level;
        uint32_t layer;
        bool cycle;
    };

    class command_buffer
    {
        friend class device;
    public:
        struct swapchain_texture
        {
            handle<texture> texture_handle;
            uint32_t width;
            uint32_t height;
        };

        command_buffer();

        explicit command_buffer(SDL_GPUCommandBuffer* command_buffer);

        ~command_buffer();

        command_buffer(const command_buffer&) = delete;

        command_buffer(command_buffer&& rhs) noexcept;

        command_buffer& operator=(const command_buffer&) = delete;

        command_buffer& operator=(command_buffer&& rhs) noexcept;

        template<class Fn>
            requires std::is_invocable_v<Fn, render_pass&>
        void render(
            const std::span<SDL_GPUColorTargetInfo> color_targets,
            const SDL_GPUDepthStencilTargetInfo& depth_stencil_target,
            std::span<handle<texture> > color_target_textures,
            const handle<texture>& depth_stencil_target_texture,
            Fn&& fn)
        {
            assert(!m_pass_in_progress && "A pass already in progress");
            m_pass_in_progress = true;
            m_tracked_resources.insert(m_tracked_resources.end(), color_target_textures.begin(), color_target_textures.end());
            m_tracked_resources.emplace_back(depth_stencil_target_texture);
            auto pass = render_pass{
                SDL_BeginGPURenderPass(m_command_buffer, color_targets.data(), color_targets.size(),
                                       &depth_stencil_target)
            };
            std::invoke(std::forward<Fn>(fn), pass);
            pass.consume_tracked_resources(m_tracked_resources);
            m_pass_in_progress = false;
        }

        template<class Fn>
            requires std::is_invocable_v<Fn, copy_pass&>
        void copy(Fn&& fn)
        {
            assert(!m_pass_in_progress && "A pass already in progress");
            m_pass_in_progress = true;
            auto pass = copy_pass{SDL_BeginGPUCopyPass(m_command_buffer)};
            std::invoke(std::forward<Fn>(fn), pass);
            pass.consume_tracked_resources(m_tracked_resources);
            m_pass_in_progress = false;
        }

        template<class Fn>
            requires std::is_invocable_v<Fn, compute_pass&>
        void compute(std::span<storage_texture> textures, std::span<storage_buffer> buffers, Fn&& fn)
        {
            assert(!m_pass_in_progress && "A pass already in progress");
            m_pass_in_progress = true;
            std::vector<SDL_GPUStorageTextureReadWriteBinding> texture_bindings(textures.size());
            for (size_t i = 0; i < textures.size(); ++i)
            {
                texture_bindings[i] = SDL_GPUStorageTextureReadWriteBinding{
                    .texture = textures[i].texture_handle->get_texture(), .mip_level = textures[i].mip_level,
                    .layer = textures[i].layer, .cycle = textures[i].cycle
                };
            }
            std::vector<SDL_GPUStorageBufferReadWriteBinding> buffer_bindings(buffers.size());
            for (size_t i = 0; i < buffers.size(); ++i)
            {
                buffer_bindings[i] = SDL_GPUStorageBufferReadWriteBinding{
                    .buffer = buffers[i].buffer_handle->get_buffer(), .cycle = buffers[i].cycle
                };
            }
            auto pass = compute_pass{
                SDL_BeginGPUComputePass(m_command_buffer, texture_bindings.data(), texture_bindings.size(),
                                        buffer_bindings.data(), buffer_bindings.size())
            };
            std::invoke(std::forward<Fn>(fn), pass);
            pass.consume_tracked_resources(m_tracked_resources);
            m_pass_in_progress = false;
        }

    private:
        SDL_GPUFence* device_submit(SDL_GPUDevice* device);

        void device_reset(SDL_GPUCommandBuffer* replacement);

        void device_mark_swapchain_texture_acquired() noexcept;

        SDL_GPUCommandBuffer* m_command_buffer;
        std::vector<handle<resource> > m_tracked_resources;
        bool m_pass_in_progress;
        bool m_has_swapchain_texture;
    };
}

