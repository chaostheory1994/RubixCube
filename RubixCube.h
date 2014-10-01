/* 
 * File:   RubixCube.h
 * Author: jacob
 *
 * Created on September 25, 2014, 11:05 AM
 */

#ifndef RUBIXCUBE_H
#define	RUBIXCUBE_H
// Includes
#include <queue>
#include <stack>
#include "Block.h"
#include "main.h"

// Default colors, unless overriden by user.
#define R_COLOR0 0,0,0
#define R_COLOR1 0,0,0
#define R_COLOR2 0,0,0
#define R_COLOR3 0,0,0
#define R_COLOR4 0,0,0
#define R_COLOR5 0,0,0
#define R_COLOR_HIDDEN 0.33, 0.33, 0.33 /* Default is Grey */

// Various Directions
// Cube Sides
#define TOP 0
#define RIGHT 1
#define FRONT 2
#define LEFT 3
#define BACK 4
#define UNDER 5

using std::queue;
using std::stack;

class Block;

class RubixCube {
public:
    // Color Struct
    // The Blocks will use it, but the Rubix Cube will initialize it.
    // This way Colors are stored per rubix cube and as such can have different colors.
    // The id is for game mechanics.
    // rgb is the colors for opengl.
    
    RubixCube();
    RubixCube(const RubixCube& orig);
    virtual ~RubixCube();
    void draw(float);
    int turn_side(int, int);
    void update_cube();
    void solve_cube();
private:
    // The structure of the cube will have 3 layers.
    // The bridge blocks
    // Connects to adjacent Corners.
    struct bridge{
        Block *b;
        Block *c[2];
    };
    // The Core Blocks
    // Connects to adjacent bridges;
    struct core{
        Block *b;
        bridge *br[4];
    };
    
    // The Corner Blocks
    // However, these dont link to anything.
    // So no struct needed.
    // These layers are structured as such:
    // c=core b=bridge blk = corner
    // blk b blk
    // b   c   b
    // blk b blk
    // We will also add an event queue of sorts.
    // This will be done with a std queue.
    // First we will create what an event will look like.
    // The packet will hold an int with the usual side values.
    // Then another in with direction. >=0 = Clockwise. Else CounterClockwise
    struct queue_packet{
        int side;
        int direction;
    };
    // Now the queue
    queue<queue_packet> q;
    // We will also have an event log to help solve the rubix cube.
    stack<queue_packet> log;
    // The rubix cube will have 26 visible blocks.
    Color *colors[6];
    // The six sides;
    core *cores[6];
    void setup_color(Color*, int, int, int, int);
};

#endif	/* RUBIXCUBE_H */

