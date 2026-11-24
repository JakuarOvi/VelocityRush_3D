# Linux / macOS (needs FreeGLUT):  make && ./velocity_rush
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
LIBS = -framework OpenGL -framework GLUT
else
LIBS = -lglut -lGLU -lGL
endif
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=build/%.o)
CXXFLAGS = -std=c++11 -O2 -Iinclude
velocity_rush: $(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -o velocity_rush $(LIBS)
build/%.o: src/%.cpp $(wildcard include/*.h)
	@mkdir -p build
	g++ $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf build velocity_rush
