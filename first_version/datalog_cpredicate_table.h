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

#ifndef CAESAR_DATALOG_CPREDICATE_TABLE_INTERFACE
#define CAESAR_DATALOG_CPREDICATE_TABLE_INTERFACE

#include "caesar_standard.h"
#include "caesar_table_1.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_CPREDICATE_TABLE_ENTRY {
     /* Relation index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION;

     /* Combination index */
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION;
} CAESAR_BODY_CPREDICATE_TABLE_ENTRY, *CAESAR_TYPE_CPREDICATE_TABLE_ENTRY;

/*============================================================================*/
/* Exported functions */

void CAESAR_DATALOG_BUILD_CPREDICATE_TABLE();
CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_CPREDICATE_TABLE_ENTRY(CAESAR_TYPE_CPREDICATE_TABLE_ENTRY CPREDICATE, CAESAR_TYPE_INDEX_TABLE_1 *INDEX);
void CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX_2(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CPREDICATE_TABLE_ENTRY *ENTRY_POINTER);
void CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CPREDICATE_TABLE_ENTRY ENTRY);
void CAESAR_DATALOG_PRINT_CPREDICATE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX);

#endif
