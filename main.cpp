#include <iostream>
#include "Engine.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto* flock = new Flock(100);
    auto * flock1 = new Flock(100);
    auto * flock2 = new Flock(100);
    auto * flock3 = new Flock(100);
    auto * flock4 = new Flock(100);

   //

    std::vector <Flock*> flocks= {flock , flock1, flock2, flock3, flock4};

    //flock.randomisePosition();

    //megaflock contains all the opencl functions with the new implementaiton
    MegaFlock* megaFlock = new MegaFlock(flocks);


    Engine* engine = new Engine(flocks, flocks.size());
    engine->initialize("Boid");





    while(!glfwWindowShouldClose(engine->getWindow())){
        engine->update();
        engine->render(megaFlock);
    }

   // megaFlock->doOpenCL();
    //delete(flocks);
    delete(engine);
    delete(megaFlock);
    std::cout << "This program ends heree" << std::endl;


/*
    float x = 20/0;
    float y = 20/0;
    float xy = x+y;

    std::cout << xy << std::endl;
*/



    return 0;
}