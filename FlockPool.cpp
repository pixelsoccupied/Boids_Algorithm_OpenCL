//
// Created by Np on 2017-11-23.
//
#include <iostream>
#include "FlockPool.h"

void FlockPool::work(std::function<void()> func) {
    {
        // Mutex starts here
        std::unique_lock<std::mutex> l(lock);

        jobStack.push(std::move(func));
        //job ready for whoever is free
        cond.notify_one();
        //mutex ends here
    }

}

FlockPool::~FlockPool() {

    {
        // Unblock any threads and tell them to stop
        std::unique_lock<std::mutex> l(lock);

        isClose = true;
        cond.notify_all();


    }


    //threads about to stop

    std::cerr << "Joining threads" << std::endl;
    for (auto& thread : threadVector)
        thread.join();

}

FlockPool::FlockPool(int threads) : isClose(false) {

    //redirect the threads to a while loop and wait around for work

    for (int i = 0; i < threads; ++i){

        threadVector.emplace_back(std::bind (&FlockPool::threadRun, this, i));

    }
}

void FlockPool::threadRun(int i) {

    std::function <void (void)> jobToDO;

    while (1)
    {
        {
            //take the lock
            std::unique_lock <std::mutex> l (lock);

            while (!isClose && jobStack.empty())
                cond.wait (l);

            if (isClose && jobStack.empty ())
            {
                std::cerr << "Thread " << i << " terminates" << std::endl;
                return;
            }

            // if (!jobStack.empty()) {
            //get the job
            jobToDO = std::move(jobStack.top());
            //std::cerr << "Thread " << i << " Got a job!" << std::endl;
            // }
            //
            jobStack.pop();
        }

        // Do the job without holding any locks
        jobToDO ();
    }

}