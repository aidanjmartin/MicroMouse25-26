#include "solver.h"

void goForward();
void turnLeft();
void turnRight();
void stopMotors();

void setup() {
  Serial.begin(115200);
  initializeFlood();
}

void loop() {
  Action nextMove = solver();

  switch (nextMove) {
    case FORWARD: goForward();  break;
    case LEFT:    turnLeft();   break;
    case RIGHT:   turnRight();  break;
    case IDLE:    stopMotors(); break;
  }
}