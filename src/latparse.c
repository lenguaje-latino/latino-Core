/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "latparse.y"


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

/* bison -y -oparse.c parse.y */
#define YYERROR_VERBOSE 1
#define YYDEBUG 1
#define YYENABLE_NLS 1
#define YYLEX_PARAM &yylval, &yylloc

#include <stddef.h>

#include "latino.h"
#include "latast.h"
#include "latlex.h"

#ifndef LAT_ERROR_FMT
#define LAT_ERROR_FMT "%s:%d:%d: %s\n"
#endif

#ifdef __linux
#include <libintl.h>
#define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#endif

int yyerror(struct YYLTYPE *yylloc_param, void *scanner, struct ast **root, const char *s);
int yylex (YYSTYPE * yylval_param,YYLTYPE * yylloc_param ,yyscan_t yyscanner);


#line 122 "latparse.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_LATPARSE_H_INCLUDED
# define YY_YY_LATPARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMERICO = 258,
    CADENA = 259,
    IDENTIFICADOR = 260,
    VAR_ARGS = 261,
    SI = 262,
    O_SI = 263,
    FIN = 264,
    SINO = 265,
    MIENTRAS = 266,
    REPETIR = 267,
    ROMPER = 268,
    CONTINUAR = 269,
    HASTA = 270,
    FUNCION = 271,
    DESDE = 272,
    RETORNO = 273,
    ELEGIR = 274,
    CASO = 275,
    DEFECTO = 276,
    ATRIBUTO = 277,
    VERDADERO = 278,
    FALSO = 279,
    NULO = 280,
    EXPONENTE = 281,
    PARA = 282,
    EN = 283,
    RANGO = 284,
    IR = 285,
    MAYOR_QUE = 286,
    MENOR_QUE = 287,
    MAYOR_IGUAL = 288,
    MENOR_IGUAL = 289,
    IGUAL_LOGICO = 290,
    DIFERENTE = 291,
    Y_LOGICO = 292,
    O_LOGICO = 293,
    INCREMENTO = 294,
    DECREMENTO = 295,
    CONCATENAR = 296,
    CONCATENAR_IGUAL = 297,
    MAS_IGUAL = 298,
    MENOS_IGUAL = 299,
    POR_IGUAL = 300,
    ENTRE_IGUAL = 301,
    MODULO_IGUAL = 302,
    REGEX = 303,
    GLOBAL = 304,
    CLASE = 305
  };
#endif
/* Tokens.  */
#define NUMERICO 258
#define CADENA 259
#define IDENTIFICADOR 260
#define VAR_ARGS 261
#define SI 262
#define O_SI 263
#define FIN 264
#define SINO 265
#define MIENTRAS 266
#define REPETIR 267
#define ROMPER 268
#define CONTINUAR 269
#define HASTA 270
#define FUNCION 271
#define DESDE 272
#define RETORNO 273
#define ELEGIR 274
#define CASO 275
#define DEFECTO 276
#define ATRIBUTO 277
#define VERDADERO 278
#define FALSO 279
#define NULO 280
#define EXPONENTE 281
#define PARA 282
#define EN 283
#define RANGO 284
#define IR 285
#define MAYOR_QUE 286
#define MENOR_QUE 287
#define MAYOR_IGUAL 288
#define MENOR_IGUAL 289
#define IGUAL_LOGICO 290
#define DIFERENTE 291
#define Y_LOGICO 292
#define O_LOGICO 293
#define INCREMENTO 294
#define DECREMENTO 295
#define CONCATENAR 296
#define CONCATENAR_IGUAL 297
#define MAS_IGUAL 298
#define MENOS_IGUAL 299
#define POR_IGUAL 300
#define ENTRE_IGUAL 301
#define MODULO_IGUAL 302
#define REGEX 303
#define GLOBAL 304
#define CLASE 305

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (ast **root, void *scanner);

#endif /* !YY_YY_LATPARSE_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  84
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1070

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  147
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  278

#define YYUNDEFTOK  2
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,     2,     2,     2,    56,     2,     2,
      61,    62,    54,    52,    66,    53,     2,    55,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,    65,
       2,    51,     2,    59,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    63,     2,    64,    58,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,    68,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   157,   157,   158,   162,   163,   164,   165,   169,   173,
     180,   184,   188,   192,   196,   203,   207,   214,   215,   216,
     217,   221,   222,   223,   227,   231,   235,   239,   243,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   270,
     273,   277,   282,   287,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   305,   306,   310,   311,   312,
     316,   317,   321,   324,   330,   331,   333,   335,   337,   339,
     341,   343,   344,   348,   351,   354,   360,   366,   367,   373,
     379,   381,   383,   385,   391,   392,   396,   399,   402,   408,
     410,   412,   415,   419,   423,   430,   434,   435,   436,   440,
     441,   445,   451,   457,   461,   462,   463,   473,   474,   475,
     476,   482,   486,   490,   491,   492,   502,   506,   507,   508,
     512,   513,   517,   521,   526,   531,   535,   541,   546,   551,
     555,   556,   557,   558,   562,   567,   572,   576
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMERICO", "CADENA", "IDENTIFICADOR",
  "VAR_ARGS", "SI", "O_SI", "FIN", "SINO", "MIENTRAS", "REPETIR", "ROMPER",
  "CONTINUAR", "HASTA", "FUNCION", "DESDE", "RETORNO", "ELEGIR", "CASO",
  "DEFECTO", "ATRIBUTO", "VERDADERO", "FALSO", "NULO", "EXPONENTE", "PARA",
  "EN", "RANGO", "IR", "MAYOR_QUE", "MENOR_QUE", "MAYOR_IGUAL",
  "MENOR_IGUAL", "IGUAL_LOGICO", "DIFERENTE", "Y_LOGICO", "O_LOGICO",
  "INCREMENTO", "DECREMENTO", "CONCATENAR", "CONCATENAR_IGUAL",
  "MAS_IGUAL", "MENOS_IGUAL", "POR_IGUAL", "ENTRE_IGUAL", "MODULO_IGUAL",
  "REGEX", "GLOBAL", "CLASE", "'='", "'+'", "'-'", "'*'", "'/'", "'%'",
  "'!'", "'^'", "'?'", "':'", "'('", "')'", "'['", "']'", "';'", "','",
  "'{'", "'}'", "$accept", "constant_expression", "primary_expression",
  "unary_expression", "multiplicative_expression", "additive_expression",
  "relational_expression", "equality_expression", "logical_not_expression",
  "logical_and_expression", "logical_or_expression", "ternary_expression",
  "concat_expression", "expression", "program", "statement_list",
  "statement", "incdec_statement", "variable_access", "field_designator",
  "global_declaration", "declaration", "labeled_statements",
  "labeled_statement_case", "labeled_statement_case_case",
  "labeled_statement_default", "selection_statement", "osi_statements",
  "osi_statement", "iteration_statement", "goto_etiqueta",
  "jump_statement", "jump_loop", "function_definition",
  "function_anonymous", "function_call", "argument_expression_list",
  "parameter_list", "list_new", "list_items", "dict_new", "dict_items",
  "dict_item", "clase_propiedad", "clase_propiedades", "clase_funcion",
  "clase_funciones", "clase_sentencia", "clase_sentencias",
  "clase_declaracion", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    61,    43,    45,    42,    47,    37,    33,    94,    63,
      58,    40,    41,    91,    93,    59,    44,   123,   125
};
# endif

#define YYPACT_NINF (-191)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-146)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     461,   626,   -34,  -191,   155,   155,   626,  -191,  -191,     5,
     -40,   155,   155,    32,    89,    35,    92,    42,  -191,   118,
    -191,   337,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,   363,   -39,  -191,  -191,  -191,  -191,  -191,   -17,  -191,
      -8,  -191,  -191,  -191,   155,   155,   155,   155,   155,   155,
    -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,   570,  -191,   -11,  -191,    -4,  -191,  -191,   570,
      43,    41,    75,   705,  -191,   782,    76,  -191,    49,   297,
    -191,  -191,  -191,    15,  -191,  -191,   117,  -191,  -191,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,    86,
     145,    12,    12,    12,   881,   741,    98,   949,    -5,  -191,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,    65,   155,
     143,   155,   145,   105,   155,   151,   112,   167,   154,  -191,
     153,   626,   141,   189,   155,   148,  -191,   172,  -191,   182,
     186,  -191,  1011,  1011,  1011,  1011,  1011,  1011,  1011,   705,
     134,   813,  -191,  -191,  -191,  -191,   -49,  -191,   155,  -191,
     155,   155,  -191,   138,    53,    53,    53,    53,    53,    53,
     450,   450,  1011,    53,    88,    88,    12,    12,    12,   -12,
     980,   155,  -191,   626,   177,  -191,   847,  -191,  1011,    -6,
     155,  -191,   139,   626,   626,  -191,  -191,   140,  -191,   155,
     144,   705,  -191,   626,  -191,   626,  -191,  -191,  -191,  -191,
     158,   626,  -191,  1011,  -191,   155,   570,   193,  -191,  -191,
    -191,   626,   505,   190,  -191,  -191,   155,  1011,   145,  -191,
    -191,   155,   197,  1011,    58,  -191,   202,    71,   151,   631,
      37,  1011,  -191,   626,   206,  -191,   157,  -191,   626,   155,
     626,  -191,   626,   211,   668,   214,   215,  -191,   626,   155,
    -191,  -191,   216,   915,  -191,   626,   220,  -191
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    67,   119,     0,     0,     0,   109,   110,     0,
       0,   114,     0,     0,     0,   117,     0,     0,    50,     0,
      61,     0,    68,    55,    54,    56,    57,    63,    58,    62,
      59,    60,     0,    64,    53,   105,     2,     3,     4,    48,
       0,     5,     6,     7,     0,     0,     0,     0,   123,   127,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      46,    40,     0,    47,    44,    45,    41,    42,    43,     0,
       0,     0,   117,   106,   107,     0,     0,   108,    67,     0,
      72,    73,    69,     0,     1,    51,     0,    65,    66,     0,
       0,     0,     0,     0,     0,     0,   114,     0,   114,     0,
     117,     9,     8,    24,     0,   124,     0,     0,     0,   128,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   117,     0,   114,     0,     0,     0,    84,    85,
       0,     0,     0,     0,   114,     0,   140,     0,   141,     0,
       0,    70,    75,    76,    77,    78,    79,    80,    74,   115,
       0,     0,    81,   120,   121,   118,     0,    29,   123,   122,
       0,   130,   126,    11,    17,    18,    19,    20,    22,    21,
      25,    26,    28,    23,    16,    15,    12,    13,    14,    10,
       0,     0,    90,     0,     0,    94,     0,    99,   100,     0,
       0,   116,     0,     0,     0,    93,    83,     0,   135,     0,
       0,   115,   143,     0,   133,     0,   137,   144,   147,   113,
      71,     0,   125,   131,   129,     0,     0,     0,    92,    95,
      71,     0,     0,    87,    86,    89,     0,   132,   117,   135,
     139,     0,     0,    27,    96,    91,     0,   117,     0,     0,
       0,    82,   112,     0,    98,   111,     0,    88,     0,     0,
       0,    97,     0,     0,     0,     0,     0,   102,     0,     0,
     136,   101,     0,     0,   103,     0,     0,   104
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -191,  -132,  -191,  -191,  -191,  -191,  -191,  -191,  -191,  -191,
    -191,  -191,  -191,   326,  -191,    94,   -15,    48,     0,  -191,
    -191,    -7,    95,  -191,   -18,  -191,  -191,   -10,  -190,  -191,
    -191,  -191,  -191,   221,  -191,    24,   -89,   -98,  -191,    70,
    -191,  -191,    69,  -191,    97,  -191,    96,  -191,    99,  -191
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,   159,    17,    18,    19,    63,    64,    22,
      23,    24,   137,   138,   203,   139,    25,   194,   195,    26,
      27,    28,    29,    30,    65,    66,    74,    32,    67,   106,
      68,   108,   109,   145,   146,   147,   148,   149,   150,    33
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    21,   166,   202,   229,   -67,    21,   160,    80,   162,
      71,    86,    98,   221,   110,    79,   141,    99,   -69,    21,
     142,    72,   -67,   -67,    31,    31,    35,    99,    87,    88,
      31,   143,  -118,   144,   199,   -69,   -69,    76,   110,    82,
      78,     3,    84,    31,   -67,   201,   -67,   127,    20,    20,
      96,     9,   129,   100,    20,   212,   231,   -69,   131,   -69,
      99,   171,    21,   172,   229,   133,   191,    20,   253,    21,
     126,   127,    79,   191,   192,   193,     2,     3,     4,   110,
      78,     3,     5,     6,     7,     8,    31,     9,    10,    11,
      12,   163,   164,    31,    77,    34,    82,    83,    13,   260,
      70,    14,   132,    99,   140,   121,   122,   123,   124,   125,
      20,   126,   127,    85,   110,  -118,   202,    20,   -52,     1,
      15,    16,   151,     2,     3,     4,   -52,   -52,   -52,     5,
       6,     7,     8,   -52,     9,    10,    11,    12,   -52,   -52,
     250,    21,   123,   124,   125,    13,   126,   127,    14,   213,
     165,     3,   197,   142,    36,    37,   128,  -134,    36,    37,
      38,    39,   169,   130,  -134,    31,  -134,    15,    16,  -117,
     200,    40,   204,   215,   135,   136,   205,  -138,    41,    42,
      43,  -138,   207,   141,  -117,   191,   228,   142,   143,    20,
    -138,  -145,   209,    21,   210,   218,   219,   127,   143,   233,
     144,   236,   245,    21,    21,   238,   252,    44,    45,   241,
     248,   255,    46,    21,   191,    21,    47,    31,    48,   262,
     267,    21,    49,   270,   271,   274,    21,    31,    31,   277,
     257,    21,   256,   206,   254,   208,    81,    31,   222,    31,
     224,    20,   214,   216,     0,    31,     0,    21,   217,     0,
      31,    20,    20,    21,     0,    31,     0,     0,    21,     0,
      21,    20,    21,    20,     0,     0,     0,     0,    21,    20,
       0,    31,     0,     0,    20,    21,     0,    31,     0,    20,
       0,     0,    31,     0,    31,     0,    31,   227,     0,     0,
       0,     0,    31,     0,     0,    20,     0,   234,   235,    31,
       0,    20,     0,     0,     0,     0,    20,   239,    20,   240,
      20,     0,     0,     0,     0,   242,    20,     0,     0,    86,
     244,     0,     0,    20,     0,   246,     0,     0,     0,     0,
      62,    69,     0,     0,     0,     0,     0,    73,    75,    89,
      90,    91,    92,    93,    94,     0,     0,   261,    95,     0,
       0,     0,   263,     0,   265,     0,   266,     0,    96,    86,
      97,     0,   272,     0,     0,     0,     0,     0,     0,   276,
     101,   102,   103,   104,   105,   107,    87,    88,     0,    89,
      90,    91,    92,    93,    94,   -69,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,     0,
      97,     0,   -69,   -69,     0,   -69,   -69,   -69,   -69,   -69,
     -69,     0,     0,     0,     0,   152,   153,   154,   155,   156,
     157,   158,     0,   161,   -69,     0,   -69,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     0,   196,     0,   198,     0,     0,
       0,   -49,     1,     0,     0,     0,     2,     3,     4,     0,
     211,     0,     5,     6,     7,     8,   110,     9,    10,    11,
      12,   111,   112,   113,   114,   115,   116,     0,    13,     0,
       0,    14,     0,     0,   105,     0,   223,   107,   120,     0,
       0,     0,   121,   122,   123,   124,   125,     0,   126,   127,
      15,    16,  -117,     0,     0,     0,     0,   226,     0,     0,
       0,     0,     0,     0,     0,     0,   232,  -117,     0,     0,
       0,   110,     0,     0,     0,   237,   111,   112,   113,   114,
     115,   116,   117,   118,     0,     0,   119,     0,     0,     0,
       0,   243,     0,   120,     0,     0,     0,   121,   122,   123,
     124,   125,   249,   126,   127,     0,     0,   251,     0,     0,
     247,     1,     0,     0,     0,     2,     3,     4,     0,     0,
       0,     5,     6,     7,     8,   264,     9,    10,    11,    12,
       0,     0,     0,     0,     0,   273,   110,    13,     0,     0,
      14,   111,   112,   113,   114,   115,   116,   117,   118,     0,
       0,   119,     0,     0,     0,     0,     0,     0,   120,    15,
      16,  -117,   121,   122,   123,   124,   125,     1,   126,   127,
       0,     2,     3,     4,     0,     0,  -117,     5,     6,     7,
       8,     0,     9,    10,    11,    12,     0,     0,     0,     0,
       0,     0,     0,    13,     0,     0,    14,   110,     0,     0,
       0,     0,   111,   112,   113,   114,   115,   116,   117,   118,
       0,     0,   119,     0,     0,    15,    16,  -117,     0,   120,
       0,     0,     0,   121,   122,   123,   124,   125,     0,   126,
     127,     0,  -117,   258,   110,     0,     0,   259,     0,   111,
     112,   113,   114,   115,   116,   117,   118,     0,     0,   119,
       0,     0,     0,     0,     0,     0,   120,     0,     0,     0,
     121,   122,   123,   124,   125,     0,   126,   127,     0,     0,
     268,   110,     0,     0,   269,     0,   111,   112,   113,   114,
     115,   116,   117,   118,     0,     0,   119,     0,     0,     0,
       0,     0,     0,   120,     0,     0,     0,   121,   122,   123,
     124,   125,     0,   126,   127,     0,     0,   110,     0,     0,
       0,   134,   111,   112,   113,   114,   115,   116,   117,   118,
       0,     0,   119,     0,     0,     0,     0,     0,     0,   120,
       0,     0,     0,   121,   122,   123,   124,   125,     0,   126,
     127,     0,   135,   136,     0,     0,     0,   168,   110,     0,
       0,     0,     0,   111,   112,   113,   114,   115,   116,   117,
     118,     0,     0,   119,     0,     0,     0,     0,     0,     0,
     120,     0,     0,     0,   121,   122,   123,   124,   125,   110,
     126,   127,     0,     0,   111,   112,   113,   114,   115,   116,
     117,   118,     0,     0,   119,     0,     0,     0,     0,     0,
       0,   120,     0,     0,     0,   121,   122,   123,   124,   125,
       0,   126,   127,   110,     0,     0,     0,   220,   111,   112,
     113,   114,   115,   116,   117,   118,     0,     0,   119,     0,
       0,     0,     0,     0,     0,   120,     0,     0,     0,   121,
     122,   123,   124,   125,     0,   126,   127,   110,     0,     0,
       0,   230,   111,   112,   113,   114,   115,   116,   117,   118,
       0,     0,   119,     0,     0,     0,     0,     0,     0,   120,
       0,     0,     0,   121,   122,   123,   124,   125,     0,   126,
     127,   110,     0,   167,     0,     0,   111,   112,   113,   114,
     115,   116,   117,   118,     0,     0,   119,     0,     0,     0,
       0,     0,     0,   120,     0,     0,     0,   121,   122,   123,
     124,   125,     0,   126,   127,   110,     0,   275,     0,     0,
     111,   112,   113,   114,   115,   116,   117,   118,     0,     0,
     119,     0,     0,     0,     0,     0,     0,   120,     0,     0,
       0,   121,   122,   123,   124,   125,   110,   126,   127,   170,
       0,   111,   112,   113,   114,   115,   116,   117,   118,     0,
       0,   119,     0,     0,     0,     0,     0,     0,   120,     0,
       0,     0,   121,   122,   123,   124,   125,   110,   126,   127,
     225,     0,   111,   112,   113,   114,   115,   116,   117,   118,
       0,     0,   119,     0,     0,     0,     0,     0,     0,   120,
       0,     0,     0,   121,   122,   123,   124,   125,     0,   126,
     127
};

static const yytype_int16 yycheck[] =
{
       0,     1,   100,   135,   194,    22,     6,    96,    15,    98,
       5,    22,    51,    62,    26,    15,     1,    66,    22,    19,
       5,    61,    39,    40,     0,     1,    60,    66,    39,    40,
       6,    16,    66,    18,   132,    39,    40,     5,    26,    15,
       5,     6,     0,    19,    61,   134,    63,    59,     0,     1,
      61,    16,    63,    61,     6,   144,    62,    61,    15,    63,
      66,    66,    62,    68,   254,    72,     8,    19,    10,    69,
      58,    59,    72,     8,     9,    10,     5,     6,     7,    26,
       5,     6,    11,    12,    13,    14,    62,    16,    17,    18,
      19,     5,     6,    69,     5,     1,    72,     5,    27,    62,
       6,    30,    61,    66,    28,    52,    53,    54,    55,    56,
      62,    58,    59,    19,    26,    66,   248,    69,     0,     1,
      49,    50,     5,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     238,   141,    54,    55,    56,    27,    58,    59,    30,     1,
       5,     6,     9,     5,     3,     4,    62,     9,     3,     4,
       5,     6,    64,    69,    16,   141,    18,    49,    50,    51,
      65,    16,    60,     1,    20,    21,     9,     5,    23,    24,
      25,     9,    29,     1,    66,     8,     9,     5,    16,   141,
      18,     9,    51,   193,     5,     9,    62,    59,    16,    60,
      18,    61,     9,   203,   204,    61,     9,    52,    53,    51,
      20,     9,    57,   213,     8,   215,    61,   193,    63,    62,
       9,   221,    67,     9,     9,     9,   226,   203,   204,     9,
     248,   231,   247,   138,   244,   141,    15,   213,   168,   215,
     171,   193,   145,   147,    -1,   221,    -1,   247,   149,    -1,
     226,   203,   204,   253,    -1,   231,    -1,    -1,   258,    -1,
     260,   213,   262,   215,    -1,    -1,    -1,    -1,   268,   221,
      -1,   247,    -1,    -1,   226,   275,    -1,   253,    -1,   231,
      -1,    -1,   258,    -1,   260,    -1,   262,   193,    -1,    -1,
      -1,    -1,   268,    -1,    -1,   247,    -1,   203,   204,   275,
      -1,   253,    -1,    -1,    -1,    -1,   258,   213,   260,   215,
     262,    -1,    -1,    -1,    -1,   221,   268,    -1,    -1,    22,
     226,    -1,    -1,   275,    -1,   231,    -1,    -1,    -1,    -1,
       4,     5,    -1,    -1,    -1,    -1,    -1,    11,    12,    42,
      43,    44,    45,    46,    47,    -1,    -1,   253,    51,    -1,
      -1,    -1,   258,    -1,   260,    -1,   262,    -1,    61,    22,
      63,    -1,   268,    -1,    -1,    -1,    -1,    -1,    -1,   275,
      44,    45,    46,    47,    48,    49,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    22,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,
      63,    -1,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    -1,    97,    61,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,    -1,   129,    -1,   131,    -1,    -1,
      -1,     0,     1,    -1,    -1,    -1,     5,     6,     7,    -1,
     144,    -1,    11,    12,    13,    14,    26,    16,    17,    18,
      19,    31,    32,    33,    34,    35,    36,    -1,    27,    -1,
      -1,    30,    -1,    -1,   168,    -1,   170,   171,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    -1,    58,    59,
      49,    50,    51,    -1,    -1,    -1,    -1,   191,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   200,    66,    -1,    -1,
      -1,    26,    -1,    -1,    -1,   209,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    -1,
      -1,   225,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,   236,    58,    59,    -1,    -1,   241,    -1,    -1,
      65,     1,    -1,    -1,    -1,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,   259,    16,    17,    18,    19,
      -1,    -1,    -1,    -1,    -1,   269,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,     1,    58,    59,
      -1,     5,     6,     7,    -1,    -1,    66,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    30,    26,    -1,    -1,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    49,    50,    51,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    -1,    58,
      59,    -1,    66,    62,    26,    -1,    -1,    66,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    -1,    58,    59,    -1,    -1,
      62,    26,    -1,    -1,    66,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    -1,    58,    59,    -1,    -1,    26,    -1,    -1,
      -1,    66,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    -1,    58,
      59,    -1,    20,    21,    -1,    -1,    -1,    66,    26,    -1,
      -1,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    52,    53,    54,    55,    56,    26,
      58,    59,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    52,    53,    54,    55,    56,
      -1,    58,    59,    26,    -1,    -1,    -1,    64,    31,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    -1,    58,    59,    26,    -1,    -1,
      -1,    64,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    -1,    58,
      59,    26,    -1,    62,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    52,    53,    54,
      55,    56,    -1,    58,    59,    26,    -1,    62,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    52,    53,    54,    55,    56,    26,    58,    59,    60,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    52,    53,    54,    55,    56,    26,    58,    59,
      60,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    52,    53,    54,    55,    56,    -1,    58,
      59
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     5,     6,     7,    11,    12,    13,    14,    16,
      17,    18,    19,    27,    30,    49,    50,    83,    84,    85,
      86,    87,    88,    89,    90,    95,    98,    99,   100,   101,
     102,   104,   106,   118,    84,    60,     3,     4,     5,     6,
      16,    23,    24,    25,    52,    53,    57,    61,    63,    67,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    86,    87,   103,   104,   107,   109,    82,
      84,     5,    61,    82,   105,    82,     5,     5,     5,    87,
      90,   102,   104,     5,     0,    84,    22,    39,    40,    42,
      43,    44,    45,    46,    47,    51,    61,    63,    51,    66,
      61,    82,    82,    82,    82,    82,   108,    82,   110,   111,
      26,    31,    32,    33,    34,    35,    36,    37,    38,    41,
      48,    52,    53,    54,    55,    56,    58,    59,    84,    63,
      84,    15,    61,    90,    66,    20,    21,    91,    92,    94,
      28,     1,     5,    16,    18,   112,   113,   114,   115,   116,
     117,     5,    82,    82,    82,    82,    82,    82,    82,    82,
     105,    82,   105,     5,     6,     5,   106,    62,    66,    64,
      60,    66,    68,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,     8,     9,    10,    96,    97,    82,     9,    82,   106,
      65,   105,    70,    93,    60,     9,    91,    29,    84,    51,
       5,    82,   105,     1,   113,     1,   115,   117,     9,    62,
      64,    62,   108,    82,   111,    60,    82,    84,     9,    97,
      64,    62,    82,    60,    84,    84,    61,    82,    61,    84,
      84,    51,    84,    82,    84,     9,    84,    65,    20,    82,
     106,    82,     9,    10,    96,     9,    85,    93,    62,    66,
      62,    84,    62,    84,    82,    84,    84,     9,    62,    66,
       9,     9,    84,    82,     9,    62,    84,     9
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    69,    70,    70,    71,    71,    71,    71,    72,    72,
      73,    73,    73,    73,    73,    74,    74,    75,    75,    75,
      75,    76,    76,    76,    77,    78,    79,    80,    81,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    83,
      83,    84,    84,    84,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    86,    86,    87,    87,    87,
      88,    88,    89,    89,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    91,    91,    91,    92,    93,    93,    94,
      95,    95,    95,    95,    96,    96,    97,    97,    97,    98,
      98,    98,    98,    98,    98,    99,   100,   100,   100,   101,
     101,   102,   103,   104,   105,   105,   105,   106,   106,   106,
     106,   106,   107,   108,   108,   108,   109,   110,   110,   110,
     111,   111,   112,   113,   113,   113,   114,   115,   115,   115,
     116,   116,   116,   116,   117,   117,   117,   118
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     3,     5,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       3,     4,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     6,     2,     1,     1,     3,     2,     4,     3,
       4,     6,     5,     4,     1,     2,     3,     5,     4,     4,
       4,    10,     9,    11,    13,     2,     2,     2,     2,     1,
       1,     7,     6,     4,     0,     1,     3,     0,     1,     1,
       3,     3,     3,     0,     1,     3,     3,     0,     1,     3,
       0,     3,     3,     2,     1,     2,     7,     2,     1,     2,
       1,     1,     2,     2,     2,     1,     2,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, root, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, root, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ast **root, void *scanner)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (root);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ast **root, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, root, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, ast **root, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , root, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, root, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ast **root, void *scanner)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (root);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ast **root, void *scanner)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 5:
#line 163 "latparse.y"
                { (yyval.node) = latA_logico(1, (yylsp[0]).first_line, (yylsp[0]).first_column);}
#line 1927 "latparse.c"
    break;

  case 6:
#line 164 "latparse.y"
            { (yyval.node) = latA_logico(0, (yylsp[0]).first_line, (yylsp[0]).first_column);}
#line 1933 "latparse.c"
    break;

  case 7:
#line 165 "latparse.y"
           { (yyval.node) = latA_nulo(NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);}
#line 1939 "latparse.c"
    break;

  case 8:
#line 169 "latparse.y"
                               {
        (yyval.node) = latA_nodo(NODO_MENOS_UNARIO, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 1948 "latparse.c"
    break;

  case 9:
#line 173 "latparse.y"
                               {
        (yyval.node) = latA_nodo(NODO_MAS_UNARIO, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 1957 "latparse.c"
    break;

  case 10:
#line 180 "latparse.y"
                                {
        (yyval.node) = latA_nodo(NODO_POTENCIA, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 1966 "latparse.c"
    break;

  case 11:
#line 184 "latparse.y"
                                      {
        (yyval.node) = latA_nodo(NODO_POTENCIA, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 1975 "latparse.c"
    break;

  case 12:
#line 188 "latparse.y"
                                {
        (yyval.node) = latA_nodo(NODO_MULTIPLICACION, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 1984 "latparse.c"
    break;

  case 13:
#line 192 "latparse.y"
                                {
        (yyval.node) = latA_nodo(NODO_DIVISION, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 1993 "latparse.c"
    break;

  case 14:
#line 196 "latparse.y"
                                          {
        (yyval.node) = latA_nodo(NODO_MODULO, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 2002 "latparse.c"
    break;

  case 15:
#line 203 "latparse.y"
                                {
        (yyval.node) = latA_nodo(NODO_RESTA, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 2011 "latparse.c"
    break;

  case 16:
#line 207 "latparse.y"
                                {
        (yyval.node) = latA_nodo(NODO_SUMA, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        if((yyval.node) == NULL) YYABORT;
    }
#line 2020 "latparse.c"
    break;

  case 17:
#line 214 "latparse.y"
                                      { (yyval.node) = latA_nodo(NODO_MAYOR_QUE, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2026 "latparse.c"
    break;

  case 18:
#line 215 "latparse.y"
                                      { (yyval.node) = latA_nodo(NODO_MENOR_QUE, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2032 "latparse.c"
    break;

  case 19:
#line 216 "latparse.y"
                                        { (yyval.node) = latA_nodo(NODO_MAYOR_IGUAL, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2038 "latparse.c"
    break;

  case 20:
#line 217 "latparse.y"
                                        { (yyval.node) = latA_nodo(NODO_MENOR_IGUAL, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2044 "latparse.c"
    break;

  case 21:
#line 221 "latparse.y"
                                      { (yyval.node) = latA_nodo(NODO_DESIGUALDAD, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2050 "latparse.c"
    break;

  case 22:
#line 222 "latparse.y"
                                         { (yyval.node) = latA_nodo(NODO_IGUALDAD, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2056 "latparse.c"
    break;

  case 23:
#line 223 "latparse.y"
                                  { (yyval.node) = latA_nodo(NODO_REGEX, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2062 "latparse.c"
    break;

  case 24:
#line 227 "latparse.y"
                               { (yyval.node) = latA_nodo(NODO_NO, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2068 "latparse.c"
    break;

  case 25:
#line 231 "latparse.y"
                                     { (yyval.node) = latA_nodo(NODO_Y, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2074 "latparse.c"
    break;

  case 26:
#line 235 "latparse.y"
                                     { (yyval.node) = latA_nodo(NODO_O, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2080 "latparse.c"
    break;

  case 27:
#line 239 "latparse.y"
                                                 { (yyval.node) = latA_si((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 2086 "latparse.c"
    break;

  case 28:
#line 243 "latparse.y"
                                       { (yyval.node) = latA_nodo(NODO_CONCATENAR, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2092 "latparse.c"
    break;

  case 29:
#line 247 "latparse.y"
                         { (yyval.node) = (yyvsp[-1].node); }
#line 2098 "latparse.c"
    break;

  case 48:
#line 266 "latparse.y"
               { (yyval.node) = latA_nodo(NODO_LOAD_VAR_ARGS , NULL, NULL, 0, 0); }
#line 2104 "latparse.c"
    break;

  case 49:
#line 270 "latparse.y"
      { /* empty */
        *root = NULL;
    }
#line 2112 "latparse.c"
    break;

  case 50:
#line 273 "latparse.y"
                     { *root = (yyvsp[0].node); }
#line 2118 "latparse.c"
    break;

  case 51:
#line 277 "latparse.y"
                               {
        if((yyvsp[0].node)){
            (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[-1].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        }
    }
#line 2128 "latparse.c"
    break;

  case 52:
#line 282 "latparse.y"
                {
        if((yyvsp[0].node)){
          (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
        }
    }
#line 2138 "latparse.c"
    break;

  case 53:
#line 287 "latparse.y"
                           { yyerrok; yyclearin;}
#line 2144 "latparse.c"
    break;

  case 65:
#line 305 "latparse.y"
                                 { (yyval.node) = latA_nodo(NODO_INC, (yyvsp[-1].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2150 "latparse.c"
    break;

  case 66:
#line 306 "latparse.y"
                                 { (yyval.node) = latA_nodo(NODO_DEC, (yyvsp[-1].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2156 "latparse.c"
    break;

  case 70:
#line 316 "latparse.y"
                                             { (yyval.node) = latA_nodo(NODO_ATRIBUTO, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2162 "latparse.c"
    break;

  case 71:
#line 317 "latparse.y"
                                         { (yyval.node) = latA_nodo(NODO_LISTA_OBTENER_ELEMENTO, (yyvsp[-1].node), (yyvsp[-3].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2168 "latparse.c"
    break;

  case 72:
#line 321 "latparse.y"
                         {
        (yyval.node) = latA_nodo(NODO_GLOBAL, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2176 "latparse.c"
    break;

  case 73:
#line 324 "latparse.y"
                                 {
        (yyval.node) = latA_nodo(NODO_GLOBAL, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2184 "latparse.c"
    break;

  case 74:
#line 330 "latparse.y"
                                     { (yyval.node) = latA_asign((yyvsp[0].node), (yyvsp[-2].node)); }
#line 2190 "latparse.c"
    break;

  case 75:
#line 331 "latparse.y"
                                                  { (yyval.node) = latA_asign(
        (latA_nodo(NODO_CONCATENAR, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column)), (yyvsp[-2].node)); }
#line 2197 "latparse.c"
    break;

  case 76:
#line 333 "latparse.y"
                                           { (yyval.node) = latA_asign(
        (latA_nodo(NODO_SUMA, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column)), (yyvsp[-2].node)); }
#line 2204 "latparse.c"
    break;

  case 77:
#line 335 "latparse.y"
                                             { (yyval.node) = latA_asign(
        (latA_nodo(NODO_RESTA, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column)), (yyvsp[-2].node)); }
#line 2211 "latparse.c"
    break;

  case 78:
#line 337 "latparse.y"
                                           { (yyval.node) = latA_asign(
        (latA_nodo(NODO_MULTIPLICACION, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column)), (yyvsp[-2].node)); }
#line 2218 "latparse.c"
    break;

  case 79:
#line 339 "latparse.y"
                                             { (yyval.node) = latA_asign(
        (latA_nodo(NODO_DIVISION, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column)), (yyvsp[-2].node)); }
#line 2225 "latparse.c"
    break;

  case 80:
#line 341 "latparse.y"
                                              { (yyval.node) = latA_asign(
        (latA_nodo(NODO_MODULO, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column)), (yyvsp[-2].node)); }
#line 2232 "latparse.c"
    break;

  case 81:
#line 343 "latparse.y"
                                                  { (yyval.node) = latA_asign((yyvsp[0].node), (yyvsp[-2].node)); }
#line 2238 "latparse.c"
    break;

  case 82:
#line 344 "latparse.y"
                                                        { (yyval.node) = latA_asign_le((yyvsp[0].node), (yyvsp[-5].node), (yyvsp[-3].node)); }
#line 2244 "latparse.c"
    break;

  case 83:
#line 348 "latparse.y"
                                                {
        (yyval.node) = latA_nodo(NODO_CASOS, (yyvsp[-1].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
    }
#line 2252 "latparse.c"
    break;

  case 84:
#line 351 "latparse.y"
                             {
        (yyval.node) = latA_nodo(NODO_CASOS, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2260 "latparse.c"
    break;

  case 85:
#line 354 "latparse.y"
                                {
        (yyval.node) = latA_nodo(NODO_CASOS, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2268 "latparse.c"
    break;

  case 86:
#line 360 "latparse.y"
                                                      {
        (yyval.node) = latA_nodo(NODO_CASO, (yyvsp[-1].node), (yyvsp[0].node), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
    }
#line 2276 "latparse.c"
    break;

  case 88:
#line 367 "latparse.y"
                                                               {
        (yyval.node) = latA_nodo(NODO_CASOS, (yyvsp[-3].node), (yyvsp[0].node), (yylsp[-3]).first_line, (yylsp[-3]).first_column);
    }
#line 2284 "latparse.c"
    break;

  case 89:
#line 373 "latparse.y"
                                 {
        (yyval.node) = latA_nodo(NODO_DEFECTO, NULL, (yyvsp[0].node), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
    }
#line 2292 "latparse.c"
    break;

  case 90:
#line 379 "latparse.y"
                                       {
        (yyval.node) = latA_si((yyvsp[-2].node), (yyvsp[-1].node), NULL); }
#line 2299 "latparse.c"
    break;

  case 91:
#line 381 "latparse.y"
                                                           {
        (yyval.node) = latA_si((yyvsp[-4].node), (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2306 "latparse.c"
    break;

  case 92:
#line 383 "latparse.y"
                                                      {
        (yyval.node) = latA_si((yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[-1].node)); }
#line 2313 "latparse.c"
    break;

  case 93:
#line 385 "latparse.y"
                                               {
        (yyval.node) = latA_nodo(NODO_ELEGIR, (yyvsp[-2].node), (yyvsp[-1].node), (yylsp[-3]).first_line, (yylsp[-3]).first_column);
    }
#line 2321 "latparse.c"
    break;

  case 96:
#line 396 "latparse.y"
                                     {
          (yyval.node) = latA_si((yyvsp[-1].node), (yyvsp[0].node), NULL);
    }
#line 2329 "latparse.c"
    break;

  case 97:
#line 399 "latparse.y"
                                                         {
          (yyval.node) = latA_si((yyvsp[-3].node), (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 2337 "latparse.c"
    break;

  case 98:
#line 402 "latparse.y"
                                                    {
          (yyval.node) = latA_si((yyvsp[-2].node), (yyvsp[-1].node), (yyvsp[0].node));
    }
#line 2345 "latparse.c"
    break;

  case 99:
#line 408 "latparse.y"
                                             {
        (yyval.node) = latA_mientras((yyvsp[-2].node), (yyvsp[-1].node)); }
#line 2352 "latparse.c"
    break;

  case 100:
#line 410 "latparse.y"
                                              {
        (yyval.node) = latA_hacer((yyvsp[0].node), (yyvsp[-2].node)); }
#line 2359 "latparse.c"
    break;

  case 101:
#line 413 "latparse.y"
                            {
        (yyval.node) = latA_desde((yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2366 "latparse.c"
    break;

  case 102:
#line 416 "latparse.y"
                           {
        (yyval.node) = latA_para((yyvsp[-7].node), NULL, (yyvsp[-3].node), NULL, (yyvsp[-1].node));
        }
#line 2374 "latparse.c"
    break;

  case 103:
#line 420 "latparse.y"
                           {
        (yyval.node) = latA_para((yyvsp[-9].node), (yyvsp[-5].node), (yyvsp[-3].node), NULL, (yyvsp[-1].node));
        }
#line 2382 "latparse.c"
    break;

  case 104:
#line 424 "latparse.y"
                           {
        (yyval.node) = latA_para((yyvsp[-11].node), (yyvsp[-7].node), (yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node));
        }
#line 2390 "latparse.c"
    break;

  case 105:
#line 430 "latparse.y"
                        { (yyval.node) = latA_nodo(NODO_ETIQUETA, (yyvsp[-1].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2396 "latparse.c"
    break;

  case 106:
#line 434 "latparse.y"
                         { (yyval.node) = latA_nodo(NODO_RETORNO, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2402 "latparse.c"
    break;

  case 107:
#line 435 "latparse.y"
                                       { (yyval.node) = latA_nodo(NODO_RETORNO, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2408 "latparse.c"
    break;

  case 108:
#line 436 "latparse.y"
                       { (yyval.node) =  latA_nodo(NODO_IR, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2414 "latparse.c"
    break;

  case 109:
#line 440 "latparse.y"
                { (yyval.node) = latA_nodo(NODO_ROMPER, NULL, NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2420 "latparse.c"
    break;

  case 110:
#line 441 "latparse.y"
                { (yyval.node) = latA_nodo(NODO_CONTINUAR, NULL, NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2426 "latparse.c"
    break;

  case 111:
#line 445 "latparse.y"
                                                                      {
        (yyval.node) = latA_funcion((yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node), (yylsp[-5]).first_line, (yylsp[-5]).first_column);
    }
#line 2434 "latparse.c"
    break;

  case 112:
#line 451 "latparse.y"
                                                        {
        (yyval.node) = latA_funcion(latA_var("anonima", (yylsp[-5]).first_line, (yylsp[-5]).first_column, false), (yyvsp[-3].node), (yyvsp[-1].node), (yylsp[-5]).first_line, (yylsp[-5]).first_column);
    }
#line 2442 "latparse.c"
    break;

  case 113:
#line 457 "latparse.y"
                                                       { (yyval.node) = latA_nodo(NODO_FUNCION_LLAMADA, (yyvsp[-3].node), (yyvsp[-1].node), (yylsp[-3]).first_line, (yylsp[-3]).first_column); }
#line 2448 "latparse.c"
    break;

  case 114:
#line 461 "latparse.y"
                  { (yyval.node) = NULL; }
#line 2454 "latparse.c"
    break;

  case 115:
#line 462 "latparse.y"
                 { (yyval.node) = latA_nodo(NODO_FUNCION_ARGUMENTOS, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2460 "latparse.c"
    break;

  case 116:
#line 463 "latparse.y"
                                              {
        if((yyvsp[-2].node)->tipo == NODO_VAR_ARGS){
            printf(LAT_ERROR_FMT, filename, (yylsp[-2]).first_line, (yylsp[-2]).first_column, "Parametro VAR_ARGS (...) debe de ser el ultimo parametro");
            YYABORT;
        }
        (yyval.node) = latA_nodo(NODO_FUNCION_ARGUMENTOS, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-2]).first_line, (yylsp[-2]).first_column);
    }
#line 2472 "latparse.c"
    break;

  case 117:
#line 473 "latparse.y"
                  { (yyval.node) = NULL; }
#line 2478 "latparse.c"
    break;

  case 118:
#line 474 "latparse.y"
                    { (yyval.node) = latA_nodo(NODO_FUNCION_PARAMETROS, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2484 "latparse.c"
    break;

  case 119:
#line 475 "latparse.y"
               { (yyval.node) = latA_nodo(NODO_FUNCION_PARAMETROS, (yyvsp[0].node), NULL, 0, 0); }
#line 2490 "latparse.c"
    break;

  case 120:
#line 476 "latparse.y"
                                       {
        if((yyvsp[-2].node)->izq->tipo == NODO_VAR_ARGS){
            printf(LAT_ERROR_FMT, (yylsp[0]).file_name, (yylsp[0]).first_line, (yylsp[0]).first_column, "Parametro VAR_ARGS (...) debe de ser el ultimo parametro");
            YYABORT;
        }
        (yyval.node) = latA_nodo(NODO_FUNCION_PARAMETROS, (yyvsp[0].node), (yyvsp[-2].node), (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2501 "latparse.c"
    break;

  case 121:
#line 482 "latparse.y"
                                  { (yyval.node) = latA_nodo(NODO_FUNCION_PARAMETROS, (yyvsp[0].node), (yyvsp[-2].node), 0, 0); }
#line 2507 "latparse.c"
    break;

  case 122:
#line 486 "latparse.y"
                         { (yyval.node) = latA_nodo(NODO_LISTA, (yyvsp[-1].node), NULL, (yylsp[-2]).first_line, (yylsp[-2]).first_column); }
#line 2513 "latparse.c"
    break;

  case 123:
#line 490 "latparse.y"
                  { (yyval.node) = NULL; }
#line 2519 "latparse.c"
    break;

  case 124:
#line 491 "latparse.y"
                 { (yyval.node) = latA_nodo(NODO_LISTA_AGREGAR_ELEMENTO, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2525 "latparse.c"
    break;

  case 125:
#line 492 "latparse.y"
                                {
        if((yyvsp[-2].node)->tipo == NODO_LOAD_VAR_ARGS){
            printf(LAT_ERROR_FMT, filename, (yylsp[-2]).first_line, (yylsp[-2]).first_column, "Parametro VAR_ARGS (...) debe de ser el ultimo parametro");
            YYABORT;
        }
        (yyval.node) = latA_nodo(NODO_LISTA_AGREGAR_ELEMENTO, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[0]).first_line, (yylsp[0]).first_column);
    }
#line 2537 "latparse.c"
    break;

  case 126:
#line 502 "latparse.y"
                         { (yyval.node) = latA_nodo(NODO_DICCIONARIO, (yyvsp[-1].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2543 "latparse.c"
    break;

  case 127:
#line 506 "latparse.y"
                  { (yyval.node) = NULL; }
#line 2549 "latparse.c"
    break;

  case 128:
#line 507 "latparse.y"
                { (yyval.node) = latA_nodo(NODO_DICC_AGREGAR_ELEMENTO, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2555 "latparse.c"
    break;

  case 129:
#line 508 "latparse.y"
                               { (yyval.node) = latA_nodo(NODO_DICC_AGREGAR_ELEMENTO, (yyvsp[0].node), (yyvsp[-2].node), (yylsp[0]).first_line, (yylsp[0]).first_column); }
#line 2561 "latparse.c"
    break;

  case 130:
#line 512 "latparse.y"
      { /* empty */ (yyval.node) = NULL; }
#line 2567 "latparse.c"
    break;

  case 131:
#line 513 "latparse.y"
                                { (yyval.node) = latA_nodo(NODO_DICC_ELEMENTO, (yyvsp[-2].node), (yyvsp[0].node), (yylsp[-2]).first_line, (yylsp[-2]).first_column); }
#line 2573 "latparse.c"
    break;

  case 132:
#line 517 "latparse.y"
                                   { (yyval.node) = latA_asign((yyvsp[0].node), (yyvsp[-2].node)); }
#line 2579 "latparse.c"
    break;

  case 133:
#line 521 "latparse.y"
                                        {
        if((yyvsp[0].node)){
            (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[-1].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        }
    }
#line 2589 "latparse.c"
    break;

  case 134:
#line 526 "latparse.y"
                      {
        if((yyvsp[0].node)){
          (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
        }
    }
#line 2599 "latparse.c"
    break;

  case 135:
#line 531 "latparse.y"
                           { yyerrok; yyclearin;}
#line 2605 "latparse.c"
    break;

  case 136:
#line 535 "latparse.y"
                                                                      {
        (yyval.node) = latA_funcion((yyvsp[-5].node), (yyvsp[-3].node), (yyvsp[-1].node), (yylsp[-5]).first_line, (yylsp[-5]).first_column);
    }
#line 2613 "latparse.c"
    break;

  case 137:
#line 541 "latparse.y"
                                    {
        if((yyvsp[0].node)){
            (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[-1].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        }
    }
#line 2623 "latparse.c"
    break;

  case 138:
#line 546 "latparse.y"
                    {
        if((yyvsp[0].node)){
          (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
        }
    }
#line 2633 "latparse.c"
    break;

  case 139:
#line 551 "latparse.y"
                           { yyerrok; yyclearin;}
#line 2639 "latparse.c"
    break;

  case 142:
#line 557 "latparse.y"
                         { (yyval.node) = latA_nodo(NODO_RETORNO, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2645 "latparse.c"
    break;

  case 143:
#line 558 "latparse.y"
                                       { (yyval.node) = latA_nodo(NODO_RETORNO, (yyvsp[0].node), NULL, (yylsp[-1]).first_line, (yylsp[-1]).first_column); }
#line 2651 "latparse.c"
    break;

  case 144:
#line 562 "latparse.y"
                                       {
        if((yyvsp[0].node)){
            (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[-1].node), (yyvsp[0].node), (yylsp[-1]).first_line, (yylsp[-1]).first_column);
        }
    }
#line 2661 "latparse.c"
    break;

  case 145:
#line 567 "latparse.y"
                      {
        if((yyvsp[0].node)){
          (yyval.node) = latA_nodo(NODO_BLOQUE, (yyvsp[0].node), NULL, (yylsp[0]).first_line, (yylsp[0]).first_column);
        }
    }
#line 2671 "latparse.c"
    break;

  case 146:
#line 572 "latparse.y"
                           { yyerrok; yyclearin;}
#line 2677 "latparse.c"
    break;

  case 147:
#line 576 "latparse.y"
                                               {
        (yyval.node) = latA_clase((yyvsp[-2].node), NULL, (yyvsp[-1].node), (yylsp[-2]).first_line, (yylsp[-2]).first_column); }
#line 2684 "latparse.c"
    break;


#line 2688 "latparse.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, root, scanner, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, root, scanner, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, root, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, root, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, root, scanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, root, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp, root, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 580 "latparse.y"


//se define para analisis sintactico (bison)
int yyerror(struct YYLTYPE *yylloc_param, void *scanner, struct ast **root,
const char *s) {
  if(!parse_silent) {
      fprintf(stderr, LAT_ERROR_FMT, yylloc_param->file_name,
        yylloc_param->first_line, yylloc_param->first_column,  s);
  }
  return 0;
}
