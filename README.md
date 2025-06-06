# ring
*std::ring* is a C++ standard library style container implementing a circular storage container in contiguous memory. It permits fast insertion at the its beginning and fast deletion at its end.
Effectively, *std::ring* maintains a fixed size array that is a size up to the max size of the container, and does bookkeeping to track the beginning and end of this sub-array.
The container uses contiguous iterators and therefore complies with C++20 ranges. 

*std::ring* meets the requirements of *Container*, *AllocatorAwareContainer*, *ContiguousContainer*, and *ReversibleContainer*. 

## Usage Nuances
*std::ring* is a fixed-size container, and so any operations that cause the container to be resized (copy construction/assignment, assign, ...) will automatically delete and deallocate 
its memory. 

## Iterator Invalidation
**Operations** | **Invalidated**
--- | ---
All read only operations | Never.
swap, std::swap | Always.
clear, operator=, assign | Always.
shrink_to_fit | If ring changes capacity, all of them. If not, none.
erase, clear | Always.
push_back, emplace_back | If the ring is full, all of them. If not, only end().
append_range | end().
resize | If the ring changed size, all of them. If not, none.
pop_front | The element erased, begin(), and rend().