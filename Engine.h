//
// Created by Np on 2017-11-19.
//

#ifndef BOIDS_ALGORITHM_OPENCL_ENGINE_H
#define BOIDS_ALGORITHM_OPENCL_ENGINE_H
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include "Flock.h"
#include "Draw.h"
#include "FlockPool.h"
#include "MegaFlock.h"
#include <tbb/task_group.h>


class Engine {
public:
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    // static float getDT();


    Engine(const std::vector<Flock *> &flock, int i);

    ~Engine();

    bool initialize(char* windowTitle);

    void update();
    void render(MegaFlock* megaFlock);

    static double limitFPS ;

    double lastTime = glfwGetTime(), timer = lastTime;
    double changeInTime = 0, currentTime = 0;
    int frames = 0 , updates = 0;

    // bool init = true;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static GLFWwindow *getWindow();
    static void setWindow(GLFWwindow *window);

public:
    static GLFWwindow* window;
    std::vector<Flock*> flock;
    FlockPool flockPool;



};


#endif //BOIDS_ALGORITHM_OPENCL_ENGINE_H
