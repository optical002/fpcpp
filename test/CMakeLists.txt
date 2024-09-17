project(test)

enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test
        data/TestNewType.cpp
        data/TestNewType.h
        data/TestRecord.cpp
        data/TestRecord.h
        monads/TestEither.cpp
        monads/TestFuture.cpp
        monads/TestOption.cpp
        reactive/TestObservable.cpp
        reactive/TestSubscription.cpp
        reactive/TestTracker.cpp
        syntax/TestForComprehension.cpp
        syntax/TestTry.cpp
        typeclasses/TestEq.cpp
        typeclasses/TestEq.h
        typeclasses/TestNum.cpp
        typeclasses/TestNum.h
        typeclasses/TestOrd.cpp
        typeclasses/TestOrd.h
        typeclasses/TestSemigroup.cpp
        typeclasses/TestSemigroup.h
        typeclasses/TestToString.cpp
        typeclasses/TestToString.h
)

target_link_libraries(test gtest_main)
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR})

add_test(AllTestsInMain core_tests)