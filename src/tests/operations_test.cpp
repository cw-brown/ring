#include "ring.hpp"

#include <cassert>
#include <algorithm>

class ring_wrapper{
    friend std::ring<int>;
};


void iota_insertion(int end_point){
    std::ranges::iota_view<int, int> a = std::views::iota(1,end_point); 
    std::ring<int> b(std::from_range, a);
    assert(std::ranges::equal(a.begin(), a.end(), b.begin(), b.end()));
}

void reversal(){ 
    std::ring<int> a(5000, 0);
    std::ring<int> b(a);
    std::reverse(a.rbegin(), a.rend());
    assert(std::ranges::equal(a.begin(), a.end(), b.begin(), b.end())); 
    std::reverse(a.begin(), a.end());
    assert(std::ranges::equal(a.begin(), a.end(), b.rbegin(), b.rend()));
}

int error_catcher(int type){
    std::ring<int> a = {1, 2, 3, 4, 5};
    switch(type){
        case(0):
            try{
                a[-1];
            } catch(const std::out_of_range& e){
                return 1;
            } return 0; 
            break;
        case(1):
            try{
                a[0];
            } catch(const std::out_of_range& e){ // if we catch an error from basic access, theres definitely a problem
                return 0;
            } return 1; 
            break;
        case(2):
            try{
                a[200];
            } catch(const std::out_of_range& e){
                return 1;
            } return 0;
            break;
        case(3):
            try{
                a.at(-1);
            } catch(const std::out_of_range& e){
                return 1;
            } return 0;
            break;
        case(4):
            try{
                a.at(0);
            } catch(const std::out_of_range& e){
                return 0;
            } return 1;
            break;
        case(5):
            try{
                a.at(200);
            } catch(const std::out_of_range& e){
                return 1;
            } return 0;
            break;
        default:
            return 0;
            break;
    }
    return 0;
}

int main(){
    // Insertion of a small iota
    iota_insertion(50);

    // Insertion of a very large iota
    iota_insertion(100000);

    // Reversal and comparison to reverse
    reversal();

    // // Catching proper errors
    assert(error_catcher(0));
    assert(error_catcher(1));
    assert(error_catcher(2));
    assert(error_catcher(3));
    assert(error_catcher(4));
    assert(error_catcher(5));




    return 0;
}