#include "Parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <algorithm>

using namespace std;

void print_usage_info() {
  printf("\nUsage:\n"
         "\t./lobster_casting [-h] [-b <begin-casting-threshold>]\n"
         "\t[-e <end-casting-threshold>] [-o <odor-locomote-threshold>]\n"
         "\t[-f <food-stop-threshold>] [-m <T-max>] [-v <velocity>]\n\n"
         "'-h' or '--help' will display this program usage information\n"
         "'begin-casting-threshold' is in seconds and defines the amout of time after\n"
         "\tlosing the food odor that the robot will wait before casting left or\n"
         "\tright. Valid range is [1, 10]. Default value is 3.\n"
         "'end-casting-threshold' is in seconds and defines the amount of time the\n"
         "\trobot will cast left or right before returning to upstream direction.\n"
         "\tValid range is [1, 10]. Default value is 4.\n"
         "'odor-locomote-threshold' is in milliseconds and defines the continuous\n"
         "\tamount of time that odor must be detected before the robot will\n"
         "\tlocomote toward it. Valid range is [100, 2000]. Default value is 500.\n"
         "'food-stop-threshold' is in milliseconds and defines the continuous amount\n"
         "\tof time that food must be detected before the robot will stop (to\n"
         "\t'eat' it). Valid range is [100, 2000]. Default value is 500.\n"
         "'T-max' is in seconds and defines the maximum about time the robot will cast\n"
         "\tbefore it gives up and stops. Valid range is [5, 100]. Default value\n"
         "\tis 15.\n"
         "'velocity' is in centimeters per second. Valid range is [1, 50]. Default\n"
         "\tvalue is 10.\n\n");
}

Parameters parse_args(int argc, char **argv) {
  // Default parameter values
  Parameters rtn;
  rtn.odor_locomote_thresh    = 5;    // default 500 milliseconds (5 cm)
  rtn.food_stop_thresh        = 5;    // default 500 milliseconds (5 cm)
  rtn.begin_casting_threshold = 30;   // if see_nothing for 3 seconds, start casting
  rtn.end_casting_threshold   = 70;   // stop casting after 4 seconds
  rtn.T_max                   = 150;  // 15 seconds
  rtn.velocity                = 0.1;  // 10 cm/sec
  rtn.error                   = false;
  
  // Parse command-line arguements
  int option_index = 0;
  static struct option long_options[] = {
    {"default", 0, 0, 0 },
    {"help",    0, 0, 1 },
    {0,         0, 0, 0 }
  };
 
  int c = 0;
  int v = 0;
  while ((c = getopt_long(argc, argv, "o:f:m:b:e:v:h",
              long_options, &option_index)) != -1) {
    switch (c) {
      case 0: // "default"
        break;
      case 1: // "help"
        print_usage_info();
        rtn.error = true;
        break;
      case 'h':
        print_usage_info();
        rtn.error = true;
        break;
      case 'o':   // milliseconds, range = [100, 2000]
        v = atoi(optarg);
        v = min ( max (v, 100), 2000);
        rtn.odor_locomote_thresh = v / 100;
        break;
      case 'f':   // milliseconds, range = [100, 2000]
        v = atoi(optarg);
        v = min ( max (v, 100), 2000);
        rtn.food_stop_thresh = v / 100;
        break;
      case 'm':   // seconds, range = [5, 100]
        v = atoi(optarg);
        v = min ( max (v, 5), 100);
        rtn.T_max = v * 10;
        break;
      case 'b':   // seconds, range = [1, 10]
        v = atoi(optarg);
        v = min ( max (v, 1), 10);
        rtn.begin_casting_threshold = v * 10;
        break;
      case 'e':   // seconds, range = [1, 10]
        v = atoi(optarg);
        v = min ( max (v, 1), 10);
        rtn.end_casting_threshold = v * 10;
        break;
      case 'v':   // centimeters per second, range = [1, 50]
        v = atoi(optarg);
        v = min ( max (v, 1), 50);
        rtn.velocity = float(v) / 100.0;
        break;
      case '?':
        if (optopt == 'o' || optopt == 'f' || optopt == 'b' ||
            optopt == 'e' || optopt == 'v' || optopt == 'm')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        rtn.error = true;
        break;
      default:
        printf("Bad arguments.\n\n");
        print_usage_info();
        rtn.error = true;
        break;
    }
  }

  rtn.end_casting_threshold += rtn.begin_casting_threshold;
  rtn.T_max = min (rtn.T_max, rtn.end_casting_threshold);
  
  for (int i = optind; i < argc; i++) {
    printf ("Non-option argument %s\n", argv[i]);
    print_usage_info();
    rtn.error = true;
  }
  
  return rtn;
}
