cmake_minimum_required(VERSION 3.29)
project(fpcpp)
set(CMAKE_CXX_STANDARD 26)

# Define a custom variable for the chaos-pp directory
set(CHAOS_PP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/chaos-pp)

add_subdirectory(core)
add_subdirectory(test)

target_include_directories(core INTERFACE ${CHAOS_PP_DIR})
target_include_directories(test PRIVATE ${CHAOS_PP_DIR})

# Precompile headers for the core and test targets
target_precompile_headers(core INTERFACE ${CHAOS_PP_DIR}/chaos/preprocessor.h)
target_precompile_headers(test PRIVATE ${CHAOS_PP_DIR}/chaos/preprocessor.h)