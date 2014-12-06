#include "MotorSchema.h"

void MSchema::locomote(Robot &create_robot, Direction dir, Parameters p) {
  // turned_right, turned_left, casting_left, and casting_right are
  // variables used to keep track of upstream direction
  switch (dir) {
    case LEFT:
      if (!turned_right && !turned_left) {
        create_robot.turn(p.velocity, 0.5);
        turned_left = 0;
      }
      break;
    case RIGHT:
      if (!turned_left && !turned_right) {
        create_robot.turn(p.velocity, -0.5);
        turned_right = 0;
      }
      break;
    case FORWARD;
      if (turned_left || casting_left)
        create_robot.turn(p.velocity, -0.5);
      else if (turned_right || casting_right)
        create_robot.turn(p.velocity, 0.5);
  
      create_robot.move(p.velocity);
      break;
    default:
      //move forward
      create_robot.move(p.velocity);
      break;
  }
}
