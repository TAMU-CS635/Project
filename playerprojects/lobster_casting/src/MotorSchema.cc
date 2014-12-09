#include "MotorSchema.h"

// Initialize workaround variables
bool MSchema::turned_left   = false;
bool MSchema::turned_right  = false;
bool MSchema::casting_left  = false;
bool MSchema::casting_right = false;

void MSchema::locomote(Robot &create_robot, Direction dir, Parameters p) {
  // turned_right, turned_left, casting_left, and casting_right are
  // variables used to keep track of upstream direction
  switch (dir) {
    case LEFT:
      if (turned_left)          // keep going left (staight)
        create_robot.move(p.velocity);
      else if (turned_right) {  // turn forward
        create_robot.turn(p.velocity, 0.5);
        turned_right = false;
      }
      else {                    // turn left
        create_robot.turn(p.velocity, 0.5);
        turned_left = true;
      }
      break;
    case RIGHT:
      if (turned_right)         // keep going right (staight)
        create_robot.move(p.velocity);
      else if (turned_left) {   // turn forward
        create_robot.turn(p.velocity, -0.5);
        turned_left = false;
      }
      else {                    // turn right
        create_robot.turn(p.velocity, -0.5);
        turned_right = true;
      }
      break;
    case FORWARD:
      if (turned_left) {
        create_robot.turn(p.velocity, -0.5);
        turned_left = false;
      }
      else if (turned_right) {
        create_robot.turn(p.velocity, 0.5);
        turned_right = false;
      }
  
      create_robot.move(p.velocity);
      break;
    default:
      //move forward
      create_robot.move(p.velocity);
      break;
  }
}
