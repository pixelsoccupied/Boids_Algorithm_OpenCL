//
// Created by Np on 2017-11-19.
//

#ifndef BOIDS_ALGORITHM_OPENCL_BOID_H
#define BOIDS_ALGORITHM_OPENCL_BOID_H
#include <Eigen/Dense>
#include <OpenCL/opencl.h>


class Boid {
    //using namespace Eigen;
private:
    float r = 0.1;
    float g = 0.3;
    float b = 0.6;
    float x;
    float y;
    Eigen::Vector2f position;
    Eigen::Vector2f velocity;
    bool boidLeft = true;
    bool boidDown = true;
    float speed;
    static int boidID;
    int iD;


    // Device input buffers
    cl_mem d_a;
    cl_mem d_b;
    // Device output buffer
    cl_mem d_c;

    cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel







    Eigen::Vector2f futurePosition;
public:
    const Eigen::Vector2f &getFuturePosition() const;

    void setFuturePosition(const Eigen::Vector2f &futurePosition);

    const Eigen::Vector2f &getVelocity() const;

    void setVelocity(const Eigen::Vector2f &velocity);



public:
    Boid();

    Boid(float r, float g, float b);

    virtual ~Boid();

    float getSpeed() const;

    void setSpeed(float speed);

    void drawBoid();

    const Eigen::Vector2f &getPosition() const;

    void setPosition(const Eigen::Vector2f &position);

    //functor operator overloading

    void operator()();

    float getR() const;

    float getG() const;

    float getB() const;


    std::vector<float> openclPositionToRender();
};


#endif //BOIDS_ALGORITHM_OPENCL_BOID_H
