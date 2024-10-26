#Variables
FLAGS := -Wall

default: run

setup:
	mkdir -p out

clean:
	rm -rf out

build: setup
	gcc $(FLAGS) src/*.c -o out/chess

run: build
	./out/chess