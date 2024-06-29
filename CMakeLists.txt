cmake_minimum_required(VERSION 3.6)
project(projects-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Enable testing")
enable_testing()

# Add third-party libraries
add_subdirectory(third_party/gtest)

add_subdirectory(coding_problems)
add_subdirectory(public_types)
add_subdirectory(utilities)

# Doxygen setup
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_CONFIGURE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")

  # Remove output directory if it exists
  if(EXISTS ${DOXYGEN_OUTPUT_DIRECTORY})
    file(REMOVE_RECURSE ${DOXYGEN_OUTPUT_DIRECTORY})
  endif()
  # Ensure output directory exists or create it
  file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})

  configure_file(${DOXYGEN_CONFIGURE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  
  add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating doxygen documentation output to \"${DOXYGEN_OUTPUT_DIRECTORY}\""
    VERBATIM
  )
else()
  message(WARNING "Doxygen not found, nable to generate doxygen documentation.")
endif()
