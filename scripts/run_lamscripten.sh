# run_lamscripten.sh
#
# Run the lamscripten executable

ROOT_DIR="$(git rev-parse --show-toplevel)"
pushd $ROOT_DIR > /dev/null

# ----------------------------- LAMBDA-SH & ARGS ------------------------------
source lambda-sh/lambda.sh

LAMBDA_PARSE_ARG with-gdb false "Determines whether or not to run the example \
    inside of gdb."

LAMBDA_COMPILE_ARGS $@

# ------------------------------ RUN LAMSCRIPTEN -------------------------------

pushd "$ROOT_DIR/build/bin" > /dev/null

if [ "$LAMBDA_with_gdb" = true ]; then
    gdb --args ./lamscripten
else
    ./lamscripten
fi

LAMBDA_INFO "Finished executing lamscripten"

popd > /dev/null  # $ROOT_DIR/build/$LAMBDA_build/bin
popd > /dev/null  # $ROOT_DIR
