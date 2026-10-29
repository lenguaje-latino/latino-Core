#!/usr/bin/env bash
# Linux, GCC/Clang; requiere encabezados de desarrollo de readline.
set -euo pipefail
raiz_core=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
build_hash=$(mktemp -d "${TMPDIR:-/tmp}/latino-hash-tests-XXXXXX")
printf 'Directorio de pruebas: %s\n' "$build_hash"
"${CC:-cc}" -D_GNU_SOURCE -std=c11 -g -O1 \
    -fno-omit-frame-pointer -fsanitize=address,undefined \
    -fno-sanitize-recover=undefined -ffunction-sections -fdata-sections \
    -I"$raiz_core/include" \
    "$raiz_core/tests/hash_regression.c" \
    "$raiz_core/src/latdic.c" "$raiz_core/src/latlist.c" \
    "$raiz_core/src/latobj.c" "$raiz_core/src/latmem.c" \
    "$raiz_core/src/latgc.c" \
    -Wl,--gc-sections -lm -o "$build_hash/hash_regression"
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:halt_on_error=1}"
for caso in growth keys long80; do
    "$build_hash/hash_regression" "$caso"
done
