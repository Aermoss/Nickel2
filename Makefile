cc := g++
projectName := Nickel2
sourceDir := ./src
binaryDir := ./bin
objectDir := $(binaryDir)/obj
shaderDir := ./shaders
includeDir := ./include ./include/imgui ./include/imguizmo
libraryDir := ./lib
projectDir := $(sourceDir)/$(projectName)
vendorDir := $(sourceDir)/Vendor
librarys := rvr openvr_api vulkan-1 mono-2.0-sgen OpenAL32.dll alut.dll assimp zlibstatic glfw3 opengl32 gdi32 odbc32 odbccp32 dwmapi winmm
executable := $(binaryDir)/Editor.exe
staticLibrary := $(libraryDir)/lib$(projectName).a
pch := ./include/$(projectName)/nkpch.hpp
pchObject := $(objectDir)/$(projectName)/nkpch.o
flags := -static -static-libgcc -static-libstdc++
defines := NICKEL2_DEBUG
warnings := all

librarys := $(foreach lib,$(librarys),-l$(lib))
warnings := $(foreach warn,$(warnings),-W$(warn))
defines := $(foreach def,$(defines),-D$(def))

editorModule := Editor
editorSources := $(wildcard $(projectDir)/$(editorModule)/*.cpp)
editorObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(editorSources))

moduleDirs := $(wildcard $(projectDir)/*)
moduleNames := $(filter-out nkpch.cpp $(editorModule),$(patsubst $(projectDir)/%,%,$(moduleDirs)))
moduleSources := $(foreach dir,$(patsubst %,$(projectDir)/%,$(moduleNames)),$(wildcard $(dir)/*.cpp))
moduleObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(moduleSources))

vendorDirs := $(wildcard $(vendorDir)/*)
vendorNames := $(patsubst $(vendorDir)/%,%,$(vendorDirs))
vendorSources := $(foreach dir,$(patsubst %,$(vendorDir)/%,$(vendorNames)),$(wildcard $(dir)/*.cpp))
vendorObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(vendorSources))

default: $(staticLibrary) run

$(pch).gch: $(pch) $(foreach file,platform base assert constants utils logger,$(patsubst %,./include/$(projectName)/Core/%.hpp,$(file)))
	$(cc) $(warnings) $< -o $@ $(foreach dir,$(includeDir),-I$(dir)) $(defines)

$(executable): $(pchObject) $(editorObjects) $(moduleObjects) $(vendorObjects) | $(binaryDir)
	$(cc) $(warnings) $^ -o $@ $(foreach dir,$(includeDir),-I$(dir)) $(foreach dir,$(libraryDir),-L$(dir)) $(librarys) $(flags) $(defines)

$(objectDir)/$(projectName)/%.o: $(sourceDir)/$(projectName)/%.cpp $(pch).gch | $(objectDir)
	$(cc) $(warnings) $< -c -o $@ $(foreach dir,$(includeDir),-I$(dir)) $(defines)

$(objectDir)/Vendor/%.o: $(sourceDir)/Vendor/%.cpp | $(objectDir)
	$(cc) $(warnings) $< -c -o $@ $(foreach dir,$(includeDir),-I$(dir)) $(defines)

$(staticLibrary): $(pchObject) $(moduleObjects) | $(libraryDir)
	python ./incVersion.py ./include/$(projectName)/Core/version.hpp
	ar rcs $@ $^

$(objectDir):
	cd $(binaryDir) && mkdir $(patsubst $(binaryDir)/%,%,$(objectDir)) && cd $(patsubst $(binaryDir)/%,%,$(objectDir)) && mkdir $(projectName) \
		&& cd $(projectName) && $(foreach dir,$(editorModule) $(moduleNames),mkdir $(dir) &&) cd .. && mkdir $(patsubst $(sourceDir)/%,%,$(vendorDir)) \
			&& cd $(patsubst $(sourceDir)/%,%,$(vendorDir)) && $(foreach dir,$(vendorNames),mkdir $(dir) &&) cd ../../..

$(binaryDir):
	mkdir $@

$(libraryDir):
	mkdir $@

clean:
	cd ./include/$(projectName) && del /q nkpch.hpp.gch && cd ../..
	cd $(binaryDir) && del /q "$(patsubst $(binaryDir)/%,%,$(executable))" && cd ..
	cd $(libraryDir) && del /q "$(patsubst $(libraryDir)/%,%,$(staticLibrary))" && cd ..
	rmdir /s /q "$(objectDir)"

run: $(executable)
	$<