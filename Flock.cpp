#include <random>
#include <iostream>
#include "Flock.h"
using namespace Eigen;

float Flock::rgb = 0.03;
int Flock::IDgen = 0;
std::random_device rd2;
std::mt19937 e23(rd2());

Flock::Flock(int numOfBoids) {

    IDgen++;

    for (int i=0; i <numOfBoids; i++){
        auto * boid = new Boid(0.8+rgb,0.2+rgb,0.4);
        flock.push_back(boid);


    }

    averagePositionOfFlock(0) =0;
    averagePositionOfFlock(1) =0;

    ID = IDgen;



    rgb += rgb;
    rgb = fmod(rgb, 1);
    //std::cout << rgb << std::endl;

}

Flock::~Flock() {
    std::cout << "Flock #" << ID << " dead" <<std::endl;
    std::cout << "Boids about to die" <<std::endl;

    for (int i=0; i< flock.size(); i++){
        delete flock[i];
    }


}

const std::vector<Boid *> &Flock::getFlock() const {
    return flock;
}

void Flock::setFlock(const std::vector<Boid *> &flock) {
    Flock::flock = flock;
}
//just to test OPENGL
void Flock::randomisePosition() {

    for (auto &i : flock) {
        std::uniform_real_distribution<> xCordinate(-1, 1);
        std::uniform_real_distribution<> yCordinate(-1, 1);

        Vector2f newPosition;
        newPosition(0) = xCordinate(e23);
        newPosition(1) = yCordinate(e23);

        i->setFuturePosition(newPosition);

    }

}

void Flock::updateBoid() {

    // std::cout << "FLOCK #" << ID << " updating its birds!" << std::endl;

   // mtx.lock();
    //openCLBoid->runCode(this);
    std::thread::id this_id = std::this_thread::get_id();



    //int i =0;
    Vector2f v1, v2, v3, v4, avgPosition;

    v1(0)=0;
    v1(1)=0;

    v2(0)=0;
    v2(1)=0;

    v3(0)=0;
    v3(1)=0;

    v4(0)=0;
    v4(1)=0;

    avgPosition(0)=0;
    avgPosition(1)=0;
    int j=0;

    for (auto &i : flock) {

        // if (i ==0) {
        //  i->setVelocity(i->getVelocity() / 2);
        //}
        // Boids try to fly towards the centre of mass of neighbouring boids.
        v1 = cohesion(i);
        //Boids try to keep a small distance away from other objects (including other boids).
        v2 = separation(i);
        //Boids try to match velocity with near boids.
        v3 = alignment(i);
        //stay within a parameter
        v4 = boundPosition(i);


        //boid threads start here?


        i->setVelocity(i->getVelocity() + v1 + v2 + v3 + v4);
        limitSpeed(i);



       // Vector2f sendPosition = i->getPosition() + i->getVelocity();
        //thread.push_back(new std::thread(init_thread , i, sendPosition));

       // Eigen::Vector2f boidsNewPosition = i->getPosition() + i->getVelocity();
       // i->setPosition(i->getPosition() + i->getVelocity());

        //launchParallelFor(i,j, boidsNewPosition);
        j++;
        avgPosition += i->getPosition();

        //std::cout << "i" <<std::endl;
    }



   // mtx.unlock();

    cohesionX.clear();
    cohesionY.clear();
    separationX.clear();
    separationY.clear();
    alignmentX.clear();
    alignmentY.clear();
    oldVelocityX.clear();
    oldVelocityY.clear();








    avgPosition(0) = avgPosition(0)/flock.size();
    avgPosition(1)= avgPosition(1)/flock.size();

    averagePositionOfFlock = avgPosition;

}
//first rule
Eigen::Vector2f Flock::cohesion(Boid *boidj) {


    Vector2f pcJ; //Pecived center, to get the average of all the birds except the guy using
    pcJ(0) = 0;
    pcJ(1) = 0;

    int neighbour = 0;
    Vector2f currentBoidPosition = boidj->getPosition();
    Vector2f otherBoidsPosition;

    //boidsPosition.

    for (auto &i : flock) {


        if (i != boidj) {
            if ( (i->getPosition() - boidj->getPosition()).norm() < 0.8) {

                otherBoidsPosition = i->getPosition();
                pcJ(0) += otherBoidsPosition(0);
                pcJ(1) += otherBoidsPosition(1);
                neighbour++;
            }
        }

    }

    //std ::cout << neighbour << std::endl;
    //Take the average of the position

    pcJ(0) = (pcJ(0)/neighbour);
    pcJ(1) = (pcJ(1)/neighbour);

    Vector2f newDirection;
    newDirection(0) = (pcJ(0) - boidj->getPosition()(0))/120;
    newDirection(1) = (pcJ(1) - boidj->getPosition()(1))/120;



    return newDirection;


}

Eigen::Vector2f Flock::separation(Boid *boidj) {

    Vector2f c;
    c(0)=0;
    c(1)=0;

    Vector2f k;
    k(0) = 0.2;
    k(1) = 0.2;

    for (auto &i : flock) {
        if (i !=  boidj){
           // if ( (i->getPosition() - boidj->getPosition()).norm() < 0.8) {
                if ((i->getPosition() - boidj->getPosition()).norm() < 0.04) {
                    c = c - (i->getPosition() - boidj->getPosition());
                }
           // }
        }

    }


    return c;


}

Eigen::Vector2f Flock::alignment(Boid *boidj) {


    Vector2f pvJ; //Pecived center, to get the average of all the birds except the guy using
    pvJ(0) = 0;
    pvJ(1) = 0;

    int neighbour = 0;
    //Vector2d keepAway(0.1, 0.1);
    Vector2f currentBoidPosition = boidj->getPosition();
    Vector2f otherBoidsVelocity;

    //boidsPosition.

    for (auto &i : flock) {
        if (i != boidj) {
          //  if ( (i->getPosition() - boidj->getPosition()).norm() < 0.8) {
                otherBoidsVelocity = i->getVelocity();
                pvJ(0) += otherBoidsVelocity(0);
                pvJ(1) += otherBoidsVelocity(1);
                neighbour++;
            }
        //}
    }

    //std ::cout << neighbour << std::endl;
    //Take the average of the position

    pvJ(0) = (pvJ(0)/neighbour);
    pvJ(1) = (pvJ(1)/neighbour);

    Vector2f newVelocity;

    newVelocity(0) = (pvJ(0) - boidj->getVelocity()(0))/100;
    newVelocity(1) = (pvJ(1) - boidj->getVelocity()(1))/100;



    return newVelocity;

}

void Flock::limitSpeed(Boid *boid) {
    // For a limiting speed vlim:

    // PROCEDURE limit_velocity(Boid b)
    float vlim = 0.009;
    Vector2f newVelocity = boid->getVelocity();

    if (boid->getVelocity().norm() > vlim) {
        newVelocity(0) = ((boid->getVelocity()(0) / boid->getVelocity().norm() ) * vlim);
        newVelocity(1) = ((boid->getVelocity()(1) / boid->getVelocity().norm() ) * vlim);

    }
    boid->setVelocity(newVelocity);
}

Eigen::Vector2f Flock::boundPosition(Boid* boid) {
    Vector2f v;
    v(0) = 0;
    v(1) =0;

    float Xmin = -1, Xmax = 1, Ymin = -1, Ymax = 1;

    if( boid->getPosition()(0) < Xmin){
        v(0) = 1;
    }
    else if( boid->getPosition()(0) > Xmax ) {
        v(0) = -1;
    }
    if( boid->getPosition()(1) < Ymin){
        v(1) = 1;
    }
    else if( boid->getPosition()(1) > Ymax ) {
        v(1) = -1;
    }
    return v;
}



const Vector2f &Flock::getAveragePositionOfFlock() const {
    return averagePositionOfFlock;
}



