BUILD_DIR=./build
CMAKE=cmake
CTEST=ctest

FILES=$(shell find . -not -path "./third_party/*" -not -path "./build/*" \( -name '*.cc' -o -name '*.c' -o -name '*.h' \))
TMPFILE=./formatted_file

.PHONY: all-gcc all-clang clean do-all-unit-tests gen-doxygen do-clang-format-check

all-gcc:
	@mkdir -p ${BUILD_DIR}
	${CMAKE} -S . -B ${BUILD_DIR} \
			-DCMAKE_BUILD_TYPE=Debug \
			-DCMAKE_C_COMPILER="gcc" \
			-DCMAKE_CXX_COMPILER="g++" \
			&& ${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory
# Can also use "cd ${BUILD_DIR} && cmake .. && make "
	
all-clang:
	@mkdir -p ${BUILD_DIR}
	${CMAKE} -S . -B ${BUILD_DIR} \
			-DCMAKE_BUILD_TYPE=Debug \
			-DCMAKE_C_COMPILER="clang" \
			-DCMAKE_CXX_COMPILER="clang++" \
			&& ${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory

all: all-gcc

do-all-unit-tests:
	${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory
	cd ${BUILD_DIR} && ${CTEST} -j8 -T test --no-compress-output

gen-doxygen: all-clang
	${CMAKE} --build ${BUILD_DIR} --target doxygen -- --no-print-directory

do-clang-format-check:
	@sh tools/check_clangformat.sh

clean:
	rm -rf ${BUILD_DIR}
