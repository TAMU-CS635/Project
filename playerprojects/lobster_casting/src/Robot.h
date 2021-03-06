#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc/playerc.h>

enum Direction {
  LEFT,
  RIGHT,
  FORWARD
};

class Robot {  // Player reference to Create robot
private:
  playerc_client_t      *client;
  playerc_position2d_t  *position2d;
  playerc_ir_t          *ir;
  playerc_opaque_t      *opaque;

  float norm[4];        // normalizing values for the 4 IR sensors

  void motor_raw(double forward_speed, double angular_speed);
  
public:
  struct ir_values {
    int left, fleft, fright, right;
  };

public:
  Robot();

  void lights(int middle_power, int left_color, int left_intensity);

  void blink_led();

  void move(double forward_speed);

  void turn(double forward_speed, double angular_speed);

  void calibrate_ir();

  ir_values read_ir_raw();

  ir_values read_ir();

  void shutdown();

  void quit(const char *msg);
};

#endif // ROBOT_H_
