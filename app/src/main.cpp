#include"boid.h"
#include <thread>
using std::thread;
boid boid_;
void think(int id_boid){
    boid_.save_boid(id_boid);
    boid_.separation(id_boid);
    boid_.alignment(id_boid);
    boid_.coesion(id_boid);
}

void move(int id_boid){
    boid_.update_position(id_boid);
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
    for (int j{0}; j < numboid; ++j){
        position.push_back(thread{move, j});
    }
    for (movers_it = position.begin(); movers_it != position.end(); ++movers_it)
	    movers_it->join();
	for (boids_it=boids.begin(); boids_it != boids.end(); ++boids_it)
		boids_it->join();
    print_to_file();
    cout << endl;
    }   
    cout << "fine iterazioni" << endl;
    return 0;
}