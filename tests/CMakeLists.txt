cmake_minimum_required(VERSION 3.19)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# get CMAKE to generate c++latest
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.20)
  if(MSVC)
    message(STATUS "recent cmake c++20 -> c++latest compatibility workaround")
    target_compile_features(${APP_TARGET} PUBLIC cxx_std_23)
  endif()
endif()

project(std_expected_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(std-expected-tester "")

target_sources(std-expected-tester PUBLIC
  src/tests.cpp
  )

target_include_directories(std-expected-tester PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(std-expected-tester
  gtest_main)

include(GoogleTest)
gtest_discover_tests(std-expected-tester)
