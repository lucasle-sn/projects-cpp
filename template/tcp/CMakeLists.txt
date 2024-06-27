add_library(tcp 
  src/server.cc
)

target_include_directories(tcp
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tcp
  public-types
)

add_executable(unit-test-tcp
  test/test_tcp.cc
)

target_link_libraries(unit-test-tcp
  gtest
  gtest_main
  tcp
)
