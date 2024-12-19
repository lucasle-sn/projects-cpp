BUILD_DIR_PARENT=./build
BUILD_TYPE=debug
C_COMPILER=gcc
CXX_COMPILER=g++
BUILD_DIR=${BUILD_DIR_PARENT}/${BUILD_TYPE}/${C_COMPILER}

FILES=$(shell find . -not -path "./third_party/*" -not -path "./build/*" \( -name '*.cc' -o -name '*.c' -o -name '*.h' \))
TMPFILE=./formatted_file

.PHONY: build all clean do-all-unit-tests gen-doxygen do-clang-format-check

build:
	@cmake -S . -B ${BUILD_DIR} \
			-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
			-DCMAKE_C_COMPILER=${C_COMPILER} \
			-DCMAKE_CXX_COMPILER=${CXX_COMPILER}
	@cmake --build ${BUILD_DIR} -j8 -- --no-print-directory
# Can also use "cd ${BUILD_DIR} && cmake .. && make "

build-debug:
	make build BUILD_TYPE=debug

build-release:
	make build BUILD_TYPE=release

build-gcc:
	make build C_COMPILER=gcc CXX_COMPILER=g++

build-clang:
	make build C_COMPILER=clang CXX_COMPILER=clang++

all:
	make build-debug C_COMPILER=gcc CXX_COMPILER=g++

do-all-unit-tests:
	make all
	cd ${BUILD_DIR} && ctest -j8 -T test --no-compress-output

gen-doxygen:
	@$(eval buildDir=build/doxygen)
	@cmake -S . -B ${buildDir} \
			-DCMAKE_BUILD_TYPE=debug \
			-DCMAKE_C_COMPILER="clang" \
			-DCMAKE_CXX_COMPILER="clang++" \
			-DDOXYGEN_BUILD_ENABLED=ON
	@cmake --build ${buildDir} --target doxygen -- --no-print-directory
	doxygen ${buildDir}/doxygen/Doxyfile

do-clang-format-check:
	@sh tools/check_clangformat.sh

do-clang-format-run:
	@sh tools/run_clangformat.sh

clean:
	@rm -rf ${BUILD_DIR_PARENT}
