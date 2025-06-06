#ifndef RING_HPP
#define RING_HPP

#include <iterator>
#include <memory>
#include <initializer_list>
#include <ranges>
#include <type_traits>

namespace std{
template<class T, class Allocator = allocator<T>>
class ring{
private:
    template<class D, class Ref, class Ptr, class ContainerPtr>
    class it{
    public:
        typedef contiguous_iterator_tag iterator_category;
        typedef D value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef ContainerPtr container_pointer;
    private:
        container_pointer _buf;
        pointer _ptr;
        size_type _idx;
        difference_type _cnt;
        pointer _sntl;
    public:
        it(): _buf(nullptr), _ptr(nullptr), _idx(0), _cnt(0), _sntl(nullptr){}
        it(container_pointer container, pointer ptr, size_type index, size_type count): _buf(container), _ptr(ptr), _idx(index), _cnt(count), _sntl(&container->data()[-1]){}
        it(const it& other): _buf(other._buf), _ptr(other._ptr), _idx(other._idx), _cnt(other._cnt), _sntl(other._sntl){}
        it(it&& other): _buf(move(other._buf)), _ptr(move(other._ptr)), _idx(other._idx), _cnt(other._cnt), _sntl(move(other._sntl)){
            other._buf = nullptr;
            other._ptr = nullptr;
            other._sntl = nullptr;
        }
        constexpr it& operator=(const it& other){
            if(this != other){
                this->_buf = other._buf;
                this->_ptr = other._ptr;
                this->_idx = other._idx;
                this->_cnt = other._cnt;
                this->_sntl = other._sntl;
            }
            return *this;
        }
        constexpr it& operator=(it&& other){
            if(this != other){
                this->_buf = move(other._buf);
                this->_ptr = move(other._ptr);
                this->_idx = other._idx;
                this->_cnt = other._cnt;
                this->_sntl = move(other._sntl);
                other._buf = nullptr;
                other._ptr = nullptr;
                other._sntl = nullptr;
            }
            return *this;
        }
        
        constexpr reference operator*() const{
            return *this->_ptr;
        }
        constexpr pointer operator->() noexcept{
            return this->_ptr;
        }
        constexpr pointer operator->() const noexcept{
            return this->_ptr;
        }
        constexpr reference operator[](const difference_type& n) noexcept{
            if(this->_idx == 0 && n < 0){
                return this->_buf->data()[this->_buf->max_size() - 1 - n];
            } else{
                return this->_buf->data()[(this->_idx + n) % this->_buf->max_size()];
            }
        }
        constexpr const reference operator[](const difference_type& n) const noexcept{
            if(this->_idx == 0 && n < 0){
                return this->_buf->data()[this->_buf->max_size() - 1 - n];
            } else{
                return this->_buf->data()[(this->_idx + n) % this->_buf->max_size()];
            }
        }
        
        constexpr bool operator==(const it& other) const noexcept{
            if(this->_cnt == 0 && (other._ptr == this->_sntl)){
                return true;
            } else if((this->_idx == other._idx) && (this->_ptr == other._ptr)){
                return true;
            } else if(this->_ptr == this->_sntl && other._ptr == other._sntl){
                return true;
            } else{
                return false;
            }
        }
        constexpr bool operator!=(const it& other) const noexcept{
            return !(*this == other);
        }
        constexpr bool operator< (const it& other) const noexcept{
            if(other._ptr == other._sntl && this->_ptr != other._sntl){
                return true; // a pointer is always less than a sentinel
            } else if(this->_buf->_tail > this->_buf->_head){
                return true;
            } else if(this->_idx < other._idx){
                return true;
            } else{
                return false;
            }
        }
        constexpr bool operator> (const it& other) const noexcept{
            return !(*this < other);
        }
        constexpr bool operator<=(const it& other) const noexcept{
            return *this == other || *this < other ? true : false;
        }
        constexpr bool operator>=(const it& other) const noexcept{
            return *this == other || *this > other ? true : false;
        }

        constexpr it& operator++() noexcept{
            this->_idx = (this->_idx + 1) % this->_buf->max_size();
            this->_ptr = &this->_buf->data()[this->_idx];
            --this->_cnt;
            return *this;
        }
        constexpr it operator++(int) noexcept{
            auto temp = *this;
            this->_idx = (this->_idx + 1) % this->_buf->max_size();
            this->_ptr = &this->_buf->data()[this->_idx];
            --this->_cnt;
            return temp;
        }
        constexpr it& operator--() noexcept{
            this->_idx = this->_idx == 0 ? this->_buf->max_size() - 1 : this->_idx - 1;
            this->_ptr = &this->_buf->data()[this->_idx];
            ++this->_cnt;
            return *this;
        }
        constexpr it operator--(int) noexcept{
            auto temp = *this;
            this->_idx = this->_idx == 0 ? this->_buf->max_size() - 1 : this->_idx - 1;
            this->_ptr = &this->_buf->data()[this->_idx];
            ++this->_cnt;
            return temp;
        }

        constexpr it& operator+=(const difference_type& n) noexcept{
            this->_idx = (this->_idx + n) % this->_buf->max_size();
            this->_ptr = &this->_buf->data()[this->_idx];
            this->_cnt -= n;
            return *this;
        }
        constexpr it& operator-=(const difference_type& n) noexcept{
            this->_idx = this->_idx == 0 ? this->_buf->max_size() - 1 - n : this->_idx - n;
            this->_ptr = &this->_buf->data()[this->_idx];
            this->_cnt += n;
            return *this;
        }
        constexpr friend it operator+(const it& pred, const difference_type& n) noexcept{
            auto temp = pred;
            temp._idx = (pred._idx + n) % pred._buf->max_size();
            temp._ptr = &pred._buf->data()[temp._idx];
            temp._cnt = pred._cnt - n;
            return temp;
        }
        constexpr friend it operator+(const difference_type& n, const it& pred) noexcept{
            auto temp = pred;
            temp._idx = (pred._idx + n) % pred._buf->max_size();
            temp._ptr = &pred._buf->data()[temp._idx];
            temp._cnt = pred._cnt - n;
            return temp;
        }
        constexpr friend it operator-(const it& pred, const difference_type& n) noexcept{
            auto temp = pred;
            temp._idx = pred._idx == 0 ? pred._buf->max_size() - 1 - n : pred._idx - n;
            if(pred._idx == 0 && pred._ptr == pred._sntl){temp._idx = pred._buf->max_size() - n;} // why?
            temp._ptr = &pred._buf->data()[temp._idx];
            temp._cnt = pred._cnt + n;
            return temp;
        }
        constexpr friend difference_type operator-(const it& lhs, const it& rhs) noexcept{
            return abs(lhs._cnt - rhs._cnt);
        }
    };
    template<class D, class Ref, class Ptr, class ContainerPtr>
    class rit{
    public:
        typedef contiguous_iterator_tag iterator_category;
        typedef D value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef ContainerPtr container_pointer;
    private:
        container_pointer _buf;
        pointer _ptr;
        size_type _idx;
        difference_type _cnt;
        pointer _sntl;
    public:
        rit(): _buf(nullptr), _ptr(nullptr), _idx(0), _cnt(0), _sntl(nullptr){}
        rit(container_pointer container, pointer ptr, size_type index, size_type count): _buf(container), _ptr(ptr), _idx(index), _cnt(count), _sntl(&container->data()[-1]){}
        rit(const rit& other): _buf(other._buf), _ptr(other._ptr), _idx(other._idx), _cnt(other._cnt), _sntl(other._sntl){}
        rit(rit&& other): _buf(move(other._buf)), _ptr(move(other._ptr)), _idx(other._idx), _cnt(other._cnt), _sntl(move(other._sntl)){
            other._buf = nullptr;
            other._ptr = nullptr;
            other._sntl = nullptr;
        }
        constexpr rit& operator=(const rit& other){
            if(this != other){
                this->_buf = other._buf;
                this->_ptr = other._ptr;
                this->_idx = other._idx;
                this->_cnt = other._cnt;
                this->_sntl = other._sntl;
            }
            return *this;
        }
        constexpr rit& operator=(rit&& other){
            if(this != other){
                this->_buf = move(other._buf);
                this->_ptr = move(other._ptr);
                this->_idx = other._idx;
                this->_cnt = other._cnt;
                this->_sntl = move(other._sntl);
                other._buf = nullptr;
                other._ptr = nullptr;
                other._sntl = nullptr;
            }
            return *this;
        }
        
        constexpr reference operator*() const{
            return *this->_ptr;
        }
        constexpr pointer operator->() noexcept{
            return this->_ptr;
        }
        constexpr pointer operator->() const noexcept{
            return this->_ptr;
        }
        constexpr reference operator[](const difference_type& n) noexcept{
            if(this->_idx == 0 && n < 0){
                return this->_buf->data()[this->_buf->max_size() - 1 - n];
            } else{
                return this->_buf->data()[(this->_idx + n) % this->_buf->max_size()];
            }
        }
        constexpr const reference operator[](const difference_type& n) const noexcept{
            if(this->_idx == 0 && n < 0){
                return this->_buf->data()[this->_buf->max_size() - 1 - n];
            } else{
                return this->_buf->data()[(this->_idx + n) % this->_buf->max_size()];
            }
        }
        
        constexpr bool operator==(const rit& other) const noexcept{
            if(this->_cnt == 0 && (other._ptr == this->_sntl)){
                return true;
            } else if((this->_idx == other._idx) && (this->_ptr == other._ptr)){
                return true;
            } else if(this->_ptr == this->_sntl && other._ptr == other._sntl){
                return true;
            } else{
                return false;
            }
        }
        constexpr bool operator!=(const rit& other) const noexcept{
            return !(*this == other);
        }
        constexpr bool operator< (const rit& other) const noexcept{
            if(other._ptr == other._sntl && this->_ptr != other._sntl){
                return true; // a pointer is always less than a sentinel
            } else if(this->_buf->_tail > this->_buf->_head){
                return true;
            } else if(this->_idx < other._idx){
                return true;
            } else{
                return false;
            }
        }
        constexpr bool operator> (const rit& other) const noexcept{
            return !(*this < other);
        }
        constexpr bool operator<=(const rit& other) const noexcept{
            return *this == other || *this < other ? true : false;
        }
        constexpr bool operator>=(const rit& other) const noexcept{
            return *this == other || *this > other ? true : false;
        }

        constexpr rit& operator--() noexcept{
            this->_idx = (this->_idx + 1) % this->_buf->max_size();
            this->_ptr = &this->_buf->data()[this->_idx];
            ++this->_cnt;
            return *this;
        }
        constexpr rit operator--(int) noexcept{
            auto temp = *this;
            this->_idx = (this->_idx + 1) % this->_buf->max_size();
            this->_ptr = &this->_buf->data()[this->_idx];
            ++this->_cnt;
            return temp;
        }
        constexpr rit& operator++() noexcept{
            this->_idx = this->_idx == 0 ? this->_buf->max_size() - 1 : this->_idx - 1;
            this->_ptr = &this->_buf->data()[this->_idx];
            --this->_cnt;
            return *this;
        }
        constexpr rit operator++(int) noexcept{
            auto temp = *this;
            this->_idx = this->_idx == 0 ? this->_buf->max_size() - 1 : this->_idx - 1;
            this->_ptr = &this->_buf->data()[this->_idx];
            --this->_cnt;
            return temp;
        }

        constexpr rit& operator-=(const difference_type& n) noexcept{
            this->_idx = (this->_idx + n) % this->_buf->max_size();
            this->_ptr = &this->_buf->data()[this->_idx];
            this->_cnt += n;
            return *this;
        }
        constexpr rit& operator+=(const difference_type& n) noexcept{
            this->_idx = this->_idx == 0 ? this->_buf->max_size() - 1 - n : this->_idx - n;
            this->_ptr = &this->_buf->data()[this->_idx];
            this->_cnt -= n;
            return *this;
        }
        constexpr friend rit operator+(const rit& pred, const difference_type& n) noexcept{
            auto temp = pred;
            temp._idx = pred._idx == 0 ? pred._buf->max_size() - 1 - n : pred._idx - n;
            if(pred._idx == 0 && pred._ptr == pred._sntl){temp._idx = pred._buf->max_size() - n;} // why?
            temp._ptr = &pred._buf->data()[temp._idx];
            temp._cnt = pred._cnt - n;
            return temp;
        }
        constexpr friend rit operator+(const difference_type& n, const rit& pred) noexcept{
            auto temp = pred;
            temp._idx = pred._idx == 0 ? pred._buf->max_size() - 1 - n : pred._idx - n;
            if(pred._idx == 0 && pred._ptr == pred._sntl){temp._idx = pred._buf->max_size() - n;} // why?
            temp._ptr = &pred._buf->data()[temp._idx];
            temp._cnt = pred._cnt - n;
            return temp;
        }
        constexpr friend rit operator-(const rit& pred, const difference_type& n) noexcept{
            auto temp = pred;
            temp._idx = (pred._idx + n) % pred._buf->max_size();
            temp._ptr = &pred._buf->data()[temp._idx];
            temp._cnt = pred._cnt + n;
            return temp;
        }
        constexpr friend difference_type operator-(const rit& lhs, const rit& rhs) noexcept{
            return abs(lhs._cnt - rhs._cnt);
        }
    };
public:
    using value_type = T;
    using allocator_type = allocator_traits<Allocator>::allocator_type;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = it<T, T&, T*, ring<T>*>;
    using const_iterator = it<T, const T&, const T*, const ring<T>*>;
    using reverse_iterator = rit<T, T&, T*, ring<T>*>;
    using const_reverse_iterator = rit<T, const T&, const T*, const ring<T>*>;
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
        return this->_size == this->_max_size;
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
    constexpr ring(): ring(Allocator()){}
    constexpr explicit ring(const Allocator& alloc) noexcept: _max_size(0), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(0)){}
    explicit ring(size_type count, const Allocator& alloc = Allocator()): _max_size(count), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(count)){
        for(size_type i = 0; i < count; ++i){
            construct_at(this->_buffer + i, value_type());
        }
    }
    constexpr explicit ring(size_type count, const_reference value, const Allocator& alloc = Allocator()): _max_size(count), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(count)){
        for(size_type i = 0; i < count; ++i){
            construct_at(this->_buffer + i, value);
            this->_incr();
        }
    }
    template<input_iterator InputIt> constexpr ring(InputIt first, InputIt last, const Allocator& alloc = Allocator()): _max_size(distance(first, last)), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(distance(first, last))){
        for(; first != last; ++first){
            construct_at(this->_buffer + this->_head, *first);
            this->_incr();
        }
    }
    template<ranges::input_range R> constexpr ring(from_range_t, R&& rg, const Allocator& alloc = Allocator()): _max_size(distance(rg.begin(), rg.end())), _head(0), _tail(0), _size(0), _alloc(alloc), _buffer(this->_alloc.allocate(distance(rg.begin(), rg.end()))){
        for(auto&& v: rg){
            construct_at(this->_buffer + this->_head, v);
            this->_incr();
        }
    }
    constexpr ring(const ring& other): _max_size(other._max_size), _head(other._head), _tail(other._tail), _size(other._size){
        /**
         * @todo
         * Make this keep the state of the copied ring. As of right now, the order is ruined during copy construction
         * This applies for all copy/move constructors, including operator=
         */
        this->_alloc = allocator_traits<Allocator>::select_on_container_copy_construction(other.get_allocator());
        this->_buffer = this->_alloc.allocate(this->_max_size);
        size_type i = 0;
        for(auto&& v : other){
            construct_at(this->_buffer + i, v);
            ++i;
        }
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
        if(this != other){
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
        }
        return *this;
    }
    constexpr ring& operator=(ring&& other) noexcept(allocator_traits<Allocator>::is_always_equal::value){
            if(this != other){
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
        }
        return *this;
    }
    constexpr ring& operator=(initializer_list<value_type> ilist){
        for(size_type i = 0; i < this->_max_size; ++i){
            destroy_at(this->_buffer + i);
        }
        this->_max_size = ilist.size();
        this->_head = 0;
        this->_tail = 0;
        this->_size = 0;
        this->_buffer = this->_alloc.allocate(this->_max_size);
        for(auto&& v : ilist){
            construct_at(this->_buffer + this->_head, v);
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

    constexpr iterator begin() noexcept{
        if(this->_full()){
            return iterator(this, &this->_buffer[this->_tail], this->_tail, this->_max_size);
        } else if(this->empty()){
            return iterator(this, &this->_buffer[0], this->_tail, 0);
        } else{
            return iterator(this, &this->_buffer[this->_tail], this->_tail, this->_size);
        }
    }
    constexpr const_iterator begin() const noexcept{
        if(this->_full()){
            return const_iterator(this, &this->_buffer[this->_tail], this->_tail, this->_max_size);
        } else if(this->empty()){
            return const_iterator(this, &this->_buffer[0], this->_tail, 0);
        } else{
            return const_iterator(this, &this->_buffer[this->_tail], this->_tail, this->_size);
        }
    }
    constexpr const_iterator cbegin() const noexcept{
        if(this->_full()){
            return const_iterator(this, &this->_buffer[this->_tail], this->_tail, this->_max_size);
        } else if(this->empty()){
            return const_iterator(this, &this->_buffer[0], this->_tail, 0);
        } else{
            return const_iterator(this, &this->_buffer[this->_tail], this->_tail, this->_size);
        }
    }

    constexpr iterator end() noexcept{
        if(this->_full() || this->empty()){
            return iterator(this, &this->_buffer[-1], this->_head, 0);
        } else{
            return iterator(this, &this->_buffer[this->_head], this->_head, 0);
        }
    }
    constexpr const_iterator end() const noexcept{
        if(this->_full() || this->empty()){
            return const_iterator(this, &this->_buffer[-1], this->_head, 0);
        } else{
            return const_iterator(this, &this->_buffer[this->_head], this->_head, 0);
        }
    }
    constexpr const_iterator cend() const noexcept{
        if(this->_full() || this->empty()){
            return const_iterator(this, &this->_buffer[-1], this->_head, 0);
        } else{
            return const_iterator(this, &this->_buffer[this->_head], this->_head, 0);
        }
    }

    constexpr reverse_iterator rbegin() noexcept{
        if(this->_full()){
            size_type loc = this->_head == 0 ? this->_max_size - 1 : this->_head - 1;
            return reverse_iterator(this, &this->_buffer[loc], loc, this->_max_size);
        } else if(this->empty()){
            return reverse_iterator(this, &this->_buffer[0], this->_head, 0);
        } else{
            size_type loc = this->_head == 0 ? this->_max_size - 1 : this->_head - 1;
            return reverse_iterator(this, &this->_buffer[loc], loc, this->_size);
        }
    }
    constexpr const_reverse_iterator rbegin() const noexcept{
        if(this->_full()){
            size_type loc = this->_head == 0 ? this->_max_size - 1 : this->_head - 1;
            return const_reverse_iterator(this, &this->_buffer[loc], loc, this->_max_size);
        } else if(this->empty()){
            return const_reverse_iterator(this, &this->_buffer[0], this->_head, 0);
        } else{
            size_type loc = this->_head == 0 ? this->_max_size - 1 : this->_head - 1;
            return const_reverse_iterator(this, &this->_buffer[loc], loc, this->_size);
        }
    }
    constexpr const_reverse_iterator crbegin() const noexcept{
        if(this->_full()){
            size_type loc = this->_head == 0 ? this->_max_size - 1 : this->_head - 1;
            return const_reverse_iterator(this, &this->_buffer[loc], loc, this->_max_size);
        } else if(this->empty()){
            return const_reverse_iterator(this, &this->_buffer[0], this->_head, 0);
        } else{
            size_type loc = this->_head == 0 ? this->_max_size - 1 : this->_head - 1;
            return const_reverse_iterator(this, &this->_buffer[loc], loc, this->_size);
        }
    }

    constexpr reverse_iterator rend() noexcept{
        size_type loc = this->_tail == 0 ? this->_max_size - 1 : this->_tail - 1;
        if(this->_full() || this->empty()){
            return reverse_iterator(this, &this->_buffer[-1], loc, 0);
        } else{
            return reverse_iterator(this, &this->_buffer[loc], loc, 0);
        }
    }
    constexpr const_reverse_iterator rend() const noexcept{
        size_type loc = this->_tail == 0 ? this->_max_size - 1 : this->_tail - 1;
        if(this->_full() || this->empty()){
            return const_reverse_iterator(this, &this->_buffer[-1], loc, 0);
        } else{
            return const_reverse_iterator(this, &this->_buffer[loc], loc, 0);
        }
    }
    constexpr const_reverse_iterator crend() const noexcept{
        size_type loc = this->_tail == 0 ? this->_max_size - 1 : this->_tail - 1;
        if(this->_full() || this->empty()){
            return const_reverse_iterator(this, &this->_buffer[-1], loc, 0);
        } else{
            return const_reverse_iterator(this, &this->_buffer[loc], loc, 0);
        }
    }

    constexpr bool empty() const{
        return this->_size == 0;
    }
    constexpr size_type size() const noexcept{
        return this->_size;
    }
    constexpr size_type max_size() const noexcept{
        return this->_max_size;
    }
    constexpr void shrink_to_fit(){
        /**
         * @todo
         * Implement. This function must shrink the ring to the current number of elements, and deallocate the now unnecessary memory.
         * If this->_full(), do nothing.
         */

    }

    constexpr void clear() noexcept{
        for(size_type i = 0; i < this->_max_size; ++i){
            destroy_at(this->_buffer + i);
        }
        this->_head = this->_tail = this->_size = 0;
    }    
    constexpr void push_back(const T& value){
        construct_at(this->_buffer + this->_head, value);
        this->_incr();
    }
    constexpr void push_back(T&& value) noexcept{
        construct_at(this->_buffer + this->_head, value);
        this->_incr();
    }
    template<class...Args> constexpr reference emplace_back(Args&&... args){
        /**
         * @todo
         * Implement. This should construct an element in place at the head of the ring, and increment. 
         * 
         */
        return value_type();
    }
    template<ranges::input_range R> constexpr void append_range(R&& rg){
        /**
         * @todo
         * Will append the compatible range rg onto the end of the array, beginning with and exceeding the _head. 
         * If this->_size + rg.size() is greater than the ring's maximum size, the ring will automatically expand to fit the range
         * THe resulting ring will satisfy this->full() == true. This makes sense because it is appending the range.
         * 
         */
    }
    constexpr void pop_back(){
        if(this->empty()) return;
        destroy_at(this->_buffer + this->_tail);
        --this->_size;
        this->_tail = (this->_tail + 1) % this->_max_size;
    }
    

    constexpr bool operator==(const ring& other) const noexcept{
        return equal(this->begin(), this->end(), other.begin(), other.end());
    }
    constexpr bool operator!=(const ring& other) const noexcept{
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& stream, ring& obj){
        for(size_t i = 0; i < obj.size(); ++i){
            stream<<obj[i];
            if(i != obj.size() - 1){
                stream<<", ";
            }
        }
        return stream;
    }
};
}
#endif