# Parent tcp_socket does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(tcp_socketTests LANGUAGES CXX)

# ---- Tests ----
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(CMAKE_SUPPRESS_DEVELOPER_ERRORS 1 CACHE INTERNAL "")
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

foreach(tgt gtest gtest_main gmock gmock_main)
  set_target_properties(${tgt} PROPERTIES COMPILE_FLAGS "")
  target_compile_options(${tgt} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/w>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-w>
  )
  set_property(TARGET ${tgt} PROPERTY CXX_CLANG_TIDY "")
endforeach()
enable_testing()
include(GoogleTest)

set(BUILD_GTEST ON CACHE BOOL "" FORCE)

add_executable(tcp_socket_test
    source/lib_test.cpp
    source/tcp_socket_test.cpp
)
set_property(TARGET tcp_socket_test PROPERTY CXX_CLANG_TIDY "")
target_link_libraries(tcp_socket_test PRIVATE tcp_socket_lib GTest::gtest)
target_compile_features(tcp_socket_test PRIVATE cxx_std_20)


# add_test(NAME tcp_socket_test COMMAND tcp_socket_test)
gtest_discover_tests(tcp_socket_test)

# ---- End-of-file commands ----

add_folders(Test)
