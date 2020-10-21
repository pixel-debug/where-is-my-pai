all:
	gcc galaxian.c -o galaxian -lGL -lglut -lGLEW -lGLU -lSOIL -lm -lSDL -lSDL_mixer

install-libraries:
	sudo apt-get update
	sudo apt-get install libsdl-mixer1.2-dev -y
	sudo apt install libsdl2-dev libsdl2-2.0-0 -y
	sudo apt-get install build-essential
	sudo apt-get install freeglut3-dev
	sudo apt-get install libglew-dev
	sudo apt-get install libsoil-dev

run:
	./galaxian

clean:
	rm galaxian