/*
The MIT License (MIT)

Copyright (c) Latino - Lenguaje de Programacion

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "latcompat.h"

double latC_strtod_c(const char *nptr, char **endptr) {
  /* Fuerza LC_NUMERIC="C" durante la conversión y restaura luego. */
  const char *oldloc = setlocale(LC_NUMERIC, NULL);
  char oldbuf[64];
  if (oldloc) {
    strncpy(oldbuf, oldloc, sizeof(oldbuf) - 1);
    oldbuf[sizeof(oldbuf) - 1] = '\0';
  } else {
    oldbuf[0] = '\0';
  }
  setlocale(LC_NUMERIC, "C");
  double v = strtod(nptr, endptr);
  if (oldbuf[0] != '\0') {
    setlocale(LC_NUMERIC, oldbuf);
  }
  return v;
}
