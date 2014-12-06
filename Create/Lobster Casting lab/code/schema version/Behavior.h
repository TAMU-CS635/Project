#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "Robot.h"
#include "Parameters.h"
#include "MotorSchema.h"
#include "PerceptualSchema.h"

#define CYCLE_TIME 10000

namespace Behavior {
  
  bool find_food(Robot &create_robot, bool hungry, Parameters p);
  
  void move_upstream(Robot &create_robot, Parameters p);
  
  void cast(Robot &create_robot, int T_since_last_detected, Parameters p);
  
};

#endif // BEHAVIOR_H_
