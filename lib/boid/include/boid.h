
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
const int numboid{50}; 
const float avoidfactor{10}; //inizializzazioni provvisorie
const float topmargin{200};
const int inttopmargin{200};
const float bottommargin{0};
const float leftmargin{0};
const int intrightmargin{200};
const float rightmargin{200};
const float d_sep{100};
const float d_ca{20};
const float alignfactor{0.1}; 
const float centeringfactor{0.01};
const float turnfactor{6};
const float maxspeed{5};
const int intmaxspeed{5};
const float minspeed{1};
const int intminspeed{1};
const int vector_x{1};
const int vector_y{2};
const int vector_vx{3};
const int vector_vy{4};
const int max_iteration{200};
mutex Mutex;
condition_variable readytowrite;

class boid{
    public:
    boid();
        boid(const int idboid);
        void separation (); 
        void alignment(); 
        void coesion(); 
        void update_position(); 
        void save_boid();

    private:
        int id_boid{0};
        float myx{0};
        float myy{0};
        float myvx{0};
        float myvy{0}; 
        //inizializzo a zero queste variabili in quanto poi dovrò leggerle 

        float save_float_from_vector(const int index, const int chose_vector); 
        void write_float_to_vector(const int index, const int chose_vector, const float data);
        //codice dei vettori: 1-vettore x, 2-vettore y, 3-vettore vx, 4-vettore vy 
        void regulate_border(); 
        void limit_speed(); 
        float compute_distance(const float otherx, const float othery);
        float find_average(const float maxvalue, const float divider);
};
void print_limits_to_file();
void print_to_file();
void create_random_vectors();

#endif