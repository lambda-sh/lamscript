# run_all_tests.sh
#
# This script is used to run all tests associated with lamscript.

ROOT_DIR="$(git rev-parse --show-toplevel)"
pushd $ROOT_DIR > /dev/null

# ----------------------------- LAMBDA-SH & ARGS ------------------------------
source lambda-sh/lambda.sh

LAMBDA_PARSE_ARG build Release "The type of build to produce."

LAMBDA_COMPILE_ARGS $@

# ------------------------------- RUNNING TESTS --------------------------------

if [ ! -d "$ROOT_DIR/build/$LAMBDA_build" ]; then
    LAMBDA_FATAL "The tests for $LAMBDA_build hasn't been built yet."\
        "Please run './scripts/compile_and_run.sh --build $LAMBDA_build"
fi

pushd $ROOT_DIR/build/$LAMBDA_build/bin > /dev/null

LAMBDA_INFO "Running tests on a $LAMBDA_build build of lamscript."
./lamscript_tests
LAMBDA_INFO "Finished executing $LAMBDA_example"

popd > /dev/null  # $ROOT_DIR/build/$LAMBDA_build/bin
popd > /dev/null  # $ROOT_DIR
