: clean
	rm -rf *.o netris
.PHONY : all

main: board.o block.o shapes.o video.o main.o
	cc -o netris board.o block.o shapes.o video.o main.o -lSDL -lSDL_image

video : board.o shape.o block.o
	cc -c -o video.o board.o block.o shapes.o -lSDL -lSDL_image
board : board.o block.o
block : block.o board.o
shapes: block.o shapes.o
