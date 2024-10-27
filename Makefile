cc := g++
pchName := nkpch
projectName := Nickel2
sourceDir := ./src
binaryDir := ./bin
objectDir := $(binaryDir)/obj
shaderDir := ./shaders
includeDir := ./include ./include/imgui ./include/imguizmo
libraryDir := ./lib
projectDir := $(sourceDir)/$(projectName)
rendererDir := $(projectDir)/Renderer
vendorDir := $(sourceDir)/Vendor
libraries := steam_api64 mono-2.0-sgen OpenAL32.dll alut.dll assimp.dll glfw3dll opengl32 Jolt avcodec.dll avdevice.dll \
	avfilter.dll avformat.dll avutil.dll postproc.dll swresample.dll swscale.dll gdi32 odbc32 odbccp32 dwmapi winmm
executable := $(binaryDir)/Editor.exe
staticLibrary := $(libraryDir)/lib$(projectName).a
pch := ./include/$(projectName)/$(pchName).hpp
flags := -static -static-libgcc -static-libstdc++
defines := NK_DEBUG
warnings := all

libraries := $(foreach lib,$(libraries),-l$(lib))
warnings := $(foreach warn,$(warnings),-W$(warn))
defines := $(foreach def,$(defines),-D$(def))

pchSource := $(projectDir)/$(pchName).cpp
pchObject := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(pchSource))

editorModule := Editor
editorSources := $(wildcard $(projectDir)/$(editorModule)/*.cpp)
editorObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(editorSources))

moduleDirs := $(wildcard $(projectDir)/*)
moduleNames := $(filter-out Debug Editor $(pchName).cpp,$(patsubst $(projectDir)/%,%,$(moduleDirs)))
moduleSources := $(foreach dir,$(patsubst %,$(projectDir)/%,$(moduleNames)),$(wildcard $(dir)/*.cpp))
moduleObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(moduleSources))

rendererDirs := $(wildcard $(rendererDir)/*)
rendererNames := $(patsubst $(rendererDir)/%,%,$(rendererDirs))
rendererSources := $(foreach dir,$(patsubst %,$(rendererDir)/%,$(rendererNames)),$(wildcard $(dir)/*.cpp))
rendererObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(rendererSources))

vendorDirs := $(wildcard $(vendorDir)/*)
vendorNames := $(patsubst $(vendorDir)/%,%,$(vendorDirs))
vendorSources := $(foreach dir,$(patsubst %,$(vendorDir)/%,$(vendorNames)),$(wildcard $(dir)/*.cpp))
vendorObjects := $(patsubst $(sourceDir)/%.cpp,$(objectDir)/%.o,$(vendorSources))

default: $(staticLibrary) run

$(pch).gch: $(pch) $(foreach file,Platform Base Assert Utils Logger,$(patsubst %,./include/$(projectName)/Core/%.hpp,$(file)))
	@python ./checkDir.py --path="$(dir $@)"
	$(cc) $(warnings) $< -o $@ $(addprefix -I,$(includeDir)) $(defines)

$(executable): $(pchObject) $(editorObjects) $(moduleObjects) $(rendererObjects) $(vendorObjects)
	@python ./checkDir.py --path="$(dir $@)"
	$(cc) $(warnings) $^ -o $@ $(addprefix -I,$(includeDir)) $(addprefix -L,$(libraryDir)) $(libraries) $(flags) $(defines)

$(objectDir)/$(projectName)/Editor/%.o: $(sourceDir)/$(projectName)/Editor/%.cpp $(wildcard $(sourceDir)/$(projectName)/Editor/*.hpp) $(pch).gch
	@python ./checkDir.py --path="$(dir $@)"
	$(cc) $(warnings) $< -c -o $@ $(addprefix -I,$(includeDir)) $(defines)

$(objectDir)/$(projectName)/%.o: $(sourceDir)/$(projectName)/%.cpp $(pch).gch
	@python ./checkDir.py --path="$(dir $@)"
	$(cc) $(warnings) $< -c -o $@ $(addprefix -I,$(includeDir)) $(defines)

$(objectDir)/Vendor/%.o: $(sourceDir)/Vendor/%.cpp
	@python ./checkDir.py --path="$(dir $@)"
	$(cc) $(warnings) $< -c -o $@ $(addprefix -I,$(includeDir)) $(defines)

$(staticLibrary): $(pchObject) $(moduleObjects) $(rendererObjects)
	@python ./checkDir.py --path="$(dir $@)"
	@python ./incVersion.py --path="./include/$(projectName)/Core/Version.hpp"
	ar rcs $@ $^

clean:
	- del /q "$(subst /,\,$(pch).gch)"
	- del /q "$(subst /,\,$(executable))"
	- del /q "$(subst /,\,$(staticLibrary))"
	- rmdir /s /q "$(objectDir)"

run: $(executable)
	"$<"