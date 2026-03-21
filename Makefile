OUT=build

all: main

main: shaders
	cmake -S . -B ${OUT} -DCMAKE_BUILD_TYPE=Debug
	cmake --build ${OUT}

release: shaders
	cmake -S . -B ${OUT} -DCMAKE_BUILD_TYPE=Release
	cmake --build ${OUT} --config Release

SHADERS := $(wildcard ./resources/shaders/*.vert ./resources/shaders/*.frag)
SPV := $(SHADERS:.frag=.frag.spv)
SPV := $(SPV:.vert=.vert.spv)

shaders: $(SPV)

%.frag.spv: %.frag
	./compile.sh $<

%.vert.spv: %.vert
	./compile.sh $<

clean:
	rm -r ${OUT}

