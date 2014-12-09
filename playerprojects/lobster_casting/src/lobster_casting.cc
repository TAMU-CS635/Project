#include "Robot.h"
#include "Behavior.h"
#include "PerceptualSchema.h"
#include "MotorSchema.h"
#include "Parameters.h"
#include <unistd.h>
int main(int argc, char **argv) {
  /* Parse program parameters from command line arguments */
  Parameters p = parse_args(argc, argv);
  if (p.error)
    return 1;
  
  /* Initialize Create Robot */
  Robot create_robot;
  //create_robot.calibrate_ir();
  /*
  // Lobster Casting behavior 
  const bool hungry = true;
  
  if (Behavior::find_food(create_robot, hungry, p))
    printf("Found food.\n");
  else
    printf("Did not find food\n");
 
  // Shut down Create robot 
*/
    create_robot.move(-5);
     sleep(3);
    create_robot.move(0);

  create_robot.shutdown();

  return 0;
}
