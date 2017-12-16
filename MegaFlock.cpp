//
// Created by Np on 2017-12-01.
//
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
#include "MegaFlock.h"

const std::vector<Flock *> &MegaFlock::getAllFlock() const {
    return allFlock;
}

void MegaFlock::setAllFlock(const std::vector<Flock *> &allFlock) {
    MegaFlock::allFlock = allFlock;
}

MegaFlock::MegaFlock(const std::vector<Flock *> &allFlock) : allFlock(allFlock) {
    addBoid();
}

const std::vector<Boid *> &MegaFlock::getAllBoid() const {
    return allBoid;
}

void MegaFlock::setAllBoid(const std::vector<Boid *> &allBoid) {
    MegaFlock::allBoid = allBoid;
}

void MegaFlock::addBoid() {
    for (auto &i : allFlock) {
        for (auto j : i->getFlock()) {
            allBoid.push_back(j);
        }
    }
}

void MegaFlock::doOpenCL() {
    //kernel for opencl


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



// Simple compute kernel which computes the addition of two input arrays

//
// OpenCL kernel. Each work item takes care of one element of c
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
"    //if (id < n)                                                 \n" \
"        c[id] = a[id] + b[id];                                  \n" \
"}                                                               \n" \
                                                                "\n" ;


    // Length of vectors
    unsigned int n = allBoid.size()*2;

    // Host input vectors
    float *h_a;
    float *h_b;
    // Host output vector
    float *h_c;



    // Size, in bytes, of each vector
    size_t bytes = n*sizeof(float);

    // Allocate memory for each vector on host
    h_a = (float*)malloc(bytes);
    h_b = (float*)malloc(bytes);
    h_c = (float*)malloc(bytes);

    // Initialize vectors on host
    int numberOfBoids = allBoid.size();
    int i;
    for( i = 0; i < numberOfBoids; i++ )
    {
        //velocity X and Y
        if (isnan(allBoid [i]->getVelocity()(0))){
            h_a[i] =0.001;
        }
        else {
            h_a[i] = allBoid[i]->getVelocity()(0);
        }

        if (isnan(allBoid [i]->getVelocity()(1))) {
            h_a[numberOfBoids + i] =0.001;
        }
        else
        {
            h_a[numberOfBoids + i] = allBoid[i]->getVelocity()(1);

        }


        //position x and y

        h_b[i] = allBoid[i]->getPosition()(0);
        h_b[numberOfBoids+i] =allBoid[i]->getPosition()(1);


    }

    cl_uint* numOfDevices;

    size_t globalSize, localSize;
    cl_int err;

    // Number of work items in each local work group
    localSize = n/5;

    // Number of total work items - localSize must be devisor
    globalSize = n;

    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);

    // Get ID for the device
    err |= clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 2, &device_id, NULL);

    //std::cout << *numOfDevices << std::endl;

    // Create a context
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);

    // Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);

    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
                                        & kernelSource, NULL, &err);

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

    //cl_event bla(reinterpret_cast<cl_event>(context));


   // clEnqueueWaitForEvents(bla,1,1);


    // Execute the kernel over the entire range of the data set
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                 0, NULL, NULL);

    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);

    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                        bytes, h_c, 0, NULL, NULL );

    //Sum up vector c and print result divided by n, this should equal 1 within error
    std :: vector <float> temp;
    float sum = 0;
    for(i=0; i<numberOfBoids*2; i++) {
        temp.push_back(h_c[i]);
        //sum += h_c[i];
    }



    for(i=0; i<numberOfBoids; i++) {
        Eigen::Vector2f tempPos;
        tempPos(0)= h_c[i];
        tempPos(1) = h_c[numberOfBoids+i];

        allBoid[i]->setPosition(tempPos);
    }


    //std::cout << (sum) << std::endl;

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

}

MegaFlock::~MegaFlock() {

    std::cout << "Deleting megaflock || \\_/ || " << std::endl;

}
