To compile code for player, use an appropirate Makefile (such as the one in player_example_cmake) or use cmake to generate one with an appropriate CMakeLists.txt file (such as the one in ~/local/share/player/examples/libplayerc).

You can also compile directly with gcc:

gcc -I ~/local/include/player-3.0 -c code.c

gcc outfile.o -o output -L ~/local/lib -rdynamic -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lplayercommon -Wl,-rpath,~/local/lib

