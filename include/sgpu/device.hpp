#pragma once

#include <queue>

#include "SDL3/SDL_gpu.h"
#include "sgpu/handle.hpp"
#include "sgpu/buffer.hpp"
#include "sgpu/compute_pipeline.hpp"
#include "sgpu/graphics_pipeline.hpp"
#include "sgpu/sampler.hpp"
#include "sgpu/shader.hpp"
#include "sgpu/texture.hpp"
#include "sgpu/transfer_buffer.hpp"
#include "sgpu/command_buffer.hpp"
#include <utility>

namespace sgpu
{
    struct resource_fence
    {
        command_buffer cmd;
        SDL_GPUFence* fence;
    };

    class device
    {
    public:
        struct acquired_swapchain_texture
        {
            handle<texture> texture_handle;
            uint32_t width;
            uint32_t height;
        };

        device() noexcept;

        device(SDL_Window* window, SDL_GPUShaderFormat shader_format, bool debug);

        ~device();

        device(const device&) = delete;
        device& operator=(const device&) = delete;

        // explicit move construct
        device(device&& other) noexcept;

        // explicit move assign
        device& operator=(device&& rhs) noexcept;

        // utility

        [[nodiscard]] bool valid() const noexcept;
        [[nodiscard]] explicit operator bool() const;
        void swap(device& rhs) noexcept;

        // details

        [[nodiscard]] SDL_GPUTextureFormat get_swapchain_texture_format() const noexcept;
        [[nodiscard]] SDL_GPUDevice* get_device() const noexcept;
        [[nodiscard]] SDL_Window* get_window() const noexcept;

        // resources

        [[nodiscard]] handle<buffer> create_buffer(const SDL_GPUBufferCreateInfo& createInfo) const noexcept;

        [[nodiscard]] handle<compute_pipeline> create_compute_pipeline(const SDL_GPUComputePipelineCreateInfo& createInfo) const noexcept;

        [[nodiscard]] handle<graphics_pipeline> create_graphics_pipeline(const SDL_GPUGraphicsPipelineCreateInfo& createInfo) const noexcept;

        [[nodiscard]] handle<sampler> create_sampler(const SDL_GPUSamplerCreateInfo& createInfo) const noexcept;

        [[nodiscard]] handle<shader> create_shader(const SDL_GPUShaderCreateInfo& createInfo) const noexcept;

        [[nodiscard]] handle<texture> create_texture(const SDL_GPUTextureCreateInfo& createInfo) const noexcept;

        [[nodiscard]] acquired_swapchain_texture wait_and_acquire_swapchain_texture(command_buffer& cmd) const noexcept;

        [[nodiscard]] handle<transfer_buffer> create_transfer_buffer(const SDL_GPUTransferBufferCreateInfo& createInfo) const noexcept;

        // recording and submission

        command_buffer acquire_command_buffer();

        void submit(command_buffer&& cmd);

        void collect_garbage();

    private:
        SDL_Window* m_window;
        SDL_GPUDevice* m_device;
        std::vector<command_buffer> m_command_pool;
        std::queue<resource_fence> m_submitted;
        SDL_GPUTextureFormat m_swapchain_texture_format;
    };
}
