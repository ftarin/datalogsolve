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

#include "datalog_substitution.h"
#include "datalog_variable_table.h"
#include "datalog_basic_data_structures.h"

/*==============================================================================*/
/* Variable Position Management */

/*------------------------------------------------------------------------------*/
/* Create a variable position */

CAESAR_TYPE_VARIABLE_POSITION CAESAR_CREATE_VARIABLE_POSITION
(
     CAESAR_TYPE_NATURAL LITERAL_INDEX,
     CAESAR_TYPE_NATURAL ARGUMENT_INDEX
     )
{
     CAESAR_TYPE_VARIABLE_POSITION POSITION;
     
     CAESAR_CREATE(POSITION, sizeof(CAESAR_BODY_VARIABLE_POSITION), CAESAR_TYPE_VARIABLE_POSITION);
     if (POSITION == NULL)
	  CAESAR_ERROR("Memory shortage for VARIABLE_POSITION");
     
     POSITION->LITERAL_INDEX = LITERAL_INDEX;
     POSITION->ARGUMENT_INDEX = ARGUMENT_INDEX;
     
     return POSITION;
}

/*------------------------------------------------------------------------------*/
/* Add a variable position to a list of variable positions */

CAESAR_TYPE_VARIABLE_POSITION_LIST CAESAR_CONSTRUCT_VARIABLE_POSITION_LIST
(
     CAESAR_TYPE_VARIABLE_POSITION POSITION,
     CAESAR_TYPE_VARIABLE_POSITION_LIST NEXT
     )
{
     CAESAR_TYPE_VARIABLE_POSITION_LIST CELL;
     
     CAESAR_CREATE(CELL, sizeof(CAESAR_BODY_VARIABLE_POSITION_LIST), CAESAR_TYPE_VARIABLE_POSITION_LIST);
     if (CELL == NULL)
	  CAESAR_ERROR("Memory shortage for domain list");
     
     CELL->POSITION = POSITION;
     CELL->NEXT = NEXT;
     
     return CELL;
}

/*------------------------------------------------------------------------------*/
/* Invert a variable position list */

CAESAR_TYPE_VARIABLE_POSITION_LIST CAESAR_REVERSE_VARIABLE_POSITION_LIST
(
     CAESAR_TYPE_VARIABLE_POSITION_LIST VARIABLE_POSITION_LIST
     )
{
     CAESAR_TYPE_VARIABLE_POSITION_LIST LIST;
     CAESAR_TYPE_VARIABLE_POSITION_LIST CELL;
     
     LIST = NULL;
     
     while (VARIABLE_POSITION_LIST != NULL) {
	  CELL = VARIABLE_POSITION_LIST;
	  VARIABLE_POSITION_LIST = VARIABLE_POSITION_LIST->NEXT;
	  CELL->NEXT = LIST;
	  LIST = CELL;
     }
     
     return LIST;
}

/*------------------------------------------------------------------------------*/
/* */

CAESAR_TYPE_BOOLEAN CAESAR_RULE_VARIABLE_IN_LIST(CAESAR_TYPE_STRING VARIABLE, CAESAR_TYPE_RULE_VARIABLE_LIST VARIABLE_LIST)
{
     CAESAR_TYPE_RULE_VARIABLE_LIST CELL;

     for (CELL = VARIABLE_LIST;
	  CELL != NULL;
	  CELL = CELL->NEXT) {

	  if (strcmp(VARIABLE, CELL->RULE_VARIABLE) == 0) {
	       
	       return CAESAR_TRUE;

	  }

     }

     return CAESAR_FALSE;
}

/*==============================================================================*/
/* Variable Map Management */

/*------------------------------------------------------------------------------*/
/* Create a variable map */

CAESAR_TYPE_VARIABLE_MAP CAESAR_CREATE_VARIABLE_MAP
(
     CAESAR_TYPE_INDEX_TABLE_1 VARIABLE_INDEX,
     CAESAR_TYPE_NATURAL CARDINAL,
     CAESAR_TYPE_VARIABLE_POSITION HEAD_POSITION,
     CAESAR_TYPE_VARIABLE_POSITION_LIST TAIL_POSITION_LIST
     )
{
     CAESAR_TYPE_VARIABLE_MAP MAP;
     
     CAESAR_CREATE(MAP, sizeof(CAESAR_BODY_VARIABLE_MAP), CAESAR_TYPE_VARIABLE_MAP);
     if (MAP == NULL)
	  CAESAR_ERROR("Memory shortage for VARIABLE_MAP");
     
     MAP->VARIABLE_INDEX = VARIABLE_INDEX;
     MAP->CARDINAL = CARDINAL;
     MAP->HEAD_POSITION = HEAD_POSITION;
     MAP->TAIL_POSITION_LIST = TAIL_POSITION_LIST;
     
     return MAP;
}

/*------------------------------------------------------------------------------*/
/* Add a variable map to a list of variable maps */

CAESAR_TYPE_VARIABLE_MAP_LIST CAESAR_CONSTRUCT_VARIABLE_MAP_LIST
(
     CAESAR_TYPE_VARIABLE_MAP MAP,
     CAESAR_TYPE_VARIABLE_MAP_LIST NEXT
     )
{
     CAESAR_TYPE_VARIABLE_MAP_LIST CELL;
     
     CAESAR_CREATE(CELL, sizeof(CAESAR_BODY_VARIABLE_MAP_LIST), CAESAR_TYPE_VARIABLE_MAP_LIST);
     if (CELL == NULL)
	  CAESAR_ERROR("Memory shortage for VARIABLE_MAP_LIST");
     
     CELL->MAP = MAP;
     CELL->NEXT = NEXT;
     
     return CELL;
}

/*------------------------------------------------------------------------------*/
/* Invert a variable position list */

CAESAR_TYPE_VARIABLE_MAP_LIST CAESAR_REVERSE_VARIABLE_MAP_LIST
(
     CAESAR_TYPE_VARIABLE_MAP_LIST VARIABLE_MAP_LIST
     )
{
     CAESAR_TYPE_VARIABLE_MAP_LIST LIST;
     CAESAR_TYPE_VARIABLE_MAP_LIST CELL;
     
     LIST = NULL;
     
     while (VARIABLE_MAP_LIST != NULL) {
	  CELL = VARIABLE_MAP_LIST;
	  VARIABLE_MAP_LIST = VARIABLE_MAP_LIST->NEXT;
	  CELL->NEXT = LIST;
	  LIST = CELL;
     }
     
     return LIST;
}

/*------------------------------------------------------------------------------*/
/* Calculates the length of a variable position list */

void CAESAR_VARIABLE_MAP_LIST_TO_ARRAY
(
     CAESAR_TYPE_VARIABLE_MAP_LIST VARIABLE_MAP_LIST,
     CAESAR_TYPE_NATURAL *LENGTH,
     CAESAR_TYPE_VARIABLE_MAP **OUTPUT_ARRAY
     )
{
     CAESAR_TYPE_VARIABLE_MAP_LIST CELL;
     CAESAR_TYPE_NATURAL COUNTER;
     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_VARIABLE_MAP *ARRAY;

     for (CELL = VARIABLE_MAP_LIST, COUNTER = 0;
	  CELL != NULL;
	  CELL = CELL->NEXT, COUNTER++);

     CAESAR_CREATE(ARRAY, COUNTER * sizeof(CAESAR_TYPE_VARIABLE_MAP), CAESAR_TYPE_VARIABLE_MAP *);
     
     *LENGTH = COUNTER;

     for (CELL = VARIABLE_MAP_LIST, i = 0;
	  CELL != NULL;
	  CELL = CELL->NEXT, i++) {

	  CAESAR_CREATE(ARRAY[i], sizeof(CAESAR_BODY_VARIABLE_MAP), CAESAR_BODY_VARIABLE_MAP *);

	  
	  ARRAY[i]->VARIABLE_INDEX = CELL->MAP->VARIABLE_INDEX;
	  ARRAY[i]->CARDINAL = CELL->MAP->CARDINAL;
	  ARRAY[i]->HEAD_POSITION = CELL->MAP->HEAD_POSITION;
	  ARRAY[i]->TAIL_POSITION_LIST = CELL->MAP->TAIL_POSITION_LIST;
	  
     }

     *OUTPUT_ARRAY = ARRAY;
}

/*------------------------------------------------------------------------------*/
/* Print a variable position list */

void CAESAR_PRINT_VARIABLE_MAP_LIST
(
     CAESAR_TYPE_VARIABLE_MAP_LIST LIST,
     CAESAR_TYPE_STRING INFO
     )
{
     int i;
     CAESAR_TYPE_VARIABLE_MAP_LIST MCELL;
     CAESAR_TYPE_VARIABLE_POSITION_LIST PCELL; 
     CAESAR_TYPE_STRING VARIABLE;
     
     printf("[DEBUGGING] %s VARIABLE MAP LIST @%d\n", INFO == NULL ? "" : INFO, (int) LIST);
     
     if (LIST == NULL) {
	  printf("\tEMPTY\n");
	  return;
     }
     
     for (MCELL = LIST, i = 0; MCELL != NULL; MCELL = MCELL->NEXT, i++) {
	  
	  CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(MCELL->MAP->VARIABLE_INDEX, &VARIABLE);
	  printf("\t{%s", VARIABLE);
	  printf("\t{card:%d", MCELL->MAP->CARDINAL);
	  printf("\tH{%d}", MCELL->MAP->HEAD_POSITION->ARGUMENT_INDEX);
	  
	  printf("\tT{");
	  for (PCELL = MCELL->MAP->TAIL_POSITION_LIST; PCELL != NULL; PCELL = PCELL->NEXT) {
	       printf(" {l %d, a %d}", PCELL->POSITION->LITERAL_INDEX, PCELL->POSITION->ARGUMENT_INDEX);
	  }
	  printf(" }\n");
     }
}
