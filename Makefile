all: main.cpp minimal.frag minimal.vert
		g++ -std=c++11 main.cpp -lGL -lGLEW -lglut -o mesa-demo

clean:
	rm mesa-demo
