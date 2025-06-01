#ifndef RING_HPP
#define RING_HPP

#include <iterator>
#include <memory>
#include <initializer_list>
#include <ranges>
#include <type_traits>

namespace std{
template<class T, class Allocator = allocator<T>>
class ring : public Container{
private:
public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
private:
    size_type _max_size;
    size_type _head;
    size_type _tail;
    size_type _size;
    allocator_type _alloc;
    pointer _buffer;
    constexpr void _M_range_check(size_type __n) const{
        if(__n >= (this->_head + this->_max_size - this->_tail)){
            throw out_of_range("ring::_M_range_check: __n (which is " + to_string(__n) + ") >= this->size() (which is " + to_string(this->_max_size) + "), or references uninitialized data beyond this->_head (which is " + to_string(this->_head) + ")");
        }
    }
    constexpr bool _full() const{
        return this->size == this->_max_size;
    }
    constexpr void _incr(){
        if(this->_full()){
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
    
    constexpr void assign(size_type count, const_reference value){
        this->_alloc.deallocate(this->_buffer, this->_max_size);
        this->_max_size = count;
        this->_head = this->_tail = this->_size = 0;
        this->_buffer = this->_alloc.allocate(count);
        for(size_type i = 0; i < count; ++i){
            this->_buffer[i] = value;
            this->_incr();
        }
    }
    template<input_iterator InputIt> constexpr void assign(InputIt first, InputIt last){
        this->_alloc.deallocate(this->_buffer, this->_max_size);
        this->_max_size = distance(first, last);
        this->_head = this->_tail = this->_size = 0;
        this->_buffer = this->_alloc.allocate(this->_max_size);
        for(; first != last; ++first){
            this->_buffer[this->_head] = *first;
            this->_incr();
        }
    }
    constexpr void assign(initializer_list<value_type> ilist){
        this->_alloc.deallocate(this->_buffer, this->_max_size);
        this->_max_size = ilist.size();
        this->_head = this->_tail = this->_size = 0;
        this->_buffer = this->_alloc.allocate(this->_max_size);
        for(auto begin = ilist.begin(), end = ilist.end(); begin != end; ++begin){
            this->_buffer[this->_head] = *begin;
            this->_incr();
        }
    }
    template<ranges::input_range R> constexpr void assign_range(R&& rg){
        this->_alloc.deallocate(this->_buffer, this->_max_size);
        this->_max_size = distance(rg.begin(), rg.end());
        this->_head = this->_tail = this->_size = 0;
        this->_buffer = this->_alloc.allocate(this->_max_size);
        for(auto begin = rg.begin(), end = rg.end(); begin != end; ++begin){
            this->_buffer[this->_head] = *begin;
            this->_incr();
        }
    }

    constexpr allocator_type get_allocator() const noexcept{
        return this->_alloc;
    }

    constexpr reference at(size_type pos){
        this->_M_range_check(pos);
        return this->_buffer[(this->_tail + pos) % this->_max_size];
    }
    constexpr const_reference at(size_type pos) const{
        this->_M_range_check(pos);
        return this->_buffer[(this->_tail + pos) % this->_max_size];
    }
    constexpr reference operator[](size_type pos){
        this->_M_range_check(pos);
        return this->_buffer[(this->_tail + pos) % this->_max_size];
    }
    constexpr const_reference operator[](size_type pos) const{
        this->_M_range_check(pos);
        return this->_buffer[(this->_tail + pos) % this->_max_size];
    }

    constexpr reference front(){
        return this->_buffer[this->_tail];
    }
    constexpr const_reference front() const{
        return this->_buffer[this->_tail];
    }
    constexpr reference back(){
        return this->_buffer[this->_head == 0 ? this->_max_size - 1 : this->_head - 1];
    }
    constexpr const_reference back() const{
        return this->_buffer[this->_head == 0 ? this->_max_size - 1 : this->_head - 1];
    }
    constexpr pointer data() noexcept{
        return this->_buffer;
    }
    constexpr const_pointer data() const noexcept{
        return this->_buffer;
    }

    constexpr bool empty() const{
        return this->size == 0;
    }
    constexpr size_type size() const noexcept{
        return this->_size;
    }
    constexpr size_type max_size() const noexcept{
        return this->_max_size;
    }

    constexpr void shrink_to_fit(){
        // Shrinks the ring down to its current number of elements. If it is full, does nothing
        // Head and tail will automatically go to 0, and the ring will be declared as full

    }

    constexpr void clear() noexcept{
        
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