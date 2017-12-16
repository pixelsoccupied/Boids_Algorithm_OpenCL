//
// Created by Np on 2017-11-19.
//


#include <iostream>
#include "Draw.h"

bool Draw::boidLeft = true;
bool Draw::boidDown = true;

void Draw::drawFlocks(MegaFlock *flock) {


    for(int i=0; i <flock->getAllBoid().size(); i++){
            Boid *boid = flock->getAllBoid()[i];


            Eigen::Vector2f  moveAwayFromFlock ;
            moveAwayFromFlock(0)=0;
            moveAwayFromFlock(1)=0;

            // moveAwayFromFlock = moveAway(boid, flock[(i+1)%flock.size()]);


            Eigen::Vector2f velocity = boid->getVelocity();


            float r = boid->getR();
            float g = boid->getG();
            float b = boid->getB();

            float x = boid->getPosition()(0);
            float y = boid->getPosition()(1);





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




        //mtx.lock();
      //   std::vector<float> positionsToRender = boid->openclPositionToRender();
       // mtx.unlock();

        glBegin(GL_TRIANGLES);
        glColor3f(r, g, b);
        //glColor3f();
        glVertex2f(0  + x  ,0.005 + y );
        glVertex2f(-0.005 + x ,  -0.005 +y );
        glVertex2f(0.005 + x , -0.005 + y ) ;

        glEnd();

        glPopMatrix();


/*
        glBegin(GL_TRIANGLES);
        glColor3f(r, g, b);
        //glColor3f();
        glVertex2f(positionsToRender[0]  ,positionsToRender[1]);
        glVertex2f(positionsToRender[2] ,  positionsToRender[3]);
        glVertex2f(positionsToRender[4], positionsToRender[5]) ;

        glEnd();
        glPopMatrix();
*/




        boidLeft = true;
        boidDown = true;


    }





}

Eigen::Vector2f Draw::moveAway(Boid *pBoid, Flock *&pFlock) {



    Eigen::Vector2f place, newPosition;
    place(0)=0;
    place(1)=0;
    newPosition(0) = 0;
    newPosition(1) = 0;

    place = pFlock->getAveragePositionOfFlock();

    newPosition(0) =(-1) * ((place(0) - pBoid->getPosition()(0))/100);
    newPosition(1) =(-1) * ((place(1) - pBoid->getPosition()(1))/100);



    return newPosition;
}

