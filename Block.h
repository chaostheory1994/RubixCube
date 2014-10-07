/* 
 * File:   Block.h
 * Author: jacob
 *
 * Created on September 25, 2014, 11:05 AM
 */

#ifndef BLOCK_H
#define	BLOCK_H
#include "RubixCube.h"
#include "main.h"


class Block {
public:
    Block(Color**);
    Block(int, Color*);
    Block(const Block& orig);
    virtual ~Block();
    void draw();
    void change_side_color(int, Color*);
    void print_debug();
    Color *get_color(int);
    void rotate_block(int, int);
private:
    // At most there will only be 3 visible sides
    // However, i will sent non visible sides to NULL because reasons.
    // 0 = Top 1 = Right 2 = Front 3 = Left 4 = back 5 = Bottom
    Color *sides[6];
    // Rotation variables
    int rotX, rotY, rotZ;
};

#endif	/* BLOCK_H */

