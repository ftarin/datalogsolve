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
 *            
 *****************************************************************************/

#include <stdio.h>
#include "datalog_basic_data_structures.h"
#include "datalog_common.h"

%}

/* Keywords */

%token _IS_IMPLIED_BY
%token _END_OF_RULE
%token _OPAR _CPAR _COMMA
%token _COLON
%token _DOMAIN_SECTION _RELATION_SECTION _RULE_SECTION

/* Tokens */
%token <CAESAR_DOMAIN_NAME> _DOMAIN_ID
%token <CAESAR_NATURAL> _NUMBER
%token <CAESAR_FILE_NAME> _FILENAME
%token <CAESAR_IONATURE> _IONATURE
%token <CAESAR_IDENTIFIER> _IDENTIFIER
%token _IDENTIFIER2

/* Grammar axiom */

%start program

%{

#include "caesar_solve_1.h"
#include "datalog_solve.h"
#include "datalog_basic_data_structures.h"
  
/* Global variables*/
  
/* Domain list */
extern CAESAR_TYPE_DOMAIN_LIST CAESAR_DATALOG_DOMAIN_LIST;
  
/* Relation list */
extern CAESAR_TYPE_RELATION_LIST CAESAR_DATALOG_RELATION_LIST;
  
/* Rule list */
extern CAESAR_TYPE_RULE_LIST CAESAR_DATALOG_RULE_LIST;

/* Maximum number of relations on the right side of a rule's body */
extern CAESAR_TYPE_NATURAL CAESAR_DATALOG_MAX_RULE_BODY_SIZE;
  
extern int yylineno;
extern char yytext[];
void yyerror();
  
%}


/* Bison stack type */

%union {
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
}

/* Non-terminals' types */

%type <CAESAR_DOMAIN> domain
%type <CAESAR_DOMAIN_LIST> domains

%type <CAESAR_RELATION> relation
%type <CAESAR_IONATURE> relation_end
%type <CAESAR_RELATION_LIST> relations
%type <CAESAR_DOMAIN_LIST> parameter_list parameter_tail

%type <CAESAR_IDENTIFIER> term
%type <CAESAR_RULE_VARIABLE_LIST> parenthesized_list argument_list argument_tail
%type <CAESAR_ATOM> atom
%type <CAESAR_LITERAL> literal
%type <CAESAR_LITERAL_LIST> tail literal_list literal_tail
%type <CAESAR_RULE> clause
%type <CAESAR_RULE_LIST> rules

%%

/*============================================================================*/

program : 
_DOMAIN_SECTION domains
{
  /* We store the reversal of the DOMAIN_LIST */
  CAESAR_DATALOG_DOMAIN_LIST = CAESAR_REVERSE_DOMAIN_LIST($2);

  CAESAR_BEGIN_DEBUG(4) {
       
       CAESAR_PRINT_DOMAIN_LIST(CAESAR_DATALOG_DOMAIN_LIST);
       
  } CAESAR_END_DEBUG;
}
_RELATION_SECTION relations
{
  /* We store the RELATION_LIST */
  CAESAR_DATALOG_RELATION_LIST = $5;

  CAESAR_BEGIN_DEBUG(4) {
       
       CAESAR_PRINT_RELATION_LIST(CAESAR_DATALOG_RELATION_LIST);  
       
  } CAESAR_END_DEBUG;
}
_RULE_SECTION rules
{
  /* We store the RULE_LIST */
  CAESAR_DATALOG_RULE_LIST = $8;

  CAESAR_BEGIN_DEBUG(4) {
       
       CAESAR_PRINT_RULE_LIST(CAESAR_DATALOG_RULE_LIST);  
       
  } CAESAR_END_DEBUG;
}
;

/*============================================================================*/
/* Domains' rules */

domains : 
domain
{
  /* Creates a DOMAIN_LIST with the first the DOMAIN */
  $$ = CAESAR_CONSTRUCT_DOMAIN_LIST($1,(CAESAR_TYPE_DOMAIN_LIST) NULL);
} 
| domains domain
{
  /* Adds the new CAESAR_TYPE_DOMAIN to the CAESAR_TYPE_DOMAIN_and propagates the DOMAIN_LIST*/
  $$ = CAESAR_CONSTRUCT_DOMAIN_LIST($2,$1);
}
;

/*----------------------------------------------------------------------------*/

domain :
_DOMAIN_ID
_NUMBER _FILENAME
{
  /* Creates a DOMAIN */
  $$ = CAESAR_CREATE_DOMAIN($1, $2, $3);
}
;

/*============================================================================*/

relations : 
/* empty */
{
  $$ = (CAESAR_TYPE_RELATION_LIST) NULL;
}
| relation relations
{
  $$ = CAESAR_CONSTRUCT_RELATION_LIST($1, $2);
}
;

/*----------------------------------------------------------------------------*/

relation : _IDENTIFIER _OPAR parameter_list
{
  $<CAESAR_DOMAIN_LIST>$ = $3;
}
_CPAR relation_end
{
  CAESAR_TYPE_NATURAL LIST_LENGTH;
  LIST_LENGTH = CAESAR_DOMAIN_LIST_LENGTH($<CAESAR_DOMAIN_LIST>4);
  $$ = CAESAR_CREATE_RELATION($1, LIST_LENGTH, $<CAESAR_DOMAIN_LIST>4, $6);
}

relation_end : _IONATURE
{
	$$ = $1
} 
| 
{
	$$ = CAESAR_OUTPUT;
}
;

/*----------------------------------------------------------------------------*/

parameter_list :
_IDENTIFIER _COLON _IDENTIFIER
{
  CAESAR_TYPE_DOMAIN CAESAR_DOMAIN;

  CAESAR_DOMAIN = CAESAR_GET_DOMAIN_BY_VARIABLE_DOMAIN_NAME($3, CAESAR_DATALOG_DOMAIN_LIST);
  if (CAESAR_DOMAIN == NULL) {
    yyerror("Domain associated with a parameter non-existent");
    exit(1);
  }
  
  $<CAESAR_DOMAIN>$ = CAESAR_DOMAIN;
}
parameter_tail
{
  $$ = CAESAR_CONSTRUCT_DOMAIN_LIST($<CAESAR_DOMAIN>4, $5);
}
;

/*----------------------------------------------------------------------------*/

parameter_tail : 
/* empty */
{
  /* Creates the empty DOMAIN_LIST */
  $$ = (CAESAR_TYPE_DOMAIN_LIST) NULL;
}
|
_COMMA parameter_list
{
  /* Propagation of the DOMAIN_LIST */
  $$ = $2;
}
;

/*----------------------------------------------------------------------------*/

rules :
/* empty */ /* TODO */
{
  $$ = (CAESAR_TYPE_RULE_LIST) NULL;
}
|
clause rules
{
  $$ = (CAESAR_TYPE_RULE_LIST) CAESAR_CONSTRUCT_RULE_LIST($1, $2);
}
;

/*----------------------------------------------------------------------------*/

clause : atom tail _END_OF_RULE
{
  CAESAR_TYPE_NATURAL CURRENT_BODY_SIZE;

  $$ = (CAESAR_TYPE_RULE) CAESAR_CREATE_RULE($1, $2);

  CURRENT_BODY_SIZE = CAESAR_LITERAL_LIST_LENGTH($2);
  if (CAESAR_DATALOG_MAX_RULE_BODY_SIZE < CURRENT_BODY_SIZE)
    CAESAR_DATALOG_MAX_RULE_BODY_SIZE = CURRENT_BODY_SIZE ;
}
;

/*----------------------------------------------------------------------------*/

literal : _IDENTIFIER parenthesized_list
{
  CAESAR_TYPE_RELATION RELATION;
  RELATION = (CAESAR_TYPE_RELATION) CAESAR_GET_RELATION_BY_RELATION_NAME($1, CAESAR_DATALOG_RELATION_LIST);
  $$ = (CAESAR_TYPE_LITERAL) CAESAR_CREATE_LITERAL(RELATION, $2);
}
;

/*----------------------------------------------------------------------------*/

atom  : _IDENTIFIER parenthesized_list
{
  CAESAR_TYPE_RELATION RELATION;
  RELATION = (CAESAR_TYPE_RELATION) CAESAR_GET_RELATION_BY_RELATION_NAME($1, CAESAR_DATALOG_RELATION_LIST);

  if (RELATION == NULL) {
    yyerror("Head relation non-declared");
    exit(1);
  } else if (RELATION->ARITY != CAESAR_RULE_VARIABLE_LIST_LENGTH($2)) {
    yyerror("The no. of arguments of the head doesn't match its previous declaration");
    exit(1);
  }

  $$ = (CAESAR_TYPE_ATOM) CAESAR_CREATE_ATOM(RELATION, $2);
}
;

/*----------------------------------------------------------------------------*/

parenthesized_list :
_OPAR argument_list _CPAR
{
  /* Propagation of the RULE_VARIABLE_LIST */
  $$ = $2;

  CAESAR_BEGIN_DEBUG(4) {
       
       CAESAR_PRINT_RULE_VARIABLE_LIST_NAMES($2);
       
  } CAESAR_END_DEBUG;
}
;

/*----------------------------------------------------------------------------*/

argument_list : term argument_tail
{
  /* */
  $$ = (CAESAR_TYPE_RULE_VARIABLE_LIST) CAESAR_CONSTRUCT_RULE_VARIABLE_LIST($1, $2);
}
;

/*----------------------------------------------------------------------------*/

argument_tail : 
/* empty */ 
{
  /* Creates the empty DOMAIN_LIST */
  $$ = (CAESAR_TYPE_RULE_VARIABLE_LIST) NULL;
}
|
_COMMA argument_list
{
  /* Propagation of the RULE_VARIABLE_LIST */
  $$ = $2;
}
;

/*----------------------------------------------------------------------------*/

term : _IDENTIFIER /* TODO */
{
  /* Propagation of the RULE_VARIABLE name */
  $$ = $1;
}
;

/*----------------------------------------------------------------------------*/

tail : 
/* empty *//* TODO */
{
  $$ = (CAESAR_TYPE_LITERAL_LIST) NULL;
}
|
_IS_IMPLIED_BY literal_list
{
  $$ = $2;
}
;

/*----------------------------------------------------------------------------*/

literal_list : literal literal_tail
{
  $$ = (CAESAR_TYPE_LITERAL_LIST) CAESAR_CONSTRUCT_LITERAL_LIST($1, $2);
}
;

/*----------------------------------------------------------------------------*/

literal_tail :
/* empty */
{
  $$ = (CAESAR_TYPE_LITERAL_LIST) NULL;
}
|
_COMMA literal_list
{
  $$ = $2;
}
;

/*----------------------------------------------------------------------------*/

%%

void yyerror(char *caesar_s)
{
  (void) fflush(stdout);
  (void) fprintf(stdout, "\nline %d: %s at or near \"%s\"\n", yylineno, caesar_s, yytext);
}
