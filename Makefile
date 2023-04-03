all:
	g++ src/main.cpp -o bin/winfetch.exe -ld3d9
	./bin/winfetch.exe