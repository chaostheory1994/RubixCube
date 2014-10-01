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
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif



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
// It will assume the point has been transformed where it needs to be.
void Block::draw(int size_b){
    // Top side
    /*if(sides[0] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[0]->r, sides[0]->g, sides[0]->b);
    glBegin(GL_POLYGON);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();
    // Right side
    if(sides[1] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[1]->r, sides[1]->g, sides[1]->b);
    glBegin(GL_POLYGON);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();
    // Front
    if(sides[2] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[2]->r, sides[2]->g, sides[2]->b);
    glBegin(GL_POLYGON);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();
    // Left
    if(sides[3] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[3]->r, sides[3]->g, sides[3]->b);
    glBegin(GL_POLYGON);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();
    // Back
    if(sides[4] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[4]->r, sides[4]->g, sides[4]->b);
    glBegin(GL_POLYGON);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();
    // Under
    if(sides[5] == NULL) glColor3f(R_COLOR_HIDDEN); // Grey
    else glColor3f(sides[5]->r, sides[5]->g, sides[5]->b);
    glBegin(GL_POLYGON);
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glVertex3f();
    glEnd();*/
    // Reset the color because reasons.
    glColor3f(0,0,0);
}

void Block::change_side_color(int i, Color* c){
    sides[i] = c;
}


