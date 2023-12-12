#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <memory>
#include <stack>

#include "helper.h"

namespace allocator {
    template <class T, size_t Size>
    class PoolAllocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;

        using propagate_on_container_copy_assignment = std::true_type;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_swap = std::true_type;

        template <class U>
        struct rebind {
            using other = PoolAllocator<U, Size>;
        };

    public:
        PoolAllocator() noexcept
            : _pool(new value_type[Size * sizeof(value_type)])
        {
            pointer begin = _pool.get();
            const pointer end = begin + Size;
            for (; begin != end; ++begin)
               _freeMemoryHeaders.push(begin);
        }

        template <class U>
        PoolAllocator(const PoolAllocator<U, Size>& other) noexcept 
            : _pool(other._pool),_freeMemoryHeaders(other._freeMemoryHeaders)
        {}

        PoolAllocator select_on_container_copy_construction() const {
            return PoolAllocator();
        }

        pointer allocate(size_t n) {
            if (n > max_size())
                throw std::invalid_argument(messages::NOT_ENOUGH_MEMORY);
            if (_freeMemoryHeaders.empty()) 
                throw std::bad_alloc();

            auto topPtr =_freeMemoryHeaders.top();
           _freeMemoryHeaders.pop();

            return topPtr;
        }

        void deallocate(pointer p, size_t n) {
            if (n > max_size()) 
                throw std::invalid_argument(messages::BAD_ARGUMENT);
            if (p == nullptr)
                throw std::invalid_argument(messages::BAD_ARGUMENT);
            if (_freeMemoryHeaders.size() == Size)
                throw std::runtime_error(messages::ALREADY_FREE);

            _freeMemoryHeaders.push(p);
        }

        const std::shared_ptr<value_type>& getPool() const {
            return _pool;
        }

        size_t getFreeSize() const {
            return _freeMemoryHeaders.size();
        }

        constexpr size_t max_size() { 
            return Size; 
        }

    private:
        std::shared_ptr<value_type> _pool;
        std::stack<pointer> _freeMemoryHeaders;
    };

    template <class T, class U, size_t Size>
    constexpr bool operator==(const PoolAllocator<T, Size>& alloc1,
                            const PoolAllocator<U, Size>& alloc2) noexcept {
        return (alloc1.getPool() == alloc2.getPool()) 
            && (alloc1.getFreeSize() == alloc2.getFreeSize());
    }

    template <class T, class U, size_t Size>
    constexpr bool operator!=(const PoolAllocator<T, Size>& alloc1,
                            const PoolAllocator<U, Size>& alloc2) noexcept {
        return !(alloc1 == alloc2);
    }
}

#endif // ALLOCATOR_H