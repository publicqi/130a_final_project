.PHONY:main

all: main
main:
	g++ -std=c++11 -g *.cpp -o main -D_GLIBCXX_USE_CXX11_ABI=1

clean:
	rm -rf *.o main