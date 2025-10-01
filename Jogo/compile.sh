g++ simple.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o simple

g++ gamer.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o gamer