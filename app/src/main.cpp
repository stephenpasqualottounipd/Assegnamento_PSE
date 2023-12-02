#include"boid.h"

int main(){
    boid boid0{0};
    boid boid1{1};
    boid boid2{2};
    boid boid3{3};
    boid boid4{4};
    boid boid5{5};
    boid boid6{6};
    boid boid7{7};
    boid boid8{8};
    boid boid9{9};
    print_limits_to_file();
    for(int i{0}; i < max_iteration; ++i){
    boid0.separation();
    boid0.alignment();
    boid0.coesion();
    boid1.separation();
    boid1.alignment();
    boid1.coesion();
    boid2.separation();
    boid2.alignment();
    boid2.coesion();
    boid3.separation();
    boid3.alignment();
    boid3.coesion();
    boid4.separation();
    boid4.alignment();
    boid4.coesion();
    boid5.separation();
    boid5.alignment();
    boid5.separation();
    boid6.separation();
    boid6.alignment();
    boid6.separation();
    boid7.separation();
    boid7.alignment();
    boid7.separation();
    boid8.separation();
    boid8.alignment();
    boid8.separation();
    boid9.separation();
    boid9.alignment();
    boid9.separation();
    boid0.update_position();
    boid1.update_position();
    boid2.update_position();
    boid3.update_position();
    boid4.update_position();
    boid5.update_position();
    boid6.update_position();
    boid7.update_position();
    boid8.update_position();
    boid9.update_position();
    cout << endl;
    print_to_file();
    }
}