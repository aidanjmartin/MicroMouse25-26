#include "solver.h"
#include "sensors.h"
#include <Arduino.h>

#define MAZE_SIZE 16

bool walls[MAZE_SIZE][MAZE_SIZE][4] = {{{false}}};
int maze[MAZE_SIZE][MAZE_SIZE];
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
      maze[x][y] = abs(x - 7) + abs(y - 7);
    }
  }
}

void floodUpdate() {
  queueStart = 0;
  queueEnd = 0;

  for (int x = 0; x < MAZE_SIZE; x++) {
    for (int y = 0; y < MAZE_SIZE; y++) {
      maze[x][y] = 255;
    }
  }

  for (int x = 7; x <= 8; x++) {
    for (int y = 7; y <= 8; y++) {
      maze[x][y] = 0;
      enqueue(x, y);
    }
  }

  while (!queueIsEmpty()) {
    Cell c = dequeue();
    int currentDist = maze[c.x][c.y];

    for (int dir = 0; dir < 4; dir++) {
      int nx = c.x + dx[dir];
      int ny = c.y + dy[dir];

      if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE) continue;
      if (walls[c.x][c.y][dir]) continue;

      int newDist = currentDist + 1;
      if (maze[nx][ny] > newDist) {
        maze[nx][ny] = newDist;
        enqueue(nx, ny);
      }
    }
  }
}

void updateWalls() {
  WallStates currentWalls = getWallStates();

  if (currentWalls.front) walls[mouseX][mouseY][direction]           = true;
  if (currentWalls.right) walls[mouseX][mouseY][(direction + 1) % 4] = true;
  if (currentWalls.left)  walls[mouseX][mouseY][(direction + 3) % 4] = true;

  // Mirror each wall onto the neighbor's opposite face so lookups from either side agree.
  for (int dir = 0; dir < 4; dir++) {
    int nx = mouseX + dx[dir];
    int ny = mouseY + dy[dir];
    int opp = (dir + 2) % 4;

    if (nx >= 0 && nx < MAZE_SIZE && ny >= 0 && ny < MAZE_SIZE) {
      walls[nx][ny][opp] = walls[mouseX][mouseY][dir];
    }
  }
}

Action solver() {
  return floodFill();
}

Action leftWallFollower() {
  WallStates w = getWallStates();
  if (w.front) {
    if (w.left) return RIGHT;
    return LEFT;
  }
  return FORWARD;
}

Action floodFill() {
  static int pendingUTurn = 0;

  // A 180 is executed as two consecutive RIGHT turns; the second fires here on the next tick.
  if (pendingUTurn > 0) {
    pendingUTurn--;
    return RIGHT;
  }

  updateWalls();
  floodUpdate();

  int currentVal = maze[mouseX][mouseY];
  int bestDir = -1;
  int bestVal = currentVal;

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

  if (bestDir == -1) return IDLE;

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
    direction = (direction + 2) % 4;
    pendingUTurn = 1;
    return RIGHT;
  }
  return IDLE;
}
