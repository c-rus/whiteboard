.PHONY: build
build:
	g++ -std=c++11 ./src/*.cpp -o ./build/whiteboard -I/Library/Frameworks/SFML/include -Wl,-rpath,./Frameworks/ -F./Frameworks -framework FLAC -framework freetype -framework ogg -framework OpenAL -framework vorbis -framework vorbisenc -framework vorbisfile -framework sfml-graphics -framework sfml-window -framework sfml-system -framework sfml-audio -framework sfml-network

.PHONY: run
run:
	./build/whiteboard ./build/scratchpad.wb