#!/usr/bin/env bash
set -euo pipefail
raiz_core=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
build_bytecode=$(mktemp -d "${TMPDIR:-/tmp}/latino-bytecode-tests-XXXXXX")
printf 'Directorio de pruebas: %s\n' "$build_bytecode"
"${CC:-cc}" -D_GNU_SOURCE -DREGISTRY_04 -std=c11 -g -O1 \
    -fno-omit-frame-pointer -fsanitize=address,undefined \
    -fno-sanitize-recover=undefined -ffunction-sections -fdata-sections \
    -I"$raiz_core/include" -I"$raiz_core/src/linenoise" \
    "$raiz_core/tests/bytecode_regression.c" \
    "$raiz_core/src/latdic.c" "$raiz_core/src/latlist.c" \
    "$raiz_core/src/latobj.c" "$raiz_core/src/latmem.c" \
    "$raiz_core/src/latgc.c" \
    -Wl,--gc-sections -lm -o "$build_bytecode/bytecode_regression"
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:halt_on_error=1}"
"$build_bytecode/bytecode_regression"
