add_library(utilities
  src/bytestream.cc
  src/client.cc
  src/server.cc
)

target_include_directories(utilities
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(utilities
  public-types
)

add_executable(unit-test-utilities
  test/test_bytestream.cc
  test/test_log.cc
  test/test_tcp.cc
)

target_link_libraries(unit-test-utilities
  gtest
  gtest_main
  utilities
)

add_test(NAME unit-test-utilities COMMAND unit-test-utilities)
