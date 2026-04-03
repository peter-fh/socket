cmake_minimum_required(VERSION 3.28)

include(cmake/prelude.cmake)

project(
    tcp_socket
    VERSION 0.1.0
    DESCRIPTION "Template CPP tcp_socket"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    tcp_socket_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    tcp_socket_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${tcp_socket_SOURCE_DIR}/source>"
)

target_compile_features(tcp_socket_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(tcp_socket_exe source/main.cpp)
add_executable(tcp_socket::exe ALIAS tcp_socket_exe)

set_property(TARGET tcp_socket_exe PROPERTY OUTPUT_NAME tcp_socket)

target_compile_features(tcp_socket_exe PRIVATE cxx_std_20)

target_link_libraries(tcp_socket_exe PRIVATE tcp_socket_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT tcp_socket_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of tcp_socket"
  )
endif()

include(cmake/dev-mode.cmake)
