rm -f gtvq
rm -f basic
rm -f math
g++ gtvq.cpp -o gtvq -std=c++11 \
&& g++ -std=c++11 -o basic.gvdl -shared -fPIC libraries/basic.cpp \
&& g++ -std=c++11 -o math.gvdl -shared -fPIC libraries/math.cpp \
&& ./gtvq examples/linking.gtvq