all: dead_reckoning

dead_reckoning: dead_reckoning.o
	gcc dead_reckoning.o -o dead_reckoning -L ~/local/lib -rdynamic -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lplayercommon -Wl,-rpath,~/local/lib
	rm *.o

dead_reckoning.o: dead_reckoning.c
	gcc -o dead_reckoning.o -I ~/local/include/player-3.0 -c dead_reckoning.c

clean:
	rm -f *.o dead_reckoning
.PHONY: clean
