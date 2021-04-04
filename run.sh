#!/usr/bin/env sh
g++ -c $1.cpp
g++ $1.o -o $1 -lsfml-graphics -lsfml-window -lsfml-system
./$1
