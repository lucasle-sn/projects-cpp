add_executable(thread-inherritance
  main.cc
)

target_link_libraries(thread-inherritance
  PRIVATE pthread
)
