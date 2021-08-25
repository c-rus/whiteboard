APP_NAME=whiteboard
PROJECT_PATH=$(PWD)/deploy
APP_PATH=/Applications/$(APP_NAME).app

op=

.PHONY: build
build:
	g++ -std=c++11 ./src/*.cpp -o ./build/whiteboard -I/Library/Frameworks/SFML/include -Wl,-rpath,./Frameworks/ -F./Frameworks -framework FLAC -framework freetype -framework ogg -framework OpenAL -framework vorbis -framework vorbisenc -framework vorbisfile -framework sfml-graphics -framework sfml-window -framework sfml-system -framework sfml-audio -framework sfml-network

.PHONY: run
run:
	./build/whiteboard ./data ./data/$(op)

.PHONY: deploy
deploy:
# copy executable
	cp ./build/whiteboard $(APP_PATH)/Contents/Resources/whiteboard
# copy assets
	rm -r $(APP_PATH)/Contents/Resources/assets/
	mkdir $(APP_PATH)/Contents/Resources/assets/
	cp -r ./assets/. $(APP_PATH)/Contents/Resources/assets/.
#copy data folder (config settings, pages)
	rm -r $(APP_PATH)/Contents/Resources/data/
	mkdir $(APP_PATH)/Contents/Resources/data/
	cp -r ./build/data/. $(APP_PATH)/Contents/Resources/data/.

.PHONY: release
release:
	make build
	make deploy
