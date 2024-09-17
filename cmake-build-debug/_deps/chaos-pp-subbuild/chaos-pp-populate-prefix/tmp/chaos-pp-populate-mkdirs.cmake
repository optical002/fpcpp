# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-src"
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-build"
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix"
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix/tmp"
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix/src/chaos-pp-populate-stamp"
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix/src"
  "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix/src/chaos-pp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix/src/chaos-pp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/work/fpcpp/cmake-build-debug/_deps/chaos-pp-subbuild/chaos-pp-populate-prefix/src/chaos-pp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
