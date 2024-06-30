find_package(Doxygen)
if(NOT DOXYGEN_FOUND)
  message(WARNING "Doxygen not found, unable to generate doxygen documentation.")
  return()
endif()

set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs")

if(EXISTS ${DOXYGEN_OUTPUT_DIRECTORY})
  file(REMOVE_RECURSE ${DOXYGEN_OUTPUT_DIRECTORY})  # Remove output directory if it exists
endif()
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})  # Ensure output directory exists or create it

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

add_custom_target(doxygen
  COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating doxygen documentation output to \"${DOXYGEN_OUTPUT_DIRECTORY}\""
  VERBATIM
)
