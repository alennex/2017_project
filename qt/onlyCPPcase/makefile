
main:
	g++ -std=c++11 main.cpp lib/alignment.cpp lib/feature.cpp lib/utility.cpp -o piaV1.out -Iinclude/ `pkg-config --cflags --libs opencv`

test:
	./piaV1.out test.csv IC0010

clean:
	rm piaV1.out *.txt
