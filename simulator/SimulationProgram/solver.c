#include "solver.h"
#include "API.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MAZE_SIZE 16


bool walls[MAZE_SIZE][MAZE_SIZE][4] = {{{false}}};
int maze [MAZE_SIZE][MAZE_SIZE];
int mouseX = 0;
int mouseY = 0;
int direction = 0; // 0=N 1=E 2=S 3=W
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

typedef struct {
    int x, y;
} Cell;

#define QUEUE_SIZE (MAZE_SIZE * MAZE_SIZE)
Cell queue[QUEUE_SIZE];
int queueStart = 0;
int queueEnd = 0;

void enqueue(int x, int y) {
    queue[queueEnd].x = x;
    queue[queueEnd].y = y;
    queueEnd = (queueEnd + 1) % QUEUE_SIZE;
}

Cell dequeue() {
    Cell c = queue[queueStart];
    queueStart = (queueStart + 1) % QUEUE_SIZE;
    return c;
}

int queueIsEmpty() {
    return queueStart == queueEnd;
}

void initializeFlood() {
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            maze[x][y] = abs(x - 7) + abs(y - 7); //Goal : center
        }
    }
}

void updateWalls() {
    // Current cell
    if (API_wallFront()) {
        walls[mouseX][mouseY][direction] = true;
    }
    if (API_wallRight()) {
        walls[mouseX][mouseY][(direction + 1) % 4] = true;
    }
    if (API_wallLeft()) {
        walls[mouseX][mouseY][(direction + 3) % 4] = true;
    }


    // Mirror walls to neighboring cells
    for (int dir = 0; dir < 4; dir++) {
        int nx = mouseX + dx[dir];
        int ny = mouseY + dy[dir];
        int opp = (dir + 2) % 4;

        if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE) {
            walls[nx][ny][opp] = walls[mouseX][mouseY][dir];
        }
    }
    // update neighbor's opposite walls, add later
}   


void floodUpdate() {
    // Initialize queue with goal cells (center of maze)
    queueStart = 0;
    queueEnd = 0;

    // reset and seed center cells
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            maze[x][y] = 255; // High value
        }
    }

    // Set center cells flood to 0 and enqueue them
    for (int x = 7; x <= 8; x++) {
        for (int y = 7; y <= 8; y++) {
            maze[x][y] = 0;
            enqueue(x, y);
        }
    }

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    while (!queueIsEmpty()) {
        Cell c = dequeue();
        int cx = c.x;
        int cy = c.y;
        int currentDist = maze[cx][cy];

        for (int dir = 0; dir < 4; dir++) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];

            if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE) continue;
            // Check for wall between (cx, cy) and (nx, ny)
            if (walls[cx][cy][dir]) continue;

            int newDist = currentDist + 1;
            if (maze[nx][ny] > newDist) {
                maze[nx][ny] = newDist;
                enqueue(nx, ny);
            }
        }
    }
}




Action solver() {
    return floodFill();
}

// This is an example of a simple left wall following algorithm.
Action leftWallFollower() {
    if(API_wallFront()) {
        if(API_wallLeft()){
            return RIGHT;
        }
        return LEFT;
    }
    return FORWARD;
}



bool isWall(int absDir) {
    // dir: 0 = N, 1 = E, 2 = S, 3 = W
    int rel = (absDir - direction + 4) % 4;

    if (rel == 0) return API_wallFront();
    if (rel == 1) return API_wallRight();
    if (rel == 3) return API_wallLeft();
    // rel == 2 means wall behind, no movement for that
    return true; // assumes wall behind
} 


// Put your implementation of floodfill here!
Action floodFill() {
    static int initialized = 0;
    static int pendingUTurn = 0;
    if (pendingUTurn > 0) {
        pendingUTurn--;
        return RIGHT;
    }
    if (!initialized) {
        initializeFlood();
        initialized = 1;
    }

    updateWalls(); // read and store walls from sensors
    floodUpdate(); // recompute flood values

    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            char text[4];
            sprintf(text, "%d", maze[x][y]);
            API_setText(x, y, text);
        }
    }

    int currentVal = maze[mouseX][mouseY];
    int bestDir = -1;
    int bestVal = currentVal;

    //Check all four directions
    for (int dir = 0; dir < 4; dir++) {
        int nx = mouseX + dx[dir];
        int ny = mouseY + dy[dir];

        if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE) continue;
        if (walls[mouseX][mouseY][dir]) continue;

        if (maze[nx][ny] <= bestVal) {
            bestVal = maze[nx][ny];
            bestDir = dir;
        }
    }


    if (bestDir == -1) return IDLE; // if no move found

    // Determine turn needed
    int turn = (bestDir - direction + 4) % 4;

    if (turn == 0) {
        mouseX += dx[direction];
        mouseY += dy[direction];
        return FORWARD;
    } else if (turn == 1) {
        direction = (direction + 1) % 4;
        return RIGHT;
    } else if (turn == 3) {
        direction = (direction + 3) % 4;
        return LEFT;
    } else if (turn == 2) {
        // 180 turn
        direction = (direction + 2) % 4;
        pendingUTurn = 1; // triggers 2nd turn next loop
        return RIGHT;
    }
    return IDLE;
}   
