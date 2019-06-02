cmake_minimum_required(VERSION 3.0.0)

project(grime)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(grimec src/compiler/main.c)

llvm_map_components_to_libnames(LLVM_LIBS core mcjit x86codegen)

target_link_libraries(grimec ${LLVM_LIBS})
