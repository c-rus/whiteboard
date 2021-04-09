.PHONY: build
build:
	g++ -std=c++11 ./src/*.cpp -o ./build/whiteboard

run:
	./build/whiteboard