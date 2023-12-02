
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
const int avoidfactor{1}; //inizializzazioni provvisorie
const int d_sep{220};
const int d_ca{220};
const int alignfactor{1};
const int centeringfactor{1};
const int turnfactor{15};
const int topmargin{200};
const int bottommargin{0};
const int leftmargin{0};
const int rightmargin{200};
const int maxspeed{5};
const int minspeed{1};
const int numboid{10}; 
const int vector_x{1};
const int vector_y{2};
const int vector_vx{3};
const int vector_vy{4};
const int max_iteration{25};

class boid{
    public:
    boid();
        boid(int idboid);
        void separation (); 
        void alignment(); 
        void coesion(); 
        //void update_position(); 

    private:
        int id_boid;
        int myx{0};
        int myy{0};
        int myvx{0};
        int myvy{0}; 
        //inizializzo a zero queste variabili in quanto poi dovrò leggerle 

        int save_int_from_vector(int index, int chose_vector); 
        void write_int_to_vector(int index, int chose_vector, int data);
        //codice dei vettori: 1-vettore x, 2-vettore y, 3-vettore vx, 4-vettore vy 
        //void regulate_border(); 
        //void limit_speed(); 
        int compute_distance(int otherx, int othery);
    
};
//void print_limits_to_file();
//void print_to_file();
#endif