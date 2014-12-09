#ifndef MOTOR_SCHEMA_H_
#define MOTOR_SCHEMA_H_

#include "Robot.h"
#include "Parameters.h"

namespace MSchema {
  
  // Workaround variables to keep track of 'upstream' direction
  extern bool turned_left;
  extern bool turned_right;
  extern bool casting_left;
  extern bool casting_right;

  
  void locomote(Robot &create_robot, Direction dir, Parameters p);
  
};

#endif // MOTOR_SCHEMA_H_
