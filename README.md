# ring
std::ring is a C++ standard library style container implementing a circular storage container in contiguous memory. It permits fast insertion at the its beginning and fast deletion at its end.
Effectively, std::ring maintains a fixed sized array that is a size up to the max size of the container, and does book keeping to track the beginning and end of this new sub-array.
The container is compatible with contiguous iterators and therefore complies with C++26 ranges. 

std::ring meets the requirements of *Container*, *AllocatorAwareContainer*, *ContiguousContainer*, and *ReversibleContainer*. 
