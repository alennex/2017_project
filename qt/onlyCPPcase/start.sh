rm main.out

#g++   -std=c++11 main.cpp lib/alignment.cpp lib/feature.cpp lib/utility.cpp -o main.out -Iinclude/ `pkg-config --cflags --libs opencv`
 
g++ -std=c++11 -Iinclude/ `pkg-config --cflags --libs opencv` main.cpp lib/alignment.cpp lib/feature.cpp lib/utility.cpp -o main.out
 ./main.out test.csv  
