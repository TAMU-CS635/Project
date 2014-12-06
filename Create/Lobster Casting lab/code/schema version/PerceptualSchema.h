#ifndef PERCEPTUAL_SCHEMA_H_
#define PERCEPTUAL_SCHEMA_H_

#include "Robot.h"
#include "Parameters.h"

enum Direction {
  LEFT,
  RIGHT,
  FORWARD
};

namespace PSchema {
  
  int odor_detected = 0;
  
  
  bool detect_food(Robot &create_robot, Parameters p);
  
  void detect_odor(Robot &create_robot, int &T_since_last_detected, Parameters p);
  
  Direction detect_mean_flow(Robot &create_robot, Parameters p);
  
};

#endif // PERCEPTUAL_SCHEMA_H_
