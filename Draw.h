//
// Created by Np on 2017-11-19.
//

#ifndef BOIDS_ALGORITHM_OPENCL_DRAW_H
#define BOIDS_ALGORITHM_OPENCL_DRAW_H
#include <vector>
#include "Flock.h"
#include "Boid.h"
#include "MegaFlock.h"
#include <GLFW/glfw3.h>

class Draw {
public:
    static bool boidLeft;
    static bool boidDown;
    static void drawFlocks(MegaFlock *flock);
    static Eigen::Vector2f moveAway(Boid *pBoid, Flock *&pFlock);




    static std::mutex mtx;


};


#endif //BOIDS_ALGORITHM_OPENCL_DRAW_H
