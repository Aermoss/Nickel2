default:
	g++ src/*.cpp -o bin/main.exe -Iinclude -Llib -limgui -lstb -lassimp -lzlibstatic -lglfw3 -lglad -lopengl32 -lgdi32 -static -static-libstdc++ -static-libgcc
	bin/main