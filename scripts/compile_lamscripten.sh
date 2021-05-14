# compile_lamscripten.sh
#
# This script is used to configure and compile lamscript.

ROOT_DIR="$(git rev-parse --show-toplevel)"
pushd "$ROOT_DIR" > /dev/null

# ----------------------------- LAMBDA-SH & ARGS ------------------------------

source lambda-sh/lambda.sh

LAMBDA_PARSE_ARG build Release "The type of build to produce."
LAMBDA_PARSE_ARG cpp-compiler g++ "The compiler to use for C++ code."
LAMBDA_PARSE_ARG cores 6 "The number of cores used for compilation."
LAMBDA_PARSE_ARG os Linux "The operating system being built on."

LAMBDA_COMPILE_ARGS $@

export CXX="$LAMBDA_cpp_compiler"

BUILD_GENERATOR="Unix Makefiles"
BUILD_COMMAND="make -j $LAMBDA_cores"

if [ "$LAMBDA_os" = "Windows" ]; then
    BUILD_GENERATOR="Visual Studio 16 2019"
    BUILD_COMMAND="MSBuild.exe lamscript.sln //t:Rebuild //p:Configuration=$LAMBDA_build"
fi

LAMBDA_INFO "Attempting to Compile a $LAMBDA_build for lamscript."

# ----------------------------------- CMAKE ------------------------------------

mkdir -p build
pushd build > /dev/null

if [ "$LAMBDA_build" = "Release" ] || [ "$LAMBDA_build" = "Debug" ]; then
    cmake .. \
        -G "$BUILD_GENERATOR" \
        -DCMAKE_BUILD_TYPE="$LAMBDA_build" \
        -DLAMSCRIPT_BUILD_EXECUTABLE=OFF \
        -DLAMSCRIPT_BUILD_LIBRARY=OFF \
        -DLAMSCRIPT_BUILD_TESTS=OFF \
        -DLAMSCRIPT_ENABLE_EXPERIMENTATION=ON
elif [ "$LAMBDA_build" = "Dist" ]; then
    cmake .. \
        -G "$BUILD_GENERATOR" \
        -DCMAKE_BUILD_TYPE=Release \
        -DLAMSCRIPT_BUILD_EXECUTABLE=OFF \
        -DLAMSCRIPT_BUILD_LIBRARY=OFF \
        -DLAMSCRIPT_BUILD_TESTS=OFF \
        -DLAMSCRIPT_ENABLE_EXPERIMENTATION=ON
else
    LAMBDA_FATAL \
        "You need to pass a valid build type in order to compile lamscript."
fi

LAMBDA_ASSERT_LAST_COMMAND_OK \
    "Couldn't generate the cmake files necessary for compiling lamscript."

# ----------------------------------- BUILD ------------------------------------

$BUILD_COMMAND

LAMBDA_ASSERT_LAST_COMMAND_OK "Couldn't successfully compile lamscripten."
LAMBDA_INFO "Successfully compiled lamscripten."

popd > /dev/null  # build
popd > /dev/null  # ROOT_DIR
