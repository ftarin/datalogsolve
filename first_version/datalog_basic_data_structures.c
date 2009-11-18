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

#include "datalog_basic_data_structures.h"
#include "string.h"

/*==============================================================================*/
/* Global variables */

/* Domain list */
CAESAR_TYPE_DOMAIN_LIST CAESAR_DATALOG_DOMAIN_LIST;

/* Relation list */
CAESAR_TYPE_RELATION_LIST CAESAR_DATALOG_RELATION_LIST;

/* Rule list */
CAESAR_TYPE_RULE_LIST CAESAR_DATALOG_RULE_LIST;

/* Maximum number of relations on the right side of a rule's body */
CAESAR_TYPE_NATURAL CAESAR_DATALOG_MAX_RULE_BODY_SIZE;

/*==============================================================================*/
/* Domain Management */

/* Creation of a domain */

CAESAR_TYPE_DOMAIN CAESAR_CREATE_DOMAIN
(
 CAESAR_TYPE_STRING NAME,
 CAESAR_TYPE_NATURAL CARDINAL,
 CAESAR_TYPE_STRING FILENAME
)
{
  CAESAR_TYPE_DOMAIN DOMAIN;

  CAESAR_CREATE(DOMAIN, sizeof(CAESAR_BODY_DOMAIN), CAESAR_TYPE_DOMAIN);
  if (DOMAIN == NULL)
    CAESAR_ERROR("Memory shortage for domain");

  DOMAIN->NAME = NAME;
  DOMAIN->CARDINAL = CARDINAL;
  DOMAIN->FILENAME = FILENAME;

  return DOMAIN;
}

/* Addition of a domain in front of a domain list */

CAESAR_TYPE_DOMAIN_LIST CAESAR_CONSTRUCT_DOMAIN_LIST
(
 CAESAR_TYPE_DOMAIN DOMAIN,
 CAESAR_TYPE_DOMAIN_LIST NEXT
 )
{
  CAESAR_TYPE_DOMAIN_LIST CAESAR_CELL;

  CAESAR_CREATE(CAESAR_CELL, sizeof(CAESAR_BODY_DOMAIN_LIST), CAESAR_TYPE_DOMAIN_LIST);
  if (CAESAR_CELL == NULL)
    CAESAR_ERROR("Memory shortage for domain list");

  CAESAR_CELL->DOMAIN = DOMAIN;
  CAESAR_CELL->NEXT = NEXT;

  return CAESAR_CELL;
}

/*----------------------------------------------------------------------------*/
/* Reversal of a domain list*/

CAESAR_TYPE_DOMAIN_LIST CAESAR_REVERSE_DOMAIN_LIST
(
 CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST
 )
{
  CAESAR_TYPE_DOMAIN_LIST CAESAR_LIST;
  CAESAR_TYPE_DOMAIN_LIST CAESAR_CELL;

  CAESAR_LIST = NULL;

  while (DOMAIN_LIST != NULL) {
    CAESAR_CELL = DOMAIN_LIST;
    DOMAIN_LIST = DOMAIN_LIST->NEXT;
    CAESAR_CELL->NEXT = CAESAR_LIST;
    CAESAR_LIST = CAESAR_CELL;
  }
  
  return CAESAR_LIST;
}

/*----------------------------------------------------------------------------*/
/* Checks the format of a variable domain name */

CAESAR_TYPE_BOOLEAN CAESAR_CHECK_VARIABLE_DOMAIN_NAME_FORMAT
(
 CAESAR_TYPE_STRING CAESAR_VARIABLE_DOMAIN_NAME
 )
{
  /* TODO: Add checking logic */
  return CAESAR_TRUE;
}

/*----------------------------------------------------------------------------*/
/* Get the domain associated with a variable domain name in a domain
   list */

CAESAR_TYPE_DOMAIN CAESAR_GET_DOMAIN_BY_VARIABLE_DOMAIN_NAME
(
 CAESAR_TYPE_STRING CAESAR_VARIABLE_DOMAIN_NAME,
 CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST
)
{
  CAESAR_TYPE_DOMAIN_LIST CAESAR_CELL;
  CAESAR_TYPE_DOMAIN DOMAIN;
  CAESAR_TYPE_STRING CAESAR_STRING_DOMAIN_NUMBER;
  /* TODO CAESAR_TYPE_STRING DOMAIN_NAME; */
  
  /* To ease the comparison of strings */
  CAESAR_TYPE_NATURAL DOMAIN_NAME_LENGTH;
  CAESAR_TYPE_NATURAL CAESAR_CELL_DOMAIN_NAME_LENGTH;



  /* We check where the alphabetical characters end */
  CAESAR_STRING_DOMAIN_NUMBER = strpbrk(CAESAR_VARIABLE_DOMAIN_NAME, "0123456789");
  
  /* TODO: We should do something with CAESAR_STRING_DOMAIN_NUMBER
     before overwriting its characters*/
  ;

  /* We reduce the CAESAR_VARIABLE_DOMAIN_NAME and calculate its
     length in order to make a correct comparison */
  if (CAESAR_STRING_DOMAIN_NUMBER != NULL)
       CAESAR_STRING_DOMAIN_NUMBER[0] = '\0';
  DOMAIN_NAME_LENGTH = strlen(CAESAR_VARIABLE_DOMAIN_NAME);
  
  /* If it's not overwritten it'll mean we found no match */
  DOMAIN = NULL;

  for (CAESAR_CELL = DOMAIN_LIST;
       CAESAR_CELL != NULL;
       CAESAR_CELL = CAESAR_CELL->NEXT) {
    CAESAR_CELL_DOMAIN_NAME_LENGTH = strlen(CAESAR_CELL->DOMAIN->NAME);
    if (CAESAR_CELL_DOMAIN_NAME_LENGTH == DOMAIN_NAME_LENGTH)
      if (strcmp(CAESAR_VARIABLE_DOMAIN_NAME,
		 CAESAR_CELL->DOMAIN->NAME) == 0) {
	DOMAIN = CAESAR_CELL->DOMAIN;
	break;
      }
  }

  return DOMAIN;
}

/*----------------------------------------------------------------------------*/
/* Get the length of the domain list given as a parameter list */

CAESAR_TYPE_NATURAL CAESAR_DOMAIN_LIST_LENGTH(CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST)
{
  CAESAR_TYPE_NATURAL LENGTH;
  CAESAR_TYPE_DOMAIN_LIST CELL;

  LENGTH = 0;
  CELL = DOMAIN_LIST;
  while (CELL != NULL) {
    LENGTH++; 
    CELL = CELL->NEXT;
  }

  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* Print domain list */

CAESAR_TYPE_DOMAIN_LIST CAESAR_PRINT_DOMAIN_LIST
(
 CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST
)
{
  int i;
  CAESAR_TYPE_DOMAIN_LIST CAESAR_CELL;

  printf("\n==================================\n");
  printf(" Printing Domain List @%d\n", (int) DOMAIN_LIST);
  printf("==================================\n");

  CAESAR_CELL = DOMAIN_LIST;

  for (i = 0; CAESAR_CELL != NULL; i++) {
    printf("Domain %d @%d:\n", i, (int) CAESAR_CELL);
    printf("\t>Name: %s\n", CAESAR_CELL->DOMAIN->NAME);
    printf("\t>Cardinal: %d\n", CAESAR_CELL->DOMAIN->CARDINAL);
    printf("\t>Filename: %s\n", CAESAR_CELL->DOMAIN->FILENAME);
    
    CAESAR_CELL = CAESAR_CELL->NEXT;
  }

  printf("__________________________________\n");

  return DOMAIN_LIST;
}

/*----------------------------------------------------------------------------*/
/* Print domain list names */

CAESAR_TYPE_DOMAIN_LIST CAESAR_PRINT_DOMAIN_LIST_NAMES
(
 CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST
)
{
  int i;
  CAESAR_TYPE_DOMAIN_LIST CAESAR_CELL;

  printf("(");

  CAESAR_CELL = DOMAIN_LIST;

  for (i = 0; CAESAR_CELL != NULL; i++) {
    printf(" %s", CAESAR_CELL->DOMAIN->NAME);
    
    CAESAR_CELL = CAESAR_CELL->NEXT;
  }

  printf(" )");

  return DOMAIN_LIST;
}


/*==============================================================================*/
/* Relation Management */

/* Creation of a relation */

CAESAR_TYPE_RELATION CAESAR_CREATE_RELATION
(
 CAESAR_TYPE_STRING NAME,
 CAESAR_TYPE_NATURAL ARITY,
 CAESAR_TYPE_DOMAIN_LIST DOMAIN_LIST,
 CAESAR_TYPE_IONATURE IONATURE
)
{
  CAESAR_TYPE_RELATION RELATION;

  CAESAR_CREATE(RELATION, sizeof(CAESAR_BODY_RELATION), CAESAR_TYPE_RELATION);
  if (RELATION == NULL)
    CAESAR_ERROR("Memory shortage for relation");

  RELATION->NAME = NAME;
  RELATION->ARITY = ARITY;
  RELATION->DOMAIN_LIST = DOMAIN_LIST;
  RELATION->IONATURE = IONATURE;

  return RELATION;
}

/* Addition of a relation in front of a relation list */

CAESAR_TYPE_RELATION_LIST CAESAR_CONSTRUCT_RELATION_LIST
(
 CAESAR_TYPE_RELATION RELATION,
 CAESAR_TYPE_RELATION_LIST NEXT
 )
{
  CAESAR_TYPE_RELATION_LIST CELL;

  CAESAR_CREATE(CELL, sizeof(CAESAR_BODY_RELATION_LIST), CAESAR_TYPE_RELATION_LIST);
  if (CELL == NULL)
    CAESAR_ERROR("Memory shortage for relation list");

  CELL->RELATION = RELATION;
  CELL->NEXT = NEXT;

  return CELL;
}

/*----------------------------------------------------------------------------*/
/* Reversal of a relation list*/

CAESAR_TYPE_RELATION_LIST CAESAR_REVERSE_RELATION_LIST
(
 CAESAR_TYPE_RELATION_LIST RELATION_LIST
 )
{
  CAESAR_TYPE_RELATION_LIST NEW_LIST;
  CAESAR_TYPE_RELATION_LIST CELL;

  NEW_LIST = NULL;

  while (RELATION_LIST != NULL) {
    CELL = RELATION_LIST;
    RELATION_LIST = RELATION_LIST->NEXT;
    CELL->NEXT = NEW_LIST;
    NEW_LIST = CELL;
  }
  
  return NEW_LIST;
}

/*----------------------------------------------------------------------------*/
/* Check existence of a relation inside a relation list*/

CAESAR_TYPE_RELATION CAESAR_GET_RELATION_BY_RELATION_NAME
(
 CAESAR_TYPE_STRING RELATION_NAME,
 CAESAR_TYPE_RELATION_LIST LIST
 )
{
  CAESAR_TYPE_RELATION RELATION;
  CAESAR_TYPE_NATURAL RELATION_NAME_LENGTH;
  
  RELATION_NAME_LENGTH = strlen(RELATION_NAME);

  RELATION = NULL;

  while (LIST != NULL) {
    if (RELATION_NAME_LENGTH == strlen(LIST->RELATION->NAME))
      if (strcmp(LIST->RELATION->NAME, RELATION_NAME) == 0) {
	RELATION = LIST->RELATION;
	break;
      }
    LIST = LIST->NEXT;
  }
  
  return RELATION;
}

/*----------------------------------------------------------------------------*/
/* Print relation list */

CAESAR_TYPE_RELATION_LIST CAESAR_PRINT_RELATION_LIST
(
 CAESAR_TYPE_RELATION_LIST RELATION_LIST
)
{
  int i;
  CAESAR_TYPE_RELATION_LIST CELL;

  printf("\n==================================\n");
  printf(" Printing Relation List @%d\n", (int) RELATION_LIST);
  printf("==================================\n");

  CELL = RELATION_LIST;

  for (i = 0; CELL != NULL; i++) {
    printf("Relation %d @%d:\n", i, (int) CELL);
    printf("\t>Name: %s\n", CELL->RELATION->NAME);
    printf("\t>Arity: %d\n", CELL->RELATION->ARITY);
    printf("\t>Domains: ");
    CAESAR_PRINT_DOMAIN_LIST_NAMES(CELL->RELATION->DOMAIN_LIST);
    printf("\n");
    printf("\t>IONature: %d\n", CELL->RELATION->IONATURE);
    
    CELL = CELL->NEXT;
  }

  printf("__________________________________\n");

  return RELATION_LIST;
}

/*==============================================================================*/
/* Rule Management */

/* Creation of a rule-variable list */

CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_CONSTRUCT_RULE_VARIABLE_LIST
(
 CAESAR_TYPE_STRING RULE_VARIABLE,
 CAESAR_TYPE_RULE_VARIABLE_LIST NEXT
 )
{
  CAESAR_TYPE_RULE_VARIABLE_LIST CELL;

  CAESAR_CREATE(CELL, sizeof(CAESAR_BODY_RULE_VARIABLE_LIST), CAESAR_TYPE_RULE_VARIABLE_LIST);
  if (CELL == NULL)
    CAESAR_ERROR("Memory shortage for rule variable list");

  CELL->RULE_VARIABLE = RULE_VARIABLE;
  CELL->NEXT = NEXT;

  return CELL;  
}

/*----------------------------------------------------------------------------*/
/* Reversal of a rule-variable list*/

CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_REVERSE_RULE_VARIABLE_LIST
(
 CAESAR_TYPE_RULE_VARIABLE_LIST LIST
 )
{
  CAESAR_TYPE_RULE_VARIABLE_LIST NEW_LIST;
  CAESAR_TYPE_RULE_VARIABLE_LIST CELL;

  NEW_LIST = NULL;

  while (LIST != NULL) {
    CELL = LIST;
    LIST = LIST->NEXT;
    CELL->NEXT = NEW_LIST;
    NEW_LIST = CELL;
  }
  
  return NEW_LIST;
}

/*----------------------------------------------------------------------------*/
/* Print rule-variable list names */

CAESAR_TYPE_RULE_VARIABLE_LIST CAESAR_PRINT_RULE_VARIABLE_LIST_NAMES
(
 CAESAR_TYPE_RULE_VARIABLE_LIST LIST
)
{
  CAESAR_TYPE_RULE_VARIABLE_LIST CELL;

  printf("(");

  CELL = LIST;
    if (CELL != NULL)
	 printf("%s", CELL->RULE_VARIABLE);
  for (CELL = CELL->NEXT; CELL != NULL; CELL = CELL->NEXT) {
    printf(",%s", CELL->RULE_VARIABLE);
  }

  printf(")");

  return LIST;
}

/*----------------------------------------------------------------------------*/
/* Creation of an atom */

CAESAR_TYPE_ATOM CAESAR_CREATE_ATOM
(
 CAESAR_TYPE_RELATION RELATION,
 CAESAR_TYPE_RULE_VARIABLE_LIST RULE_VARIABLE_LIST
)
{
  CAESAR_TYPE_ATOM ATOM;

  CAESAR_CREATE(ATOM, sizeof(CAESAR_BODY_ATOM), CAESAR_TYPE_ATOM);
  if (ATOM == NULL)
    CAESAR_ERROR("Memory shortage for atom");

  ATOM->RELATION = RELATION;
  ATOM->RULE_VARIABLE_LIST = RULE_VARIABLE_LIST;

  return ATOM;
}

/*----------------------------------------------------------------------------*/
/* Creation of a literal */

CAESAR_TYPE_LITERAL CAESAR_CREATE_LITERAL
(
 CAESAR_TYPE_RELATION RELATION,
 CAESAR_TYPE_RULE_VARIABLE_LIST RULE_VARIABLE_LIST
)
{
  CAESAR_TYPE_LITERAL LITERAL;

  CAESAR_CREATE(LITERAL, sizeof(CAESAR_BODY_LITERAL), CAESAR_TYPE_LITERAL);
  if (LITERAL == NULL)
    CAESAR_ERROR("Memory shortage for literal");

  LITERAL->RELATION = RELATION;
  LITERAL->RULE_VARIABLE_LIST = RULE_VARIABLE_LIST;

  return LITERAL;
}

/*----------------------------------------------------------------------------*/
/* Creation of a literal list */

CAESAR_TYPE_LITERAL_LIST CAESAR_CONSTRUCT_LITERAL_LIST
(
 CAESAR_TYPE_LITERAL LITERAL,
 CAESAR_TYPE_LITERAL_LIST NEXT
 )
{
  CAESAR_TYPE_LITERAL_LIST CELL;

  CAESAR_CREATE(CELL, sizeof(CAESAR_BODY_LITERAL_LIST), CAESAR_TYPE_LITERAL_LIST);
  if (CELL == NULL)
    CAESAR_ERROR("Memory shortage for literal list");

  CELL->LITERAL = LITERAL;
  CELL->NEXT = NEXT;

  return CELL;  
}

/*----------------------------------------------------------------------------*/
/* Reversal of a literal list*/

CAESAR_TYPE_LITERAL_LIST CAESAR_REVERSE_LITERAL_LIST
(
 CAESAR_TYPE_LITERAL_LIST LIST
 )
{
  CAESAR_TYPE_LITERAL_LIST NEW_LIST;
  CAESAR_TYPE_LITERAL_LIST CELL;

  NEW_LIST = NULL;

  while (LIST != NULL) {
    CELL = LIST;
    LIST = LIST->NEXT;
    CELL->NEXT = NEW_LIST;
    NEW_LIST = CELL;
  }
  
  return NEW_LIST;
}

/*----------------------------------------------------------------------------*/
/* Creation of a rule */

CAESAR_TYPE_RULE CAESAR_CREATE_RULE
(
 CAESAR_TYPE_ATOM HEAD,
 CAESAR_TYPE_LITERAL_LIST TAIL
)
{
  CAESAR_TYPE_RULE RULE;

  CAESAR_CREATE(RULE, sizeof(CAESAR_BODY_RULE), CAESAR_TYPE_RULE);
  if (RULE == NULL)
    CAESAR_ERROR("Memory shortage for literal");

  RULE->HEAD = HEAD;
  RULE->TAIL = TAIL;

  return RULE;
}

/*----------------------------------------------------------------------------*/
/* Creation of a rule list */

CAESAR_TYPE_RULE_LIST CAESAR_CONSTRUCT_RULE_LIST
(
 CAESAR_TYPE_RULE RULE,
 CAESAR_TYPE_RULE_LIST NEXT
 )
{
  CAESAR_TYPE_RULE_LIST CELL;

  CAESAR_CREATE(CELL, sizeof(CAESAR_BODY_RULE_LIST), CAESAR_TYPE_RULE_LIST);
  if (CELL == NULL)
    CAESAR_ERROR("Memory shortage for rule list");

  CELL->RULE = RULE;
  CELL->NEXT = NEXT;

  return CELL;  
}

/*----------------------------------------------------------------------------*/
/* Reversal of a rule list*/

CAESAR_TYPE_RULE_LIST CAESAR_REVERSE_RULE_LIST
(
 CAESAR_TYPE_RULE_LIST LIST
 )
{
  CAESAR_TYPE_RULE_LIST NEW_LIST;
  CAESAR_TYPE_RULE_LIST CELL;

  NEW_LIST = NULL;

  while (LIST != NULL) {
    CELL = LIST;
    LIST = LIST->NEXT;
    CELL->NEXT = NEW_LIST;
    NEW_LIST = CELL;
  }
  
  return NEW_LIST;
}

/*----------------------------------------------------------------------------*/
/* Print rule list */

CAESAR_TYPE_RULE_LIST CAESAR_PRINT_RULE_LIST
(
 CAESAR_TYPE_RULE_LIST LIST
)
{
  int i;
  CAESAR_TYPE_RULE_LIST CELL;
  CAESAR_TYPE_RULE_VARIABLE_LIST RVL_CELL;
  CAESAR_TYPE_LITERAL_LIST LL_CELL;

  printf("\n==================================\n");
  printf(" Printing Relation List @%d\n", (int) LIST);
  printf("==================================\n");

  CELL = LIST;

  for (i = 0; CELL != NULL; i++) {
    printf("Rule %d @%d:\n", i, (int) CELL);

    /* Head */
    printf("\t>Head: %s (", CELL->RULE->HEAD->RELATION->NAME);
    for (RVL_CELL = CELL->RULE->HEAD->RULE_VARIABLE_LIST;
	 RVL_CELL != NULL;
	 RVL_CELL = RVL_CELL->NEXT)
      printf("%s,", RVL_CELL->RULE_VARIABLE);
    printf("\b)\n");

    /* Tail */
    printf("\t>Tail:\n");
    for (LL_CELL = CELL->RULE->TAIL;
	 LL_CELL != NULL;
	 LL_CELL = LL_CELL->NEXT
	 )
      {
	printf("\t\t>Literal: %s (", LL_CELL->LITERAL->RELATION->NAME);
	for (RVL_CELL = LL_CELL->LITERAL->RULE_VARIABLE_LIST;
	     RVL_CELL != NULL;
	     RVL_CELL = RVL_CELL->NEXT)
	  printf("%s,", RVL_CELL->RULE_VARIABLE);
	printf("\b)\n");
      }

    CELL = CELL->NEXT;
  }

  printf("__________________________________\n");

  return LIST;
}

/*----------------------------------------------------------------------------*/
/* Get the length of the rule-variable list given as a parameter list */

CAESAR_TYPE_NATURAL CAESAR_RULE_VARIABLE_LIST_LENGTH
(
 CAESAR_TYPE_RULE_VARIABLE_LIST LIST
 )
{
  CAESAR_TYPE_NATURAL LENGTH;
  CAESAR_TYPE_RULE_VARIABLE_LIST CELL;

  LENGTH = 0;
  CELL = LIST;
  while (CELL != NULL) {
    LENGTH++; 
    CELL = CELL->NEXT;
  }

  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* Get the length of the literal list given as a parameter list */

CAESAR_TYPE_NATURAL CAESAR_LITERAL_LIST_LENGTH
(
 CAESAR_TYPE_LITERAL_LIST LIST
 )
{
  CAESAR_TYPE_NATURAL LENGTH;
  CAESAR_TYPE_LITERAL_LIST CELL;

  LENGTH = 0;
  CELL = LIST;
  while (CELL != NULL) {
    LENGTH++; 
    CELL = CELL->NEXT;
  }

  return LENGTH;
}
