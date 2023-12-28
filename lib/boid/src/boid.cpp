
#include<cstdlib>
#include<ctime>
using std::rand;
using std::srand;
#include <mutex>
using std::mutex;
using std::unique_lock;
#include<iomanip>
#include "boid.h"

const int id_default{999};
const float myx_default{5};
const float myy_default{5};
const float myvx_default{2};
const float myvy_default{2};
vector <float> xpos(numboid, 1);
vector <float> ypos(numboid, 2);
vector <float> xspeed(numboid, 4);
vector <float> yspeed(numboid, 6);
const long int constructorreads{4};
const long int separationreads{2*numboid};
const long int alignmentsreads{4*numboid};
long int no_of_writes{0};
long int no_of_reads{numboid * (constructorreads + separationreads + alignmentsreads + separationreads)};
//separationreads == coesionreads

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
        myx = save_float_from_vector(id_boid, vector_x);
        myy = save_float_from_vector(id_boid, vector_y);
        myvx = save_float_from_vector(id_boid, vector_vx);
        myvy = save_float_from_vector(id_boid, vector_vy);
    }

    float boid::compute_distance(const float otherx, const float othery){
        float distance{0};
        distance = sqrt((myx-otherx)*(myx-otherx))+((myy-othery)*(myy-othery));
        return distance;
    }

    void boid::separation(){
        float close_dx{0};
        float close_dy{0};
        float otherx{0};
        float othery{0};
        float distance{0};
        for(int i{0}; i < numboid; i++){
            otherx = save_float_from_vector(i, vector_x);
            othery = save_float_from_vector(i, vector_y);
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
        float xvel_avg{0};
        float yvel_avg{0};
        float neighboring_boids{0};
        float othervx{0};
        float othervy{0};
        float otherx{0};
        float othery{0};
        float distance{0};
        for(int i{0}; i < numboid; i++){
            othervx = save_float_from_vector(i, vector_vx);
            othervy = save_float_from_vector(i, vector_vy);
            otherx = save_float_from_vector(i, vector_x);
            othery = save_float_from_vector(i, vector_y);
            distance = compute_distance(otherx, othery);
            if (distance <= d_ca && id_boid != i){
                xvel_avg += othervx;
                yvel_avg += othervy;
                neighboring_boids++;
            }
        }
        if (neighboring_boids > 0){
            xvel_avg = find_average(xvel_avg, neighboring_boids);
            yvel_avg = find_average(yvel_avg, neighboring_boids);
            myvx += ((xvel_avg) - myvx) * alignfactor;
            myvy += ((yvel_avg) - myvy) * alignfactor;
        }
    }

    void boid::coesion(){
        float xpos_avg{0};
        float ypos_avg{0};
        float neighboring_boids{0};
        float otherx{0};
        float othery{0};
        float distance{0};
        for(int i{0}; i < numboid; i++){
            otherx = save_float_from_vector(i, vector_x);
            othery = save_float_from_vector(i, vector_y);
            distance = compute_distance(otherx, othery);
            if (distance <= d_ca && id_boid != i){
                neighboring_boids++;
                xpos_avg += otherx;
                ypos_avg += othery;
            }
        }
        if (neighboring_boids > 0){
            xpos_avg = find_average(xpos_avg, neighboring_boids);
            ypos_avg = find_average(ypos_avg, neighboring_boids);
            myvx += ((xpos_avg) - myx) * centeringfactor;
            myvy += ((ypos_avg) - myy) * centeringfactor;
        }
    }

    void boid::regulate_border(){
        if (myx < (leftmargin+maxspeed)){
            myvx = myvx + turnfactor;
        }
        if (myx > (rightmargin-maxspeed)){
            myvx = myvx - turnfactor;
        }
        if (myy < (bottommargin+maxspeed)){
            myvy = myvy + turnfactor;
        }
        if (myy > (topmargin-maxspeed)){
            myvy = myvy - turnfactor;
        }
    }

    void boid::limit_speed(){
        float speed{0};
        speed = sqrt(myvx * myvx + myvy * myvy);
        if (speed > maxspeed){
            myvx = (myvx / speed) * maxspeed;
            myvy = (myvy / speed) * maxspeed;
        }
        if (speed < minspeed){
            if (myvx != 0.00){
                myvx = (myvx / speed) * minspeed;
            }
            else{
                myvx = minspeed;
            }
            if (myvy != 0.00){
                myvy = (myvy / speed) * minspeed;
            }
            else{
                myvy = minspeed;
            }
        }

    }

    void boid::update_position(){
        limit_speed();
        regulate_border();
        limit_speed();
        myx = myx + myvx;
        myy = myy + myvy;
        write_float_to_vector(id_boid, vector_x, myx);
        write_float_to_vector(id_boid, vector_y, myy);
        write_float_to_vector(id_boid, vector_vx, myvx);
        write_float_to_vector(id_boid, vector_vy, myvy);
    }

    float boid::save_float_from_vector(const int index, const int chose_vector){
        unique_lock<mutex> mlock(Mutex);
        float value{0};
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
        if (no_of_reads == 0){
            no_of_writes = 4 * numboid;
            mlock.unlock();
            readytowrite.notify_one();
        }
        return value;
    }

    void boid::write_float_to_vector(const int index, const int chose_vector, const float data){
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
    ofstream fout("coordinates.txt", ios::app);
    fout << std::fixed << std::setprecision(1);
    for(int i{0}; i < numboid; ++i){
        fout  << xpos.at(i) << " " << ypos.at(i) << " ";  
    }
    fout << endl;
    fout.close();
}

void print_limits_to_file(){
    ofstream fout("coordinates.txt");
    fout << leftmargin << " " << rightmargin << " " << bottommargin << " " << topmargin << endl;
    fout.close();
}

void create_random_vectors(){
    srand(std::time(0));
    for (int i = 0; i < numboid; i++){
        float xx = static_cast<float>(rand() % (inttopmargin + 1));
        float yy = static_cast<float>(rand() % (intrightmargin + 1));
        xpos[i] = xx;
        ypos[i] = yy;
        float vxx = minspeed + static_cast<float>((rand() % ((intmaxspeed - intminspeed))));
        float vyy = minspeed + static_cast<float>((rand() % ((intmaxspeed - intminspeed))));
        xspeed[i] = vxx;
        yspeed[i]= vyy;
    }
}

float boid::find_average(const float maxvalue, const float divider){
    float average{0};
    average = maxvalue / divider;
    return average;
}








