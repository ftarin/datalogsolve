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

#ifndef DATALOG_DOMAIN_TABLE_H_
#define DATALOG_DOMAIN_TABLE_H_

#include "datalog_solve.h"

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_DOMAIN_TABLE_ENTRY
{
	/* Name */
	CAESAR_TYPE_STRING NAME;
	/* Cardinal */
	CAESAR_TYPE_NATURAL CARDINAL;
	/* Indexes of the ELEMENTS table which are in at least one fact */
	CAESAR_TYPE_TABLE_1 FILTERED_ELEMENTS;
	/* Table with all the elements (strings) of the domain */
	CAESAR_TYPE_TABLE_1 ELEMENTS;
} CAESAR_BODY_DOMAIN_TABLE_ENTRY, *CAESAR_TYPE_DOMAIN_TABLE_ENTRY;

typedef struct CAESAR_STRUCT_DOMAIN_ELEMENTS_TABLE_BASE
{
	/* Element */
	CAESAR_TYPE_STRING ELEMENT;
} CAESAR_BODY_DOMAIN_ELEMENTS_TABLE_BASE,
		*CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE;

typedef struct CAESAR_STRUCT_DOMAIN_TABLE_MARK
{
	/* Domain cardinal */
	CAESAR_TYPE_NATURAL CARDINAL;

	/* Filtered domain elements table */
	CAESAR_TYPE_TABLE_1 FILTERED_ELEMENTS;

	/* Domain elements table */
	CAESAR_TYPE_TABLE_1 ELEMENTS;
} CAESAR_BODY_DOMAIN_TABLE_MARK, *CAESAR_TYPE_DOMAIN_TABLE_MARK;

/*============================================================================*/
/* Exported functions */

CAESAR_TYPE_NATURAL DOMAIN_TABLE_ENTRY_COUNT();
void ELEMENT_TABLE_PRINT(CAESAR_TYPE_FILE FILE, CAESAR_TYPE_TABLE_1 TABLE);
CAESAR_TYPE_NATURAL ELEMENT_TABLE_BUILD(
		CAESAR_TYPE_TABLE_1 *DOMAIN_ELEMENTS_TABLE_POINTER,
		CAESAR_TYPE_STRING DOMAIN_FILENAME);
void DOMAIN_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX,
		CAESAR_TYPE_DOMAIN_TABLE_ENTRY ENTRY);
void DOMAIN_TABLE_GET_ELEMENT_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 DOMAIN_INDEX,
		CAESAR_TYPE_INDEX_TABLE_1 ELEMENT_INDEX, CAESAR_TYPE_STRING *STRING);
CAESAR_TYPE_BOOLEAN DOMAIN_TABLE_GET_INDEX_AND_VALUE_BY_NAME(
		CAESAR_TYPE_STRING NAME, CAESAR_TYPE_INDEX_TABLE_1 *INDEX,
		CAESAR_TYPE_POINTER *VALUE);
void ELEMENT_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_TABLE_1 TABLE,
		CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_STRING *STRING);
void DOMAIN_TABLE_BUILD();
void DOMAIN_TABLE_ADD_FILTERED_ELEMENT(CAESAR_TYPE_INDEX_TABLE_1 domain_index,
		CAESAR_TYPE_INDEX_TABLE_1 element_index);
void FILTERED_ELEMENT_TABLE_BUILD(
		CAESAR_TYPE_TABLE_1 *DOMAIN_ELEMENTS_TABLE_POINTER);
void DOMAIN_TABLE_PRINT();
CAESAR_TYPE_BOOLEAN DOMAIN_TABLE_GET_FILTERED_CARDINALITY_BY_NAME(
		CAESAR_TYPE_STRING name, CAESAR_TYPE_NATURAL *cardinality);
CAESAR_TYPE_INDEX_TABLE_1 DOMAIN_TABLE_GET_ELEMENT_BY_FILTERED_ELEMENT_INDEX(CAESAR_TYPE_INDEX_TABLE_1 domain_index, CAESAR_TYPE_INDEX_TABLE_1 filtered_element_index);

#endif /*DATALOG_DOMAIN_TABLE_H_*/
