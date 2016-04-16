#pragma once

#include <numeric>

namespace litehtml
{

struct allocator_helper
{
    using alloc_func = void*(*)(size_t);
    using dealloc_func = void(*)(void*);

    static void* allocate(size_t bytes);
    static void deallocate(void* ptr);

    static alloc_func   get_alloc_func();
    static dealloc_func get_dealloc_func();

    // throw exception if not all allocated memory deallocated
    static void set_functions(alloc_func alloc, dealloc_func dealloc);
};

template <typename T>
struct lite_allocator
{
    using value_type = T;
    using pointer = T*;
    using const_pointer	= const T*;
    using reference	= T&;
    using const_reference = const T&;
    using size_type	= std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind { using other = lite_allocator<U>; };

    lite_allocator() = default;
    lite_allocator(const lite_allocator&) = default;
    template <typename U>
    lite_allocator(const lite_allocator<U>&) {};
    ~lite_allocator() = default;

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }
        
    pointer allocate(size_type n, std::allocator<void>::const_pointer /*hint*/ = 0)
    {
        return reinterpret_cast<pointer>(allocator_helper::allocate(n * sizeof(T)));
    }

    void deallocate(pointer p, size_type /*n*/)
    {
        allocator_helper::deallocate(p);
    }

    size_type max_size() const { return std::numeric_limits<size_type>::max(); }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p)
    {
        p->~U();
    }
};

template <typename T1, typename T2>
bool operator==(const lite_allocator<T1>& lhs, const lite_allocator<T2>& rhs)
{
    return true;
}

template <typename T1, typename T2>
bool operator!=(const lite_allocator<T1>& lhs, const lite_allocator<T2>& rhs)
{
    return false;
}

} // namespace litehtml