rm -f gtvq
rm -f basic
rm -f math
g++ gtvq.cpp -o gtvq -std=c++11 -Ofast -march=native \
&& g++ -std=c++11 -o basic.gvdl -shared -fPIC libraries/basic.cpp -Ofast -march=native  \
&& g++ -std=c++11 -o math.gvdl -shared -fPIC libraries/math.cpp -Ofast -march=native  \
&& ./gtvq examples/linking.gtvq