#ifndef SOLVER_H
#define SOLVER_H

typedef enum Heading {NORTH, EAST, SOUTH, WEST} Heading;
typedef enum Action {LEFT, FORWARD, RIGHT, IDLE} Action;

// These functions are implemented in solver.cpp
Action solver();
Action leftWallFollower();
Action floodFill();

#endif