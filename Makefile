BUILD_DIR=./build
CMAKE=cmake
CTEST=ctest

FILES=$(shell find . -not -path "./third_party/*" -not -path "./build/*" \( -name '*.cc' -o -name '*.c' -o -name '*.h' \))
TMPFILE=./formatted_file

.PHONY: all clean do-all-unit-tests gen-doxygen do-clang-format-check

all:
	@mkdir -p ${BUILD_DIR}
	${CMAKE} -S . -B ${BUILD_DIR} && ${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory
# Can also use "cd ${BUILD_DIR} && cmake .. && make "
	
do-all-unit-tests:
	${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory
	cd ${BUILD_DIR} && ${CTEST} -j8 -T test --no-compress-output

gen-doxygen: all
	${CMAKE} --build ${BUILD_DIR} --target doxygen -- --no-print-directory

do-clang-format-check:
	@sh tools/check_clangformat.sh

clean:
	rm -rf ${BUILD_DIR}
