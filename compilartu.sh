#!/bin/bash
set -e  # Exit if any command fails

# Delete binaries
rm -f gtvq
rm -f *.gvdl

# Compile gtvq
g++ gtvq.cpp -o gtvq -std=c++11 -Ofast -march=native

# Compile libraries
g++ -std=c++11 -o basic.gvdl -shared -fPIC libraries/basic.cpp -Ofast -march=native 
g++ -std=c++11 -o math.gvdl -shared -fPIC libraries/math.cpp -Ofast -march=native
g++ -std=c++11 -o ui.gvdl -shared -fPIC libraries/ui.cpp -Ofast -march=native `fltk-config --cxxflags --ldstaticflags --use-images`

# Compile  
./gtvq $1