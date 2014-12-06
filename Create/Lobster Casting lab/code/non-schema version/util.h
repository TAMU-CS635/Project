#ifndef UTIL_H_
#define UTIL_H_

int parse_args(int argc, char **argv,
               int &food_stop_thresh,
               int &odor_locomote_thresh,
               int &begin_casting_thresh,
               int &end_casting_thresh,
               int &Tmax,
               float &velocity);

#endif // UTIL_H_
