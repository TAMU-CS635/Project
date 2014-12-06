#include "Bahavior.h"

bool Behavior::find_food(Robot &create_robot, bool hungry, Parameters p) {
  if (!hungry) {
    printf("Not hungry; stopping\n");
    return false;
  }

  // find_food behavior keeps track of time since odor was last detected
  static int T_since_last_detected = 0;
  while (true) {
    // if detect_food perceptual schema returns true, robot stops
    if (PSchema::detect_food(create_robot))
      return true;
    
    // update T_since_last_detected with detect_odor perceptual schema
    PSchema::detect_odor(create_robot, T_since_last_detected, p);
    
    if (T_since_last_detected == 0)
      Behavior::move_upstream(create_robot, p);
    else if (T_since_last_detected > 0 &&
             T_since_last_detected < p.T_max)
      Behavior::cast(create_robot, T_since_last_detected, p);
    else { //T_since_last_detected >= p.T_max
      printf("Reached Tmax; giving up\n");
      return false;
    }
    
    // repeat on a 10 ms cycle
    usleep(CYCLE_TIME);
  }
}

void Behavior::move_upstream(Robot &create_robot, Parameters p) {
  Direction dir = PSchema::detect_mean_flow(create_robot, p);
  MSchema::locomote(create_robot, dir, p);
}

// Create robots aren't omnidirectional like lobsters are. So to cast we need to
// turn one direction, go straight for a while, then turn back forward and repeat
void Behavior::cast(Robot &create_robot, int T_since_last_detected, Parameters p) {
  // T_casting_timer enables repeating casting behavior 
  int T_casting_timer = T_since_last_detected % (p.end_casting_threshold + 1);
  
  static Direction dir = FORWARD;
  // At begin_casting_threshold, robot casts randomly left or right
  if (T_casting_timer == p.begin_casting_threshold) {
    dir = PSchema::random();
    MSchema::locomote(create_robot, dir, p);
    if (dir == LEFT)
      MSchema::casting_left = true;
    else
      MSchema::casting_right = true;
  }
  // At end_casting_threshold, robot returns upstream
  if (T_casting_timer == p.end_casting_threshold) {
    MSchema::locomote(create_robot, dir == LEFT? RIGHT : LEFT, p);
    if (dir == LEFT)
      MSchema::casting_left = false;
    else
      MSchema::casting_right = false;
  }
  // Robot moves in straight lines
  else
    Mschema::locomote(create_robot, FORWARD, p);
}
