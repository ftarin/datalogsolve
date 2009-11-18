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
#include "datalog_common.h"
#include "string.h"

#define GENERATED_VARIABLE_MAX_SIZE 12

/*==============================================================================*/
/* Global variables */

/* Variable table */
static CAESAR_TYPE_TABLE_1 VARIABLE_TABLE;

/*------------------------------------------------------------------------------*/
/* Get index by variable */

CAESAR_TYPE_INDEX_TABLE_1 VARIABLE_TABLE_GET_INDEX_BY_NAME(
		CAESAR_TYPE_STRING VARIABLE)
{
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;
	CAESAR_TYPE_POINTER ENTRY;
	CAESAR_TYPE_BOOLEAN SUCCESS;

	SUCCESS = CAESAR_SEARCH_TABLE_1(VARIABLE_TABLE, (CAESAR_TYPE_POINTER)
			&VARIABLE, &INDEX, &ENTRY);

	/* TODO: Conditional compiling of the assert */
	CAESAR_ASSERT(SUCCESS);

	return INDEX;
}

CAESAR_TYPE_INDEX_TABLE_1 VARIABLE_TABLE_GENERATE_NEW_VARIABLE(
		CAESAR_TYPE_STRING *NAME)
{
	char NEW_NAME[GENERATED_VARIABLE_MAX_SIZE];
	CAESAR_TYPE_STRING NEW_NAME_COPIED;
	CAESAR_TYPE_POINTER NEW_ENTRY;
	CAESAR_TYPE_INDEX_TABLE_1 VARIABLE_INDEX;
	CAESAR_TYPE_NATURAL i;

	VARIABLE_INDEX = CAESAR_GET_INDEX_TABLE_1(VARIABLE_TABLE);

	sprintf(NEW_NAME, "V%d", (CAESAR_TYPE_NATURAL) VARIABLE_INDEX);
	NEW_NAME_COPIED = CAESAR_COPY_STRING(NEW_NAME);

	NEW_ENTRY = CAESAR_PUT_BASE_TABLE_1(VARIABLE_TABLE);

	*((CAESAR_TYPE_STRING *)NEW_ENTRY) = NEW_NAME_COPIED;

	CAESAR_SEARCH_AND_PUT_TABLE_1(VARIABLE_TABLE, &VARIABLE_INDEX, &NEW_ENTRY);

	if (NAME != NULL)
	{
		*NAME = *((CAESAR_TYPE_STRING *) NEW_ENTRY);
	}

	return VARIABLE_INDEX;
}

/*------------------------------------------------------------------------------*/
/* Build the variable table */

void CAESAR_DATALOG_BUILD_VARIABLE_TABLE()
{
	CAESAR_TYPE_TABLE_1 AUX_VARIABLE_TABLE;
	CAESAR_TYPE_POINTER BASE;
	CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;
	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;

	BASE_AREA = CAESAR_STRING_AREA_1();
	MARK_AREA = CAESAR_EMPTY_AREA_1();

	CAESAR_CREATE_TABLE_1( &AUX_VARIABLE_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	NULL, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	);

	CAESAR_TYPE_RULE_LIST RULE_CELL;
	CAESAR_TYPE_LITERAL_LIST LITERAL_CELL;
	CAESAR_TYPE_RULE_VARIABLE_LIST VARIABLE_CELL;

	CAESAR_TYPE_RULE RULE;
	CAESAR_TYPE_ATOM ATOM;
	CAESAR_TYPE_LITERAL LITERAL;
	CAESAR_TYPE_STRING VARIABLE;

	/* TODO: These loops must disappear adding information to the
	 table as needed... */

	for (RULE_CELL = CAESAR_DATALOG_RULE_LIST; RULE_CELL != NULL; RULE_CELL
			= RULE_CELL->NEXT)
	{

		RULE = RULE_CELL->RULE;

		ATOM = RULE->HEAD;

		for (VARIABLE_CELL = ATOM->RULE_VARIABLE_LIST; VARIABLE_CELL != NULL; VARIABLE_CELL
				= VARIABLE_CELL->NEXT)
		{

			VARIABLE = VARIABLE_CELL->RULE_VARIABLE;

			BASE = CAESAR_PUT_BASE_TABLE_1(AUX_VARIABLE_TABLE);

			*((CAESAR_TYPE_STRING *)BASE) = VARIABLE;

			CAESAR_SEARCH_AND_PUT_TABLE_1(AUX_VARIABLE_TABLE, &INDEX,
					&EXISTING_ENTRY_POINTER);
		}

		for (LITERAL_CELL = RULE->TAIL; LITERAL_CELL != NULL; LITERAL_CELL
				= LITERAL_CELL->NEXT)
		{

			LITERAL = LITERAL_CELL->LITERAL;

			for (VARIABLE_CELL = LITERAL->RULE_VARIABLE_LIST; VARIABLE_CELL
					!= NULL; VARIABLE_CELL = VARIABLE_CELL->NEXT)
			{

				VARIABLE = VARIABLE_CELL->RULE_VARIABLE;

				BASE = CAESAR_PUT_BASE_TABLE_1(AUX_VARIABLE_TABLE);

				*((CAESAR_TYPE_STRING *)BASE) = VARIABLE;

				CAESAR_SEARCH_AND_PUT_TABLE_1(AUX_VARIABLE_TABLE, &INDEX,
						&EXISTING_ENTRY_POINTER);
			}
		}
	}

	VARIABLE_TABLE = AUX_VARIABLE_TABLE;

}

/*------------------------------------------------------------------------------*/
/* Get a variable entry by index */

void CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX,
		CAESAR_TYPE_STRING *ENTRY_POINTER)
{
	CAESAR_TYPE_POINTER BASE;

	CAESAR_RETRIEVE_I_B_TABLE_1(VARIABLE_TABLE, INDEX, &BASE);

	*ENTRY_POINTER = *((CAESAR_TYPE_STRING *) BASE);
}
