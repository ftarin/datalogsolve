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

#ifndef CAESAR_DATALOG_SUBSTITUTION_INTERFACE
#define CAESAR_DATALOG_SUBSTITUTION_INTERFACE

#include "caesar_standard.h"
#include "datalog_basic_data_structures.h"
#include "caesar_table_1.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

/*------------------------------------------------------------------------------*/
/* */

typedef struct CAESAR_STRUCT_VARIABLE_POSITION {
     /* Literal number */
     CAESAR_TYPE_NATURAL LITERAL_INDEX;

     /* Argument number */
     CAESAR_TYPE_NATURAL ARGUMENT_INDEX;
} CAESAR_BODY_VARIABLE_POSITION, *CAESAR_TYPE_VARIABLE_POSITION;


typedef struct CAESAR_STRUCT_VARIABLE_POSITION_LIST *CAESAR_TYPE_VARIABLE_POSITION_LIST;

typedef struct CAESAR_STRUCT_VARIABLE_POSITION_LIST {
     /* Variable tail-position */
     CAESAR_TYPE_VARIABLE_POSITION POSITION;

     /* Next item*/
     CAESAR_TYPE_VARIABLE_POSITION_LIST NEXT;
} CAESAR_BODY_VARIABLE_POSITION_LIST;

/*------------------------------------------------------------------------------*/
/* Map for free and linked variables */

typedef struct CAESAR_STRUCT_VARIABLE_MAP {
     /* Variable */
     CAESAR_TYPE_INDEX_TABLE_1 VARIABLE_INDEX;

     /* CHECK: Size of the domain associated to the variable */
     CAESAR_TYPE_NATURAL CARDINAL;

     /* Position in the head where it is found the variable */
     CAESAR_TYPE_VARIABLE_POSITION HEAD_POSITION;

     /* Positions in the tail where it is found the variable */
     CAESAR_TYPE_VARIABLE_POSITION_LIST TAIL_POSITION_LIST;
} CAESAR_BODY_VARIABLE_MAP, *CAESAR_TYPE_VARIABLE_MAP;


typedef struct CAESAR_STRUCT_VARIABLE_MAP_LIST *CAESAR_TYPE_VARIABLE_MAP_LIST;

typedef struct CAESAR_STRUCT_VARIABLE_MAP_LIST {
     /* Head-variable map */
     CAESAR_TYPE_VARIABLE_MAP MAP;

     /* Next item */
     CAESAR_TYPE_VARIABLE_MAP_LIST NEXT;
} CAESAR_BODY_VARIABLE_MAP_LIST;

/*============================================================================*/
/* Exported functions */

/*------------------------------------------------------------------------------*/
/* Variable Position Management */

extern CAESAR_TYPE_VARIABLE_POSITION CAESAR_CREATE_VARIABLE_POSITION(
     CAESAR_TYPE_NATURAL LITERAL_INDEX,
     CAESAR_TYPE_NATURAL ARGUMENT_INDEX
     );
extern CAESAR_TYPE_VARIABLE_POSITION_LIST CAESAR_CONSTRUCT_VARIABLE_POSITION_LIST(
     CAESAR_TYPE_VARIABLE_POSITION POSITION,
     CAESAR_TYPE_VARIABLE_POSITION_LIST NEXT
     );
extern CAESAR_TYPE_VARIABLE_POSITION_LIST CAESAR_REVERSE_VARIABLE_POSITION_LIST(
     CAESAR_TYPE_VARIABLE_POSITION_LIST VARIABLE_POSITION_LIST
     );
extern CAESAR_TYPE_BOOLEAN CAESAR_RULE_VARIABLE_IN_LIST(CAESAR_TYPE_STRING VARIABLE, CAESAR_TYPE_RULE_VARIABLE_LIST VARIABLE_LIST);

/*------------------------------------------------------------------------------*/
/* Variable Map Management */

extern CAESAR_TYPE_VARIABLE_MAP CAESAR_CREATE_VARIABLE_MAP(
     CAESAR_TYPE_INDEX_TABLE_1 VARIABLE_INDEX,
     CAESAR_TYPE_NATURAL CARDINAL,
     CAESAR_TYPE_VARIABLE_POSITION HEAD_POSITION,
     CAESAR_TYPE_VARIABLE_POSITION_LIST TAIL_POSITION_LIST
     );
extern CAESAR_TYPE_VARIABLE_MAP_LIST CAESAR_CONSTRUCT_VARIABLE_MAP_LIST(
     CAESAR_TYPE_VARIABLE_MAP MAP,
     CAESAR_TYPE_VARIABLE_MAP_LIST NEXT
     );
extern CAESAR_TYPE_VARIABLE_MAP_LIST CAESAR_REVERSE_VARIABLE_MAP_LIST(
     CAESAR_TYPE_VARIABLE_MAP_LIST VARIABLE_MAP_LIST
     );
extern void CAESAR_VARIABLE_MAP_LIST_TO_ARRAY
(
     CAESAR_TYPE_VARIABLE_MAP_LIST VARIABLE_MAP_LIST,
     CAESAR_TYPE_NATURAL *LENGTH,
     CAESAR_TYPE_VARIABLE_MAP **ARRAY
     );
extern void CAESAR_PRINT_VARIABLE_MAP_LIST(
     CAESAR_TYPE_VARIABLE_MAP_LIST LIST,
     CAESAR_TYPE_STRING INFO
     );

#endif
