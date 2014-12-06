#include "Parameters.h"

Parameters parse_args(argc, argv) {
}

Parameters parse_args(int argc, char **argv, int &a, int&b) {
  // Default parameter values
  Parameters rtn;
  rtn.odor_locomote_thresh  = 10;   // default 0.1 second (1 cm)
  rtn.food_stop_thresh      = 10;   // default 0.1 second (1 cm)
  rtn.begin_casting_thresh  = 30;   // if see_nothing for 3 seconds, start casting
  rtn.end_casting_thresh    = 50;   // stop casting after 2 seconds
  rtn.velocity              = 0.1;  // 10 cm/sec
  
  // Parse command-line arguements
  int option_index = 0;
  static struct option long_options[] = {
    {"default", 0, 0, 0 },
    {0,         0, 0, 0 }
  };
 
  int c;
  while ((c = getopt_long(argc, argv, "o:f:b:e:v:",
              long_options, &option_index)) != -1)
    switch (c) {
      case 0: // "default"
        break;
      case 'o':
        int v = atoi(optarg);
        // range check here
        rtn.odor_locomote_thresh = atoi(optarg);
        break;
      case 'f':
        rtn.food_stop_thresh = atoi(optarg);
        break;
      case 'b':
        rtn.begin_casting_thresh = atoi(optarg);
        break;
      case 'e':
        rtn.end_casting_thresh = atoi(optarg);
        break;
      case 'v':
        rtn.velocity = atoi(optarg);
        break;
      case '?':
        if (optopt == 'b' || optopt == 'e' || optopt == 'v')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        printf("Bad arguments.\n\nUsage: lobster_casting [-b <#>] [-e <#>] [-v <#>]\n");
        return 1;
    }
  
  for (int i = optind; i < argc; i++)
    printf ("Non-option argument %s\n", argv[i]);
  
  return rtn;
}