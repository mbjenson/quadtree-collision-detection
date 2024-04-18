
all:
	g++ -std=c++17 -O3 -o app ./src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system

run:
	./app

clean:
	rm app