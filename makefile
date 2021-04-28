APP_NAME = whiteboard
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
	cp ./build/whiteboard ./deploy/$(APP_NAME).app/Contents/Resources/whiteboard
# copy assets
	rm -r ./deploy/$(APP_NAME).app/Contents/Resources/assets/
	mkdir ./deploy/$(APP_NAME).app/Contents/Resources/assets/
	cp -r ./assets/. ./deploy/$(APP_NAME).app/Contents/Resources/assets/.
#copy data folder (config settings, pages)
	rm -r ./deploy/$(APP_NAME).app/Contents/Resources/data/
	mkdir ./deploy/$(APP_NAME).app/Contents/Resources/data/
	cp -r ./build/data/. ./deploy/$(APP_NAME).app/Contents/Resources/data/.
# copy application
	rm -r ~/../../Applications/$(APP_NAME).app/
	mkdir ~/../../Applications/$(APP_NAME).app/
	cp -r ./deploy/$(APP_NAME).app/. ~/../../Applications/$(APP_NAME).app/.

.PHONY: release
release:
	make build
	make deploy
	make zipper

.PHONY: zipper
zipper:
# zip app component for itch.io
	cd ./deploy; zip -r ./whiteboard.zip ./whiteboard.app
