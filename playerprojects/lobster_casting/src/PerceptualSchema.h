#ifndef PERCEPTUAL_SCHEMA_H_
#define PERCEPTUAL_SCHEMA_H_

#include "Robot.h"
#include "Parameters.h"

namespace PSchema {
  
  extern Robot::ir_values ir;
  extern int odor_detected;
  extern int food_detected;
  
  
  bool detect_food(Robot &create_robot, Parameters p);
  
  void detect_odor(Robot &create_robot, int &T_since_last_detected, Parameters p);
  
  Direction detect_mean_flow(Robot &create_robot, Parameters p);

  Direction random();
  
};

#endif // PERCEPTUAL_SCHEMA_H_
