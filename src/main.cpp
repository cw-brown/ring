#include <iostream>
#include <vector>

#include "ring.hpp"


int main(){
    static_assert(std::contiguous_iterator<std::ring<int>::const_iterator>);
    std::ring<float> a = {1,2,3,4,5, 300};
    std::vector<float> b(a.begin(), a.end()); 
    for(auto&& v : b)
        std::cout<<v<<", ";
    return 0;
}