//
// Created by Np on 2017-11-19.
//

#include "Boid.h"
#include <random>
#include <GLFW/glfw3.h>
#include <iostream>


//Seed and intialize only once
std::random_device rd;
std::mt19937 e2(rd());


int Boid::boidID = 0;



Boid::Boid() {

    std::cout << "Boid class constructor" << std::endl;

    boidID++;


    iD = boidID;


    std::uniform_real_distribution<> xCordinate(-0.2, 0.2);
    std::uniform_real_distribution<> yCordinate(-0.2, 0.2);

    position(0) =xCordinate(e2);
    position(1) =yCordinate(e2);


    x =  position(0);
    y = position(1);



}

Boid::Boid(float r, float g, float b) : r(r), g(g), b(b) {

    this->r = static_cast<float>(fmod(this->r, 1));
    this->g = static_cast<float>(fmod(this->g, 1));
    this->b = static_cast<float>(fmod(this->b, 1));

    boidID++;


    iD = boidID;





    //randomise position

    std::uniform_real_distribution<> xCordinate(-1, 1);
    std::uniform_real_distribution<> yCordinate(-1, 1);


    velocity(0)=xCordinate(e2);
    velocity(1)=yCordinate(e2);


    //std::uniform_real_distribution<> speedVal(0.1, 0.03);


    position(0) =xCordinate(e2);
    position(1) =yCordinate(e2);

    futurePosition(0) = position(0);
    futurePosition(1) = position(1);


    x =  position(0);
    y = position(1);


}



Boid::~Boid() {


    std::cout << "boid's dead " << std::endl;

}


float Boid::getSpeed() const {
    return speed;
}

void Boid::setSpeed(float speed) {
    Boid::speed = speed;
}

//deprecated function
void Boid::drawBoid() {


    Eigen::Vector2f temp;
    temp(0) = x;
    temp(1) = y;

    //std::cout<< velocity << "velocity! "<< std::endl;

    //move left n right
    if (boidLeft) {
        if (x > (-1 + 0.01)){
            x -= velocity.norm();
        }
        else {
            boidLeft = false;
        }
    }
    else {
        if (x < (1-0.01)) {
            x += velocity.norm();
        }
        else {
            boidLeft = true;
        }
    }

    //move up n down
    if (boidDown) {
        if (y > (-1+0.01)) {
            y -= velocity.norm();
        }
        else {
            boidDown = false;
        }
    }
    else {
        if (y < (1 - 0.01)) {
            y += velocity.norm();
        }
        else {
            boidDown = true;
        }
    }
    position(0) = x;
    position(1) = y;

/*
    dot = x1*x2 + y1*y2      # dot product between [x1, y1] and [x2, y2]
    det = x1*y2 - y1*x2      # determinant
    angle = atan2(det, dot)  # atan2(y, x) or atan2(sin, cos)*/

    //activate for possible bonus marks

    /*float dot = velocity.dot(position);
    float det = (velocity(1)*position(0)) - (velocity(0)*position(1));
    float rad = atan2(det, dot);*/

/*


    float result = (atan (abs(velocity(1))/abs(velocity(0)))) + (270 * (3.141592653589793238462643383279502884197169399375105820/180));

    float degrees = (result * (180 /3.141592653589793238462643383279502884197169399375105820));

    // std::cout << angle << std::endl;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(degrees, 0.0, 0.0, 1.0);
    glTranslatef(-x, -y, 0);

*/


    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);

    //glColor3f();
    glVertex2f(0  + x,0.01 + y);
    glVertex2f(-0.01 + x,  -0.01 +y);
    glVertex2f(0.01 + x, -0.01 + y);

    glEnd();

    glPopMatrix();



}

const Eigen::Vector2f &Boid::getPosition() const {
    return position;
}

void Boid::setPosition(const Eigen::Vector2f &position) {

    //mutex lock (std::mutex, std::unique_lock);

    //std::cout << "Hi from parallel" << std::endl;


    Boid::position = position;
}

const Eigen::Vector2f &Boid::getFuturePosition() const {
    return futurePosition;
}

void Boid::setFuturePosition(const Eigen::Vector2f &futurePosition) {
    Boid::futurePosition = futurePosition;
}

const Eigen::Vector2f &Boid::getVelocity() const {
    return velocity;
}

void Boid::setVelocity(const Eigen::Vector2f &velocity) {
    Boid::velocity = velocity;
}

void Boid::operator()() {


    std::cout << "hELLOO" << std::endl;

}

float Boid::getR() const {
    return r;
}

float Boid::getG() const {
    return g;
}

float Boid::getB() const {
    return b;
}

std::vector<float> Boid::openclPositionToRender() {

    std::vector<float> temp;
    temp.clear();



    const char *kernelSource =                                       "\n" \
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable                    \n" \
"__kernel void vecAdd(  __global float *a,                       \n" \
"                       __global float *b,                       \n" \
"                       __global float *c,                       \n" \
"                       const unsigned int n)                    \n" \
"{                                                               \n" \
"    //Get our global thread ID                                  \n" \
"    int id = get_global_id(0);                                  \n" \
"                                                                \n" \
"    //Make sure we do not go out of bounds                      \n" \
"    if (id < n)                                                 \n" \
"        c[id] = a[id] + b[id];                                  \n" \
"}                                                               \n" \
                                                                "\n" ;



    // Length of vectors
    unsigned int n = 6;


    //input A
    float* h_a = new float[6];
    h_a[0] = 0;
    h_a[1] = 0.005;
    h_a[2] = -0.005;
    h_a[3] = -0.005;
    h_a[4] = 0.005;
    h_a[5] = -0.005;


    float* h_b = new float[6];
    h_a[0] = x;
    h_a[1] = y;
    h_a[2] = x;
    h_a[3] = y;
    h_a[4] = x;
    h_a[5] = y;


    //output
    float* h_c = new float[6];
    size_t bytes = n*sizeof(float);



    size_t globalSize, localSize;
    cl_int err;

    // Number of work items in each local work group
    localSize;

    // Number of total work items - localSize must be devisor
    globalSize = n;

    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);

    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);

    // Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    // Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);

    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
                                        (const char **) & kernelSource, NULL, &err);

    // Build the program executable
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "vecAdd", &err);

    // Create the input and output arrays in device memory for our calculation
    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);

    // Write our data set into the input array in device memory
    err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,
                               bytes, h_a, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0,
                                bytes, h_b, 0, NULL, NULL);

    // Set the arguments to our compute kernel
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);


    // Execute the kernel over the entire range of the data set
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                 0, NULL, NULL);

    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);

    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                        bytes, h_c, 0, NULL, NULL );




    for (int i=0; i<n;i++){

        temp.push_back(h_c[i]);


    }

    // release OpenCL resources
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_c);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    //release host memory
    free(h_a);
    free(h_b);
    free(h_c);






    return temp;
}





