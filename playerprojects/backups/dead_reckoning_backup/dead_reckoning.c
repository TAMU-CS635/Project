#include <stdio.h>
#include <getopt.h>

#include <libplayerc/playerc.h>

int
main(int argc, char **argv)
{
  // Initialize movement parameters  (default to a square)
  int n = 4;
  int l = 65;
  int v = 50;
  int a = 90;

  // Parse command-line arguements
  int option_index = 0;
  static struct option long_options[] = {
    {"line",     0, 0, 0 },
    {"triangle", 0, 0, 1 },
    {"square",   0, 0, 2 },
    {"pentagon", 0, 0, 3 },
    {"hexagon",  0, 0, 4 },
    {0,          0, 0, 0 }
  };

  int shape_set = false;
  int angle_set = false;
  int num_set = false;
  int c;
  while ((c = getopt_long(argc, argv, "l:v:n:a:",
              long_options, &option_index)) != -1)
    switch (c) {
      case 0: // line
        n = 2; a = 180; shape_set = true; break;
      case 1: // triangle
        n = 3; a = 120; shape_set = true; break;
      case 2: // square
        n = 4; a = 90; shape_set = true; break;
      case 3: // pentagon
        n = 5; a = 72; shape_set = true; break;
      case 4: // hexagon
        n = 6; a = 60; shape_set = true; break;
      case 'l': // 0 to 200 cm (default 100)
        l = atoi(optarg); break;
      case 'v': // 0 to 50 cm/sec (default 50)
        v = atoi(optarg); break;
      case 'n': // 0 to 10 (default 4)
        n = atoi(optarg); num_set = true; break;
      case 'a': // 0 to 180 degrees (default 90)
        a = atoi(optarg); angle_set = true; break;
      case '?':
        if (optopt == 'v' || optopt == 'a' || optopt == 'l' || optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        printf("Bad arguments.\n\nUsage: dead_reckoning -n <number of sides> "
               "-l <length of each side> -v <velocity (cm/sec)> "
               "-a <turning angle (degrees)>");
        exit(1);
    }
  int i = 0;
  for (i = optind; i < argc; i++)
    printf ("Non-option argument %s\n", argv[i]);

  if(!shape_set && (!num_set || !angle_set)) {
    printf("\nYou need to specify a shape (by name, or with the arguments "
           "'n' and 'a').\n\nUsage: dead_reckoning --<shape> "
           "[-l <side length>] [-v <velocity>]\n\n"
           "Alternative usage: dead_reckoning -n <number of sides> "
           "-a <angle of each corner in degrees> "
           "[-l <length of each side>] [-v <velocity>]\n\n"
           "Valid shapes are: line, triangle, square, pentagon, hexagon\n"
           "Side length is in cm. Valid range is [0,200]. Default value is 65.\n"
           "Velocity is in cm/sec. Valid range is [0,50]. Default value is 50.\n\n"
           "e.g. dead_reckoning --hexagon\n"
           "e.g. dead_reckoning --hexagon -l 100\n"
           "e.g. dead_reckoning --hexagon -l 100 -v 40\n"
           "e.g. dead_reckoning -n 6 -a 60 -l 100 -v 40\n\n");
    exit(1);
  }

  // Initialize player
  i = 0;
  playerc_client_t *client;
  playerc_position2d_t *position2d;

  // Create a client and connect it to the server.
  client = playerc_client_create(NULL, "localhost", 6665);
  if (0 != playerc_client_connect(client))
    return -1;

  // Create and subscribe to a position2d device.
  position2d = playerc_position2d_create(client, 0);
  if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE))
    return -1;

  // Read initial position
  playerc_client_read(client);
  double start_x = position2d->px;
  double start_y = position2d->py;
  double start_a = position2d->pa;

  // Convert parameters to motor control arguments
  double vel = 0.01 * v;
  int move_time = l * 28000;
  move_time *= 50.0 / (double)v;
  double angle_vel = -1.57;
  int turn_time = (int)(((double)a/90.0) * 560000);

  // Move the robot (e.g. square)
  for (i = 0; i < n; ++i) {
    if (playerc_position2d_set_cmd_vel(position2d, vel, 0, 0, 1)) return -1;
    usleep(move_time);
    if (playerc_position2d_set_cmd_vel(position2d, 0, 0, angle_vel, 1)) return -1;
    usleep(turn_time);
  }
  // Stop
  if (playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1)) return -1;
  
  // Read final position, get error
  playerc_client_read(client);
  double error_x = position2d->px - start_x;
  double error_y = position2d->py - start_y;
  double error_a = position2d->pa - start_a;

  // Report odometry error
  printf("\n\nMovement error according to onboard odometry:\n"
         "\tX pos: %fcm\n\tY pos: %fcm\n\tAngle: %f degrees\n",
         error_x * 100, error_y * 100, error_a * -57.325);

  // Shutdown
  playerc_position2d_unsubscribe(position2d);
  playerc_position2d_destroy(position2d);
  playerc_client_disconnect(client);
  playerc_client_destroy(client);

  return 0;
}
