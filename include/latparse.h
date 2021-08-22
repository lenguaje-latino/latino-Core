/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_LATPARSE_H_INCLUDED
# define YY_YY_LATPARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMERICO = 258,                /* NUMERICO  */
    CADENA = 259,                  /* CADENA  */
    IDENTIFICADOR = 260,           /* IDENTIFICADOR  */
    VAR_ARGS = 261,                /* VAR_ARGS  */
    SI = 262,                      /* SI  */
    O_SI = 263,                    /* O_SI  */
    FIN = 264,                     /* FIN  */
    SINO = 265,                    /* SINO  */
    MIENTRAS = 266,                /* MIENTRAS  */
    REPETIR = 267,                 /* REPETIR  */
    ROMPER = 268,                  /* ROMPER  */
    CONTINUAR = 269,               /* CONTINUAR  */
    HASTA = 270,                   /* HASTA  */
    FUNCION = 271,                 /* FUNCION  */
    DESDE = 272,                   /* DESDE  */
    RETORNO = 273,                 /* RETORNO  */
    ELEGIR = 274,                  /* ELEGIR  */
    CASO = 275,                    /* CASO  */
    DEFECTO = 276,                 /* DEFECTO  */
    ATRIBUTO = 277,                /* ATRIBUTO  */
    VERDADERO = 278,               /* VERDADERO  */
    FALSO = 279,                   /* FALSO  */
    NULO = 280,                    /* NULO  */
    EXPONENTE = 281,               /* EXPONENTE  */
    PARA = 282,                    /* PARA  */
    EN = 283,                      /* EN  */
    RANGO = 284,                   /* RANGO  */
    IR = 285,                      /* IR  */
    MAYOR_QUE = 286,               /* MAYOR_QUE  */
    MENOR_QUE = 287,               /* MENOR_QUE  */
    MAYOR_IGUAL = 288,             /* MAYOR_IGUAL  */
    MENOR_IGUAL = 289,             /* MENOR_IGUAL  */
    IGUAL_LOGICO = 290,            /* IGUAL_LOGICO  */
    DIFERENTE = 291,               /* DIFERENTE  */
    Y_LOGICO = 292,                /* Y_LOGICO  */
    O_LOGICO = 293,                /* O_LOGICO  */
    INCREMENTO = 294,              /* INCREMENTO  */
    DECREMENTO = 295,              /* DECREMENTO  */
    CONCATENAR = 296,              /* CONCATENAR  */
    CONCATENAR_IGUAL = 297,        /* CONCATENAR_IGUAL  */
    MAS_IGUAL = 298,               /* MAS_IGUAL  */
    MENOS_IGUAL = 299,             /* MENOS_IGUAL  */
    POR_IGUAL = 300,               /* POR_IGUAL  */
    ENTRE_IGUAL = 301,             /* ENTRE_IGUAL  */
    MODULO_IGUAL = 302,            /* MODULO_IGUAL  */
    REGEX = 303,                   /* REGEX  */
    GLOBAL = 304,                  /* GLOBAL  */
    CLASE = 305                    /* CLASE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
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
