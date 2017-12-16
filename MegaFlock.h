//
// Created by Np on 2017-12-01.
//

#ifndef BOIDS_ALGORITHM_OPENCL_MEGAFLOCK_H
#define BOIDS_ALGORITHM_OPENCL_MEGAFLOCK_H


#include "Flock.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <OpenCL/opencl.h>
class MegaFlock {
    std::vector <Flock*> allFlock;
    std::vector <Boid*> allBoid;


public:
    virtual ~MegaFlock();

    const std::vector<Flock *> &getAllFlock() const;

    void setAllFlock(const std::vector<Flock *> &allFlock);

    MegaFlock(const std::vector<Flock *> &allFlock);

    const std::vector<Boid *> &getAllBoid() const;

    void setAllBoid(const std::vector<Boid *> &allBoid);

    void addBoid();

    void doOpenCL();
};


#endif //BOIDS_ALGORITHM_OPENCL_MEGAFLOCK_H
