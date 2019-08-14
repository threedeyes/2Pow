CFLAGS=-O3
LIBS=-lroot -lbe
CC=g++
OBJS = Application.o MainWindow.o GameManager.o BoardView.o Tile.o


all: build

build: $(OBJS)
	$(CC) $(CFLAGS) -o 2Pow $^ $(LIBS)
	rc Resources.rdef
	xres -o 2Pow Resources.rsrc
	mimeset -f 2Pow

.cpp.o:
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f 2Pow $(OBJS)
