run:
	g++ src/sample/*.cpp -o bin/main.exe -Iinclude -Llib -Lbin -lnickel2 -lOpenAL32.dll -lalut.dll -limgui -lstb -lassimp -lzlibstatic -lglfw3 -lglad -lopengl32 -lgdi32 -static -static-libstdc++ -static-libgcc
	bin/main

build:
	g++ src/static/nickel2.cpp -c -o bin/nickel2.o -Iinclude
	ar rvs bin/libnickel2.a bin/nickel2.o

build-and-run:
	g++ src/static/nickel2.cpp -c -o bin/nickel2.o -Iinclude
	ar rvs bin/libnickel2.a bin/nickel2.o
	g++ src/sample/*.cpp -o bin/main.exe -Iinclude -Llib -Lbin -lnickel2 -lOpenAL32.dll -lalut.dll -limgui -lstb -lassimp -lzlibstatic -lglfw3 -lglad -lopengl32 -lgdi32 -static -static-libstdc++ -static-libgcc
	bin/main