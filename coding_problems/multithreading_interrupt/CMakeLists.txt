add_executable(multithreading-interrupt
  main.cc
)

target_link_libraries(multithreading-interrupt
  PRIVATE pthread
)

add_executable(multithreading-interrupt-cpp
  main_cpp.cc
)
add_compile_options(-pthread)

target_link_libraries(multithreading-interrupt-cpp
  PRIVATE pthread
)
