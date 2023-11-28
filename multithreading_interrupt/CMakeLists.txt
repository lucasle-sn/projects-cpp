add_executable(multithreading-interrupt
    main.cc
)

target_link_libraries(multithreading-interrupt
  pthread
)
