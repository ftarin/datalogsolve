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

#ifndef DATALOG_PQUERY_TABLE_H_
#define DATALOG_PQUERY_TABLE_H_

#include "datalog_solve.h"

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_PQUERY_TABLE_ENTRY { 
/* TODO: THIS SHOULD BE IMPROVED IN FUTURE VERSIONS FOR ANY LENGTH ARRAYS */

     /* cpredicate index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDICES[1];
     
     /* Relation index */
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDICES[1];

     /* Query length */
     CAESAR_TYPE_NATURAL LENGTH;
} CAESAR_BODY_PQUERY_TABLE_ENTRY, *CAESAR_TYPE_PQUERY_TABLE_ENTRY;

/*============================================================================*/
/* Exported functions */

void PQUERY_SET_VALUE_AT(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX
			 );
void PQUERY_SET_VALUE_AT_2(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX
			   );
void PQUERY_GET_VALUE_AT(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 *RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 *COMBINATION_INDEX
			 );
void PQUERY_TABLE_BUILD();
void PQUERY_TABLE_NEW_ENTRY(
     CAESAR_TYPE_TABLE_1 QUERY_TABLE,
     CAESAR_TYPE_POINTER *QUERY_TABLE_ENTRY
			    );
CAESAR_TYPE_BOOLEAN PQUERY_TABLE_ADD_ENTRY(
     CAESAR_TYPE_TABLE_1 QUERY_TABLE,
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX
					   );
void PQUERY_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_PQUERY_TABLE_ENTRY PQUERY);
CAESAR_TYPE_NATURAL PQUERY_TABLE_GET_ENTRY_COUNT();
void PQUERY_TABLE_PRINT_ENTRY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX);
void PQUERY_TABLE_PRINT(
     CAESAR_TYPE_FILE FILE,
     CAESAR_TYPE_TABLE_1 TABLE
			);

#endif /*DATALOG_PQUERY_TABLE_H_*/
