project(core)

add_executable(core core.cpp)
target_include_directories(core PRIVATE ${CMAKE_SOURCE_DIR})