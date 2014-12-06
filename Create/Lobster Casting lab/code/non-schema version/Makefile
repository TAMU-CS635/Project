all: lobster_casting

lobster_casting: lobster_casting.o Create.o util.o
	g++ lobster_casting.o Create.o util.o -o lobster_casting -L ~/local/lib -rdynamic -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lplayercommon -Wl,-rpath,~/local/lib
	rm *.o

lobster_casting.o: lobster_casting.cc Create.h util.h
	g++ -o lobster_casting.o -I ~/local/include/player-3.0 -c lobster_casting.cc

Create.o: Create.cc Create.h
	g++ -o Create.o -I ~/local/include/player-3.0 -c Create.cc

util.o: util.cc util.h
	g++ -o util.o -I ~/local/include/player-3.0 -c util.cc

clean:
	rm -f *.o lobster_casting
.PHONY: clean
