
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
const int numboid{10}; 
const int avoidfactor{1}; //inizializzazioni provvisorie
const int topmargin{200};
const int bottommargin{0};
const int leftmargin{0};
const int rightmargin{200};
const int d_sep{100};
const int d_ca{(topmargin + rightmargin)/(numboid)};
const int alignfactor{1}; //max a 1
const int numcenteringfactor{1};
const int dencenteringfactor{10}; //deve essere inferiore a 1
const int turnfactor{6};
const int maxspeed{5};
const int minspeed{1};
const int vector_x{1};
const int vector_y{2};
const int vector_vx{3};
const int vector_vy{4};
const int vector2_x{5};
const int vector2_y{6};
const int vector2_vx{7};
const int vector2_vy{8};
const int max_iteration{50};
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
        int myx{0};
        int myy{0};
        int myvx{0};
        int myvy{0}; 
        //inizializzo a zero queste variabili in quanto poi dovrò leggerle 

        int save_int_from_vector(const int index, const int chose_vector); 
        void write_int_to_vector(const int index, const int chose_vector, const int data);
        //codice dei vettori: 1-vettore x, 2-vettore y, 3-vettore vx, 4-vettore vy 
        void regulate_border(); 
        void limit_speed(); 
        int compute_distance(const int otherx, const int othery);
};
void print_limits_to_file();
void print_to_file();
void create_random_vectors();

#endif