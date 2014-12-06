all: lobster_casting

lobster_casting: lobster_casting.o Behavior.o PerceptualSchema.o MotorSchema.o Robot.o Parameters.o
	g++ lobster_casting.o Behavior.o PerceptualSchema.o MotorSchema.o Robot.o Parameters.o -o lobster_casting -L ~/local/lib -rdynamic -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lplayercommon -Wl,-rpath,~/local/lib
	rm *.o

lobster_casting.o: lobster_casting.cc Behavior.h PerceptualSchema.h MotorSchema.h Robot.h Parameters.h
	g++ -o lobster_casting.o -I ~/local/include/player-3.0 -c lobster_casting.cc

Behavior.o: Behavior.cc Behavior.h PerceptualSchema.h MotorSchema.h Robot.h Parameters.h
	g++ -o Behavior.o -I ~/local/include/player-3.0 -c Behavior.cc

PerceptualSchema.o: PerceptualSchema.cc PerceptualSchema.h Robot.h Parameters.h
	g++ -o PerceptualSchema.o -I ~/local/include/player-3.0 -c PerceptualSchema.cc

MotorSchema.o: MotorSchema.cc MotorSchema.h Robot.h Parameters.h
	g++ -o MotorSchema.o -I ~/local/include/player-3.0 -c MotorSchema.cc

Robot.o: Robot.cc Robot.h
	g++ -o Robot.o -I ~/local/include/player-3.0 -c Robot.cc

Parameters.o: Parameters.cc Parameters.h
	g++ -o Parameters.o -c Parameters.cc

clean:
	rm -f *.o lobster_casting
.PHONY: clean
