#include "PerceptualSchema.h"

bool PSchema::detect_food(Robot &create_robot, Parameters p) {
  // food_detected is a signal processing counter to avoid false positives
  static int food_detected = 0;  
  Robot::ir_values ir = create_robot.read_ir();
  
  if (ir.left > p.food_ir_thresh  || ir.fleft > p.food_ir_thresh ||
      ir.fright > p.food_ir_thresh  || ir.right > p.food_ir_thresh )
    ++food_detected;
  else
    food_detected = 0;
  
  // only return true if food was detected for food_stop_thresh consecutive cycles
  if (food_detected > p.food_stop_thresh)
    return true;
  else
    return false;
}

void PSchema::detect_odor(Robot &create_robot, int &T_since_last_detected, Parameters p) {
  Robot::ir_values ir = create_robot.read_ir();
  
  // odor_detected is a signal processing counter to avoid false positives
  // it is stored in PSchema so that it can be accessed by detect_mean_flow
  if (ir.left < p.odor_ir_thresh  || ir.fleft < p.odor_ir_thresh ||
      ir.fright < p.odor_ir_thresh  || ir.right < p.odor_ir_thresh ) {
    T_since_last_detected = 0;
    ++odor_detected;
  }
  else {
    ++T_since_last_detected;
    odor_detected = 0;
  }
}

Direction PSchema::detect_mean_flow(Robot &create_robot, Parameters p) {
  // only return a left/right direction if odor was detected for
  // odor_locomote_thresh consecutive cycles by detect_odor
  if (odor_detected < p.odor_locomote_thresh)
    return FORWARD;
  else if (ir.left < p.odor_ir_thresh && ir.right > p.odor_ir_thresh)
    return LEFT;
  else if (ir.right < p.odor_ir_thresh && ir.left > p.odor_ir_thresh)
    return RIGHT;
  else
    return FORWARD;
}

Direction PSchema::random() {
  return ((float) rand() / RAND_MAX > 0.5) ? LEFT : RIGHT;
}