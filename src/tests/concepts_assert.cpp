#include "ring.hpp"

int main(){
    // Iterator assertion
    static_assert(std::contiguous_iterator<std::ring<int>::iterator>);
    static_assert(std::contiguous_iterator<std::ring<int>::const_iterator>);
    static_assert(std::contiguous_iterator<std::ring<int>::reverse_iterator>);
    static_assert(std::contiguous_iterator<std::ring<int>::const_reverse_iterator>);
    
    // Range assertion
    static_assert(std::ranges::sized_range<std::ring<int>>);
    static_assert(std::ranges::contiguous_range<std::ring<int>>);
    static_assert(std::ranges::common_range<std::ring<int>>);
    return 0;
}