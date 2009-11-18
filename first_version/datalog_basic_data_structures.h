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

#ifndef CAESAR_DATALOG_BASIC_DATA_STRUCTURES_INTERFACE
#define CAESAR_DATALOG_BASIC_DATA_STRUCTURES_INTERFACE

#include <string.h>

#include "caesar_standard.h"
#include "caesar_solve_1.h"
#include "caesar_area_1.h"
#include "caesar_table_1.h"
#include "caesar_hash.h"

#include "datalog_common.h"

/*============================================================================*/
/* Types */

/*----------------------------------------------------------------------------*/
/* Domain type */

typedef struct CAESAR_STRUCT_DOMAIN {
  /* Name of the domain */
  CAESAR_TYPE_STRING NAME;

  /* Number of elements in the domain */
  CAESAR_TYPE_NATURAL CARDINAL;

  /* File where its members are stored */
  CAESAR_TYPE_STRING FILENAME;
} CAESAR_BODY_DOMAIN, *CAESAR_TYPE_DOMAIN;

/*----------------------------------------------------------------------------*/
/* Domain list type */

typedef struct CAESAR_STRUCT_DOMAIN_LIST *CAESAR_TYPE_DOMAIN_LIST;

typedef struct CAESAR_STRUCT_DOMAIN_LIST {
  /* Domain */
  CAESAR_TYPE_DOMAIN DOMAIN;

  /* Successor domain */
  CAESAR_TYPE_DOMAIN_LIST NEXT;
} CAESAR_BODY_DOMAIN_LIST;

/*----------------------------------------------------------------------------*/
/* Relation type */

typedef struct CAESAR_STRUCT_RELATION {
  /* Name of the relation */
  CAESAR_TYPE_STRING NAME;

  /* Arity */
  CAESAR_TYPE_NATURAL ARITY;

  /* Origin and format of information */
  CAESAR_TYPE_IONATURE IONATURE;

  /* Arguments' domain list */
  CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST;
} CAESAR_BODY_RELATION, *CAESAR_TYPE_RELATION;

/*----------------------------------------------------------------------------*/
/* Relation list type */

typedef struct CAESAR_STRUCT_RELATION_LIST *CAESAR_TYPE_RELATION_LIST;

typedef struct CAESAR_STRUCT_RELATION_LIST {
  /* Relation*/
  CAESAR_TYPE_RELATION RELATION;
  
  /* Successor relation */
  CAESAR_TYPE_RELATION_LIST NEXT;
} CAESAR_BODY_RELATION_LIST;

/*----------------------------------------------------------------------------*/
/* Rule-variable list type */

typedef struct CAESAR_STRUCT_RULE_VARIABLE_LIST *CAESAR_TYPE_RULE_VARIABLE_LIST;

typedef struct CAESAR_STRUCT_RULE_VARIABLE_LIST {
  /* Variable */
  CAESAR_TYPE_STRING RULE_VARIABLE;

  /* Successor */
  CAESAR_TYPE_RULE_VARIABLE_LIST NEXT;
} CAESAR_BODY_RULE_VARIABLE_LIST;

/*----------------------------------------------------------------------------*/
/* Atom type */

typedef struct CAESAR_STRUCT_ATOM {
  /* Relation */
  CAESAR_TYPE_RELATION RELATION;

  /* Rule-variable list */
  CAESAR_TYPE_RULE_VARIABLE_LIST RULE_VARIABLE_LIST;
} CAESAR_BODY_ATOM, *CAESAR_TYPE_ATOM;

/*----------------------------------------------------------------------------*/
/* Literal type */

typedef struct CAESAR_STRUCT_LITERAL {
  /* Relation */
  CAESAR_TYPE_RELATION RELATION;

  /* Rule-variable list */
  CAESAR_TYPE_RULE_VARIABLE_LIST RULE_VARIABLE_LIST;
} CAESAR_BODY_LITERAL, *CAESAR_TYPE_LITERAL;

/*----------------------------------------------------------------------------*/
/* Literal list type */

typedef struct CAESAR_STRUCT_LITERAL_LIST *CAESAR_TYPE_LITERAL_LIST;

typedef struct CAESAR_STRUCT_LITERAL_LIST {
  /* Literal */
  CAESAR_TYPE_LITERAL LITERAL;

  /* Successor literal */
  CAESAR_TYPE_LITERAL_LIST NEXT;
} CAESAR_BODY_LITERAL_LIST;

/*----------------------------------------------------------------------------*/
/* Rule type */

typedef struct CAESAR_STRUCT_RULE {
  /* Head of rule */
  CAESAR_TYPE_ATOM HEAD;

  /* Tail of rule */
  CAESAR_TYPE_LITERAL_LIST TAIL;
} CAESAR_BODY_RULE, *CAESAR_TYPE_RULE;

/*----------------------------------------------------------------------------*/
/* Rule list type */

typedef struct CAESAR_STRUCT_RULE_LIST *CAESAR_TYPE_RULE_LIST;

typedef struct CAESAR_STRUCT_RULE_LIST {
  /* Rule */
  CAESAR_TYPE_RULE RULE;

  /* Successor rule */
  CAESAR_TYPE_RULE_LIST NEXT;
} CAESAR_BODY_RULE_LIST;

/*============================================================================*/
/* Domain Management */
extern CAESAR_TYPE_DOMAIN CAESAR_CREATE_DOMAIN(CAESAR_TYPE_STRING NAME,CAESAR_TYPE_NATURAL CARDINAL,CAESAR_TYPE_STRING FILENAME);
extern CAESAR_TYPE_DOMAIN_LIST CAESAR_CONSTRUCT_DOMAIN_LIST(CAESAR_TYPE_DOMAIN DOMAIN,CAESAR_TYPE_DOMAIN_LIST NEXT);
extern CAESAR_TYPE_DOMAIN_LIST CAESAR_REVERSE_DOMAIN_LIST(CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST);
extern CAESAR_TYPE_DOMAIN CAESAR_GET_DOMAIN_BY_VARIABLE_DOMAIN_NAME(CAESAR_TYPE_STRING CAESAR_VARIABLE_DOMAIN_NAME,CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST);
extern CAESAR_TYPE_NATURAL CAESAR_DOMAIN_LIST_LENGTH(CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST);
extern CAESAR_TYPE_DOMAIN_LIST CAESAR_PRINT_DOMAIN_LIST(CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST);

/*============================================================================*/
/* Relation Management */
extern CAESAR_TYPE_RELATION CAESAR_CREATE_RELATION(CAESAR_TYPE_STRING NAME,CAESAR_TYPE_NATURAL ARITY,CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST,CAESAR_TYPE_IONATURE IONATURE);
extern CAESAR_TYPE_RELATION_LIST CAESAR_CONSTRUCT_RELATION_LIST(CAESAR_TYPE_RELATION RELATION,CAESAR_TYPE_RELATION_LIST NEXT);
extern CAESAR_TYPE_RELATION_LIST CAESAR_REVERSE_RELATION_LIST(CAESAR_TYPE_RELATION_LIST RELATION_LIST);
extern CAESAR_TYPE_RELATION_LIST CAESAR_PRINT_RELATION_LIST(CAESAR_TYPE_RELATION_LIST RELATION_LIST);
extern CAESAR_TYPE_NATURAL CAESAR_LITERAL_LIST_LENGTH(CAESAR_TYPE_LITERAL_LIST LIST);
CAESAR_TYPE_RELATION CAESAR_GET_RELATION_BY_RELATION_NAME(CAESAR_TYPE_STRING RELATION_NAME, CAESAR_TYPE_RELATION_LIST LIST);

/*============================================================================*/
/* Rule Management */
extern CAESAR_TYPE_RULE CAESAR_CREATE_RULE(CAESAR_TYPE_ATOM HEAD, CAESAR_TYPE_LITERAL_LIST TAIL);
extern CAESAR_TYPE_RULE_LIST CAESAR_CONSTRUCT_RULE_LIST(CAESAR_TYPE_RULE RULE,CAESAR_TYPE_RULE_LIST NEXT);
extern CAESAR_TYPE_RULE_LIST CAESAR_REVERSE_RULE_LIST(CAESAR_TYPE_RULE_LIST RULE_LIST);
extern CAESAR_TYPE_RULE_LIST CAESAR_PRINT_RULE_LIST(CAESAR_TYPE_RULE_LIST RULE_LIST);

extern CAESAR_TYPE_ATOM CAESAR_CREATE_ATOM(CAESAR_TYPE_RELATION RELATION,CAESAR_TYPE_RULE_VARIABLE_LIST RULE_VARIABLE_LIST);

extern CAESAR_TYPE_LITERAL CAESAR_CREATE_LITERAL(CAESAR_TYPE_RELATION RELATION, CAESAR_TYPE_RULE_VARIABLE_LIST RULE_VARIABLE_LIST);
extern CAESAR_TYPE_LITERAL_LIST CAESAR_CONSTRUCT_LITERAL_LIST(CAESAR_TYPE_LITERAL LITERAL, CAESAR_TYPE_LITERAL_LIST NEXT);
extern CAESAR_TYPE_NATURAL CAESAR_LITERAL_LIST_LENGTH(CAESAR_TYPE_LITERAL_LIST LIST);

extern CAESAR_TYPE_NATURAL CAESAR_RULE_VARIABLE_LIST_LENGTH(CAESAR_TYPE_RULE_VARIABLE_LIST LIST);
extern CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_PRINT_RULE_VARIABLE_LIST_NAMES(CAESAR_TYPE_RULE_VARIABLE_LIST LIST);

/*==============================================================================*/
/* Variable Management */

extern CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_CONSTRUCT_RULE_VARIABLE_LIST(CAESAR_TYPE_STRING RULE_VARIABLE, CAESAR_TYPE_RULE_VARIABLE_LIST NEXT);
extern CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_REVERSE_RULE_VARIABLE_LIST(CAESAR_TYPE_RULE_VARIABLE_LIST LIST);

/* Global variables */

/* Domain list */
extern CAESAR_TYPE_DOMAIN_LIST CAESAR_DATALOG_DOMAIN_LIST;

/* Relation list */
extern CAESAR_TYPE_RELATION_LIST CAESAR_DATALOG_RELATION_LIST;

/* Rule list */
extern CAESAR_TYPE_RULE_LIST CAESAR_DATALOG_RULE_LIST;

/* Maximum number of relations on the right side of a rule's body */
extern CAESAR_TYPE_NATURAL CAESAR_DATALOG_MAX_RULE_BODY_SIZE;


#endif
