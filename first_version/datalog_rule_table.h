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

#ifndef CAESAR_DATALOG_RULE_TABLE_INTERFACE
#define CAESAR_DATALOG_RULE_TABLE_INTERFACE

#include "datalog_basic_data_structures.h"
#include "datalog_substitution.h"

#include "caesar_standard.h"
#include "caesar_table_1.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_RULE_TABLE_ENTRY {
     /* Rule address */
     CAESAR_TYPE_RULE RULE;

     /* Size of the rule tail */
     CAESAR_TYPE_NATURAL LENGTH;

     /* Relation head  index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;

     /* crule table */
     CAESAR_TYPE_TABLE_1 CRULE_TABLE;

     /* List of maps for free variables */
     CAESAR_TYPE_VARIABLE_MAP_LIST FREE_VARIABLE_MAP_LIST;

     CAESAR_TYPE_VARIABLE_MAP *FREE_VARIABLE_MAP_ARRAY;

     CAESAR_TYPE_NATURAL FREE_VARIABLE_NUMBER;

     /* List of maps for linked variables */
     CAESAR_TYPE_VARIABLE_MAP_LIST LINKED_VARIABLE_MAP_LIST;

     CAESAR_TYPE_POINTER TAIL_TO_CONCRETIZE;

     CAESAR_TYPE_NATURAL *TAIL_TO_CONCRETIZE_DISPLAY;

     CAESAR_TYPE_INDEX_TABLE_1 *TAIL_TO_CONCRETIZE_RELATION_INDICES;
} CAESAR_BODY_RULE_TABLE_ENTRY, *CAESAR_TYPE_RULE_TABLE_ENTRY;

typedef struct CAESAR_STRUCT_RULE_I_TABLE_ENTRY {
     /* Global rule index*/
     CAESAR_TYPE_INDEX_TABLE_1 RULE_INDEX;

} CAESAR_BODY_RULE_I_TABLE_ENTRY, *CAESAR_TYPE_RULE_I_TABLE_ENTRY;

/*============================================================================*/
/* Exported functions */

void CAESAR_DATALOG_BUILD_RULE_TABLE();
CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_RULE_TABLE_ENTRY(CAESAR_TYPE_RULE_TABLE_ENTRY, CAESAR_TYPE_INDEX_TABLE_1 *);
void CAESAR_DATALOG_GET_RULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1, CAESAR_TYPE_RULE_TABLE_ENTRY);
void CAESAR_DATALOG_PRINT_RULE(CAESAR_TYPE_INDEX_TABLE_1 INDEX);

void CAESAR_DATALOG_BUILD_RULE_I_TABLE(CAESAR_TYPE_TABLE_1 *);
CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_RULE_I_TABLE_ENTRY(CAESAR_TYPE_TABLE_1, CAESAR_TYPE_RULE_I_TABLE_ENTRY, CAESAR_TYPE_INDEX_TABLE_1 *);
void CAESAR_DATALOG_GET_RULE_I_BY_INDEX(CAESAR_TYPE_TABLE_1 RULE_I_TABLE, CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_RULE_I_TABLE_ENTRY ENTRY);
CAESAR_TYPE_NATURAL CAESAR_DATALOG_COUNT_RULE_I(CAESAR_TYPE_TABLE_1);

#endif
