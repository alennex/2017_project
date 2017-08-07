rm main.out

g++   -std=c++11 main.cpp lib/alignment.cpp lib/feature.cpp lib/utility.cpp -o main.out -Iinclude/ `pkg-config --cflags --libs opencv`
 
 ./main.out test.csv  