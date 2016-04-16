#include <atomic>
#include <exception>
#include "allocator.h"
 
namespace litehtml
{

allocator_helper::alloc_func allocate_function = ::malloc;
allocator_helper::dealloc_func deallocate_function = ::free;
std::atomic<size_t> allocations = 0;
 
void* allocator_helper::allocate(size_t bytes)
{
    void* ptr = allocate_function(bytes);
    allocations++;
    return ptr;
}
 
void allocator_helper::deallocate(void* ptr)
{
    deallocate_function(ptr);
    allocations--;
}
 
allocator_helper::alloc_func allocator_helper::get_alloc_func()
{
    return allocate_function;
}
 
allocator_helper::dealloc_func allocator_helper::get_dealloc_func()
{
    return deallocate_function;
}
 
void allocator_helper::set_functions(alloc_func alloc, dealloc_func dealloc)
{
    if (allocations != 0)
    {
        throw std::exception("Not all memory deallocated");
    }
    allocate_function = alloc;
    deallocate_function = dealloc;
}
 
} // namespace litehtml