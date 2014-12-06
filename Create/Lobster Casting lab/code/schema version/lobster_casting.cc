#include "Robot.h"
#include "Behavior.h"
#include "PerceptualSchema.h"
#include "MotorSchema.h"
#include "Parameters.h"
 
int main(int argc, char **argv) {
  /* Parse program parameters from command line arguments */
  Parameters p = parse_args(argc, argv);
  
  /* Initialize Create Robot */
  Robot create_robot;
  create_robot.lights_off();
  create_robot.calibrate_ir();
  
  /* Lobster Casting behavior */
  const bool hungry = true;
  
  if (Behavior::find_food(create_robot, hungry, p))
    printf("Found food.\n");
  else
    printf("Did not find food\n");
 
  /* Shut down Create robot */
  create.shutdown();

  return 0;
}
