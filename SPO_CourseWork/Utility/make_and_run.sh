#!/bin/bash

g++ -o my_app test_launch.cpp main.cpp functions.cpp -lssl -lcrypto
./my_app ../../
