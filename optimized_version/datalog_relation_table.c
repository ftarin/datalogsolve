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

#include "datalog_successor.h"
#include "datalog_rule_table.h"
#include "datalog_variable_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_relation_table.h"
#include "datalog_combination_table.h"
#include "datalog_domain_table.h"
#include "datalog_fact_table.h"

void RELATION_TABLE_CALCULATE_FILTERED_DOMAINS();

/*============================================================================*/
/* Types */

typedef CAESAR_TYPE_STRING *CAESAR_TYPE_RELATION_TABLE_BASE;

typedef struct CAESAR_STRUCT_RELATION_TABLE_MARK
{
	/* Relation arity */
	CAESAR_TYPE_NATURAL ARITY;

	/* Domain indexes */
	CAESAR_TYPE_INDEX_TABLE_1 *DOMAINS;

	/* Filtered domain tables */
	CAESAR_TYPE_TABLE_1 *FILTERED_DOMAINS;

	/* Combination table */
	CAESAR_TYPE_TABLE_1 COMBINATIONS;

	/* Fact table */
	CAESAR_TYPE_TABLE_1 FACTS; /* TODO */

	/* Rule table */
	CAESAR_TYPE_TABLE_1 RULES; /* TODO */
} CAESAR_BODY_RELATION_TABLE_MARK, *CAESAR_TYPE_RELATION_TABLE_MARK;

/*==============================================================================*/
/* Global variables */

/* Global relation table */
static CAESAR_TYPE_TABLE_1 RELATION_TABLE;

/*==============================================================================*/
/* Relation Table Management */

/*------------------------------------------------------------------------------*/
/* Get relation by relation name */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_GET_RELATION_BY_NAME(
		CAESAR_TYPE_STRING NAME, CAESAR_TYPE_RELATION_TABLE_ENTRY ENTRY)
{
	CAESAR_TYPE_BOOLEAN SUCCESS;
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;
	CAESAR_TYPE_RELATION_TABLE_MARK MARK;

	SUCCESS = CAESAR_SEARCH_TABLE_1(RELATION_TABLE,
			(CAESAR_TYPE_POINTER) &NAME, &INDEX, (CAESAR_TYPE_POINTER *)&MARK);

	CAESAR_DATALOG_GET_RELATION_BY_INDEX(INDEX, ENTRY); /* TODO: Avoids some code */

	return SUCCESS;
}

/*------------------------------------------------------------------------------*/
/* Get index by relation name */

CAESAR_TYPE_INDEX_TABLE_1 CAESAR_DATALOG_GET_RELATION_INDEX_BY_NAME(
		CAESAR_TYPE_STRING RELATION_NAME)
{
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;
	CAESAR_TYPE_POINTER ENTRY;
	CAESAR_TYPE_BOOLEAN SUCCESS;

	SUCCESS = CAESAR_SEARCH_TABLE_1(RELATION_TABLE, (CAESAR_TYPE_POINTER)
			&RELATION_NAME, &INDEX, &ENTRY);

	/* TODO: Conditional compiling of the assert */
	CAESAR_ASSERT(SUCCESS);

	return INDEX;
}

/*------------------------------------------------------------------------------*/
/* Get relation by index */

void CAESAR_DATALOG_GET_RELATION_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX,
		CAESAR_TYPE_RELATION_TABLE_ENTRY ENTRY)
{
	CAESAR_TYPE_RELATION_TABLE_BASE BASE;
	CAESAR_TYPE_RELATION_TABLE_MARK MARK;

	CAESAR_RETRIEVE_I_B_TABLE_1(RELATION_TABLE, INDEX,
			(CAESAR_TYPE_POINTER *) &BASE );

	CAESAR_RETRIEVE_I_M_TABLE_1(RELATION_TABLE, INDEX,
			(CAESAR_TYPE_POINTER *) &MARK );

	ENTRY->NAME = *((CAESAR_TYPE_STRING *) BASE); /* TODO: SHOULD BE CHECKED */
	ENTRY->ARITY = MARK->ARITY;
	ENTRY->DOMAINS = MARK->DOMAINS;
	ENTRY->FILTERED_DOMAINS = MARK->FILTERED_DOMAINS;
	ENTRY->COMBINATIONS = MARK->COMBINATIONS;
	ENTRY->FACTS = MARK->FACTS;
	ENTRY->RULES = MARK->RULES;
}

/*------------------------------------------------------------------------------*/
/* Construction of the relation tables */

void CAESAR_DATALOG_BUILD_RELATION_TABLE()
{
	CAESAR_TYPE_AREA_1 BASE_AREA;
	CAESAR_TYPE_AREA_1 MARK_AREA;

	CAESAR_TYPE_RELATION_TABLE_BASE BASE;
	CAESAR_TYPE_RELATION_TABLE_MARK MARK;
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;

	CAESAR_TYPE_RELATION_LIST RCELL;
	CAESAR_TYPE_DOMAIN_LIST DCELL;
	CAESAR_TYPE_TABLE_1 AUX_RELATION_TABLE;
	int i;

	BASE_AREA = CAESAR_STRING_AREA_1();
	MARK_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_RELATION_TABLE_MARK));

	/**************************************/
	/* We create the empty RELATION_TABLE */
	/**************************************/

	CAESAR_CREATE_TABLE_1( &AUX_RELATION_TABLE, BASE_AREA, MARK_AREA, 0, /* LIMIT_SIZE by default */
	0, /* HASH_SIZE by default */
	CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	(CAESAR_TYPE_COMPARE_FUNCTION) CAESAR_COMPARE_STRING_AREA_1, /* COMPARE_FUNCTION */
	NULL, /* HASH_FUNCTION */
	NULL, /* PRINT_FUNCTION */
	CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	);

	/* Return the table */
	RELATION_TABLE = AUX_RELATION_TABLE;

	/********************************************************/
	/* We iterate over each relation of the datalog program */
	/********************************************************/

	for (RCELL = CAESAR_DATALOG_RELATION_LIST; RCELL != NULL; RCELL
			= RCELL->NEXT)
	{

		/*======================================*/
		/* Search the position of the new entry */
		/*======================================*/

		BASE
				= (CAESAR_TYPE_RELATION_TABLE_BASE) CAESAR_PUT_BASE_TABLE_1(AUX_RELATION_TABLE);
		MARK
				= (CAESAR_TYPE_RELATION_TABLE_MARK) CAESAR_PUT_MARK_TABLE_1(AUX_RELATION_TABLE);
		INDEX
				= (CAESAR_TYPE_INDEX_TABLE_1) CAESAR_PUT_INDEX_TABLE_1(AUX_RELATION_TABLE);

		/*======================================*/
		/* Add the information of the new entry */
		/*======================================*/

		/*----------------------------------------------*/
		/* The base field is directly the relation name */
		/*----------------------------------------------*/

		*BASE = RCELL->RELATION->NAME;

		/*------------------------------------------------*/
		/* The mark field contains more complicated stuff */
		/*------------------------------------------------*/

		{ /* This bloc is for documenting purposes */

			/***********************************/
			/* Store the arity of the relation */
			/***********************************/

			MARK->ARITY = RCELL->RELATION->ARITY;

			/**************************************************/
			/* Store the domains associated with the relation */
			/**************************************************/

			{ /* This bloc is for documenting purposes */

				/*================================================================*/
				/* Create the space needed for storing the indexes of the domains */
				/*================================================================*/

				CAESAR_CREATE(MARK->DOMAINS, MARK->ARITY * sizeof (CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
				CAESAR_CREATE(MARK->FILTERED_DOMAINS, MARK->ARITY * sizeof (CAESAR_TYPE_TABLE_1), CAESAR_TYPE_TABLE_1 *);

				/*=======================================================*/
				/* Iterate over the domains associated with the relation */
				/*=======================================================*/

				for (i = 0; i < MARK->ARITY; i++)
				{
					FILTERED_ELEMENT_TABLE_BUILD(&(MARK->FILTERED_DOMAINS[i]));
				}
				
				CAESAR_TYPE_INDEX_TABLE_1 INDEX;
				CAESAR_TYPE_POINTER ENTRY;
				CAESAR_TYPE_BOOLEAN SUCCESS;

				for (i = 0, DCELL = RCELL->RELATION->DOMAIN_LIST; i
						< MARK->ARITY && DCELL != NULL; i++, DCELL
						= DCELL->NEXT)
				{

					/*-------------------------------------------------------------------------*/
					/* Get and store in the entry the index of each domain in the domain table */
					/*-------------------------------------------------------------------------*/

					SUCCESS = DOMAIN_TABLE_GET_INDEX_AND_VALUE_BY_NAME(
							DCELL->DOMAIN->NAME, &INDEX, &ENTRY);
					CAESAR_ASSERT(SUCCESS == CAESAR_TRUE);

					/* The index must be reasonable */

					MARK->DOMAINS[i] = INDEX;
				}
			}

			/*********************************/
			/* Store a new combination table */
			/*********************************/

			COMBINATION_TABLE_BUILD(&(MARK->COMBINATIONS), MARK->ARITY);

			/*********************************************************/
			/* Store a new fact table if the relation is extensional */
			/*********************************************************/

			if (RCELL->RELATION->IONATURE == CAESAR_INPUT_TUPLES)
			{
				CAESAR_DATALOG_BUILD_FACT_TABLE(&(MARK->FACTS), MARK->ARITY,
						*((CAESAR_TYPE_STRING *)BASE));
			}
			else
			{
				MARK->FACTS = NULL;
			}

			/************************************************/
			/* Store the rules associated with the relation */
			/************************************************/

			CAESAR_DATALOG_BUILD_RULE_I_TABLE(&(MARK->RULES));

		} /* This bloc is for documenting purposes */

		/* Add the new entry to the table */
		CAESAR_PUT_TABLE_1(AUX_RELATION_TABLE);
	}

	CAESAR_BEGIN_DEBUG(4)
	{

		CAESAR_DATALOG_PRINT_RELATION_TABLE();

	}CAESAR_END_DEBUG;

	RELATION_TABLE_CALCULATE_FILTERED_DOMAINS();
}

static void FILTERED_DOMAIN_TABLE_ADD_FILTERED_ELEMENT(CAESAR_TYPE_TABLE_1 table,
		CAESAR_TYPE_INDEX_TABLE_1 element_index)
{
	CAESAR_TYPE_POINTER BASE;
	CAESAR_TYPE_POINTER old_entry;
	CAESAR_TYPE_INDEX_TABLE_1 index;

	BASE = CAESAR_PUT_BASE_TABLE_1(table);

	*((CAESAR_TYPE_INDEX_TABLE_1 *) BASE) = element_index;

	CAESAR_SEARCH_AND_PUT_TABLE_1(table, &index, &old_entry);
}

CAESAR_TYPE_NATURAL RELATION_TABLE_ENTRY_COUNT()
{
	return CAESAR_PUT_INDEX_TABLE_1(RELATION_TABLE);
}

void RELATION_TABLE_CALCULATE_FILTERED_DOMAINS()
{
	CAESAR_TYPE_NATURAL relation_index;
	CAESAR_TYPE_NATURAL relation_table_size;
	CAESAR_BODY_RELATION_TABLE_ENTRY relation;
	CAESAR_TYPE_NATURAL i;
	CAESAR_TYPE_INDEX_TABLE_1 element_index;
	CAESAR_TYPE_BOOLEAN is_variable;
	CAESAR_TYPE_POINTER fact;
	CAESAR_TYPE_NATURAL fact_counter;
	CAESAR_TYPE_NATURAL fact_table_size;
	
	relation_table_size = RELATION_TABLE_ENTRY_COUNT();

	for (relation_index = 0; relation_index < relation_table_size; relation_index++)
	{
		CAESAR_DATALOG_GET_RELATION_BY_INDEX(relation_index, &relation);

		if (relation.FACTS != NULL)
		{
			fact_table_size = FACT_TABLE_GET_ENTRY_COUNT(relation.FACTS);

			for (fact_counter = 0; fact_counter < fact_table_size; fact_counter++)
			{
				COMBINATION_TABLE_GET_BINARY_ENTRY_BY_INDEX(relation.FACTS,
						fact_counter, &fact);
				for (i = 0; i < relation.ARITY; i++)
				{
					COMBINATION_GET_VALUE_AT(fact, i, relation.ARITY,
							&element_index, &is_variable);
					DOMAIN_TABLE_ADD_FILTERED_ELEMENT(relation.DOMAINS[i],
							element_index);
					FILTERED_DOMAIN_TABLE_ADD_FILTERED_ELEMENT(
							relation.FILTERED_DOMAINS[i], element_index);
				}
			}
		}
	}
}

/*------------------------------------------------------------------------------*/
/* Print the relation table */

void CAESAR_DATALOG_PRINT_RELATION_TABLE()
{
	CAESAR_FORMAT_TABLE_1(RELATION_TABLE, 2);
	CAESAR_PRINT_TABLE_1(stdout, RELATION_TABLE);
}

/*------------------------------------------------------------------------------*/
/* Count the number of entries in the relation table */

CAESAR_TYPE_NATURAL CAESAR_DATALOG_GET_RELATION_COUNT()
{
	return (CAESAR_TYPE_NATURAL) CAESAR_PUT_INDEX_TABLE_1(RELATION_TABLE);
}
