#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <libplayerc/playerc.h>
#include <unistd.h>
int should_quit = 0;

void sighand(int s)
{
	fprintf(stderr, ": shutting down\n");
	fflush(stderr);
	should_quit = 1;
	signal(s, NULL);
}	

void lights(playerc_opaque_t *opaque, int middle_power, int left_color, int left_intensity) {
	uint8_t d[8];
	player_opaque_data_t data;

	if (opaque != NULL)
	{
		d[0] = 2;
		d[1] = 1;
		d[2] = middle_power; // middle light
		d[3] = 0;
		d[4] = 0;
		d[5] = 0;
		d[6] = left_color; // left light color (0 = green, 255 = orange)
		d[7] = left_intensity; // left light intensity (0 to 255)
		data.data_count = 8;
		data.data = &(d[0]);
		playerc_opaque_cmd(opaque, &data);
	}
}

int main(int argc, const char **argv)
{
	playerc_client_t *client;
	playerc_position2d_t *position2d;
	playerc_bumper_t *bumper;
	playerc_ir_t *ir;
	playerc_opaque_t *opaque;
	playerc_power_t *power;
	int state =-10;
	int lastpos;
	int enablebumpers = 1; 
	int wiggle = 0;
	int docked =0;
	int i;
	int k;


	if (argc != 1)
	{
		printf("Usage: %s\n", argv[0]);
		return -1;
	}

	signal(SIGINT, sighand);

	// Create a client object and connect to the server.
	client = playerc_client_create(NULL, "localhost", 6665);
	if (playerc_client_connect(client) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}

	// Create a position2d proxy and susbscribe
	position2d = playerc_position2d_create(client, 0);
	if (playerc_position2d_subscribe(position2d, PLAYERC_OPEN_MODE) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}

	// Create a bumper proxy and subscribe
	bumper = playerc_bumper_create(client, 0);
	if (playerc_bumper_subscribe(bumper, PLAYERC_OPEN_MODE) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	power = playerc_power_create(client, 0);
	if (playerc_power_subscribe(power, PLAYERC_OPEN_MODE) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}
	// create an ir interface
	ir = playerc_ir_create(client, 0);
	if (playerc_ir_subscribe(ir, PLAYERC_OPEN_MODE) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}

	// create an opaque interface
	opaque = playerc_opaque_create(client, 0);
	if (playerc_opaque_subscribe(opaque, PLAYERC_OPEN_MODE) != 0)
	{
		fprintf(stderr, "error: %s\n", playerc_error_str());
		return -1;
	}	

	// Enable the robots motors
	playerc_position2d_enable(position2d, 1);
	// Set lights to off
	lights(opaque, 0, 0, 0);
	playerc_client_read(client);
	lastpos = opaque->data[4];
	
	while (!should_quit)
	{
	playerc_client_datamode (client, PLAYERC_DATAMODE_PULL);     

        /*---- code below was taken from http://playerstage.sourceforge.net/doc/Player-2.0.0/player/ ----*/

        if (playerc_client_datamode (client, PLAYERC_DATAMODE_PULL) != 0)
        {
              fprintf(stderr, "error: %s\n", playerc_error_str());
              return -1;   
        }

        if (playerc_client_set_replace_rule (client, -1, -1, PLAYER_MSGTYPE_DATA, -1, 1) != 0)
        {
              fprintf(stderr, "error: %s\n", playerc_error_str());
              return -1;
        }
		playerc_client_read(client);
		printf("state %d \n",state);
		printf("Pos %d ,last pose %d \n",opaque->data[4],lastpos);			
		printf("Power %d",power->charging);		
		switch(state)
		{
		case -1:
		printf("I am here %d",opaque->data[4]);
		if ((opaque->data[4] == 242) ||(opaque->data[4] == 250)||(opaque->data[4] == 246)||(opaque->data[4] == 254))
		{
		printf("getting out of forcefield");
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0.2, 1);
		sleep(2);
		state =0;
		}
	//	else if(opaque->data ==255)
		else
		{
		state =0;
		}
		lastpos = opaque->data[4];		
		break;

		case 0:
                enablebumpers = 1;
		playerc_position2d_set_cmd_vel(position2d, 0.05, 0, 0, 1);
		if (bumper->bumpers[0] == 1 || bumper->bumpers[1] == 1)
		{
//		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		state= 1;
		}
		if (ir->data.ranges[1]==1||ir->data.ranges[2]==1||ir->data.ranges[3]==1||ir->data.ranges[4]==1)
		{
		
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(1);
		state =2;
		}
		if (power->charging == 2)
		{
		printf("Docked ! ");		
		docked =1;
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(4);
		state = 10;
		}	
		if (opaque->data[4] != 255)
		{
		state =3;
		}
		lastpos = opaque->data[4];		
		break;
//		printf("IR = %f ", ir->data.ranges[0]);
//		printf("IR left = %f.IR Front left = %f,IR Front Right = %f,IR Right = %f",ir->data.ranges[1],ir->data.ranges[2],ir->data.ranges[3],ir->data.ranges[4]);
//	        printf("Left bumper (%d) Right bumper (%d) \n",bumper->bumpers[0],bumper->bumpers[1]);
               case 1:
					
		if (bumper->bumpers[0] ==1 && bumper->bumpers[1] ==1)
		{
		playerc_position2d_set_cmd_vel(position2d,- 0.1, 0,0, 1);
		sleep(2);
		playerc_position2d_set_cmd_vel(position2d,0, 0,0.4, 1);
		sleep(2);
		state = 0;
		}
                else if(bumper->bumpers[0] ==  1 )
		{
		playerc_position2d_set_cmd_vel(position2d, 0, 0,- 0.4, 1);
		sleep(2);
		state =0;
		}
               else if (bumper->bumpers[1] == 1)
		{
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0.4, 1);
	 	state =0;
		sleep(2);
		}
	        else {state =0;}
               lastpos = opaque->data[4];
		break;
		
		case 2:
		if (ir->data.ranges[2] ==1)
		{

		playerc_position2d_set_cmd_vel(position2d,- 0.2, 0, 0, 1);
		sleep(2);
		//playerc_position2d_set_cmd_vel(position2d,- 0.2, 0, 0, 1);
		playerc_position2d_set_cmd_vel(position2d, 0, 0,- 0.4, 1);
	//	playerc_position2d_set_cmd_vel(position2d, 0, 0,- 0.4, 1);
                sleep(2);
		state =0;
		}

                else if (ir->data.ranges[3] ==1)
                {

                playerc_position2d_set_cmd_vel(position2d,- 0.2, 0, 0, 1);
                sleep(2);
		//      playerc_position2d_set_cmd_vel(position2d,- 0.2, 0, 0, 1);
                playerc_position2d_set_cmd_vel(position2d, 0, 0, 0.4, 1);
            //    playerc_position2d_set_cmd_vel(position2d, 0, 0,- 0.4, 1);
                sleep(2);
                state =0;
		}
 		/*else if (ir->data.ranges[1] ==1)
                {

                playerc_position2d_set_cmd_vel(position2d,- 0.2, 0, 0, 1);
                playerc_position2d_set_cmd_vel(position2d,- 0.2, 0, 0, 1);
                sleep(1);
                playerc_position2d_set_cmd_vel(position2d, 0, 0,- 0.4, 1);
                playerc_position2d_set_cmd_vel(position2d, 0, 0,- 0.4, 1);
                }*/
	       else 
              {state = 0;}
		lastpos = opaque->data[4];
		break;
               case 3:
		playerc_client_read(client);
			
		printf("In state 3 current pos %d",opaque->data[4]);
		//printf("Moving forward \n");
		if (power->charging == 2)
		{
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(4);
		docked =1;		
		state = 10;
		}
		if (ir->data.ranges[1]==1||ir->data.ranges[2]==1||ir->data.ranges[3]==1||ir->data.ranges[4]==1)
		{
		printf("At dock maybe ! ");
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(1);
		if ( docked == 0)		
		{state =2;}
		}
		
		if (docked == 0)
		{		
		if ((lastpos==255)&&(opaque->data[4]==255))
	
		{
		printf("Lost going to search");
		state =0;
		}
		 if ((lastpos==244)&&(opaque->data[4]==244))
	
		{
		printf("Moving on to middle from green");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, -0.1, 1);
		}
		
		 if ((lastpos==244)&&(opaque->data[4]==246))
	
		{
		printf("Moving on to middle from green field");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, -0.2, 1);
		}
		 if ((lastpos==244)&&(opaque->data[4]==255))
	
		{
		printf("Going in the wrong way turning and explore");
		playerc_position2d_set_cmd_vel(position2d,0, 0, 0.2, 1);
		sleep(4);
		state = 0;
		}

		 if ((lastpos==255)&&(opaque->data[4]==244))
	
		{
		printf("Moving on to middle from green");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, -0.1, 1);
		}
		
		 if ((lastpos==252)&&(opaque->data[4]==244))
	
		{
		printf("Moving on to middle got distracted");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, -0.1, 1);
		}
		 if ((lastpos==254)&&(opaque->data[4]==246))
	
		{
		printf("Near the goal but distracted");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, -0.4, 1);
		}
		 if ((lastpos==246)&&(opaque->data[4]==246))
	
		{
		printf("Near the goal but distracted");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, -0.4, 1);
		}
		// For red buoy
		 if ((lastpos==255)&&(opaque->data[4]==248))
	
		{
		printf("Moving on to middle from red");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, 0.1, 1);
		}
		 if ((lastpos==248)&&(opaque->data[4]==248))
	
		{
		printf("Moving on to middle from red");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, 0.1, 1);
		}
		if ((lastpos==248)&&(opaque->data[4]==250))
	
		{
		printf("Moving on to middle from red force field");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, 0.2, 1);
		}
		if ((lastpos==248)&&(opaque->data[4]==255))
	
		{
		printf("Going in the wrong way turning and explore");
		playerc_position2d_set_cmd_vel(position2d,0, 0, -0.2, 1);
		sleep(4);
		state = 0;
		}

		 if ((lastpos==252)&&(opaque->data[4]==248))
	
		{
		printf("Moving on to middle got distracted");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, 0.1, 1);
		}
		 if ((lastpos==254)&&(opaque->data[4]==250))
	
		{
		printf("Near the goal but distracted");
		playerc_position2d_set_cmd_vel(position2d,0.05, 0, 0.4, 1);
		}

	
		// RGF
		 if((opaque->data[4]==252)||(opaque->data[4]==254))
		{
		printf("Going straight \n");
		playerc_position2d_set_cmd_vel(position2d, 0.15, 0, 0, 1);
		enablebumpers = 0;
		if (power->charging == 2)
		{
		printf("Docked !");		
		docked =1;		
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(4);
		state = 10;
		}		
		}
		// For Force field
		if ((lastpos==255)&&(opaque->data[4]==242))
	
		{
		printf("Moving on wrong angle");
		playerc_position2d_set_cmd_vel(position2d,0, 0, -0.2, 1);
		sleep(4);
		}

		//obstacle avoidance
		if (bumper->bumpers[0] == 1 || bumper->bumpers[1] == 1)
		{
//		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		if (enablebumpers == 1)
		{state= 1;}
		else 
		{		
		printf("Sleeping at the dock");		
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		
		sleep(2);
		printf("wiggle yeah !");
		if ( wiggle == 0)
		{
		
		for( k=0;k<25;k++)
		{
		printf("25 times loop");		
		playerc_client_read(client);		
		}
		if (power->charging == 2)
		{
		printf("Docked !");		
		docked =1;		
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(4);
		state = 10;
		}	
		if (docked == 0)
		{		
		playerc_position2d_set_cmd_vel(position2d, -0.05, 0, 0.1, 1);
		sleep(1);		
		wiggle =1;
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		}
		}
		if (wiggle == 1)
		{
		for (i =0;i <25;i++)
		{
		printf("25 times");			
		playerc_client_read(client);}
		
		if (power->charging == 2)
		{
		printf("Docked ! ");		
		docked =1;
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);
		sleep(4);
		state = 10;
		}		
		if (docked ==0)
		{		
		playerc_position2d_set_cmd_vel(position2d, -0.05, 0, -0.1, 1);
		sleep(1);		
		wiggle =0;
		playerc_position2d_set_cmd_vel(position2d, 0, 0, 0, 1);		
		}
		}
				
		}
		}
		}
		
		/*if (opaque->data[4]==242)
		{
		printf("at the edge so stop and full turn \n");
		playerc_position2d_set_cmd_vel(position2d, 0, 0,0.4, 1);
		sleep(2);
		playerc_position2d_set_cmd_vel(position2d, 0.05, 0,0, 1);
		sleep(1);		
		}*/
		
				
		lastpos = opaque->data[4];
		break;
		
		}	
	}

	// Disable the robots motors
	playerc_position2d_enable(position2d, 0);

	// Unsubscribe and destroy
	playerc_position2d_unsubscribe(position2d);
	playerc_position2d_destroy(position2d);

	playerc_bumper_unsubscribe(bumper);
	playerc_bumper_destroy(bumper);
	
	playerc_power_unsubscribe(power);
	playerc_power_destroy(power);

	playerc_client_disconnect(client);
	playerc_client_destroy(client);

	return 0;
}
