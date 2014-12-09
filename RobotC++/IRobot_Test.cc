#include "Create.h"
#include <stdio.h>
#include <unistd.h>
#include <libplayerc/playerc.h>
float velocity = 0.1;
int main(int argc, char **argv){
    Create create;
    create.lights(0, 0, 0);
    int i =0;
    //create.motor_raw(0, -0.349);
    //for(; i < 44; ++i, usleep(10000)) {}    
    //create.motor_raw(0,0);
    create.calibrate_ir();
    /* Pause to let user straighten out the robot */
    printf("IR calibration done\n");
    printf("Straighten out the Create robot if needed, press enter when done.\n");
    getchar();
    printf("Beginning lobster casting behavior\n");
    create.move(velocity);
    usleep(100000);
    return 0;
}
