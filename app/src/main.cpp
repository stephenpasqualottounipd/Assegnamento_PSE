#include"boid.h"
#include <thread>
using std::thread;
void think(int id_boid){
    boid boid_(id_boid);
    boid_.separation();
    boid_.alignment();
    boid_.coesion();
    boid_.update_position();
}

int main(){
    print_limits_to_file();
    create_random_vectors();
    for(int i{0}; i < max_iteration; ++i){
    vector<thread> boids;
    vector<thread> position;
    vector<thread>::iterator movers_it;
    vector<thread>::iterator boids_it;
    for (int j{0}; j < numboid; ++j){
        boids.push_back(thread{think, j});
    }
	for (boids_it=boids.begin(); boids_it != boids.end(); ++boids_it)
		boids_it->join();
    print_to_file();
    }   
    cout << "fine iterazioni" << endl;
    return 0;
}