#!/usr/bin/env bash
set -euo pipefail
core=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
build=$(mktemp -d "${TMPDIR:-/tmp}/latino-constants-tests-XXXXXX")
printf 'Directorio de pruebas: %s\n' "$build"
flags=(-D_GNU_SOURCE -std=c11 -g -O1 -fsanitize=address,undefined -fno-sanitize-recover=undefined -fno-omit-frame-pointer -I"$core/include" -I"$core/src/linenoise")
"${CC:-cc}" "${flags[@]}" -Dmain=latino_cli_main -c "$core/src/latino.c" -o "$build/latino.o"
fuentes=(
    "$core/src/latast.c"
    "$core/src/latclass.c"
    "$core/src/latdic.c"
    "$core/src/latdo.c"
    "$core/src/latgc.c"
    "$core/src/latlex.c"
    "$core/src/latlist.c"
    "$core/src/latmem.c"
    "$core/src/latobj.c"
    "$core/src/latparse.c"
    "$core/src/latlocale.c"
    "$core/src/latbaselib.c"
    "$core/src/latdevlib.c"
    "$core/src/latdiclib.c"
    "$core/src/latfilelib.c"
    "$core/src/latlistlib.c"
    "$core/src/latmathlib.c"
    "$core/src/latpaqlib.c"
    "$core/src/latrepl.c"
    "$core/src/latstrlib.c"
    "$core/src/latsyslib.c"
    "$core/src/linenoise/utf8.c"
    "$core/src/linenoise/linenoise.c"
)
"${CC:-cc}" "${flags[@]}" "$core/tests/constants_regression.c" "${fuentes[@]}" "$build/latino.o" -lm -ldl "${LATINO_TEST_READLINE:--lreadline}" -o "$build/constants_regression"
export ASAN_OPTIONS="${ASAN_OPTIONS:-detect_leaks=1:halt_on_error=1}"
export UBSAN_OPTIONS="${UBSAN_OPTIONS:-halt_on_error=1:print_stacktrace=1}"
"$build/constants_regression"
