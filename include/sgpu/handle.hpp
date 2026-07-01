#pragma once

#include <utility>
#include "sgpu/resource.hpp"

namespace sgpu
{
    template<class T>
    class handle
    {
    public:
        handle() : m_ptr(nullptr) {}

        explicit handle(T* ptr, bool add_ref = true) : m_ptr(ptr)
        {
            if (m_ptr && add_ref)
            {
                resource_add_ref(m_ptr);
            }
        }

        handle(const handle& other) : m_ptr(other.m_ptr)
        {
            if (m_ptr)
            {
                resource_add_ref(m_ptr);
            }
        }

        template<class U>
            requires std::is_convertible_v<U*, T*> && (!std::is_same_v<U, T>)
        handle(const handle<U>& other) : m_ptr(other.m_ptr)
        {
            if (m_ptr)
            {
                resource_add_ref(m_ptr);
            }
        }

        ~handle()
        {
            if (m_ptr)
            {
                resource_remove_ref(m_ptr);
            }
        }

        template<class U>
            requires std::is_convertible_v<U*, T*>
        handle& operator=(const handle<U>& rhs)
        {
            handle(rhs).swap(*this);
            return *this;
        }

        handle(handle&& rhs) noexcept : m_ptr(std::exchange(rhs.m_ptr, nullptr)) {}

        handle& operator=(handle&& rhs) noexcept
        {
            handle(std::move(rhs)).swap(*this);
            return *this;
        }

        template<class U>
        friend class handle;

        template<class U>
            requires std::is_convertible_v<U*, T*>
        handle(handle<U>&& rhs) noexcept : m_ptr(std::exchange(rhs.m_ptr, nullptr)) {}

        template<class U>
            requires std::is_convertible_v<U*, T*>
        handle& operator=(handle<U>&& rhs) noexcept
        {
            handle(std::move(rhs)).swap(*this);
            return *this;
        }

        handle& operator=(const handle& rhs)
        {
            handle(rhs).swap(*this);
            return *this;
        }

        handle& operator=(T* rhs)
        {
            handle(rhs).swap(*this);
            return *this;
        }

        void reset()
        {
            handle().swap(*this);
        }

        void reset(T* rhs)
        {
            handle(rhs).swap(*this);
        }

        void reset(T* rhs, bool add_ref)
        {
            handle(rhs, add_ref).swap(*this);
        }

        T* get() const noexcept
        {
            return m_ptr;
        }

        T* detach() noexcept
        {
            return std::exchange(m_ptr, nullptr);
        }

        T& operator*() const noexcept
        {
            return *m_ptr;
        }

        T* operator->() const noexcept
        {
            return m_ptr;
        }

        explicit operator bool() const noexcept
        {
            return m_ptr != nullptr;
        }

        void swap(handle& other) noexcept
        {
            using std::swap;
            swap(m_ptr, other.m_ptr);
        }

    private:
        T* m_ptr;
    };

    template<class T, class... Args>
    [[nodiscard]] handle<T> make_handle(Args&&... args)
    {
        return handle<T>(new T(std::forward<Args>(args)...));
    }

    template<class T, class U>
    bool operator==(const handle<T>& a, const handle<U>& b) noexcept
    {
        return a.get() == b.get();
    }

    template<class T, class U>
    bool operator!=(const handle<T>& a, const handle<U>& b) noexcept
    {
        return a.get() != b.get();
    }

    template<class T, class U>
    bool operator==(const handle<T>& a, U* b) noexcept
    {
        return a.get() == b;
    }

    template<class T, class U>
    bool operator!=(const handle<T>& a, U* b) noexcept
    {
        return a.get() != b;
    }

    template<class T, class U>
    bool operator==(T* a, const handle<U>& b) noexcept
    {
        return a == b.get();
    }

    template<class T, class U>
    bool operator!=(T* a, const handle<U>& b) noexcept
    {
        return a != b.get();
    }

    template<class T>
    bool operator==(const handle<T>& p, std::nullptr_t) noexcept
    {
        return p.get() == nullptr;
    }

    template<class T>
    bool operator==(std::nullptr_t, const handle<T>& p) noexcept
    {
        return p.get() == nullptr;
    }

    template<class T>
    bool operator!=(const handle<T>& p, std::nullptr_t) noexcept
    {
        return p.get() != nullptr;
    }

    template<class T>
    bool operator!=(std::nullptr_t, const handle<T>& p) noexcept
    {
        return p.get() != nullptr;
    }

    template<class T>
    bool operator<(const handle<T>& a, const handle<T>& b) noexcept
    {
        return std::less<T*>()(a.get(), b.get());
    }

    template<class T>
    void swap(handle<T>& lhs, handle<T>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}

template<class T>
struct std::hash<sgpu::handle<T> >
{
    [[nodiscard]] std::size_t operator()(const sgpu::handle<T>& p) noexcept
    {
        return std::hash<T*>()(p.get());
    }
};
