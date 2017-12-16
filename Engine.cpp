//
// Created by Np on 2017-11-19.
//

#include "Engine.h"
#include "MegaFlock.h"


int Engine::SCREEN_WIDTH = 1024;
int Engine::SCREEN_HEIGHT = 768;
GLFWwindow *Engine::window = NULL;

double Engine::limitFPS = 1.0 / 30.0;


Engine::~Engine() {


    std:: cout << "Window is closed!" << std::endl;

    for (auto &i : flock) {
        delete i;
    }

    glfwDestroyWindow(this->window);
    glfwTerminate();

}

bool Engine::initialize(char *windowTitle) {

    if (!glfwInit()) {
        std::cout << "Error initializing GLFW" << std::endl;
        return false;
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, NULL, NULL);
    if (window == NULL) {
        std::cout << "Error creating window" << std::endl;
        return false;
    }
    glfwSetKeyCallback(window, key_callback);


    //GLFW Setup
    glfwMakeContextCurrent(window);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwSwapInterval(1);


    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int xPos = (mode->width - SCREEN_WIDTH) / 2;
    int yPos = (mode->height - SCREEN_HEIGHT) / 2;
    glfwSetWindowPos(window, xPos, yPos);



    return true;
}

void Engine::update() {

    //timer starts here
    currentTime = glfwGetTime();
    changeInTime += (currentTime - lastTime) / limitFPS;
    lastTime = currentTime;


    glfwPollEvents();

}


void Engine::render(MegaFlock* megaFlock){

    glClearColor(0.1, 0.15, 0.12, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    //30 frames
    while (changeInTime >= 1.0) {
        //tbb::task_group group;
        // Update function
        for (auto &i : flock) {

           // i->updateBoid();
           // group.run([&]{i->updateBoid();});


            flockPool.work(std::bind(&Flock::updateBoid, i));
        }


        megaFlock->doOpenCL();


       // group.wait();
        updates++;
        changeInTime--;
    }


    Draw::drawFlocks(megaFlock);



    frames++;
    if (glfwGetTime() - timer > 1.0) {
        timer++;
        std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
        updates = 0, frames = 0;
    }


    glfwSwapBuffers(window);


}

void Engine::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, 1);

}

GLFWwindow *Engine::getWindow() {
    return window;
}

void Engine::setWindow(GLFWwindow *window) {
    Engine::window = window;
}

Engine::Engine(const std::vector<Flock *> &flock, int numberOfFlocks) : flock(flock), flockPool(numberOfFlocks) {}
