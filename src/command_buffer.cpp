#include "sgpu/command_buffer.hpp"

namespace sgpu
{
    command_buffer::command_buffer() : m_command_buffer(nullptr),
                                       m_pass_in_progress(false),
                                       m_has_swapchain_texture(false) {}

    command_buffer::command_buffer(SDL_GPUCommandBuffer* command_buffer) : m_command_buffer(command_buffer),
                                                                           m_pass_in_progress(false),
                                                                           m_has_swapchain_texture(false) {}

    command_buffer::~command_buffer()
    {
        if (m_command_buffer)
        {
            assert(!m_has_swapchain_texture &&
                "Cannot cancel a command buffer after acquiring a swapchain texture; submit it instead");
            SDL_CancelGPUCommandBuffer(m_command_buffer);
        }
    }

    command_buffer::command_buffer(command_buffer&& rhs) noexcept : m_command_buffer(rhs.m_command_buffer),
                                                                    m_tracked_resources(
                                                                        std::move(rhs.m_tracked_resources)),
                                                                    m_pass_in_progress(rhs.m_pass_in_progress),
                                                                    m_has_swapchain_texture(rhs.m_has_swapchain_texture)
    {
        rhs.m_command_buffer = nullptr;
        rhs.m_pass_in_progress = false;
        rhs.m_has_swapchain_texture = false;
    }

    command_buffer& command_buffer::operator=(command_buffer&& rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(m_command_buffer, rhs.m_command_buffer);
            std::swap(m_tracked_resources, rhs.m_tracked_resources);
            std::swap(m_pass_in_progress, rhs.m_pass_in_progress);
            std::swap(m_has_swapchain_texture, rhs.m_has_swapchain_texture);
        }
        return *this;
    }

    SDL_GPUFence* command_buffer::device_submit(SDL_GPUDevice* device)
    {
        assert(!m_pass_in_progress && "Cannot submit command buffer while a pass is in progress");
        const auto fence = SDL_SubmitGPUCommandBufferAndAcquireFence(m_command_buffer);
        m_command_buffer = nullptr;
        m_has_swapchain_texture = false;
        return fence;
    }

    void command_buffer::device_reset(SDL_GPUCommandBuffer* replacement)
    {
        assert(!m_pass_in_progress && "Cannot reset command buffer while a pass is in progress");
        m_command_buffer = replacement;
        m_tracked_resources.clear();
        m_has_swapchain_texture = false;
    }

    void command_buffer::device_mark_swapchain_texture_acquired() noexcept
    {
        m_has_swapchain_texture = true;
    }
}
