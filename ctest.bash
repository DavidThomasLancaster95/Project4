#!/bin/bash
rm play.exe
g++ -Wall -Werror -std=c++17 -g *.cpp -o play

rm actual.txt
./play.exe tests/in$1.txt > actual.txt
diff -w actual.txt tests/out$1.txt