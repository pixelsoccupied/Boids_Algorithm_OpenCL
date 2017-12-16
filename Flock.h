//
// Created by Np on 2017-11-19.
//

#ifndef BOIDS_ALGORITHM_OPENCL_FLOCK_H
#define BOIDS_ALGORITHM_OPENCL_FLOCK_H

#include <tbb/task.h>


#include <vector>
#include "Boid.h"
#include <Eigen/Dense>
#include <thread>
#include <OpenCL/opencl.h>


class OpenCLBoid;



class Flock {
private:
    std::vector <Boid*> flock;
    static float rgb;
    Eigen::Vector2f averagePositionOfFlock;

    static int IDgen;
    int ID;

    OpenCLBoid* openCLBoid;
    int sizeOfArray;
//vectors for openCL fucntion
    std::vector<float > arrayVelocityX;
    std::vector<float > arrayVelocityY;

    std::vector<float > arrayPositionX;
    std::vector<float > arrayPositionY;



    std::vector<float > cohesionX;
    std::vector<float > cohesionY;

    std::vector<float > separationX;
    std::vector<float > separationY;

    std::vector<float > alignmentX;
    std::vector<float > alignmentY;

    std::vector<float > oldVelocityX;
    std::vector<float > oldVelocityY;







    std::mutex mtx;












public:

    Flock(int numOfBoids);

    virtual ~Flock();

    const std::vector<Boid *> &getFlock() const;

    void setFlock(const std::vector<Boid *> &flock);

    void randomisePosition();


    void updateBoid();

    Eigen::Vector2f cohesion(Boid *pBoid);

    Eigen::Vector2f separation(Boid *pBoid);

    Eigen::Vector2f alignment(Boid *boid);

    //supporting methods
    void limitSpeed(Boid *boid);
    Eigen::Vector2f boundPosition(Boid *boid);


    const Eigen::Vector2f &getAveragePositionOfFlock() const;



    int getSizeOfArray() const;


};


#endif //BOIDS_ALGORITHM_OPENCL_FLOCK_H
