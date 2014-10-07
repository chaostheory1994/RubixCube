/* 
 * File:   Block.cpp
 * Author: jacob
 * 
 * Created on September 25, 2014, 11:05 AM
 */

#include "Block.h"
#include "RubixCube.h"
#include "main.h"
#include <cstdlib>
#include <iostream>
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using std::cout;
using std::endl;

/*
 * This is the Block's Contructor.
 * side: Which side of the rubix cube it is on.
 * visible_side: Which sides of this block are visible.
 * c: The colors for the visible sides of the block.    
 */
Block::Block(Color* c[]) {
    int i;
    // lets copy over the colors
    for(i = 0; i < 6; i++) sides[i] = c[i];
}

Block::Block(int i, Color* c){
    // Initialize sides.
    int j;
    for(j = 0; j < 6; j++) sides[j] = NULL;
    // Set single color specified by user.
    sides[i] = c;
}
Block::Block(const Block& orig) {
}

Block::~Block() {
}


// This is the method to draw all the side.
// We will translate using the coordinates.
void Block::draw(){
    // Add rotational stuff.
    glPushMatrix();
    glTranslatef(0.5, 0.5, 0.5);
    // Top side
    if(sides[0] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[0]->r, sides[0]->g, sides[0]->b);
    glBegin(GL_POLYGON);
    glVertex3f(-0.4, 0.4, 0.4);
    glVertex3f(-0.4, 0.4, -0.4);
    glVertex3f(0.4, 0.4, -0.4);
    glVertex3f(0.4, 0.4, 0.4);
    glEnd();
    // Right side
    if(sides[1] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[1]->r, sides[1]->g, sides[1]->b);
    glBegin(GL_POLYGON);
    glVertex3f(0.4, 0.4, 0.4);
    glVertex3f(0.4, -0.4, 0.4);
    glVertex3f(0.4, -0.4, -0.4);
    glVertex3f(0.4, 0.4, -0.4);
    glEnd();
    // Front
    if(sides[2] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[2]->r, sides[2]->g, sides[2]->b);
    glBegin(GL_POLYGON);
    glVertex3f(-0.4, 0.4, -0.4);
    glVertex3f(-0.4, -0.4, -0.4);
    glVertex3f(0.4, -0.4, -0.4);
    glVertex3f(0.4, 0.4, -0.4);
    glEnd();
    // Left
    if(sides[3] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[3]->r, sides[3]->g, sides[3]->b);
    glBegin(GL_POLYGON);
    glVertex3f(-0.4, 0.4, 0.4);
    glVertex3f(-0.4, -0.4, 0.4);
    glVertex3f(-0.4, -0.4, -0.4);
    glVertex3f(-0.4, 0.4, -0.4);
    glEnd();
    // Back
    if(sides[4] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[4]->r, sides[4]->g, sides[4]->b);
    glBegin(GL_POLYGON);
    glVertex3f(-0.4, 0.4, 0.4);
    glVertex3f(-0.4, -0.4, 0.4);
    glVertex3f(0.4, -0.4, 0.4);
    glVertex3f(0.4, 0.4, 0.4);
    glEnd();
    // Under
    if(sides[5] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[5]->r, sides[5]->g, sides[5]->b);
    glBegin(GL_POLYGON);
    glVertex3f(-0.4, -0.4, 0.4);
    glVertex3f(-0.4, -0.4, -0.4);
    glVertex3f(0.4, -0.4, -0.4);
    glVertex3f(0.4, -0.4, 0.4);
    glEnd();
    glPopMatrix();
    // Reset the color because reasons.
    glColor3f(0,0,0);
}

void Block::change_side_color(int i, Color* c){
    sides[i] = c;
}

void Block::print_debug(){
    int out[6];
    int i;
    for(i = 0; i < 6; i++){
        if(sides[i] == NULL) out[i] = -1;
        else out[i] = sides[i]->id;
    }
    cout << "Top: " << out[0]
            << " Right: " << out[1]
            << " Front: " << out[2]
            << " Left: " << out[3]
            << " Back: " << out[4]
            << " Under: " << out[5]
            << endl;
}

Color *Block::get_color(int s){
    return sides[s];
}

/* A method to rotate the individual block.
 * s: the side that it is rotating around.
 * d: the direction.
 */
void Block::rotate_block(int s, int d){
    int dir;
    Color* temp;
    if(d >= 0) dir = 1;
    else dir = -1;
    if (dir >= 0){
        switch(s){
            case 0:
            case 5:
                temp = sides[1];
                sides[1] = sides[4];
                sides[4] = sides[3];
                sides[3] = sides[2];
                sides[2] = temp;
                break;
            case 1:
            case 3:
                temp = sides[0];
                sides[0] = sides[4];
                sides[4] = sides[5];
                sides[5] = sides[2];
                sides[2] = temp;
                break;
            case 2:
            case 4:
                temp = sides[0];
                sides[0] = sides[1];
                sides[1] = sides[5];
                sides[5] = sides[3];
                sides[3] = temp;
                break;
        }
    }
    else {
        switch(s){
            case 0:
            case 5:
                temp = sides[1];
                sides[1] = sides[2];
                sides[2] = sides[3];
                sides[3] = sides[4];
                sides[4] = temp;
                break;
            case 1:
            case 3:
                temp = sides[0];
                sides[0] = sides[2];
                sides[2] = sides[5];
                sides[5] = sides[4];
                sides[4] = temp;
                break;
            case 2:
            case 4:
                temp = sides[0];
                sides[0] = sides[3];
                sides[3] = sides[5];
                sides[5] = sides[1];
                sides[1] = temp;
                break;
        }
    }
}
