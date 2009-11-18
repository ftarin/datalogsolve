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

#include <stdio.h>
#include "datalog_fact_table.h"
#include "datalog_common.h"
#include "datalog_combination_table.h"

/*------------------------------------------------------------------------------*/
/* Calculate the entry size of a fact table */

CAESAR_TYPE_NATURAL CAESAR_DATALOG_FACT_TABLE_ENTRY_SIZE(
		CAESAR_TYPE_NATURAL ARITY)
{
	return sizeof(CAESAR_TYPE_INDEX_TABLE_1) * ARITY;
}

/*------------------------------------------------------------------------------*/
/* Edit an entry's field of the fact table */

void CAESAR_DATALOG_EDIT_FACT_TABLE_ENTRY(CAESAR_TYPE_POINTER FACT_TABLE_ENTRY,
		CAESAR_TYPE_NATURAL INDEX, CAESAR_TYPE_NATURAL ARITY,
		CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX)
{
	/* The index must always be lower than the arity */
	CAESAR_ASSERT(INDEX < ARITY);

	((CAESAR_TYPE_INDEX_TABLE_1 *) FACT_TABLE_ENTRY)[INDEX]
			= TABLE_INDEX;
}

/*------------------------------------------------------------------------------*/
/* Get an entry's field of the fact table */

void CAESAR_DATALOG_GET_FACT_TABLE_ENTRY(CAESAR_TYPE_POINTER FACT_TABLE_ENTRY,
		CAESAR_TYPE_NATURAL INDEX, CAESAR_TYPE_NATURAL ARITY,
		CAESAR_TYPE_INDEX_TABLE_1 *TABLE_INDEX)
{
	/* The index must always be lower than the arity */
	CAESAR_ASSERT(INDEX < ARITY);

	*TABLE_INDEX = ((CAESAR_TYPE_INDEX_TABLE_1 *) FACT_TABLE_ENTRY)[INDEX];
}

/*------------------------------------------------------------------------------*/
/* Print a fact table */

void CAESAR_DATALOG_PRINT_FACT_TABLE(CAESAR_TYPE_FILE FILE,
		CAESAR_TYPE_TABLE_1 TABLE, CAESAR_TYPE_NATURAL ARITY)
{
	CAESAR_TYPE_INDEX_TABLE_1 ENTRY_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 ENTRY_MEMBER_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM;
	CAESAR_TYPE_POINTER ENTRY;

	fprintf(FILE, "Fact Table:\n");

	/* We get the maximum index */
	MAXIMUM = CAESAR_PUT_INDEX_TABLE_1(TABLE);

	/* We iterate through all the indexes */
	for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM; TABLE_INDEX++)
	{
		fprintf(FILE, "\n[BASE %02d]", TABLE_INDEX);

		/* We get the address of each index's entry */
		CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, TABLE_INDEX, &ENTRY);

		/* We iterate through each index's member */
		for (ENTRY_INDEX = 0; ENTRY_INDEX < ARITY; ENTRY_INDEX++)
		{

			CAESAR_DATALOG_GET_FACT_TABLE_ENTRY(ENTRY, ENTRY_INDEX, ARITY,
					&ENTRY_MEMBER_INDEX);

			fprintf(FILE, "\tElem. %d", ENTRY_MEMBER_INDEX);
		}
	}

	fprintf(FILE, "\n\n");
}

/*------------------------------------------------------------------------------*/
/* Add an entry to a fact table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_FACT_TABLE_ENTRY(
		CAESAR_TYPE_TABLE_1 FACT_TABLE, CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
	CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;

	return CAESAR_SEARCH_AND_PUT_TABLE_1(FACT_TABLE, INDEX,
			&EXISTING_ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Create an entry to add to a fact table in the future */

void CAESAR_DATALOG_CREATE_FACT_TABLE_ENTRY(CAESAR_TYPE_TABLE_1 FACT_TABLE,
		CAESAR_TYPE_POINTER *FACT_TABLE_ENTRY)
{
	*FACT_TABLE_ENTRY = CAESAR_PUT_BASE_TABLE_1(FACT_TABLE);
}

/*------------------------------------------------------------------------------*/
/* Construction of the fact table */

void CAESAR_DATALOG_BUILD_FACT_TABLE(CAESAR_TYPE_TABLE_1 *FACT_TABLE_POINTER,
		CAESAR_TYPE_NATURAL ARITY, CAESAR_TYPE_STRING RELATION_NAME)
{
	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;
	CAESAR_TYPE_TABLE_1 FACT_TABLE;

	BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1) * ARITY);
	MARK_AREA = CAESAR_EMPTY_AREA_1();

	/*************************/
	/* Create the fact table */
	/*************************/

	CAESAR_CREATE_TABLE_1( &FACT_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	NULL, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION *//* TODO: PUT THE CAESAR_7_HASH */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1 /* OVERFLOW_FUNCTION */
	);

	/***************************************************/
	/* Fill the fact table with the tuples information */
	/***************************************************/

	{

		CAESAR_TYPE_STRING FACT_FILENAME;
		CAESAR_TYPE_FILE FACT_FILE;

		/*========================================*/
		/* Build the filename of the input tuples */
		/*========================================*/

		CAESAR_CREATE(FACT_FILENAME, strlen(RELATION_NAME) + 11, CAESAR_TYPE_STRING);

		strcpy(FACT_FILENAME, RELATION_NAME);
		strcat(FACT_FILENAME, ".tuples");

		/*============================*/
		/* Open the input tuples file */
		/*============================*/

		if ((FACT_FILE = fopen(FACT_FILENAME, "r")) == NULL)
		{
			CAESAR_ERROR("File ``%s'' (input tuples) is unreadable ",
					FACT_FILENAME);
		}

		/*===================================================*/
		/* Read the input tuples file and store its contents */
		/*===================================================*/

		{ /* This bloc is for documenting purposes */

			CAESAR_TYPE_STRING STRING_BUFFER;
			CAESAR_TYPE_NATURAL STRING_BUFFER_SIZE;
			CAESAR_TYPE_STRING FIELD;
			CAESAR_TYPE_STRING FIELD_DELIMITERS;
			CAESAR_TYPE_NATURAL i;
			CAESAR_TYPE_POINTER FACT_ENTRY;
			CAESAR_TYPE_INDEX_TABLE_1 INDEX;

			STRING_BUFFER = NULL;
			STRING_BUFFER_SIZE = 0;
			FIELD = NULL;
			FIELD_DELIMITERS = "\t ";

			/*----------------------------------*/
			/* Read the head line and ignore it */
			/*----------------------------------*/

			getline(&STRING_BUFFER, &STRING_BUFFER_SIZE, FACT_FILE);

			/*---------------------------------------*/
			/* Iterate over each line and analize it */
			/*---------------------------------------*/

			while (getline(&STRING_BUFFER, &STRING_BUFFER_SIZE, FACT_FILE) > 0)
			{

				/*********************************************/
				/* Tokenize each line and store its contents */
				/*********************************************/

				CAESAR_DATALOG_CREATE_FACT_TABLE_ENTRY(FACT_TABLE, &FACT_ENTRY);

				FIELD = strtok(STRING_BUFFER, FIELD_DELIMITERS);
				INDEX = (CAESAR_TYPE_INDEX_TABLE_1) atol(FIELD);
				CAESAR_DATALOG_EDIT_FACT_TABLE_ENTRY(FACT_ENTRY, 0, ARITY,
						INDEX);

				for (i = 1; i < ARITY; i++)
				{
					FIELD = strtok(NULL, FIELD_DELIMITERS);
					INDEX = (CAESAR_TYPE_INDEX_TABLE_1) atol(FIELD);
					CAESAR_DATALOG_EDIT_FACT_TABLE_ENTRY(FACT_ENTRY, i, ARITY,
							INDEX);
				}

				CAESAR_DATALOG_ADD_FACT_TABLE_ENTRY(FACT_TABLE, &INDEX);

			}

		} /* This bloc is for documenting purposes */
	}

	CAESAR_BEGIN_DEBUG(4)
	{

		CAESAR_DATALOG_PRINT_FACT_TABLE(stdout, FACT_TABLE, ARITY);

	}CAESAR_END_DEBUG;

	/* Return the table */
	*FACT_TABLE_POINTER = FACT_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Search for the presence of a fact table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_IS_FACT(CAESAR_TYPE_TABLE_1 FACT_TABLE,
		CAESAR_TYPE_POINTER FACT_TABLE_ENTRY)
{
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;
	CAESAR_TYPE_POINTER ENTRY;

	return CAESAR_SEARCH_TABLE_1(FACT_TABLE,
			(CAESAR_TYPE_POINTER) FACT_TABLE_ENTRY, &INDEX, &ENTRY);
}

/*------------------------------------------------------------------------------*/
/* Get a fact entry by index */

void FACT_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_TABLE_1 FACT_TABLE,
		CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_NATURAL ARITY,
		CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION)
{
	CAESAR_TYPE_POINTER BASE;
	CAESAR_TYPE_NATURAL i;

	CAESAR_RETRIEVE_I_B_TABLE_1(FACT_TABLE, INDEX, &BASE);

	COMBINATION->LENGTH = ARITY;

	for (i = 0; i < ARITY; i++)
	{
		COMBINATION_GET_VALUE_AT(BASE, i, ARITY, &(COMBINATION->ARGUMENTS[i]),
				&(COMBINATION->ARGUMENT_IS_VARIABLE[i]));
	}
}

/*------------------------------------------------------------------------------*/
/* Get the number of pquery in the table */

CAESAR_TYPE_NATURAL FACT_TABLE_GET_ENTRY_COUNT(CAESAR_TYPE_TABLE_1 FACT_TABLE)
{
	return CAESAR_PUT_INDEX_TABLE_1(FACT_TABLE);
}
