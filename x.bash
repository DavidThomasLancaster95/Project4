#!/bin/bash
rm play.exe
g++ -Wall -Werror -std=c++17 -g *.cpp -o play

./play.exe data.txt
