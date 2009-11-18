%{
/******************************************************************************
 *                    C A E S A R _ D A T A L O G _ 1               
 *-----------------------------------------------------------------------------
 *   Technical University of Valencia
 *   DSIC / ELP
 *   Camino de Vera s/n
 *   46022 Valencia
 *   SPAIN
 *-----------------------------------------------------------------------------
 *   Module             :       %M%
 *   Authors            :       Maria ALPUENTE, Marco FELIU, Christophe JOUBERT
 *                              and Alicia VILLANUEVA
 *   Version            :       %R%.%L%
 *   Date               :       %E% %U%
 *****************************************************************************/

%}

%{

#include "caesar_solve_1.h"
#include "datalog_solve.h"
#include "datalog_basic_data_structures.h"
#include "datalog_parser.h"

extern YYSTYPE yylval;

%}


/* Options for the lexical scanner */

%option yylineno
%option case-sensitive


/* Start conditions */

/* Auxiliar regular expressions */

delimiter	[ \t\r]+
comment		#[^\n]*
identifier	[[:alpha:]][[:alnum:]_]*
number		[[:digit:]]+

%%

"### "[dD][oO][mM][aA][iI][nN][sS].*		{
                                CAESAR_BEGIN_DEBUG(4) {
                                printf("DOMAIN_SECTION");
                                } CAESAR_END_DEBUG;
				return _DOMAIN_SECTION;
			}

"### "[rR][eE][lL][aA][tT][iI][oO][nN][sS].*	{
                                CAESAR_BEGIN_DEBUG(4) {
				printf("RELATION_SECTION");
                                } CAESAR_END_DEBUG;
				return _RELATION_SECTION;
			}

"### "[rR][uU][lL][eE][sS].*		{
                                CAESAR_BEGIN_DEBUG(4) {
				printf("RULE_SECTION");
                                } CAESAR_END_DEBUG;
				return _RULE_SECTION;
			}

{delimiter} {}
{comment}		{
                                CAESAR_BEGIN_DEBUG(4) {
				printf("\n%3d\t**COMENTARIO**", yylineno);
                                } CAESAR_END_DEBUG;
			}
\n			{
                                CAESAR_BEGIN_DEBUG(4) {
				printf("\n%3d ", yylineno);
                                } CAESAR_END_DEBUG;
			}

("inputtuples"|"input"|"outputtuples"|"output")/(({delimiter}*)"\n")	{
                                CAESAR_BEGIN_DEBUG(4) {
                                printf("\n\tIONATURE: %s", yytext);
                                } CAESAR_END_DEBUG;
                                if (yytext[0] == 'i')
                                    yylval.CAESAR_IONATURE = CAESAR_INPUT_TUPLES;
                                else
                                    yylval.CAESAR_IONATURE = CAESAR_OUTPUT_TUPLES;
				return _IONATURE;
			}

":-"			{ return _IS_IMPLIED_BY; }
":"			{ return _COLON; }
"."			{ return _END_OF_RULE; }
","			{ return _COMMA; }
"("			{ return _OPAR; }
")"			{ return _CPAR; }

{number}		{ 
                          CAESAR_BEGIN_DEBUG(4) {
                          printf("\n\tNUMBER");
                          } CAESAR_END_DEBUG;
                          yylval.CAESAR_NATURAL = atoi(yytext); /* Care with int -> unsigned */
                          return _NUMBER;
                        }
(([[:alnum:]]|"."){2,})/(({delimiter}*)"\n")	{
                          CAESAR_BEGIN_DEBUG(4) {
                          printf("\n\tFILENAME");
                          } CAESAR_END_DEBUG;
                          yylval.CAESAR_FILE_NAME = CAESAR_COPY_STRING(yytext);
                          return _FILENAME; 
                        }


[A-z]+/({delimiter}+{number})	{
                                CAESAR_BEGIN_DEBUG(4) {
printf("\n\tDOMAIN_ID: %s", yytext);
                                } CAESAR_END_DEBUG;
yylval.CAESAR_DOMAIN_NAME = CAESAR_COPY_STRING(yytext);
				return _DOMAIN_ID;
			}

{identifier}		{
                                CAESAR_BEGIN_DEBUG(4) {
printf("\n\tIDENTIFIER: %s", yytext);
                                } CAESAR_END_DEBUG;
yylval.CAESAR_IDENTIFIER = CAESAR_COPY_STRING(yytext);
				return _IDENTIFIER;
			}

.			{
                                CAESAR_BEGIN_DEBUG(4) {
				printf("ERROR. Unknown character on line %d\n", yylineno);
                                } CAESAR_END_DEBUG;
			}


%%

