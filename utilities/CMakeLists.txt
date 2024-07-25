add_library(utilities
  src/bytestream.cc
  src/log.cc
  src/test_fixture.cc
  src/thread.cc
)

target_include_directories(utilities
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(utilities
  PUBLIC public-types Threads::Threads fmt::fmt
  PRIVATE gtest gtest_main
)

add_executable(unit-test-utilities
  test/test_bytestream.cc
  test/test_log.cc
  test/test_thread.cc
)

target_link_libraries(unit-test-utilities
  gtest
  gtest_main
  utilities
)

add_test(NAME unit-test-utilities COMMAND unit-test-utilities)
