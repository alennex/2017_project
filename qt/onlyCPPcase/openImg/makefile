main:
	g++ -std=c++11 openImg.cpp -o openImg.out  `pkg-config --cflags --libs opencv`

static:
	g++ -std=c++11 -static-libstdc++ openImg.cpp -o openImg.out  `pkg-config --cflags --libs opencv`

test:
	./openImg.out test.png

clean:
	rm openImg.out
