#include <Arduino.h>
#include "sensors.h"

#define FRONT_SENSOR_PIN        A0
#define FRONT_LEFT_SENSOR_PIN   A1
#define FRONT_RIGHT_SENSOR_PIN  A2
#define LEFT_SENSOR_PIN         A3
#define RIGHT_SENSOR_PIN        A4

// Tune empirically against the actual IR sensors before competition.
#define SHORT_RANGE_THRESHOLD   500
#define LONG_RANGE_THRESHOLD    300

WallStates getWallStates() {
  WallStates states;

  int fl_val = analogRead(FRONT_LEFT_SENSOR_PIN);
  int fr_val = analogRead(FRONT_RIGHT_SENSOR_PIN);
  int l_val  = analogRead(LEFT_SENSOR_PIN);
  int r_val  = analogRead(RIGHT_SENSOR_PIN);

  states.front = (fl_val > SHORT_RANGE_THRESHOLD) || (fr_val > SHORT_RANGE_THRESHOLD);
  states.left  = (l_val  > SHORT_RANGE_THRESHOLD);
  states.right = (r_val  > SHORT_RANGE_THRESHOLD);

  return states;
}

