#include "sgpu/device.hpp"

namespace sgpu
{
    device::device() noexcept : m_window(nullptr),
                                m_device(nullptr),
                                m_swapchain_texture_format(SDL_GPU_TEXTUREFORMAT_INVALID) {}

    device::device(SDL_Window* window, const SDL_GPUShaderFormat shader_format, const bool debug) : m_window(window),
        m_device(SDL_CreateGPUDevice(shader_format, debug, nullptr))
    {
        SDL_ClaimWindowForGPUDevice(m_device, m_window);
        SDL_SetGPUSwapchainParameters(m_device, m_window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);
        m_swapchain_texture_format = SDL_GetGPUSwapchainTextureFormat(m_device, m_window);
    }

    device::~device()
    {
        if (valid())
        {
            SDL_WaitForGPUIdle(m_device);
            while (!m_submitted.empty())
            {
                auto submission = std::move(m_submitted.front());
                m_submitted.pop();
                if (submission.fence)
                {
                    SDL_ReleaseGPUFence(m_device, submission.fence);
                }
            }
            m_command_pool.clear();
            SDL_ReleaseWindowFromGPUDevice(m_device, m_window);
            SDL_DestroyGPUDevice(m_device);
        }
    }

    device::device(device&& other) noexcept : m_window(other.m_window),
                                              m_device(other.m_device),
                                              m_swapchain_texture_format(other.m_swapchain_texture_format)
    {
        other.m_device = nullptr;
        other.m_window = nullptr;
        other.m_swapchain_texture_format = SDL_GPU_TEXTUREFORMAT_INVALID;
    }

    device& device::operator=(device&& rhs) noexcept
    {
        device(std::move(rhs)).swap(*this);
        return *this;
    }

    bool device::valid() const noexcept
    {
        return m_window != nullptr && m_device != nullptr;
    }

    device::operator bool() const
    {
        return valid();
    }

    SDL_GPUTextureFormat device::get_swapchain_texture_format() const noexcept
    {
        return m_swapchain_texture_format;
    }

    SDL_GPUDevice* device::get_device() const noexcept
    {
        return m_device;
    }

    SDL_Window* device::get_window() const noexcept
    {
        return m_window;
    }

    handle<buffer> device::create_buffer(const SDL_GPUBufferCreateInfo& createInfo) const noexcept
    {
        if (auto buf = SDL_CreateGPUBuffer(m_device, &createInfo))
        {
            return make_handle<buffer>(m_device, buf);
        }

        return {};
    }

    handle<compute_pipeline> device::create_compute_pipeline(
        const SDL_GPUComputePipelineCreateInfo& createInfo) const noexcept
    {
        if (auto pipeline = SDL_CreateGPUComputePipeline(m_device, &createInfo))
        {
            return make_handle<compute_pipeline>(m_device, pipeline);
        }

        return {};
    }

    handle<graphics_pipeline> device::create_graphics_pipeline(
        const SDL_GPUGraphicsPipelineCreateInfo& createInfo) const noexcept
    {
        if (auto pipeline = SDL_CreateGPUGraphicsPipeline(m_device, &createInfo))
        {
            return make_handle<graphics_pipeline>(m_device, pipeline);
        }

        return {};
    }

    handle<sampler> device::create_sampler(const SDL_GPUSamplerCreateInfo& createInfo) const noexcept
    {
        if (auto created_sampler = SDL_CreateGPUSampler(m_device, &createInfo))
        {
            return make_handle<sampler>(m_device, created_sampler);
        }

        return {};
    }

    handle<shader> device::create_shader(const SDL_GPUShaderCreateInfo& createInfo) const noexcept
    {
        if (auto created_shader = SDL_CreateGPUShader(m_device, &createInfo))
        {
            return make_handle<shader>(m_device, created_shader);
        }

        return {};
    }

    handle<texture> device::create_texture(const SDL_GPUTextureCreateInfo& createInfo) const noexcept
    {
        if (auto created_texture = SDL_CreateGPUTexture(m_device, &createInfo))
        {
            return make_handle<texture>(m_device, created_texture);
        }

        return {};
    }

    device::acquired_swapchain_texture device::wait_and_acquire_swapchain_texture(command_buffer& cmd) const noexcept
    {
        acquired_swapchain_texture acquired{};
        SDL_GPUTexture* swapchain_texture = nullptr;
        if (SDL_WaitAndAcquireGPUSwapchainTexture(
            cmd.m_command_buffer,
            m_window,
            &swapchain_texture,
            &acquired.width,
            &acquired.height))
        {
            if (swapchain_texture)
            {
                cmd.device_mark_swapchain_texture_acquired();
                acquired.texture_handle = make_handle<texture>(m_device, swapchain_texture, false);
            }
        }
        return acquired;
    }

    handle<transfer_buffer> device::create_transfer_buffer(
        const SDL_GPUTransferBufferCreateInfo& createInfo) const noexcept
    {
        if (auto created_transfer_buffer = SDL_CreateGPUTransferBuffer(m_device, &createInfo))
        {
            return make_handle<transfer_buffer>(m_device, created_transfer_buffer);
        }

        return {};
    }

    command_buffer device::acquire_command_buffer()
    {
        if (!m_command_pool.empty())
        {
            command_buffer cmd = std::move(m_command_pool.back());
            m_command_pool.pop_back();
            cmd.device_reset(SDL_AcquireGPUCommandBuffer(m_device));
            return cmd;
        }

        return command_buffer(SDL_AcquireGPUCommandBuffer(m_device));
    }

    void device::submit(command_buffer&& cmd)
    {
        const auto fence_ptr = cmd.device_submit(m_device);
        m_submitted.emplace(resource_fence{.cmd = std::move(cmd), .fence = fence_ptr});
    }

    void device::collect_garbage()
    {
        for (size_t i = 0; i < m_submitted.size(); ++i)
        {
            auto submission = std::move(m_submitted.front());
            m_submitted.pop();
            if (SDL_QueryGPUFence(m_device, submission.fence))
            {
                SDL_ReleaseGPUFence(m_device, submission.fence);
                m_command_pool.emplace_back(std::move(submission.cmd));
            } else
            {
                m_submitted.emplace(std::move(submission));
            }
        }
    }

    void device::swap(device& rhs) noexcept
    {
        using std::swap;
        swap(m_window, rhs.m_window);
        swap(m_device, rhs.m_device);
        swap(m_swapchain_texture_format, rhs.m_swapchain_texture_format);
    }
}
