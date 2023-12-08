
#ifndef boid_h
#define boid_h
#include<vector>
using std::vector;
#include<cmath>
using std::sqrt;
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include<fstream>
using std::ofstream;
using std::fstream;
using std::ios;
using std::is_empty;
#include <mutex>
using std::mutex;
#include <condition_variable>
using std::condition_variable;
const int avoidfactor{1}; //inizializzazioni provvisorie
const int d_sep{300};
const int d_ca{300};
const int alignfactor{1};
const int centeringfactor{1};
const int turnfactor{6};
const int topmargin{200};
const int bottommargin{0};
const int leftmargin{0};
const int rightmargin{200};
const int maxspeed{5};
const int minspeed{1};
const int numboid{5}; 
const int vector_x{1};
const int vector_y{2};
const int vector_vx{3};
const int vector_vy{4};
const int max_iteration{2};

class boid{
    public:
    boid();
        //boid(int id_boid);
        void separation (int id_boid); 
        void alignment(int id_boid); 
        void coesion(int id_boid); 
        void update_position(int id_boid); 
        void save_boid(int id_boid);

    private:
        //int id_boid{};
        int myx{0};
        int myy{0};
        int myvx{0};
        int myvy{0}; 
        //inizializzo a zero queste variabili in quanto poi dovrò leggerle 

        int save_int_from_vector(int index, int chose_vector); 
        void write_int_to_vector(int index, int chose_vector, int data);
        //codice dei vettori: 1-vettore x, 2-vettore y, 3-vettore vx, 4-vettore vy 
        void regulate_border(); 
        void limit_speed(); 
        int compute_distance(int otherx, int othery);
        mutex Mutex;
        //std::condition_variable readytoread;
        condition_variable readytowrite;
};
void print_limits_to_file();
void print_to_file();
void create_random_vectors();

#endif