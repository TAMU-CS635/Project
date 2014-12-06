#include "Create.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>
#include <libplayerc/playerc.h>
 
int main(int argc, char **argv) {
  /* Threshold parameters */
  int food_stop_thresh      = 10;   // need to see food for 1 cm consecutively (100ms) to stop
  int odor_locomote_thresh  = 10;   // need to smell food for 100ms to change direction
  int begin_casting_thresh  = 30;   // if see_nothing for 3 seconds, start casting
  int end_casting_thresh    = 70;   // stop casting after 2 seconds
  int Tmax                  = 150;  // Tmax
  float velocity            = 0.1;  // default 10 cm/sec
  
  if (parse_args(argc, argv,
                 food_stop_thresh,
                 odor_locomote_thresh,   
                 begin_casting_thresh,
                 end_casting_thresh,
                 Tmax,
                 velocity))
    return 1;
  
  /* Set up Player connection to Create */
  Create create;
  create.lights(0, 0, 0);

  /* Spin in place to read IR values and normalize them */
  printf("\nCalibrating IR sensors... ");
  create.calibrate_ir();

  /* Pause to let user straighten out the robot */
  printf("IR calibration done\n");
  printf("Straighten out the Create robot if needed, press enter when done.\n");
  getchar();
  printf("Beginning lobster casting behavior\n");
  
  /* Initialize Forward Movement 
        - Move at 10 cm/sec and read sensors at rate of 100Hz */
  create.move(velocity);
 
  // consecutive cycles where the following states are seen
  int found_scent_ahead = 0;
  int found_scent_left = 0;
  int found_scent_right = 0;
  int found_food = 0;
  int see_nothing = 0;
  int giveup_time = 0;
  
  /* THRESHOLDS */
  const int food_thresh = 2500;       // IR reflectivity over threshold marks food
  const int scent_thresh = 750;       // IR reflectivity below threshold marks food odor
  
  bool moving_to_scent = false;
  bool turned_left = false;
  bool turned_right = false;
  bool cast_left = false;
  bool casted_left = false;
  bool casted_right = false;

  srand(time(NULL));
 
  do {
    // get sensor values
    Create::ir_values ir = create.read_ir();
    printf("IR left: %i\n", ir.left);
    printf("IR front-left: %i\n", ir.fleft);
    printf("IR front-right: %i\n", ir.fright);
    printf("IR right: %i\n\n", ir.right);
 
    // Found food source
    if (ir.left > food_thresh || ir.fleft > food_thresh ||
        ir.fright > food_thresh || ir.right > food_thresh) {
      // update found_food count
      ++found_food;
      printf("Glimpsed food\n");
      if (found_food >= food_stop_thresh) {
        // notify we found food, stop
        create.lights(255, 0, 255);
        create.move(0);
        printf("Detected food\n");
        break;
      }
    }
    else {
      found_food = 0;
    }
 
    // Don't see either food or scent (i.e. only see butcher paper)
    if (ir.left > scent_thresh && ir.left < food_thresh &&
        ir.fleft > scent_thresh && ir.fleft < food_thresh &&
        ir.fright > scent_thresh && ir.fright < food_thresh &&
        ir.right > scent_thresh && ir.right < food_thresh) {
      // Update see_nothing count
      ++see_nothing;
      ++giveup_time;
      create.lights(0, 255, 255);
      //create.move(0.1);
      printf("Detected nothing: %i\n", see_nothing);
      // casting behavior
      if (see_nothing == begin_casting_thresh) {
        cast_left = ((float)rand() / RAND_MAX) < 0.5 ? true : false;
        if (cast_left) {
          create.turn(velocity, 0.5);
          casted_left = true;
          }
        else {
          create.turn(velocity, -0.5);
          casted_right = true;
        }
      }
      if (see_nothing >= begin_casting_thresh &&
          see_nothing < end_casting_thresh) {
        // notify we are casting
        //create.blink_led();
        create.lights(0, 0, 255);
        printf("Casting\n");
      }
      if (see_nothing == end_casting_thresh) {
        see_nothing = 0;
        create.lights(0, 255, 255);
        printf("Stopped casting\n");
        if (cast_left) {
          create.turn(velocity, -0.5);
          casted_left = false;
        }
        else {
          create.turn(velocity, 0.5);
          casted_right = false;
        }
      }
      if (giveup_time >= Tmax) {
        printf("Reached Tmax. Giving up.\n");
        create.lights(0, 0, 0);
        create.move(0);
        break;
      }
    }
    else {
      see_nothing = 0;
      giveup_time = 0;
    }
 
    // Sense food scent ahead
    if (ir.fleft < scent_thresh || ir.fright < scent_thresh) {
      ++found_scent_ahead;
      // continue forward
      create.move(velocity);
      // notify we have found food scent
      create.lights(255, 0, 0);
      printf("Glimpsed odor ahead\n");
      if (found_scent_ahead >= odor_locomote_thresh)
        printf("Detected odor ahead\n");
      if (casted_left) {
        create.turn(velocity, -0.5);
        casted_left = false;
      }
      if (casted_right) {
        create.turn(velocity, 0.5);
        casted_right = false;
      }
    }
    else {
      found_scent_ahead = 0;
      create.lights(0, 0, 0);
    }
    
    // sense food scent to left
    if (ir.left < scent_thresh) {
      ++found_scent_left;
      printf("Glimpsed odor left\n");
      if (found_scent_left >= odor_locomote_thresh &&
          found_scent_right == 0 &&         // don't turn if there is also food scent to right
          !turned_left && !turned_right) {  // and don't turn if already turned
        // notify we have found food scent
        create.lights(255, 0, 0);
        // move left until no longer see food on left, then turn forward
        create.turn(velocity, 0.5);
        turned_left = true;
        printf("Detected odor left\n");
      }
      if (turned_right || casted_right) {
        create.turn(velocity, 0.5);
        turned_right = false;
        casted_right = false;
      }
    }
    else {
      if (turned_left) {
        create.turn(velocity, -0.5);
        turned_left = false;
      }
      found_scent_left = 0;      
      create.lights(0, 0, 0);
    }
    // sense food scent to right
    if (ir.right < scent_thresh) {
      ++found_scent_right;
      printf("Glimpsed odor right\n");
      if (found_scent_right >= odor_locomote_thresh &&
          found_scent_left == 0 &&          // don't turn if there is also food scent to left
          !turned_left && !turned_right) {  // and don't turn if already turned
        // notify we have found food scent
        create.lights(255, 0, 0);
        // move right until no longer see food on right, then turn forward
        create.turn(velocity, -0.5);
        turned_right = true;
        printf("Detected odor right\n");
      }
      if (turned_left || casted_left) {
        create.turn(velocity, -0.5);
        turned_left = false;
        casted_left = false;
      }
    }
    else {
      if (turned_right) {
        create.turn(velocity, 0.5);
        turned_right = false;
      }
      found_scent_right = 0;
      create.lights(0, 0, 0);
    }

    usleep(10000);
  } while(true);
 
  create.shutdown();

  return 0;
}

