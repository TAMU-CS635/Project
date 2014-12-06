#ifndef MOTOR_SCHEMA_H_
#define MOTOR_SCHEMA_H_

#include "Robot.h"
#include "Parameters.h"

namespace MSchema {
  
  // Workaround variables to keep track of 'upstream' direction
  bool turned_left      = false;
  bool turned_right     = false;
  bool casting_left     = false;
  bool casting_right    = false;

  
  locomote(Robot &create_robot, Direction dir, Parameters p);
  
};

#endif // MOTOR_SCHEMA_H_
