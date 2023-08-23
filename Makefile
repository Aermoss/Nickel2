cc := g++
warnings := -Wall
includeDirs := -Iinclude -Iinclude/bullet
libDirs := -Llib -Lbin
bullet := -lBullet2FileLoader.dll -lBullet3Collision.dll -lBullet3Common.dll -lBullet3Dynamics.dll -lBullet3Geometry.dll -lBullet3OpenCL_clew.dll -lBulletCollision.dll \
	-lBulletDynamics.dll -lBulletExampleBrowserLib.dll -lBulletFileLoader.dll -lBulletInverseDynamics.dll -lBulletInverseDynamicsUtils.dll -lBulletSoftBody.dll -lLinearMath.dll
libs := ${bullet} -lnickel2 -lvulkan-1 -lOpenAL32.dll -lalut.dll -lstb -lassimp -lzlibstatic -lglfw3 -lglad -lopengl32 -lgdi32 -lodbc32 -lodbccp32

all: compile-shaders build run

run:
	${cc} -m64 src/sample/*.cpp -o bin/main.exe ${warnings} ${includeDirs} ${libDirs} ${libs} -static -static-libstdc++ -static-libgcc
	bin/main

build:
	${cc} -m64 src/static/nickel2.cpp -c -o bin/nickel2.o ${warnings} ${includeDirs}
	ar rvs bin/libnickel2.a bin/nickel2.o

compile-shaders:
	bin/glslc.exe shaders/vulkan/default.vert -o shaders/vulkan/default.vert.spv
	bin/glslc.exe shaders/vulkan/default.frag -o shaders/vulkan/default.frag.spv