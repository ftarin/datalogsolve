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

#ifndef CAESAR_DATALOG_PRULE_TABLE_INTERFACE
#define CAESAR_DATALOG_PRULE_TABLE_INTERFACE

#include "caesar_standard.h"
#include "caesar_table_1.h"

#include "datalog_cpredicate_table.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_PRULE_TABLE_ENTRY {
     /* cpredicate index */
     CAESAR_TYPE_INDEX_TABLE_1 CPREDICATE_INDEX;
     
     /* Relation index */
     CAESAR_TYPE_INDEX_TABLE_1 RULE_INDEX;

} CAESAR_BODY_PRULE_TABLE_ENTRY, *CAESAR_TYPE_PRULE_TABLE_ENTRY;

/*============================================================================*/
/* Exported functions */

void CAESAR_DATALOG_BUILD_PRULE_TABLE();
CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_PRULE_TABLE_ENTRY(CAESAR_TYPE_PRULE_TABLE_ENTRY , CAESAR_TYPE_INDEX_TABLE_1 *INDEX);
void CAESAR_DATALOG_GET_PRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_PRULE_TABLE_ENTRY *ENTRY_POINTER);
/* void CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_STRING *ENTRY_POINTER); */
void CAESAR_DATALOG_PRINT_PRULE_TABLE();
void CAESAR_DATALOG_PRINT_PRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX);

#endif
