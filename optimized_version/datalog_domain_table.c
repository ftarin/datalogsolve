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

#include "datalog_domain_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_common.h" 

/* The only domain table */
static CAESAR_TYPE_TABLE_1 DOMAIN_TABLE;

/*------------------------------------------------------------------------------*/
/* Count the number of entries in the domain table */

CAESAR_TYPE_NATURAL DOMAIN_TABLE_ENTRY_COUNT()
{
	return (CAESAR_TYPE_NATURAL) CAESAR_PUT_INDEX_TABLE_1(DOMAIN_TABLE);
}

CAESAR_TYPE_BOOLEAN DOMAIN_TABLE_GET_INDEX_AND_VALUE_BY_NAME(
		CAESAR_TYPE_STRING NAME, CAESAR_TYPE_INDEX_TABLE_1 *INDEX,
		CAESAR_TYPE_POINTER *VALUE)
{
	return CAESAR_SEARCH_TABLE_1(DOMAIN_TABLE, (CAESAR_TYPE_POINTER) &NAME,
			INDEX, VALUE);
}

CAESAR_TYPE_INDEX_TABLE_1 DOMAIN_TABLE_GET_ELEMENT_BY_FILTERED_ELEMENT_INDEX(CAESAR_TYPE_INDEX_TABLE_1 domain_index, CAESAR_TYPE_INDEX_TABLE_1 filtered_element_index)
{
	CAESAR_BODY_DOMAIN_TABLE_ENTRY domain;
	CAESAR_TYPE_INDEX_TABLE_1 *BASE;
	
	DOMAIN_TABLE_GET_ENTRY_BY_INDEX(domain_index, &domain);
	CAESAR_RETRIEVE_I_B_TABLE_1(domain.FILTERED_ELEMENTS, filtered_element_index, (CAESAR_TYPE_POINTER *)&BASE);
	return *BASE;
}

CAESAR_TYPE_BOOLEAN DOMAIN_TABLE_GET_FILTERED_CARDINALITY_BY_NAME(
		CAESAR_TYPE_STRING name, CAESAR_TYPE_NATURAL *cardinality)
{
	CAESAR_TYPE_INDEX_TABLE_1 domain_index;
	CAESAR_TYPE_POINTER domain_entry;
	CAESAR_BODY_DOMAIN_TABLE_ENTRY domain;
	CAESAR_TYPE_BOOLEAN success;

	success = CAESAR_SEARCH_TABLE_1(DOMAIN_TABLE, (CAESAR_TYPE_POINTER) &name,
			&domain_index, &domain_entry);

	if (success)
	{
		DOMAIN_TABLE_GET_ENTRY_BY_INDEX(domain_index, &domain);
		*cardinality = CAESAR_PUT_INDEX_TABLE_1(domain.FILTERED_ELEMENTS);
	}
	return success;
}

void ELEMENT_TABLE_PRINT(CAESAR_TYPE_FILE FILE, CAESAR_TYPE_TABLE_1 TABLE)
{
	CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM;
	CAESAR_TYPE_POINTER ENTRY;

	fprintf(FILE, "\nDomain elements Table:");

	/* We get the maximum index */
	MAXIMUM = CAESAR_PUT_INDEX_TABLE_1(TABLE);

	/* We iterate through all the indexes */
	for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM; TABLE_INDEX++)
	{
		/* We get the address of each index's entry */
		CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, TABLE_INDEX, &ENTRY);

		fprintf(FILE, "\n\t%d\t%s", TABLE_INDEX, ((CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE) ENTRY)->ELEMENT);
	}
}

/*------------------------------------------------------------------------------*/
/* Construction of the domain elements tables */

CAESAR_TYPE_NATURAL ELEMENT_TABLE_BUILD(
		CAESAR_TYPE_TABLE_1 *DOMAIN_ELEMENTS_TABLE_POINTER,
		CAESAR_TYPE_STRING DOMAIN_FILENAME)
{
	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;
	CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE BASE;

	/* TODO:
	 HERE I MUST TO CHOOSE BETWEEN STORING ALL THE INFO IN THE TABLES OR
	 DISPERSED ON MEMORY. FROM THAT DECISION I'LL CHOOSE TO USE ONE OF
	 THE 2 TYPES TYPES *BODY* OR *TYPE*  */

	/* Creation of the table of domains */

	CAESAR_TYPE_TABLE_1 DOMAIN_ELEMENTS_TABLE;

	BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_DOMAIN_ELEMENTS_TABLE_BASE));
	MARK_AREA = CAESAR_EMPTY_AREA_1();

	CAESAR_CREATE_TABLE_1( &DOMAIN_ELEMENTS_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	(CAESAR_TYPE_COMPARE_FUNCTION) CAESAR_COMPARE_STRING_AREA_1, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	);

	/* TODO: Read the elements from the domain files and save them
	 * under the correct table */

	CAESAR_TYPE_FILE DOMAIN_FILE;

	/* Opening of the datalog file */
	if ((DOMAIN_FILE = fopen(DOMAIN_FILENAME, "r")) == NULL)
	{
		CAESAR_ERROR("File ``%s'' (domain) is unreadable ", DOMAIN_FILENAME);
	}

	const CAESAR_TYPE_INTEGER STRING_BUFFER_SIZE = 256;

	char STRING_BUFFER[STRING_BUFFER_SIZE];

	CAESAR_TYPE_POINTER SUCCESS;

	/* TODO: need a getline function */
	/*  fgetc(DOMAIN_FILE); */

	CAESAR_TYPE_NATURAL CARDINAL;

	CARDINAL = 0;
	SUCCESS = fgets(STRING_BUFFER, STRING_BUFFER_SIZE, DOMAIN_FILE);
	while (SUCCESS != NULL)
	{
		BASE
				= (CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE) CAESAR_PUT_BASE_TABLE_1(DOMAIN_ELEMENTS_TABLE);

		if (STRING_BUFFER[strlen(STRING_BUFFER) - 1] == '\n')
			STRING_BUFFER[strlen(STRING_BUFFER) - 1] = '\0';
		BASE->ELEMENT = CAESAR_COPY_STRING(STRING_BUFFER);
		CAESAR_PUT_TABLE_1(DOMAIN_ELEMENTS_TABLE);
		CARDINAL++;

		SUCCESS = fgets(STRING_BUFFER, STRING_BUFFER_SIZE, DOMAIN_FILE);
	}

	/* TODO: REMOVE BECAUSE WE ARE FILLING THE TABLE ALREADY WITH THE
	 * FILE CONTENTS BASE->ELEMENT = "BES"; */

	/* TODO: TABLE DEBUGGING */
	/* CAESAR_FORMAT_TABLE_1(DOMAIN_ELEMENTS_TABLE, 2); */
	/* CAESAR_PRINT_TABLE_1(stdout, DOMAIN_ELEMENTS_TABLE); */

	CAESAR_BEGIN_DEBUG(4)
	{
		ELEMENT_TABLE_PRINT(stdout, DOMAIN_ELEMENTS_TABLE);

	}CAESAR_END_DEBUG;

	/* Return the table */
	*DOMAIN_ELEMENTS_TABLE_POINTER = DOMAIN_ELEMENTS_TABLE;

	return CARDINAL;
}

/*------------------------------------------------------------------------------*/
/* Get the a domain by its index */

void DOMAIN_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX,
		CAESAR_TYPE_DOMAIN_TABLE_ENTRY ENTRY)
{
	CAESAR_TYPE_POINTER BASE;
	CAESAR_TYPE_POINTER MARK;

	CAESAR_RETRIEVE_I_B_TABLE_1(DOMAIN_TABLE, INDEX, &BASE);

	CAESAR_RETRIEVE_I_M_TABLE_1(DOMAIN_TABLE, INDEX, &MARK);

	ENTRY->NAME = *((CAESAR_TYPE_STRING *) BASE);
	ENTRY->CARDINAL = ((CAESAR_TYPE_DOMAIN_TABLE_MARK) MARK)->CARDINAL;
	ENTRY->ELEMENTS = ((CAESAR_TYPE_DOMAIN_TABLE_MARK) MARK)->ELEMENTS;
	ENTRY->FILTERED_ELEMENTS= ((CAESAR_TYPE_DOMAIN_TABLE_MARK) MARK)->FILTERED_ELEMENTS;
}

/*------------------------------------------------------------------------------*/
/* Get the a domain element by its index and the domain index */

void DOMAIN_TABLE_GET_ELEMENT_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 DOMAIN_INDEX,
		CAESAR_TYPE_INDEX_TABLE_1 ELEMENT_INDEX, CAESAR_TYPE_STRING *STRING)
{
	CAESAR_BODY_DOMAIN_TABLE_ENTRY DOMAIN_BODY;
	CAESAR_TYPE_DOMAIN_TABLE_ENTRY DOMAIN;
	DOMAIN = &DOMAIN_BODY;

	DOMAIN_TABLE_GET_ENTRY_BY_INDEX(DOMAIN_INDEX, DOMAIN);
	ELEMENT_TABLE_GET_ENTRY_BY_INDEX(DOMAIN->ELEMENTS, ELEMENT_INDEX, STRING);
}

/*------------------------------------------------------------------------------*/
/* Get the a domain element by its index and table */

void ELEMENT_TABLE_GET_ENTRY_BY_INDEX(CAESAR_TYPE_TABLE_1 TABLE,
		CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_STRING *STRING)
{
	CAESAR_TYPE_POINTER BASE;

	CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, INDEX, &BASE);

	CAESAR_BEGIN_DEBUG(5)
	{

		fprintf(stdout, "TABLA DE DOMINIOS [%d elementos] -> acceso al %i\n", CAESAR_PUT_INDEX_TABLE_1(TABLE), INDEX);

	}CAESAR_END_DEBUG;

	*STRING = *((CAESAR_TYPE_STRING *) BASE);
}

void DOMAIN_TABLE_BUILD()
{
	/* Creation of TABLE STRUCTURES to memorize the DATALOG-BES */

	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;
	CAESAR_TYPE_POINTER BASE;
	CAESAR_TYPE_DOMAIN_TABLE_MARK MARK;
	CAESAR_TYPE_TABLE_1 DOMAIN_ELEMENTS_TABLE;

	/* Creation of the table of domains */

	CAESAR_TYPE_TABLE_1 AUX_DOMAIN_TABLE;

	BASE_AREA = CAESAR_STRING_AREA_1();
	MARK_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_DOMAIN_TABLE_MARK));

	CAESAR_CREATE_TABLE_1( &AUX_DOMAIN_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	(CAESAR_TYPE_COMPARE_FUNCTION) CAESAR_COMPARE_STRING_AREA_1, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	);

	CAESAR_TYPE_DOMAIN_LIST CELL;

	for (CELL = CAESAR_DATALOG_DOMAIN_LIST; CELL != NULL; CELL = CELL->NEXT)
	{

		/* Search where it's gonna be hold the new entry */
		BASE = CAESAR_PUT_BASE_TABLE_1(AUX_DOMAIN_TABLE);
		MARK
				= (CAESAR_TYPE_DOMAIN_TABLE_MARK) CAESAR_PUT_MARK_TABLE_1(AUX_DOMAIN_TABLE);

		/* Add the information to the new entry */
		/*
		 CAESAR_COPY_AREA_1(BASE,&(CELL->CAESAR_DOMAIN->CAESAR_NAME), sizeof (CAESAR_TYPE_STRING));*/
		*((CAESAR_TYPE_STRING *)BASE) = CELL->DOMAIN->NAME;

		/* We should tell the user that the cardinal provided by him is incorrect if it is */
		/* TODO: Decidir esto MARK->CARDINAL = CELL->DOMAIN->CARDINAL;*/
		MARK->CARDINAL = ELEMENT_TABLE_BUILD(&DOMAIN_ELEMENTS_TABLE,
				CELL->DOMAIN->FILENAME);
		/* 	  MARK->CARDINAL = COUNT_NUMBER_OF_LINES(CELL->DOMAIN->FILENAME); */
		MARK->ELEMENTS = DOMAIN_ELEMENTS_TABLE;

		FILTERED_ELEMENT_TABLE_BUILD(&(MARK->FILTERED_ELEMENTS));

		/* TODO: DEFINIR ESTRUCTURAS STRUCT_MARK_DOMAIN */
		/* Encapsulation on table creation functions INIT_AUX_DOMAIN_TABLE */

		/* Add the new entry to the table */
		CAESAR_PUT_TABLE_1(AUX_DOMAIN_TABLE);
	}

	CAESAR_BEGIN_DEBUG(4)
	{
		CAESAR_FORMAT_TABLE_1(AUX_DOMAIN_TABLE, 2);
		CAESAR_PRINT_TABLE_1(stdout, AUX_DOMAIN_TABLE);
	}CAESAR_END_DEBUG;

	/* Return the table */
	DOMAIN_TABLE = AUX_DOMAIN_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Add a filtered domain element to the proper domain */

void DOMAIN_TABLE_ADD_FILTERED_ELEMENT(CAESAR_TYPE_INDEX_TABLE_1 domain_index,
		CAESAR_TYPE_INDEX_TABLE_1 element_index)
{
	CAESAR_TYPE_POINTER BASE;
	CAESAR_BODY_DOMAIN_TABLE_ENTRY domain;
	CAESAR_TYPE_POINTER old_entry;
	CAESAR_TYPE_INDEX_TABLE_1 index;

	DOMAIN_TABLE_GET_ENTRY_BY_INDEX(domain_index, &domain);

	BASE = CAESAR_PUT_BASE_TABLE_1(domain.FILTERED_ELEMENTS);

	*((CAESAR_TYPE_INDEX_TABLE_1 *) BASE) = element_index;

	CAESAR_SEARCH_AND_PUT_TABLE_1(domain.FILTERED_ELEMENTS, &index, &old_entry);
}

/*------------------------------------------------------------------------------*/
/* Construction of the filtered domain elements tables */

void FILTERED_ELEMENT_TABLE_BUILD(
		CAESAR_TYPE_TABLE_1 *FILTERED_ELEMENTS_TABLE_POINTER)
{
	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;

	/* Creation of the table of domains */

	CAESAR_TYPE_TABLE_1 FILTERED_ELEMENTS_TABLE;

	BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1));
	MARK_AREA = CAESAR_EMPTY_AREA_1();

	CAESAR_CREATE_TABLE_1( &FILTERED_ELEMENTS_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	NULL, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	);

	/* Return the table */
	*FILTERED_ELEMENTS_TABLE_POINTER = FILTERED_ELEMENTS_TABLE;

	return;
}

/*------------------------------------------------------------------------------*/
/* Construction of the filtered domain elements tables */

void DOMAIN_TABLE_PRINT()
{
	CAESAR_TYPE_NATURAL i;
	CAESAR_TYPE_NATURAL domain_table_size;
	CAESAR_TYPE_NATURAL filtered_elements_table_size;
	CAESAR_BODY_DOMAIN_TABLE_ENTRY domain;

	domain_table_size = DOMAIN_TABLE_ENTRY_COUNT();
	for (i = 0; i < domain_table_size; i++)
	{
		DOMAIN_TABLE_GET_ENTRY_BY_INDEX(i, &domain);
		filtered_elements_table_size
				= CAESAR_PUT_INDEX_TABLE_1(domain.FILTERED_ELEMENTS);
		fprintf(stdout,
				"Domain %d - %s: cardinal = %d, filtered cardinal = %d\n", i,
				domain.NAME, domain.CARDINAL, filtered_elements_table_size);
	}
}
