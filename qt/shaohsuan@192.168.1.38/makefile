main:
	g++ -std=c++11 main.cpp lib/alignment.cpp lib/feature.cpp lib/utility.cpp -o piaV1.out -Iinclude/ `pkg-config --cflags --libs opencv`

static:
	g++ -std=c++11 -static-libstdc++ main.cpp lib/alignment.cpp lib/feature.cpp lib/utility.cpp -o piaV1.out -Iinclude/ `pkg-config --cflags --libs opencv`

test:
	./piaV1.out test.csv IC0400
	#./piaV1.out test.csv IC0061

clean:
	rm -rf piaV1.out *.txt IC0*
	rm -rf 2017*.csv
