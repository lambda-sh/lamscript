# setup.sh
#
# Setup the Lamscript workspace for development & building.
ROOT_DIR="$(git rev-parse --show-toplevel)"
pushd $ROOT_DIR > /dev/null

# ------------------------------ UPDATE SUBMODULES -----------------------------

git submodule update --init --recursive

# ------------------------------- LAMBDA-SH SETUP ------------------------------

source lambda-sh/lambda.sh

LAMBDA_INFO "Successfully installed submodules and setup lambda.sh"

LAMBDA_PARSE_ARG within-ci false \
    "Used when setup is being done within a CI system."
LAMBDA_COMPILE_ARGS $@

if [ "$LAMBDA_within_ci" = true ]; then
    exit
fi

# ------------------------------ PRE-COMMIT SETUP ------------------------------

if command -v pre-commit > /dev/null; then
    pre-commit install > /dev/null
    LAMBDA_INFO "Installed pre-commit hooks."
else
    LAMBDA_FATAL "pre-commit isn't installed. Aborting setup."
fi

LAMBDA_INFO "Setup successfully completed."

popd > /dev/null  # ROOT_DIR
