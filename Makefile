BUILD_DIR=./build
CMAKE=cmake
CTEST=ctest

.PHONY: all clean do-all-unit-tests

all:
	@mkdir -p ${BUILD_DIR}
	${CMAKE} -S . -B ${BUILD_DIR} && ${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory
# Can also use "cd ${BUILD_DIR} && cmake .. && make "
	
do-all-unit-tests:
	${CMAKE} --build ${BUILD_DIR} -j8 -- --no-print-directory
	cd ${BUILD_DIR} && ${CTEST} -j8 -T test --no-compress-output

clean:
	rm -rf ${BUILD_DIR}
