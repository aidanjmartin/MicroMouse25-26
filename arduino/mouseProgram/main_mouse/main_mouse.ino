#include "solver.h"

// Define pin assignments for motors and sensors


// Function prototypes for motor control
void goForward();
void turnLeft();
void turnRight();
void stopMotors();

// Setup func
void setup() {
  // Initialize serial for debug
  Serial.begin(115200);

  // Initialize the micromouse state
  initializeFlood();
}

// Loop func
void loop() {
  // get next action from solver
  Action nextMove = solver();

  switch (nextMove) {
    case FORWARD:
      goForward();
      break;
    case LEFT:
      turnLeft();
      break;
    case RIGHT:
      turnRight();
      break;
    case IDLE:
      stopMotors();
      break;
  }
}