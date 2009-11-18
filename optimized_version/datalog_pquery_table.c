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

#include "datalog_pquery_table.h"
#include "datalog_combination_table.h"
#include "datalog_relation_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_variable_table.h"

static CAESAR_TYPE_TABLE_1 PQUERY_TABLE;

/*------------------------------------------------------------------------------*/
/* Creation of a query table entry */
void PQUERY_TABLE_NEW_ENTRY(CAESAR_TYPE_TABLE_1 QUERY_TABLE,
		CAESAR_TYPE_POINTER *QUERY_TABLE_ENTRY)
{
	*QUERY_TABLE_ENTRY = CAESAR_PUT_BASE_TABLE_1(QUERY_TABLE);
}

void PQUERY_SET_VALUE_AT(CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
		CAESAR_TYPE_NATURAL INDEX, CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
		CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX)
{
	CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;

	/* The index must be reasonable */
	CAESAR_ASSERT(INDEX < 1); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */

	/* We see the entry as an array of table indexes */
	INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;

	/* We update the values */
	INDEX_ARRAY[INDEX * 2] = RELATION_INDEX;
	INDEX_ARRAY[INDEX * 2 + 1] = COMBINATION_INDEX;
}

void PQUERY_SET_VALUE_AT_2(CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
		CAESAR_TYPE_NATURAL INDEX, CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
		CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX)
{
	CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;

	/* The index must be reasonable */
	CAESAR_ASSERT(INDEX < CAESAR_DATALOG_MAX_RULE_BODY_SIZE); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */

	/* We see the entry as an array of table indexes */
	INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;

	/* We update the values */
	INDEX_ARRAY[INDEX * 2] = RELATION_INDEX;
	INDEX_ARRAY[INDEX * 2 + 1] = COMBINATION_INDEX;
}

/*------------------------------------------------------------------------------*/
/* Read a query table entry */
void PQUERY_GET_VALUE_AT(CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
		CAESAR_TYPE_NATURAL INDEX, CAESAR_TYPE_INDEX_TABLE_1 *RELATION_INDEX,
		CAESAR_TYPE_INDEX_TABLE_1 *COMBINATION_INDEX)
{
	CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;

	/* The index must be reasonable */
	CAESAR_ASSERT(INDEX < 1); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */

	/* We see the entry as an array of table indexes */
	INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;

	/* We update the values */
	*RELATION_INDEX = INDEX_ARRAY[INDEX * 2];
	*COMBINATION_INDEX = INDEX_ARRAY[INDEX * 2 + 1];
}

/*------------------------------------------------------------------------------*/
/* Addition of an entry to a query table */
CAESAR_TYPE_BOOLEAN PQUERY_TABLE_ADD_ENTRY(CAESAR_TYPE_TABLE_1 QUERY_TABLE,
		CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
	CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;
	CAESAR_TYPE_BOOLEAN SUCCESS;

	SUCCESS = CAESAR_SEARCH_AND_PUT_TABLE_1(QUERY_TABLE, INDEX,
			&EXISTING_ENTRY_POINTER);

	return SUCCESS;
}

/*------------------------------------------------------------------------------*/
/* Get a pquery entry by index */

void PQUERY_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX,
		CAESAR_TYPE_PQUERY_TABLE_ENTRY PQUERY)
{
	CAESAR_TYPE_POINTER BASE;

	CAESAR_RETRIEVE_I_B_TABLE_1(PQUERY_TABLE, INDEX, &BASE);

	PQUERY_GET_VALUE_AT(BASE, 0, PQUERY->RELATION_INDICES,
			PQUERY->COMBINATION_INDICES);

	PQUERY->LENGTH = 1;
}

/*------------------------------------------------------------------------------*/
/* Get the number of pquery in the table */

CAESAR_TYPE_NATURAL PQUERY_TABLE_GET_ENTRY_COUNT()
{
	return CAESAR_PUT_INDEX_TABLE_1(PQUERY_TABLE);
}

/*------------------------------------------------------------------------------*/
/* Printing of a pquery entry */

void PQUERY_TABLE_PRINT_ENTRY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
	CAESAR_TYPE_NATURAL i;
	CAESAR_TYPE_STRING STRING;

	CAESAR_BODY_PQUERY_TABLE_ENTRY PQUERY_BODY;
	CAESAR_TYPE_PQUERY_TABLE_ENTRY PQUERY;
	PQUERY = &PQUERY_BODY;

	CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
	CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
	RELATION = &RELATION_BODY;

	CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
	CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
	COMBINATION = &COMBINATION_BODY;

	PQUERY_TABLE_GET_ENTRY_BY_INDEX(INDEX, PQUERY);
	CAESAR_DATALOG_GET_RELATION_BY_INDEX(PQUERY->RELATION_INDICES[0], RELATION);

	CAESAR_CREATE(COMBINATION->ARGUMENTS, RELATION->ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
	CAESAR_CREATE(COMBINATION->ARGUMENT_IS_VARIABLE, RELATION->ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);
	COMBINATION_TABLE_GET_ENTRY_BY_INDEX(RELATION->COMBINATIONS,
			PQUERY->COMBINATION_INDICES[0], RELATION->ARITY, COMBINATION);

	printf("X1_%s(", RELATION->NAME);

	/* TODO: Throw this prinf away */
	printf("[PQ %d]", INDEX);

	i = 0;
	if (i < RELATION->ARITY)
	{
		if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE)
		{
			CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i],
					&STRING);
			printf("%s", STRING);
		}
		else
		{
			/* TODO: CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);*/
			STRING="TODO";
			printf("%s", STRING);
		}
	}
	for (i = 1; i < RELATION->ARITY; i++)
		if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE)
		{
			CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i],
					&STRING);
			printf(",%s", STRING);
		}
		else
		{
			/* TODO: CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);*/
			STRING="TODO";
			printf(",%s", STRING);
		}

	printf(")");
	CAESAR_DELETE(COMBINATION->ARGUMENTS);
	CAESAR_DELETE(COMBINATION->ARGUMENT_IS_VARIABLE);
}

/*------------------------------------------------------------------------------*/
/* Printing of the query table */

void PQUERY_TABLE_PRINT(CAESAR_TYPE_FILE FILE, CAESAR_TYPE_TABLE_1 TABLE)
{
	CAESAR_TYPE_INDEX_TABLE_1 ENTRY_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 ENTRY_RELATION_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 ENTRY_COMBINATION_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM;
	CAESAR_TYPE_POINTER ENTRY;

	fprintf(FILE, "Query table:\n");

	/* We get the maximum index */
	MAXIMUM = CAESAR_PUT_INDEX_TABLE_1(TABLE);

	/* We iterate through all the indexes */
	for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM; TABLE_INDEX++)
	{
		fprintf(FILE, "\n[BASE %02d]", TABLE_INDEX);

		/* We get the address of each index's entry */
		CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, TABLE_INDEX, &ENTRY);

		/* TODO: BIG DEAL HERE: We need the size of the query */
		/* We iterate through each index's member */
		for (ENTRY_INDEX = 0; ENTRY_INDEX < 1; ENTRY_INDEX++)
		{

			PQUERY_GET_VALUE_AT(ENTRY, ENTRY_INDEX, &ENTRY_RELATION_INDEX,
					&ENTRY_COMBINATION_INDEX);

			fprintf(FILE, "\tRelation: %02d", ENTRY_RELATION_INDEX);
			fprintf(FILE, "\tCombination: %02d", ENTRY_COMBINATION_INDEX);

		}
	}

	fprintf(FILE, "\n\n");
}

/*------------------------------------------------------------------------------*/
/* Construction of the pquery table */

void PQUERY_TABLE_BUILD()
{
	CAESAR_TYPE_TABLE_1 AUX_QUERY_TABLE;
	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;

	/* TODO: CHECK THIS MARK AREA ¿IT'S USE */

	BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1) * 2);
	MARK_AREA = CAESAR_NATURAL_AREA_1(1);

	CAESAR_CREATE_TABLE_1( &AUX_QUERY_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	NULL, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION *//* TODO: PUT THE CAESAR_7_HASH */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1 /* OVERFLOW_FUNCTION */
	);

	/* TODO: In the future the queries will be explicit and these
	 * code should be delete */

	/* For iterating through the list */
	CAESAR_TYPE_RELATION_LIST RELATION_CELL;
	CAESAR_TYPE_RELATION RELATION;

	/* For searching on the tables */
	CAESAR_TYPE_TABLE_1 COMBINATION_TABLE;
	CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 QUERY_INDEX;
	CAESAR_TYPE_POINTER AUX_QUERY_TABLE_ENTRY;
	/*      //CAESAR_TYPE_POINTER RELATION_TABLE_ENTRY; */

	CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_TABLE_ENTRY_BODY;
	CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION_TABLE_ENTRY;
	RELATION_TABLE_ENTRY = &RELATION_TABLE_ENTRY_BODY;

	CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY;
	CAESAR_TYPE_NATURAL i;

	for (RELATION_CELL = CAESAR_DATALOG_RELATION_LIST; RELATION_CELL != NULL; RELATION_CELL
			= RELATION_CELL->NEXT)
	{

		RELATION = RELATION_CELL->RELATION;

		if (RELATION->IONATURE == CAESAR_OUTPUT_TUPLES)
		{

			/* We search the relation entry */

			CAESAR_BEGIN_DEBUG(4)
			{

				fprintf(stdout, "\n\nWe are looking a %s in:\n", RELATION->NAME);
				CAESAR_DATALOG_PRINT_RELATION_TABLE();

			}CAESAR_END_DEBUG;

			RELATION_INDEX
					= CAESAR_DATALOG_GET_RELATION_INDEX_BY_NAME(RELATION->NAME);

			CAESAR_DATALOG_GET_RELATION_BY_INDEX(RELATION_INDEX,
					RELATION_TABLE_ENTRY);

			COMBINATION_TABLE = RELATION_TABLE_ENTRY->COMBINATIONS;

			for (i = 0; i < RELATION->ARITY; i++)
			{
				/* We edit the new combination to be added */
				COMBINATION_TABLE_NEW_ENTRY(COMBINATION_TABLE,
						&COMBINATION_TABLE_ENTRY);
				
				/* We generate a random variable */
				COMBINATION_SET_VALUE_AT(COMBINATION_TABLE_ENTRY, i,
						RELATION->ARITY, VARIABLE_TABLE_GENERATE_NEW_VARIABLE(NULL), CAESAR_TRUE);
			}

			/* We store the new combination */
			COMBINATION_TABLE_ADD_ENTRY(COMBINATION_TABLE, &COMBINATION_INDEX);

			/* We create the query entry */
			PQUERY_TABLE_NEW_ENTRY(AUX_QUERY_TABLE, &AUX_QUERY_TABLE_ENTRY);

			/* We put the values in the entry*/
			PQUERY_SET_VALUE_AT(AUX_QUERY_TABLE_ENTRY, 0, RELATION_INDEX,
					COMBINATION_INDEX);

			/* We add the new entry to the table */
			PQUERY_TABLE_ADD_ENTRY(AUX_QUERY_TABLE, &QUERY_INDEX);
		}
	}

	PQUERY_TABLE = AUX_QUERY_TABLE;
}
