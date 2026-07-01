#pragma once

#include <atomic>
#include <cstdint>

namespace sgpu
{
    class resource
    {
    public:
        resource() noexcept;

        resource(const resource&) = delete;

        resource& operator=(const resource&) = delete;

        resource(resource&&) = delete;

        resource& operator=(resource&&) = delete;

        [[nodiscard]] std::int_least32_t use_count() const noexcept;

    protected:
        virtual ~resource();

    private:
        friend void resource_add_ref(resource* ptr) noexcept;

        friend void resource_remove_ref(resource* ptr) noexcept;

        mutable std::atomic_int_least32_t m_ref_count{0};
    };

    inline resource::resource() noexcept = default;

    inline std::int_least32_t resource::use_count() const noexcept
    {
        return m_ref_count.load(std::memory_order_relaxed);
    }

    inline resource::~resource() = default;

    inline void resource_add_ref(resource* ptr) noexcept
    {
        ptr->m_ref_count.fetch_add(1, std::memory_order_relaxed);
    }

    inline void resource_remove_ref(resource* ptr) noexcept
    {
        if (ptr->m_ref_count.fetch_sub(1, std::memory_order_release) == 1)
        {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete ptr;
        }
    }
}
