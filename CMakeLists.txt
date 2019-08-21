cmake_minimum_required(VERSION 2.8.12)
project(grime)

set(CMAKE_C_STANDARD 11)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE COMPILER_HEADERS ${PROJECT_SOURCE_DIR}/src/compiler/*.h)
file(GLOB_RECURSE COMPILER_SOURCE ${PROJECT_SOURCE_DIR}/src/compiler/*.c)

add_executable(grimec ${COMPILER_HEADERS} ${COMPILER_SOURCE})

llvm_map_components_to_libnames(LLVM_LIBS core mcjit x86codegen)

target_link_libraries(grimec ${LLVM_LIBS})

install(TARGETS grimec)
