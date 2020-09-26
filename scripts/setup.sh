ROOT_DIR="$(git rev-parse --show-toplevel)"
pushd $ROOT_DIR > /dev/null

# Fetch submodules.
git submodule update --init

# Source lambda-sh for our bash scripts.
source lambda-sh/lambda.sh

LAMBDA_INFO "Successfully installed submodules and setup lambda.sh"

if command -v pre-commit > /dev/null; then
    pre-commit install > /dev/null
    LAMBDA_INFO "Installed pre-commit hooks."
else
    LAMBDA_FATAL "pre-commit isn't installed. Aborting setup."
fi

LAMBDA_INFO "Setup successfully completed."

popd > /dev/null  # ROOT_DIR
