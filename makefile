CXX = clang++
LDFLAGS = -L/opt/homebrew/lib/
LDLIBS = -lSDL2
CXXFLAGS = -Wall -std=c++17 -I/opt/homebrew/include/

objects = main.o renderer.o userview.o env.o starfield.o

.PHONY: all clean

all: starfield

clean:
	rm *.o starfield

starfield: $(objects)
	$(CXX) $(LDFLAGS) $(LDLIBS) $(objects) -o starfield

starfield.o: starfield.cpp starfield.h

renderer.o: renderer.cpp renderer.h

userview.o: userview.cpp userview.h

env.o: env.cpp env.h userview.h renderer.h starfield.h

main.o: main.cpp starfield.h
