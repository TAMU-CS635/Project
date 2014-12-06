#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "Create.h"

struct Parameters {
  
  /* Static thresholds */
  const int food_ir_thresh      = 2000; // IR reflectivity over threshold marks food
  const int odor_ir_thresh      = 750;  // IR reflectivity below threshold marks food odor
  
  /* User-defined parameters */
  int odor_locomote_thresh;
  int food_stop_thresh;
  int begin_casting_thresh;
  int end_casting_thresh;
  int velocity;
};

Parameters parse_args(int argc, char **argv);

#endif // PARAMETERS_H_
