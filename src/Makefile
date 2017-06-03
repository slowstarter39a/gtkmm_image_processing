CC=g++
TARGET=magnolia
CFLAGS=-std=c++11

SRC=main.cc magnolia_main_window.cc magnolia_image_window.cc

all:
	$(CC) -o $(TARGET) $(SRC) `pkg-config gtkmm-3.0 --cflags --libs` $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o

