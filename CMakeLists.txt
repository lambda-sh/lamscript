cmake_minimum_required(VERSION 3.0.0)

if (${DISTRIBUTION_BUILD})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds/Dist/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds/Dist/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds/Dist/bin)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/builds/${CMAKE_BUILD_TYPE}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/builds/${CMAKE_BUILD_TYPE}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/builds/${CMAKE_BUILD_TYPE}/bin)
endif()

# ----------------------------------- SETTINGS ---------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------- LAMSCRIPT --------------------------------

project(lamscript)

file(
    GLOB_RECURSE
    LAMSCRIPT_SRC
    ${CMAKE_SOURCE_DIR}/src/Lamscript/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Lamscript/*.h
)

add_executable(lamscript ${LAMSCRIPT_SRC})

target_compile_definitions(
    lamscript
    PUBLIC LAMSCRIPT_BUILD_AS_EXECUTABLE
)


target_include_directories(lamscript PUBLIC ${CMAKE_SOURCE_DIR}/src)


add_library(lamscript_lib STATIC ${LAMSCRIPT_SRC})
target_include_directories(lamscript_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

# ----------------------------------- TESTING ----------------------------------

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest)

file(
  GLOB_RECURSE
  LAMSCRIPT_TEST_SRC
  ${CMAKE_SOURCE_DIR}/tests/*.cpp
)

add_executable(lamscript_test ${LAMSCRIPT_TEST_SRC})
add_test(NAME lamscript_test COMMAND lamscript_test)

target_link_libraries(
    lamscript_test
    PUBLIC lamscript_lib gtest
)

target_include_directories(
    lamscript_test
    PUBLIC ${CMAKE_SOURCE_DIR}/src
)
