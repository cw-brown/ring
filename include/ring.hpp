#ifndef RING_HPP
#define RING_HPP

#include <iterator>
#include <memory>
#include <initializer_list>
#include <ranges>
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
    template<input_iterator InputIt> constexpr ring(InputIt first, InputIt last, const Allocator& alloc = Allocator()): _max_size(distance(first, last)), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(distance(first, last))){
        for(; first != last; ++first){
            this->_buffer[_head] = *first;
            this->_incr();
        }
    }
    template<ranges::input_range R> constexpr ring(from_range_t, R&& rg, const Allocator& alloc = Allocator()): _max_size(distance(rg.begin(), rg.end())), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(distance(rg.begin(), rg.end()))){
        for(auto begin = rg.begin(), end = rg.end(); begin != end; ++begin){
            this->_buffer[_head] = *begin;
            this->_incr();
        }
    }
    constexpr ring(const ring& other): _max_size(other._max_size), _head(other._head), _tail(other._tail), _size(other._size), _alloc(allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator())), _buffer(other._buffer){}
    constexpr ring(ring&& other) noexcept: _max_size(other._max_size), _head(other._head), _tail(other._tail), _size(other._size), _alloc(move(other.get_allocator())), _buffer(move(other._buffer)){
        other._max_size = other._head = other._tail = other._size = 0;
        other._buffer = nullptr;
    }
    constexpr ring(const ring& other, const type_identity_t<Allocator>& alloc): _max_size(other._max_size), _head(other._head), _tail(other._tail), _size(other._size), _alloc(alloc), _buffer(other._buffer){};
    constexpr ring(ring&& other, const type_identity_t<Allocator>& alloc): _max_size(other._max_size), _head(other._head), _tail(other._tail), _size(other._size), _alloc(alloc), _buffer(move(other._buffer)){
        other._max_size = other._head = other._tail = other._size = 0;
        other._buffer = nullptr;
    }
    ring(initializer_list<T> init, const Allocator& alloc = Allocator()): _max_size(init.size()), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(init.size())){
        for(auto begin = init.begin(), end = init.end(); begin != end; ++begin){
            this->_buffer[_head] = *begin;
            this->_incr();
        }
    }

    constexpr ring& operator=(const ring& other){
        this->_max_size = other._max_size;
        this->_head = other._head;
        this->_tail = other._tail;
        this->size = other._size;
        if(allocator_traits<Allocator>::propogate_on_container_copy_assignment::value){
            if(this->_alloc != other._alloc){
                this->_alloc.deallocate(this->_buffer, this->_max_size);
                this->_alloc = other._alloc;
                this->_buffer = this->_alloc.allocate(this->_max_size);
            } 
        }
        this->_buffer = other._buffer;
        return *this;
    }
    constexpr ring& operator=(ring&& other) noexcept(allocator_traits<Allocator>::propogate_on_container_move_assignment::value || allocator_traits<Allocator>::is_always_equal::value){
        this->_head = other._head;
        this->_tail = other._tail;
        this->_size = other._size;
        if(allocator_traits<Allocator>::propogate_on_container_move_assignment::value){
            this->_alloc = other._alloc;
        } else if(allocator_traits<Allocator>::propogate_on_container_move_assignment::value && (this->_alloc != other._alloc)){
            this->_alloc.deallocate(this->_buffer, this->_max_size);
            this->_max_size = other._max_size;
            this->_buffer = this->_alloc.allocate(this->_max_size);
        }
        this->_buffer = move(other._buffer);
        other._buffer = nullptr;
        other._max_size = other._head = other._tail = other._size = 0;
        return *this;
    }
    constexpr ring& operator=(initializer_list<value_type> ilist){
        this->_max_size = ilist.size();
        this->_head = 0;
        this->_tail = 0;
        this->_size = 0;
        for(auto begin = ilist.begin(), end = ilist.end(); begin != end; ++begin){
            this->_buffer[_head] = *begin;
            this->_incr();
        }
        return *this;
    }
    
    

    constexpr bool full() const{
        return this->_size == this->_max_size;
    }
    pointer data(){
        return this->_buffer;
    }
    constexpr size_type size() const{
        return this->_size;
    }
    constexpr allocator_type get_allocator() const noexcept{
        return this->_alloc;
    }


    friend ostream& operator<<(ostream& stream, ring& obj){
        for(size_t i = 0; i < obj.size(); ++i){
            stream<<obj.data()[i];
            if(i != obj.size() - 1){
                stream<<", ";
            }
        }
        return stream;
    }

};
}
#endif