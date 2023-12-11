
#include<cstdlib>
#include<ctime>
using std::rand;
using std::srand;
#include <mutex>
using std::mutex;
using std::unique_lock;
#include "boid.h"

const int id_default{999};
const int myx_default{5};
const int myy_default{5};
const int myvx_default{2};
const int myvy_default{2};
vector <int> xpos(numboid, 1);
vector <int> ypos(numboid, 2);
vector <int> xspeed(numboid, 4);
vector <int> yspeed(numboid, 6);
vector <int> auxxpos(numboid, 1);
vector <int> auxypos(numboid, 2);
vector <int> auxxspeed(numboid, 4);
vector <int> auxyspeed(numboid, 6);
const long int constructorreads{4};
const long int separationreads{2*numboid};
const long int alignmentsreads{4*numboid};
long int no_of_writes{0};
long int no_of_reads{numboid * (constructorreads + separationreads + alignmentsreads + separationreads)};
//separationreads == coesionreads
int this_read {1};

    boid::boid()
        :id_boid{id_default},
        myx{myx_default},
        myy{myy_default},
        myvx{myvx_default},
        myvy{myvy_default}
    {} //il costruttore di default non verrà utilizzato

    boid::boid(const int idboid)
        :id_boid{idboid}
    {
        myx = save_int_from_vector(id_boid, vector_x);
        myy = save_int_from_vector(id_boid, vector_y);
        myvx = save_int_from_vector(id_boid, vector_vx);
        myvy = save_int_from_vector(id_boid, vector_vy);
    }

    int boid::compute_distance(const int otherx, const int othery){
        int distance{0};
        distance = sqrt(((myx-otherx)^2)+((myy-othery)^2));
        return distance;
    }

    void boid::separation(){
        int close_dx{0};
        int close_dy{0};
        int otherx{0};
        int othery{0};
        int distance{0};
        for(int i{0}; i < numboid; i++){
            otherx = save_int_from_vector(i, vector_x);
            othery = save_int_from_vector(i, vector_y);
            distance = compute_distance(otherx, othery);
            if (distance <= d_sep && id_boid != i){
            close_dx += myx - otherx;
            close_dy += myy - othery;
            }
        }
        myvx = close_dx * avoidfactor;
        myvy = close_dy * avoidfactor;
    }

    void boid::alignment(){
        int xvel_avg{0};
        int yvel_avg{0};
        int neighboring_boids{0};
        int othervx{0};
        int othervy{0};
        int otherx{0};
        int othery{0};
        int distance{0};
        for(int i{0}; i < numboid; i++){
            othervx = save_int_from_vector(i, vector_vx);
            othervy = save_int_from_vector(i, vector_vy);
            otherx = save_int_from_vector(i, vector_x);
            othery = save_int_from_vector(i, vector_y);
            distance = compute_distance(otherx, othery);
            if (distance <= d_ca && id_boid != i){
                xvel_avg += othervx;
                yvel_avg += othervy;
                neighboring_boids++;
            }
        }
        if (neighboring_boids > 0){
            xvel_avg = xvel_avg / neighboring_boids;
            yvel_avg = yvel_avg / neighboring_boids;
            myvx += (xvel_avg - myvx) * alignfactor;
            myvy += (yvel_avg - myvy) * alignfactor;
        }
    }

    void boid::coesion(){
        int xpos_avg{0};
        int ypos_avg{0};
        int neighboring_boids{0};
        int otherx{0};
        int othery{0};
        int distance{0};
        for(int i{0}; i < numboid; i++){
            otherx = save_int_from_vector(i, vector_x);
            othery = save_int_from_vector(i, vector_y);
            distance = compute_distance(otherx, othery);
            if (distance <= d_ca && id_boid != i){
                xpos_avg += otherx;
                ypos_avg += othery;
                neighboring_boids++;
            }
        }
        if (neighboring_boids > 0){
            //xpos_avg = xpos_avg / neighboring_boids;
            //ypos_avg = ypos_avg / neighboring_boids;
            myvx += (((xpos_avg - myx)) / (dencenteringfactor * neighboring_boids)) * numcenteringfactor;
            myvy += (((xpos_avg - myx)) / (dencenteringfactor * neighboring_boids)) * numcenteringfactor;
        }
    }

    void boid::regulate_border(){
        if (myx < leftmargin){
            myvx = myvx + turnfactor;
        }
        if (myx > rightmargin){
            myvx = myvx - turnfactor;
        }
        if (myy < bottommargin){
            myvy = myvy + turnfactor;
        }
        if (myy > topmargin){
            myvy = myvy - turnfactor;
        }
    }

    void boid::limit_speed(){
        int speed{0};
        speed = sqrt(myvx * myvx + myvy * myvy);
        if (speed > maxspeed){
            myvx = (myvx * maxspeed) / speed;
            myvy = (myvy * maxspeed) / speed;
        }
        if (speed < minspeed){
            myvx = (myvx * minspeed) / speed;
            myvy = (myvy * minspeed) / speed;
        }

    }

    void boid::update_position(){
        limit_speed();
        regulate_border();
        limit_speed();
        myx = myx + myvx;
        myy = myy + myvy;
        write_int_to_vector(id_boid, vector_x, myx);
        write_int_to_vector(id_boid, vector_y, myy);
        write_int_to_vector(id_boid, vector_vx, myvx);
        write_int_to_vector(id_boid, vector_vy, myvy);
        cout << myx << " "<< myy << " " << myvx << " " << myvy << " boid no: "<< id_boid << endl;
    }

    int boid::save_int_from_vector(const int index, const int chose_vector){
        unique_lock<mutex> mlock(Mutex);
        int value{0};
        no_of_reads--;
        switch (chose_vector){
            case vector_x:
            value = xpos.at(index);
            break;
            case vector_y:
            value = ypos.at(index);
            break;
            case vector_vx:
            value = xspeed.at(index);
            break;
            case vector_vy:
            value = yspeed.at(index);
            break;
            default:
            cerr << "You are asking for a value that does not exist!" << endl;
            exit(EXIT_FAILURE);
        }
        if (no_of_reads == 1){
            no_of_writes = 4 * numboid;
            mlock.unlock();
            readytowrite.notify_one();
        }
        return value;
    }

    void boid::write_int_to_vector(const int index, const int chose_vector, const int data){
        unique_lock<mutex> mlock(Mutex);
        while (no_of_writes == 0 || no_of_reads > 0){
            readytowrite.wait(mlock);
        }
        switch (chose_vector){
            case vector_x:
            xpos.at(index) = data;
            break;
            case vector_y:
            ypos.at(index) = data;
            break;
            case vector_vx:
            xspeed.at(index) = data;
            break;
            case vector_vy:
            yspeed.at(index) = data;
            break;
            default:
            cerr << "You are asking for a value that does not exist!" << endl;
            exit(EXIT_FAILURE);
        }
        --no_of_writes;
        if(no_of_writes == 0){
            no_of_reads = numboid * (constructorreads + separationreads + alignmentsreads + separationreads);
        }
        mlock.unlock();
        readytowrite.notify_one();
    }

void print_to_file(){
    ofstream fout("coordinate.txt", ios::app);
    for(int i{0}; i < numboid; ++i){
        fout << xpos.at(i) << " " << ypos.at(i) << " ";  
    }
    fout << endl;
    fout.close();
}

void print_limits_to_file(){
    ofstream fout("coordinate.txt");
    fout << leftmargin << " " << rightmargin << " " << bottommargin << " " << topmargin << endl;
    fout.close();
}

void create_random_vectors(){
    srand(std::time(0));
    for (int i = 0; i < numboid; i++){
        int xx = (rand() % (topmargin + 1));
        int yy = (rand() % (topmargin + 1));
        xpos[i] = xx;
        ypos[i] = yy;
        int vxx = (rand() % (maxspeed + 1));
        int vyy = (rand() % (maxspeed + 1));
        xspeed[i] = vxx;
        yspeed[i]= vyy;
        cout << xx << " "<< yy << " " << vxx << " " << vyy << " random no: "<< i << endl;
    }
}









