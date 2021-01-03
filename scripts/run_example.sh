# run_example.sh
#
# This script is used to run lamscript examples with a built interpreter.

ROOT_DIR="$(git rev-parse --show-toplevel)"
pushd $ROOT_DIR > /dev/null

# ----------------------------- LAMBDA-SH & ARGS ------------------------------
source lambda-sh/lambda.sh

LAMBDA_PARSE_ARG example "" "The name of the example file located in examples/ \
    to run."
LAMBDA_PARSE_ARG with-gdb false "Determines whether or not to run the example \
    inside of gdb."

LAMBDA_COMPILE_ARGS $@

# ----------------------------- RUN THE EXAMPLE ------------------------------

pushd "$ROOT_DIR/build/bin" > /dev/null

LAMBDA_INFO "Running a $LAMBDA_build build on example $LAMBDA_example."

if [ "$LAMBDA_with_gdb" = true ]; then
    gdb --args ./lamscript "$ROOT_DIR/examples/$LAMBDA_example"
else
    ./lamscript "$ROOT_DIR/examples/$LAMBDA_example"
fi

LAMBDA_INFO "Finished executing $LAMBDA_example"

popd > /dev/null  # $ROOT_DIR/build/$LAMBDA_build/bin
popd > /dev/null  # $ROOT_DIR
