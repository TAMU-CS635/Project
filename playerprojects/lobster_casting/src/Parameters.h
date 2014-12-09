#ifndef PARAMETERS_H_
#define PARAMETERS_H_

struct Parameters {
  
  /* Static thresholds */
  static const int food_ir_thresh      = 2500; // IR reflectivity over threshold marks food
  static const int odor_ir_thresh      = 750;  // IR reflectivity below threshold marks food odor
  
  /* User-defined parameters */
  int odor_locomote_thresh;
  int food_stop_thresh;
  int begin_casting_threshold;
  int end_casting_threshold;
  int T_max;
  float velocity;

  /* Error check */
  bool error;
};

Parameters parse_args(int argc, char **argv);

#endif // PARAMETERS_H_
