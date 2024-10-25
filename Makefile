default: run

setup:
	mkdir -p out

clean:
	rm -rf out

build: setup
	gcc src/chess.c -o out/chess

run: build
	./out/chess