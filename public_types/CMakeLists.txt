add_library(public_types INTERFACE)

target_include_directories(public_types INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE: include>
)

install(DIRECTORY include/ DESTINATION include)
