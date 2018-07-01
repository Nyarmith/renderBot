cmake_minimum_required(VERSION 3.5)
project(renderBot)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
set(TEST_COMMAND "${CMAKE_SOURCE_DIR}/tools/run_test.sh")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

set_property(TARGET CONAN_PKG::boost_python PROPERTY INTERFACE_LINK_LIBRARIES "")
# Handle ABI differences gracefully, which are populated by profile setting.
# We want to keep libstdc++ mostly because of CI platform (Travis) that don't have
# libstdc++11 installed.
if(CONAN_SETTINGS_COMPILER_LIBCXX STREQUAL "libstdc++11")
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
elseif(CONAN_SETTINGS_COMPILER_LIBCXX STREQUAL "libstdc++")
    add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
endif()

# Debug compilation time
#add_definitions(-ftime-report)
# TODO: Conditional on clang
add_definitions(-fsanitize=thread -fPIC -fno-limit-debug-info)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -fPIC -pie")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
# Clang bug gives incorrect warnings on unused local typedefs within
# templates: https://bugs.llvm.org/show_bug.cgi?id=24883
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-local-typedefs")
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 14)
# Allows us flexibility to link against shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE true)
#set(CMAKE_VERBOSE_MAKEFILE true)
add_subdirectory(src)
add_subdirectory(config)

# Tests
include(CTest)
enable_testing()
add_subdirectory(test)
add_subdirectory(bench)

# Packaging
set(CPACK_GENERATOR "TGZ")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)

