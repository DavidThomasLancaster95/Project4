#!/bin/bash
rm play.exe
g++ -Wall -Werror -std=c++17 -g *.cpp -o play

./play.exe tests/in40.txt > actual.txt
diff -w actual.txt tests/out40.txt

./play.exe tests/in44.txt > actual.txt
diff -w actual.txt tests/out44.txt

./play.exe tests/in45.txt > actual.txt
diff -w actual.txt tests/out45.txt

./play.exe tests/in46.txt > actual.txt
diff -w actual.txt tests/out46.txt

./play.exe tests/in47.txt > actual.txt
diff -w actual.txt tests/out47.txt

./play.exe tests/in48.txt > actual.txt
diff -w actual.txt tests/out48.txt

./play.exe tests/in49.txt > actual.txt
diff -w actual.txt tests/out49.txt

./play.exe tests/in54.txt > actual.txt
diff -w actual.txt tests/out54.txt

./play.exe tests/in55.txt > actual.txt
diff -w actual.txt tests/out55.txt

./play.exe tests/in61.txt > actual.txt
diff -w actual.txt tests/out61.txt

./play.exe tests/in62.txt > actual.txt
diff -w actual.txt tests/out62.txt
