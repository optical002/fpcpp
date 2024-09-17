# CMake generated Testfile for 
# Source directory: C:/work/fpcpp/test
# Build directory: C:/work/fpcpp/cmake-build-debug/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[AllTestsInMain]=] "core_tests")
set_tests_properties([=[AllTestsInMain]=] PROPERTIES  _BACKTRACE_TRIPLES "C:/work/fpcpp/test/CMakeLists.txt;42;add_test;C:/work/fpcpp/test/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
