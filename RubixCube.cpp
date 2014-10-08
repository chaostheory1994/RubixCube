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
#include <iostream>
#include <cmath>
#include "main.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using std::queue;
using std::stack;
using std::cout;
using std::endl;
        
RubixCube::RubixCube() {
    int i, j, k;
    for(i = 0; i < 6; i++) colors[i] = new Color;
    // Initialize Variables
    main_degrees = 0;
    main_progress = 0;
    opp_degrees = 0;
    opp_progress = 0;
    main_rotator = -1;
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
    cores[1]->br[1]->c[0] = cores[0]->br[2]->c[0];
    cores[1]->br[2]->c[0] = cores[0]->br[2]->c[1];
    cores[1]->br[1]->c[1] = cores[5]->br[2]->c[0];
    cores[1]->br[2]->c[1] = cores[5]->br[2]->c[1];
    // Left side.
    cores[3]->br[1]->c[0] = cores[0]->br[1]->c[0];
    cores[3]->br[2]->c[0] = cores[0]->br[1]->c[1];
    cores[3]->br[1]->c[1] = cores[5]->br[1]->c[0];
    cores[3]->br[2]->c[1] = cores[5]->br[1]->c[1];
    // Set the colors.
    // Right side.
    cores[1]->br[1]->c[0]->change_side_color(1, colors[1]);
    cores[1]->br[2]->c[0]->change_side_color(1, colors[1]);
    cores[1]->br[1]->c[1]->change_side_color(1, colors[1]);
    cores[1]->br[2]->c[1]->change_side_color(1, colors[1]);
    // Left side.
    cores[3]->br[1]->c[0]->change_side_color(3, colors[3]);
    cores[3]->br[2]->c[0]->change_side_color(3, colors[3]);
    cores[3]->br[1]->c[1]->change_side_color(3, colors[3]);
    cores[3]->br[2]->c[1]->change_side_color(3, colors[3]);
    
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
    cores[2]->br[2] = cores[1]->br[2];
    cores[2]->br[3] = cores[5]->br[3];
    // Back Side
    cores[4]->br[0] = cores[0]->br[0];
    cores[4]->br[1] = cores[1]->br[1];
    cores[4]->br[2] = cores[3]->br[1];
    cores[4]->br[3] = cores[5]->br[0];
    //for(i = 0; i < 4; i++) cores[4]->br[i]->b->change_side_color(4, colors[4]);
    // Now  lets color those in.
    for(i = 2; i < 5; i+=2){
        for(j = 0; j < 4; j++){
            cores[i]->br[j]->b->change_side_color(i, colors[i]);
        }
        for(j = 1; j < 3; j++){
            for(k = 0; k < 2; k++){
                cores[i]->br[j]->c[k]->change_side_color(i, colors[i]);
            }
        }
    }
    
    // Instead of having an array of adjacent sides in the core
    // We will instead remember its opposite core.
    // Much easier to do.
    cores[0]->opp = 5;
    cores[5]->opp = 0;
    cores[1]->opp = 3;
    cores[3]->opp = 1;
    cores[2]->opp = 4;
    cores[4]->opp = 2;
    
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

void RubixCube::setup_color(Color *c, float r, float g, float b, int id){
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
    int i;
    int rotation_main = main_degrees - (main_progress + (main_degrees * f));
    int rotation_opp = opp_degrees - (opp_progress + (opp_degrees * f));
    // This is where we draw a cube! Yay!
    // Since we assume we've been translated to the lower left corner of the cube.
    // We can just start to draw.
    // There are gonna be three ways to draw this which is based on which sides are rotating.
    // The top to bottom will be the default.
    // These if statements are pretty ugly and im sorry for that.
    // They basically just print the sides that are able to turn.
    // Then fill in the middle layer.
        // Default but also the rotation on the Y axis.
        // Now we draw the top side.
        glPushMatrix();
        // Top, Left, Front Rotation
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(-1.5, 0.5, -1.5);
        cores[0]->br[3]->c[0]->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Front
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(-0.5, 0.5, -1.5);
        cores[0]->br[3]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Front, Right
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(0.5, 0.5, -1.5);
        cores[0]->br[3]->c[1]->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Left
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        glTranslatef(-1.5, 0.5, -0.5);
        cores[0]->br[1]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Top
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        glTranslatef(-0.5, 0.5, -0.5);
        cores[0]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Right
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        glTranslatef(0.5, 0.5, -0.5);
        cores[0]->br[2]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Back, Left
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(-1.5, 0.5, 0.5);
        cores[0]->br[0]->c[0]->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Back
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(-0.5, 0.5, 0.5);
        cores[0]->br[0]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Top, Back, Right
        if(main_rotator == 0) glRotatef(rotation_main, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(0.5, 0.5, 0.5);
        cores[0]->br[0]->c[1]->draw();
        glPopMatrix();
        // Lets draw the bottom layer now
        glPushMatrix();
        // Bottom, Front, Left
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
		glTranslatef(-1.5, -1.5, -1.5);
        // Placed at front left of cube layer.
        cores[5]->br[3]->c[0]->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom, Front
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(-0.5, -1.5, -1.5);
        cores[5]->br[3]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom, Front, Right
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
		glTranslatef(0.5, -1.5, -1.5);
        cores[5]->br[3]->c[1]->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom, Left
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        glTranslatef(-1.5, -1.5, -0.5);
        cores[5]->br[1]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        glTranslatef(-0.5, -1.5, -0.5);
        cores[5]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom Right
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        glTranslatef(0.5, -1.5, -0.5);
        cores[5]->br[2]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom, Back, Left
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(-1.5, -1.5, 0.5);
        cores[5]->br[0]->c[0]->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom, Back
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(-0.5, -1.5, 0.5);
        cores[5]->br[0]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Bottom, Back, Right
        if(main_rotator == 0) glRotatef(rotation_opp, 0, 1, 0);
        else if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(0.5, -1.5, 0.5);
        cores[5]->br[0]->c[1]->draw();
        glPopMatrix();
        // Draw the rest
        glPushMatrix();
        // Front, Left
        if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(-1.5, -0.5, -1.5);
        cores[2]->br[1]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Front
        if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(-0.5, -0.5, -1.5);
        cores[2]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Front Right
        if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_main, 0, 0, 1);
        glTranslatef(0.5, -0.5, -1.5);
        cores[2]->br[2]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Back, Left
        if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(-1.5, -0.5, 0.5);
        cores[4]->br[2]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Back
        if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(-0.5, -0.5, 0.5);
        cores[4]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Back Right
        if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        else if(main_rotator == 2) glRotatef(rotation_opp, 0, 0, 1);
        glTranslatef(0.5, -0.5, 0.5);
        cores[4]->br[1]->b->draw();
        glPopMatrix();
        // Draw Final 2 cores
        glPushMatrix();
        // Left
        if(main_rotator == 1) glRotatef(rotation_opp, 1, 0, 0);
        glTranslatef(-1.5, -0.5, -0.5);
        cores[3]->b->draw();
        glPopMatrix();
        glPushMatrix();
        // Right
        if(main_rotator == 1) glRotatef(rotation_main, 1, 0, 0);
        glTranslatef(0.5, -0.5, -0.5);
        cores[1]->b->draw();
        glPopMatrix();
    glColor3f(0,0,0);
}

/* This is called when we want to turn a side.
 * Will mainly be called when input is read.
 * int s: the side that will turn. Represented by side preprocessor variables.
 * int d: the direction clockwise or counterclockwise >=0 is clockwise. else counter clockwise.
 */
int RubixCube::push_turn(int s, int d){
    // This is a simple method that adds the event to the event queue.
    queue_packet *qp = new queue_packet;
    q.push(qp);
    return 1;
    // There all done.
}

/* This is called to update the cube and is used mainly for animation purposes.
 * It will recheck the event queue for possible inputs.
 * t: The amount of time that should have passed since the last update.
 */
void RubixCube::update_cube(int t){
    queue_packet* temp;
    // This is where all the heavy duty stuff happens.
    // First lets update rotation.
    // We must check the queue to see if we must add to the total rotation
    while(true){
        if(q.empty()) break;
        temp = q.front();
        if(temp->side == main_rotator ||
                temp->side == cores[main_rotator]->opp){
            // Oh hey, we have a viable addition to our movement.
            // Lets execute it.
            turn_side(temp->side, temp->direction, true);
            // Before we pop, lets add it to the log.
            log.push(temp);
            q.pop();
        }
        else break;
    }
    // We know that the time t has passed which will allow us to convert
    // The ms. in the #define variables to theta.
    if(main_rotator != -1){
        // Since i want the rotation to speed up as u add more turns to it.
        // I am going to use that with the equation msec*(total_turn/turn_speed)
        // This is a good scalar value to then multiply by parameter t
        // Giving us the total theta turned.
		main_progress += t * (main_degrees / BLOCK_ROTATE_SPEED);
		opp_progress += t * (opp_degrees / BLOCK_ROTATE_SPEED);
        // We must also check for finished animations because important reason.
		if ((main_progress >= main_degrees && main_degrees > 0) ||
			(main_progress <= main_degrees && main_degrees < 0)){
			main_progress = 0;
			main_degrees = 0;
		}
		if ((opp_progress >= opp_degrees && opp_degrees > 0) ||
			(opp_progress <= main_degrees && opp_degrees < 0)){
			opp_degrees = 0;
			opp_progress = 0;
		}
        // Are both animations finished? Then the rubix cube should remember that
        // And allow itself to execute a new turn.
        if(main_progress == main_degrees && opp_progress == opp_degrees){
            main_degrees = 0;
            main_progress = 0;
            opp_degrees = 0;
            opp_progress = 0;
            main_rotator = -1;
        }
        // That is pretty much it for this method.
    }
}

/* This function will simply solve the rubix cube for the user.
 * It will mainly empty the event queue.
 * Then it will add all the events on the stack to the queue.
 */
void RubixCube::solve_cube(){
    
}

void RubixCube::print_debug(){
    int i, j;
    const char* bridge_dir[4] = {"Top", "Left", "Right", "Bottom"};
    for(i = 0; i < 6; i++){
        cout << "Looking at Side " << i << endl;
        for(j = 0; j < 4; j++){
            cout << "Looking at " << bridge_dir[j] << " bridge." << endl;
            cout << "Looking at " << 0 << " corner." << endl;
            cores[i]->br[j]->c[0]->print_debug();
            cout << "Bridge debug." << endl;
            cores[i]->br[j]->b->print_debug();
            cout << "Looking at " << 1 << "corner." << endl;
            cores[i]->br[j]->c[1]->print_debug();
        }
    }
	cout << check_integrity() << endl;
}

/*
 * This is the actual internal turning of a side.
 * Very heavy stuff :)
 * This method turns the cube memory wise and sets up the turn visually.
 * This should only be called once as to tell the cube a side is turning 90 deg.
 * This method does not iterate the animation.
 * s: The side that is turning. This side will not actually change at all though
 * Only the adjacent side will.
 * d: The direction. < 0 is counterclockwise >= 0 is clockwise
 * anim: a boolean to say if we should animate this turn or not.
 * anim is mainly used by the shuffle method as to free the user from watching it shuffle.
 */
void RubixCube::turn_side(int s, int d, bool anim){
    // We can assume this is a valid turn.
    // First lets update the cube.
    // Each core has its opposite side and as such we know which sides dont get changed.
    int dir;
    bridge *temp;
    Block *temp_block;
    if( d < 0) dir = -1;
    else dir = 1;
    // Lets update the cube's memory
    // We have the core number and its bridges. 
    // With that this will be cake.
    // Which direction are we going?
    if(dir < 0){
        // CClockwise.
        // All we have to do is setup the bridges.
        // Then a loop should be able to clean up the rest.
        // We also have to update the bridge's parrent node as well.
        // This is a little tricky since we cant be sure which of the two parent nodes
        // are the node we are currently turning. As such we have to check.
        temp = cores[s]->br[0];
        cores[s]->br[0] = cores[s]->br[2];
        cores[s]->br[2] = cores[s]->br[3];
        cores[s]->br[3] = cores[s]->br[1];
        cores[s]->br[1] = temp;
        // Fix orientations.
        temp_block = cores[s]->br[1]->c[0];
        cores[s]->br[1]->c[0] = cores[s]->br[1]->c[1];
        cores[s]->br[1]->c[1] = temp_block;
        
        
        temp_block = cores[s]->br[2]->c[0];
        cores[s]->br[2]->c[0] = cores[s]->br[2]->c[1];
        cores[s]->br[2]->c[1] = temp_block;
        
    }
    else{
        // Clockwise
        temp = cores[s]->br[0]; 
        cores[s]->br[0] = cores[s]->br[1];
        cores[s]->br[1] = cores[s]->br[3];
        cores[s]->br[3] = cores[s]->br[2];
        cores[s]->br[2] = temp;
		
        temp_block = cores[s]->br[0]->c[0];
        cores[s]->br[0]->c[0] = cores[s]->br[0]->c[1];
        cores[s]->br[0]->c[1] = temp_block;
        
        temp_block = cores[s]->br[3]->c[0];
        cores[s]->br[3]->c[0] = cores[s]->br[3]->c[1];
        cores[s]->br[3]->c[1] = temp_block;
        
    }
	
    // We have to fix all the corners of the adjacent sides manually.
	// For each of these, we only have to update the corners for 2 sides.
	// This is because the bridges are being update and they are shared between adjacent sides.
	// As such updating the top side bridges will update the top of the 2 adjacent sides that are shared between the top and the turning side.
	// THen u update the opposite side (bottom in example) and will get all bridges update.
	// We will still have to updat ethe bridges directly touching the turning side however.
    if(s == 0){
		// Fixing Back and Top
		cores[4]->br[0] = cores[0]->br[0];
		cores[4]->br[1]->c[0] = cores[0]->br[0]->c[0];
		cores[4]->br[2]->c[0] = cores[0]->br[0]->c[1];

		// Left and Top
		cores[3]->br[0] = cores[0]->br[1];

		// Right and Top
		cores[1]->br[0] = cores[0]->br[2];
		
		// Front and Top
		cores[2]->br[0] = cores[0]->br[3];
		cores[2]->br[1]->c[0] = cores[0]->br[3]->c[0];
		cores[2]->br[2]->c[0] = cores[0]->br[3]->c[1];
    }
    else if(s == 5){
		// Same order as s == 1 except Bottom instead of Top.
		cores[4]->br[3] = cores[5]->br[0];
		cores[4]->br[1]->c[1] = cores[5]->br[0]->c[0];
		cores[4]->br[2]->c[1] = cores[5]->br[0]->c[1];

		cores[3]->br[3] = cores[5]->br[1];

		cores[1]->br[3] = cores[5]->br[2];

		cores[2]->br[3] = cores[5]->br[3];
		cores[2]->br[1]->c[1] = cores[5]->br[3]->c[0];
		cores[2]->br[2]->c[1] = cores[5]->br[3]->c[1];
    }
    else if(s == 1){
		// Updating Top and Right
		cores[0]->br[2] = cores[1]->br[0];
		cores[0]->br[0]->c[1] = cores[1]->br[0]->c[0];
		cores[0]->br[3]->c[1] = cores[1]->br[0]->c[1];

		// Updating Back and Right
		cores[4]->br[2] = cores[1]->br[1];

		// Updating Front and Right
		cores[2]->br[2] = cores[1]->br[1];
		
		// Updating Bottom and Right
		cores[5]->br[2] = cores[1]->br[3];
		cores[5]->br[0]->c[1] = cores[1]->br[3]->c[0];
		cores[5]->br[3]->c[1] = cores[1]->br[3]->c[1];
    }
    else if(s == 3){
        // Same order as s == 1 except Left instead of Right
		cores[0]->br[1] = cores[3]->br[0];
		cores[0]->br[0]->c[0] = cores[3]->br[0]->c[0];
		cores[0]->br[3]->c[0] = cores[3]->br[0]->c[1];

		cores[4]->br[1] = cores[3]->br[1];

		cores[2]->br[1] = cores[3]->br[1];

		cores[5]->br[1] = cores[3]->br[3];
		cores[5]->br[0]->c[0] = cores[3]->br[3]->c[0];
		cores[5]->br[3]->c[0] = cores[3]->br[3]->c[1];
    }
    else if(s == 2){
        // Updating Top and Front
		cores[0]->br[3] = cores[2]->br[0];
		cores[0]->br[1]->c[1] = cores[2]->br[0]->c[0];
		cores[0]->br[2]->c[1] = cores[2]->br[0]->c[1];

		// Updating Left and Front
		cores[3]->br[2] = cores[2]->br[1];

		// Updating Right and Front
		cores[1]->br[2] = cores[2]->br[2];

		// Updating Bottom and Front
		cores[5]->br[3] = cores[2]->br[3];
		cores[5]->br[1]->c[1] = cores[2]->br[3]->c[0];
		cores[5]->br[2]->c[1] = cores[2]->br[3]->c[1];
    }
    else if(s == 4){
		// Same order as s == 2 except Back instead of Front
		cores[0]->br[0] = cores[4]->br[0];
		cores[0]->br[1]->c[0] = cores[4]->br[0]->c[0];
		cores[0]->br[2]->c[0] = cores[4]->br[0]->c[1];

		cores[3]->br[1] = cores[4]->br[1];

		cores[1]->br[1] = cores[4]->br[2];

		cores[5]->br[0] = cores[4]->br[3];
		cores[5]->br[1]->c[0] = cores[4]->br[3]->c[0];
		cores[5]->br[2]->c[0] = cores[4]->br[3]->c[1];
    }
	
	
    
    // Now we rotate the individual blocks.
    cores[s]->br[0]->c[0]->rotate_block(s, d);
    cores[s]->br[0]->b->rotate_block(s, d);
    cores[s]->br[0]->c[1]->rotate_block(s, d);
    
    cores[s]->br[3]->c[0]->rotate_block(s, d);
    cores[s]->br[3]->b->rotate_block(s, d);
    cores[s]->br[3]->c[1]->rotate_block(s, d);
    
    cores[s]->br[1]->b->rotate_block(s, d);
    cores[s]->br[2]->b->rotate_block(s, d);

	// Now that all that nonsense is finished and the memory is updated, lets setup animations.
	if (anim){
		// Its very simple, we add/subtract from the opp or main degree variables and set the main rotator to 0, 1, 2.
		switch (s){
		case 0:
			main_rotator = 0;
			main_degrees += (90 * dir);
			break;
		case 1:
			main_rotator = 1;
			main_degrees += (90 * dir);
			break;
		case 2:
			main_rotator = 2;
			main_degrees += (90 * dir);
			break;
		case 3:
			main_rotator = 1;
			opp_degrees += (90 * dir);
			break;
		case 4:
			main_rotator = 2;
			opp_degrees += (90 * dir);
			break;
		case 5:
			main_rotator = 0;
			opp_degrees += (90 * dir);
			break;
		}
	}
    
}

/* This is an obvious method. It will randomly shuffle the cube
 * It will also add the events to the action log.
 * It will turn the cube without making animations.
 * This way the user cant see it shuffle.
 * Usually used to initialize a random cube.
 */
void RubixCube::shuffle_cube(int i){
    if(i == 1) turn_side(5, 1, true);
    if(i == 2) turn_side(5, -1, false);
    if(i == 3) turn_side(3, 1, false);
    if(i == 4) turn_side(2, 1, false);
	print_debug();
}

bool RubixCube::check_integrity(){
	if (cores[0]->br[0] != cores[4]->br[0]) cout << "Top Back Failed" << endl;
	if (cores[0]->br[1] != cores[3]->br[0]) cout << "Top Left Failed" << endl;
	if (cores[0]->br[2] != cores[1]->br[0]) cout << "Top Right Failed" << endl;
	if (cores[0]->br[3] != cores[2]->br[0]) cout << "Top Front Failed" << endl;
	if (cores[5]->br[0] != cores[4]->br[3]) cout << "Bottom Back Failed" << endl;
	if (cores[5]->br[1] != cores[3]->br[3]) cout << "Bottom Left Failed" << endl;
	if (cores[5]->br[2] != cores[1]->br[3]) cout << "Bottom Right Failed" << endl;
	if (cores[5]->br[3] != cores[2]->br[3]) cout << "Bottom Front Failed" << endl;
	return (cores[0]->br[0] == cores[4]->br[0] &&
		cores[0]->br[1] == cores[3]->br[0] &&
		cores[0]->br[2] == cores[1]->br[0] &&
		cores[0]->br[3] == cores[2]->br[0] &&
		cores[5]->br[0] == cores[4]->br[3] &&
		cores[5]->br[1] == cores[3]->br[3] &&
		cores[5]->br[2] == cores[1]->br[3] &&
		cores[5]->br[3] == cores[2]->br[3]);
}