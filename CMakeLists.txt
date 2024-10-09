cmake_minimum_required(VERSION 3.29)
project(fpcpp)
set(CMAKE_CXX_STANDARD 26)

include(FetchContent)
FetchContent_Declare(
  chaos-pp
  GIT_REPOSITORY https://github.com/rofl0r/chaos-pp.git
  GIT_TAG        master  # Use a specific tag or commit hash if needed
)
FetchContent_MakeAvailable(chaos-pp)

add_subdirectory(core)
add_subdirectory(test)

target_include_directories(core INTERFACE ${chaos-pp_SOURCE_DIR})
target_include_directories(test PRIVATE ${chaos-pp_SOURCE_DIR})

# Precompile headers for the core and test targets
target_precompile_headers(core INTERFACE ${chaos-pp_SOURCE_DIR}/chaos/preprocessor.h)
target_precompile_headers(test PRIVATE ${chaos-pp_SOURCE_DIR}/chaos/preprocessor.h)