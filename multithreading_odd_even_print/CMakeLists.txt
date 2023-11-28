add_executable(multi-threading-odd-even-print-c
  print_odd_even.c
)

target_link_libraries(multi-threading-odd-even-print-c
  pthread
)

add_executable(multi-threading-odd-even-print-condition-var
  print_odd_even_cond.c
)

target_link_libraries(multi-threading-odd-even-print-condition-var
  pthread
)

add_executable(multi-threading-odd-even-print-semaphore
  print_odd_even_semaphore.c
)

target_link_libraries(multi-threading-odd-even-print-semaphore
  pthread
)


add_executable(multi-threading-odd-even-print-cc
  print_odd_even.cc
)

target_link_libraries(multi-threading-odd-even-print-cc
  pthread
)
