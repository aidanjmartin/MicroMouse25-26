#ifndef SENSORS_H
#define SENSORS_H

typedef struct {
  bool front;
  bool left;
  bool right;
} WallStates;

WallStates getWallStates();

#endif