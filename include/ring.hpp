#ifndef RING_HPP
#define RING_HPP

#include <iterator>
#include <memory>
#include <initializer_list>
#include <ranges>
#include <concepts>
#include <type_traits>

namespace std{
template<class T, class Allocator = allocator<T>>
class ring final{
private:
    template<class D, bool constness>
    class it{

    };
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = it<T, false>;
private:
    size_type _max_size;
    size_type _head;
    size_type _tail;
    size_type _size;
    allocator_type _alloc;
    pointer _buffer;
    void _incr(){
        if(this->full()){
            this->_tail = (this->_tail + 1) % this->_max_size;
        } else{
            ++this->_size;
        }
        this->_head = (this->_head + 1) % this->_max_size;
    };
public:
    constexpr ring() noexcept(noexcept(Allocator())): ring(Allocator()){}
    constexpr explicit ring(const Allocator& alloc) noexcept: _max_size(0), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(0)){}
    explicit ring(size_type count, const Allocator& alloc = Allocator()): _max_size(count), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(count)){
        for(size_type i = 0; i < count; ++i){
            this->_buffer[i] = value_type();
            this->_incr();
        }
    }
    constexpr explicit ring(size_type count, const_reference value, const Allocator& alloc = Allocator()): _max_size(count), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(count)){
        for(size_type i = 0; i < count; ++i){
            this->_buffer[i] = value;
            this->_incr();
        }
    }
    // template<class InputIt> constexpr ring(InputIt first, InputIt last, const Allocator& alloc = Allocator()): _max_size(distance(first, last)), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(distance(first, last))){
    //     for(;first != last; ++first){
    //         this->_buffer[_head] = *first;
    //         this->_incr();
    //     }
    // }


    constexpr bool full() const{
        return this->_size == this->_max_size;
    }
    pointer test(){
        return this->_buffer;
    }
    constexpr size_type size() const{
        return this->_size;
    }


    friend ostream& operator<<(ostream& stream, ring& obj){
        for(size_t i = 0; i < obj.size(); ++i){
            stream<<obj.test()[i];
            if(i != obj.size() - 1){
                stream<<", ";
            }
        }
        return stream;
    }

};
}
#endif