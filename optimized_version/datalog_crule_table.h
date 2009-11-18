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

#ifndef CAESAR_DATALOG_CRULE_TABLE_INTERFACE
#define CAESAR_DATALOG_CRULE_TABLE_INTERFACE

#include "datalog_solve.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_CRULE_TABLE_ENTRY {
     /* rule index */
     CAESAR_TYPE_INDEX_TABLE_1 RULE_INDEX;

     /* cpredicate indices */
     CAESAR_TYPE_INDEX_TABLE_1 *CPREDICATE_INDICES;

     /* length of cpredicate indices */
     /*CAESAR_TYPE_NATURAL LENGTH;*/
     
     /* TODO: This length field is superfluous but i'm gonna use it as long as i don't break the relation-table 
      functions in another file so i can cleanly include the code for looking for a rule length from the 
      implementation file */
} CAESAR_BODY_CRULE_TABLE_ENTRY, *CAESAR_TYPE_CRULE_TABLE_ENTRY;

/*============================================================================*/
/* Exported functions */

void CAESAR_DATALOG_BUILD_CRULE_TABLE();
void CAESAR_DATALOG_BUILD_CRULE_I_TABLE(CAESAR_TYPE_TABLE_1 *, CAESAR_TYPE_NATURAL);
CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_CRULE_TABLE_ENTRY(CAESAR_TYPE_CRULE_TABLE_ENTRY , CAESAR_TYPE_INDEX_TABLE_1 *INDEX);
void CAESAR_DATALOG_GET_CRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CRULE_TABLE_ENTRY ENTRY_POINTER);
void CAESAR_DATALOG_PRINT_CRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX);

#endif
