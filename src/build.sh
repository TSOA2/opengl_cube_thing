#!/usr/bin/bash

{
	gcc -c main.c -o main.o -Wall
	gcc -c shaders.c -o shaders.o -Wall
	gcc -c glad.c -o glad.o -Wall
	gcc *.o -o a.out `pkg-config --static --libs glfw3` -Wall -lGL -lX11 -lpthread -lm
	rm -fR *.o
}
