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

#ifndef CAESAR_DATALOG_RELATION_TABLE_INTERFACE
#define CAESAR_DATALOG_RELATION_TABLE_INTERFACE

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

typedef struct CAESAR_STRUCT_RELATION_TABLE_ENTRY {
     /* Relation name */
     CAESAR_TYPE_STRING NAME;

     /* Relation arity */
     CAESAR_TYPE_NATURAL ARITY;

     /* Domain indexes */
     CAESAR_TYPE_INDEX_TABLE_1 *DOMAINS;

     /* Combination table */
     CAESAR_TYPE_TABLE_1 COMBINATIONS;

     /* Fact table */
     CAESAR_TYPE_TABLE_1 FACTS; /* TODO */

     /* Rule table */
     CAESAR_TYPE_TABLE_1 RULES; /* TODO */

} CAESAR_BODY_RELATION_TABLE_ENTRY, *CAESAR_TYPE_RELATION_TABLE_ENTRY;

/*============================================================================*/
/* Exported functions */

extern void CAESAR_DATALOG_BUILD_RELATION_TABLE();
extern CAESAR_TYPE_INDEX_TABLE_1 CAESAR_DATALOG_GET_RELATION_INDEX_BY_NAME(CAESAR_TYPE_STRING RELATION_NAME);
extern void CAESAR_DATALOG_GET_RELATION_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_RELATION_TABLE_ENTRY ENTRY);
extern CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_GET_RELATION_BY_NAME(CAESAR_TYPE_STRING NAME, CAESAR_TYPE_RELATION_TABLE_ENTRY);
extern void CAESAR_DATALOG_PRINT_RELATION_TABLE();
extern CAESAR_TYPE_NATURAL CAESAR_DATALOG_GET_RELATION_COUNT();

#endif
