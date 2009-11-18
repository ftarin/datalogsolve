
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     _IS_IMPLIED_BY = 258,
     _END_OF_RULE = 259,
     _OPAR = 260,
     _CPAR = 261,
     _COMMA = 262,
     _COLON = 263,
     _DOMAIN_SECTION = 264,
     _RELATION_SECTION = 265,
     _RULE_SECTION = 266,
     _DOMAIN_ID = 267,
     _NUMBER = 268,
     _FILENAME = 269,
     _IONATURE = 270,
     _IDENTIFIER = 271,
     _IDENTIFIER2 = 272
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 74 "datalog_parser.y"

  char *NAIVE;
  CAESAR_TYPE_STRING CAESAR_IDENTIFIER;
  CAESAR_TYPE_STRING CAESAR_DOMAIN_NAME;
  CAESAR_TYPE_STRING CAESAR_FILE_NAME;
  CAESAR_TYPE_NATURAL CAESAR_NATURAL;
  CAESAR_TYPE_DOMAIN CAESAR_DOMAIN;
  CAESAR_TYPE_DOMAIN_LIST CAESAR_DOMAIN_LIST;
  CAESAR_TYPE_RELATION CAESAR_RELATION;
  CAESAR_TYPE_RELATION_LIST CAESAR_RELATION_LIST;
  CAESAR_TYPE_IONATURE CAESAR_IONATURE;
  CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_RULE_VARIABLE_LIST;
  CAESAR_TYPE_ATOM CAESAR_ATOM;
  CAESAR_TYPE_LITERAL CAESAR_LITERAL;
  CAESAR_TYPE_LITERAL_LIST CAESAR_LITERAL_LIST;
  CAESAR_TYPE_RULE CAESAR_RULE;
  CAESAR_TYPE_RULE_LIST CAESAR_RULE_LIST;



/* Line 1676 of yacc.c  */
#line 90 "datalog_parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


