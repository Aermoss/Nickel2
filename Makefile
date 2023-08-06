default:
	g++ src/*.cpp -o bin/main.exe -Iinclude -Llib -limgui -lstb -lassimp -lzlibstatic -lglfw3 -lglad -lopengl32 -lgdi32 -static -static-libstdc++ -static-libgcc
	bin/main

static-lib:
	g++ src/static/nickel2.cpp -c -o bin/nickel2.o -Iinclude
	ar rvs bin/libnickel2.a bin/nickel2.o