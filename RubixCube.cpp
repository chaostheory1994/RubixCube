/* 
 * File:   RubixCube.cpp
 * Author: jacob
 * 
 * Created on September 25, 2014, 11:05 AM
 */

#include "RubixCube.h"
#include "Block.h"
#include <cstdlib>
#include <queue>
#include <stack>
#include "main.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using std::queue;
using std::stack;
        
RubixCube::RubixCube() {
    int i, j, k;
    for(i = 0; i < 6; i++) colors[i] = new Color;
    // First we setup some colors
    setup_color(colors[0], R_COLOR0, 1);
    setup_color(colors[1], R_COLOR1, 2);
    setup_color(colors[2], R_COLOR2, 3);
    setup_color(colors[3], R_COLOR3, 4);
    setup_color(colors[4], R_COLOR4, 5);
    setup_color(colors[5], R_COLOR5, 6);
    
    // All Colors are setup, next we must setup a block system.
    // This can be done in 3 phases.
    // This first phase will initialize the top and under side.
    for(i = 0; i < 2; i++){
        cores[i*5] = new core;
        cores[i*5]->b = new Block(i*5, colors[i*5]);
        // Now we will setup the left and right adjacent bridges.
        for(j = 1; j < 3; j++){
            cores[i*5]->br[j] = new bridge;
            cores[i*5]->br[j]->b = new Block(i*5, colors[i*5]);
            // Next we setup the corner Blocks
            for(k = 0; k < 2; k++){
                cores[i*5]->br[j]->c[k] = new Block(i*5, colors[i*5]);
            }
        }
        // Now we will complete the top and bottom bridges.
        for(j = 0; j < 2; j++){
            // Lets create the new bridges
            cores[i*5]->br[j*3] = new bridge;
            cores[i*5]->br[j*3]->b = new Block(i*5, colors[i*5]);
            // Now lets link these to the correct corners.
            // Left Block
            cores[i*5]->br[j*3]->c[0] = cores[i*5]->br[1]->c[j/1];
            // Right Block
            cores[i*5]->br[j*3]->c[1] = cores[i*5]->br[2]->c[j/1];
        }
    }
    // The second phase is easier.
    // We will be creating the left and right side of the rubix cube.
    // It will involve creating 4 more bridges and linking them to the right corners.
    // We will also need to create the two cores as well.
    for(i = 1; i < 4; i += 2){
        // Create the core.
        cores[i] = new core;
        cores[i]->b = new Block(i, colors[i]);
        // Create the two missing bridges.
        // The two missing bridges are to the left and to the right of the core.
        for(j = 1; j < 3; j++){
            cores[i]->br[j] = new bridge;
            // I am going to hard link these later for sanity's sake!
            // Create the bridge's block.
            cores[i]->br[j]->b = new Block(i, colors[i]);
        }
        // Link Bridges
        if(i == 1) k = 2;
        else k = 1;
        cores[i]->br[0] = cores[0]->br[k];
        cores[i]->br[3] = cores[5]->br[k];
        // Set those colors.
        cores[i]->br[0]->b->change_side_color(i, colors[i]);
        cores[i]->br[3]->b->change_side_color(i, colors[i]);
    }
    // Hard code links in.
    // Right side.
    cores[1]->br[1]->c[0] = cores[0]->br[2]->c[1];
    cores[1]->br[2]->c[0] = cores[0]->br[2]->c[0];
    cores[1]->br[1]->c[1] = cores[5]->br[2]->c[0];
    cores[1]->br[2]->c[1] = cores[5]->br[2]->c[1];
    // Left side.
    cores[3]->br[1]->c[0] = cores[0]->br[2]->c[1];
    cores[3]->br[2]->c[0] = cores[0]->br[2]->c[0];
    cores[3]->br[1]->c[1] = cores[5]->br[2]->c[0];
    cores[3]->br[2]->c[1] = cores[5]->br[2]->c[1];
    // Set the colors.
    // Right side.
    cores[1]->br[1]->c[0]->change_side_color(1, colors[1]);
    cores[1]->br[2]->c[0]->change_side_color(1, colors[1]);
    cores[1]->br[1]->c[1]->change_side_color(1, colors[1]);
    cores[1]->br[2]->c[1]->change_side_color(1, colors[1]);
    // Left side.
    cores[3]->br[1]->c[0]->change_side_color(4, colors[4]);
    cores[3]->br[2]->c[0]->change_side_color(4, colors[4]);
    cores[3]->br[1]->c[1]->change_side_color(4, colors[4]);
    cores[3]->br[2]->c[1]->change_side_color(4, colors[4]);
    
    //Third and final phase.
    // Setup the front and back sides!
    for(i = 2; i < 5; i+=2){
        // First setup the core.
        cores[i] = new core;
        cores[i]->b = new Block(i, colors[i]);
    }
    // Linking Time
    // Front Side
    cores[2]->br[0] = cores[0]->br[3];
    cores[2]->br[1] = cores[3]->br[2];
    cores[2]->br[2] = cores[1]->br[1];
    cores[2]->br[3] = cores[5]->br[0];
    // Back Side
    cores[4]->br[0] = cores[0]->br[0];
    cores[4]->br[1] = cores[1]->br[1];
    cores[4]->br[2] = cores[3]->br[2];
    cores[4]->br[3] = cores[5]->br[3];
    // Now  lets color those in.
    for(i = 2; i < 5; i+=2){
        for(j = 0; j < 4; j++){
            cores[i]->br[j]->b->change_side_color(i, colors[i]);
        }
    }
    // Jobs Done!
}

/* Will no implement since the project only asks for 1 cube.*/
RubixCube::RubixCube(const RubixCube& orig) {
}

/* Deconstructor */
RubixCube::~RubixCube() {
    // Delete all the blocks.
    // Delete all the bridges
    // Delete all the cores.
    // Delete all the events.
    // Jobs done.
}

void RubixCube::setup_color(Color *c, int r, int g, int b, int id){
    c->r = r;
    c->g = g;
    c->b = b;
    c->id = id;
}

/* Used to draw the whole rubix cube.
 * Will mainly just transform and call draw to all the blocks.
 * If an animation is applied, that will also be included in the transform.
 */
void RubixCube::draw(float f){
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(0, 1.0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1.0, 0, 0);
    glVertex3f(1.0, 1.0, 0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0, 1.0, 1.0);
    glVertex3f(0, 0, 1.0);
    glVertex3f(1.0, 0, 1.0);
    glVertex3f(1.0, 1.0, 1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0, 0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(1.0, 0, 0);
    glVertex3f(1.0, 0, 1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0, 1.0, 1.0);
    glVertex3f(0, 1.0, 0);
    glVertex3f(1.0, 1.0, 0);
    glVertex3f(1.0, 1.0, 1.0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(1.0, 1.0, 1.0);
    glVertex3f(1.0, 0, 1.0);
    glVertex3f(1.0, 0, 0);
    glVertex3f(1.0, 1.0, 0);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0, 1.0, 1.0);
    glVertex3f(0, 0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1.0, 0);
    glEnd();
    glColor3f(0,0,0);
}

/* This is called when we want to turn a side.
 * Will mainly be called when input is read.
 * int s: the side that will turn. Represented by side preprocessor variables.
 * int d: the direction clockwise or counterclockwise >=0 is clockwise. else counter clockwise.
 */
int RubixCube::turn_side(int s, int d){
    
}

/* This is called to update the cube and is used mainly for animation purposes.
 * It will recheck the event queue for possible inputs.
 */
void RubixCube::update_cube(){
    
}

/* This function will simply solve the rubix cube for the user.
 * It will mainly empty the event queue.
 * Then it will add all the events on the stack to the queue.
 */
void RubixCube::solve_cube(){
    
}