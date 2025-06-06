#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    static_assert(std::contiguous_iterator<std::ring<float>::const_reverse_iterator>);

    std::ring<int> a = {1,2,3, 5, 90, 87};
    a.push_back(12);
    a.push_back(13);
    std::ring<int> b(a.crbegin() + 1, a.crend());

    for(auto&& v : a)
        std::cout<<v<<", ";
    std::cout<<std::endl;

    for(auto&& v : b)
        std::cout<<v<<", ";
    
    return 0;
}