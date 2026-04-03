#!/bin/bash
echo "CLANG_TIDY_WRAPPER CALLED with sysroot: $(xcrun --show-sdk-path)" >&2
exec /opt/homebrew/opt/llvm/bin/clang-tidy \
  --extra-arg=-isysroot \
  --extra-arg="$(xcrun --show-sdk-path)" \
  "$@"
