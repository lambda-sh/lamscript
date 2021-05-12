# ----------------------------------- SETTINGS ---------------------------------

cmake_minimum_required(VERSION 3.0.0)

project(lamscript)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# ----------------------------------- OPTIONS ----------------------------------

option(LAMSCRIPT_BUILD_EXECUTABLE "Build lamscript as an executable" ON)
option(LAMSCRIPT_BUILD_LIBRARY "Build lamscript as a static library" ON)
option(
    LAMSCRIPT_BUILD_TESTS
    "Build the tests for lamscript. (Must be building as a library)"
    ON)

# -------------------------------- DEPENDENCIES --------------------------------

# SPDLog -- Utilized for fast logging output and control.
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/spdlog)

# ----------------------------------- LAMSCRIPT --------------------------------

if (LAMSCRIPT_BUILD_EXECUTABLE)
    file(
        GLOB_RECURSE
        LAMSCRIPT_SRC
        ${CMAKE_SOURCE_DIR}/src/Lamscript/*.cpp
        ${CMAKE_SOURCE_DIR}/src/Lamscript/*.h)

    add_executable(lamscript ${LAMSCRIPT_SRC})
    target_link_libraries(lamscript spdlog::spdlog)

    target_compile_definitions(
        lamscript
        PUBLIC LAMSCRIPT_BUILD_AS_EXECUTABLE)

    target_include_directories(lamscript PUBLIC ${CMAKE_SOURCE_DIR}/src)
endif()

# -------------------------------- LAMSCRIPT LIB -------------------------------

if (LAMSCRIPT_BUILD_LIBRARY)
    add_library(lamscript_lib STATIC ${LAMSCRIPT_SRC})
    target_include_directories(lamscript_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

    target_link_libraries(lamscript_lib spdlog::spdlog)

    if (${CMAKE_BUILD_TYPE} EQUAL Debug)
        target_compile_definitions(
            lamscript
            PUBLIC LAMSCRIPT_LOGGING_ENABLED)
        target_compile_definitions(
            lamscript_lib
            PUBLIC LAMSCRIPT_LOGGING_ENABLED)
    endif()
endif()

# ----------------------------------- TESTING ----------------------------------

if (LAMSCRIPT_BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest)

    file(
      GLOB_RECURSE
      LAMSCRIPT_TEST_SRC
      ${CMAKE_SOURCE_DIR}/tests/*.cpp)

    add_executable(lamscript_tests ${LAMSCRIPT_TEST_SRC})
    add_test(NAME lamscript_tests COMMAND lamscript_tests)

    target_link_libraries(
        lamscript_tests
        PUBLIC lamscript_lib gtest)

    target_include_directories(
        lamscript_tests
        PUBLIC ${CMAKE_SOURCE_DIR}/src)
endif()


# ------------------------------- LAMSCRIPTEN ----------------------------------

file(
    GLOB_RECURSE
    LAMSCRIPTEN_SRC
    ${CMAKE_SOURCE_DIR}/src/Lamscripten/*.cpp
    ${CMAKE_SOURCE_DIR}/src/Lamscripten/*.h)

add_executable(lamscripten ${LAMSCRIPTEN_SRC})
target_include_directories(lamscripten PUBLIC ${CMAKE_SOURCE_DIR}/src)