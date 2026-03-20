OUT=build

all: main

main:
	cmake -S . -B ${OUT} -DCMAKE_BUILD_TYPE=Debug
	cmake --build ${OUT}

release:
	cmake -S . -B ${OUT} -DCMAKE_BUILD_TYPE=Release
	cmake --build ${OUT} --config Release

clean:
	rm -r ${OUT}

