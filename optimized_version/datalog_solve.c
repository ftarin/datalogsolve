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

#include "datalog_solve.h"
#include "datalog_relation_table.h"
#include "datalog_domain_table.h"
#include "datalog_predicate_table.h"
#include "datalog_combination_table.h"
#include "datalog_variable_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_successor.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/*
 * Considerations:
 * 
 * - BES MANAGEMENT
 * 		- We use only SOLVE_MODE BFS if option -tuples exists and OPTIMIZED DFS MODE 5 otherwise 
 * 		- We should use the best hash functions for the natural boolean variables
 * 		- As we don't consider complex queries, we reuse the PQUERY for the PCQUERY
 * 		- Each query has only one predicate
 * 
 * - DEBUG LEVELS
 * 		(0) 
 * 			- Assertions activated
 * 
 * 		(1) 
 * 			- Print boolean variables types in each iteration
 * 
 * 		(2)
 * 			- Print state of the tables used for encoding the datalog program
 * 
 * - FLAGS
 * 		- Everything is in DFS mode
 */

/*==============================================================================*/
/* Global variables */

/* TODO: To place and comment properly */

static CAESAR_TYPE_FILE *OUTPUT_FILES;

static CAESAR_TYPE_STRING *OUTPUT_FILES_NAMES;

/*==============================================================================*/
/* BES Management */

/*------------------------------------------------------------------------------*/
/* Block sign calculation */
static CAESAR_TYPE_BOOLEAN CAESAR_BLOCK_SIGN(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	return CAESAR_MINIMAL_FIXED_POINT_SOLVE_1;
}

/*------------------------------------------------------------------------------*/
/* Block unique resolution calculation */
static CAESAR_TYPE_BOOLEAN CAESAR_BLOCK_UNIQUE_RESOLUTION(
		CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	if (caesar_output_tuples == CAESAR_TRUE)
		return CAESAR_FALSE;
	else
		return CAESAR_TRUE;
}

/*------------------------------------------------------------------------------*/
/* Resolution mode of a block */
static CAESAR_TYPE_NATURAL CAESAR_BLOCK_SOLVE_MODE(
		CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	if (caesar_output_tuples == CAESAR_TRUE)
		return 1; /* BFS General */
	else
		return 5; /* Optimized BFS */
}

/*------------------------------------------------------------------------------*/
/* Area of a block variable */
static CAESAR_TYPE_AREA_1 CAESAR_BLOCK_VARIABLE_AREA(
		CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	return CAESAR_NATURAL_AREA_1 (sizeof (CAESAR_TYPE_BOOLEAN_VARIABLE));
}

/*------------------------------------------------------------------------------*/
/* CAESAR_LIMIT_SIZE of a bloc's variable table */
static CAESAR_TYPE_NATURAL CAESAR_BLOCK_LIMIT_SIZE(
		CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	return 0;
}

/*------------------------------------------------------------------------------*/
/* CAESAR_HASH_SIZE of a bloc's variable table */
static CAESAR_TYPE_NATURAL CAESAR_BLOCK_HASH_SIZE(
		CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	return 0;
}

/*------------------------------------------------------------------------------*/
/* CAESAR_BLOCK_PRIME of a bloc's variable table */
static CAESAR_TYPE_BOOLEAN CAESAR_BLOCK_PRIME(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
	return CAESAR_TRUE;
}

/*------------------------------------------------------------------------------*/
/* Kind of a variable */
static CAESAR_TYPE_BOOLEAN CAESAR_VARIABLE_KIND(CAESAR_TYPE_POINTER VARIABLE)
{
	CAESAR_TYPE_BOOLEAN_VARIABLE *V;
	V = (CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE;

	switch (V->BLOCK_INDEX)
	{
	case 0:
		switch (V->TYPE)
		{
		case 0: /* X0 */
			return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1; /* Returns an AND */
		case 1: /* X1 */
			return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1; /* Returns an OR */
		case 2: /* X2 */
			return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1; /* Returns an AND */
		default:
			CAESAR_ERROR("Boolean variable not defined! Type: %d Block: %d",
					V->BLOCK_INDEX, V->TYPE);
		}
		break;
	case 1:
		switch (V->TYPE)
		{
		case 0: /* X3 */
		case 1: /* X5 */
		case 2: /* X7 */
			return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1;
		case 3: /* X8 */
		default:
			return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1;
		}
		break;
	case 2:
		switch (V->TYPE)
		{
		case 0: /* X4 */
			return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1;
		case 1: /* X6 */
			return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1;
		case 2: /* True */
			return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1;
		case 3: /* False */
		default:
			return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1;
		}
		break;
	default:
		CAESAR_ERROR(
				"Block not defined! Boolean variable not defined! Type: %d Block: %d",
				V->BLOCK_INDEX, V->TYPE);
	}

	/* This code should not be executed */
	CAESAR_ERROR(
			"CAESAR_VARIABLE_KIND: code not to be executed! Type: %d Block: %d",
			V->BLOCK_INDEX, V->TYPE);
	return CAESAR_FALSE;
}

/*------------------------------------------------------------------------------*/
/* Comparison between a bloc's variables */
static CAESAR_TYPE_BOOLEAN CAESAR_VARIABLE_COMPARE(
		CAESAR_TYPE_POINTER VARIABLE_1, CAESAR_TYPE_POINTER VARIABLE_2)
{
	if (*VARIABLE_1 == *VARIABLE_2)
		return CAESAR_TRUE;
	else
		return CAESAR_FALSE;
}

/*------------------------------------------------------------------------------*/
/* Hash-code calculation of a bloc's variables */
static CAESAR_TYPE_NATURAL CAESAR_VARIABLE_HASH(CAESAR_TYPE_POINTER VARIABLE,
		CAESAR_TYPE_NATURAL MODULUS)
{
	return CAESAR_0_HASH(VARIABLE, sizeof(CAESAR_TYPE_BOOLEAN_VARIABLE),
			MODULUS);
}

/*------------------------------------------------------------------------------*/
/* Printing of a variable */
void CAESAR_VARIABLE_PRINT(CAESAR_TYPE_FILE FILE, CAESAR_TYPE_POINTER VARIABLE)
{
	CAESAR_TYPE_BOOLEAN_VARIABLE *V;
	V = (CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE;

	switch (V->BLOCK_INDEX)
	{
	case 0:
		switch (V->TYPE)
		{
		case 0: /* X0 */
			(void) fprintf(FILE, "X0(V)");
			break;
		case 1: /* X1 */
			(void) fprintf(FILE, "X1(V)");
			break;
		case 2: /* X2 */
			(void) fprintf(FILE, "X2(·)");
			break;
		default:
			CAESAR_ERROR("Boolean variable not defined! Type: %d Block: %d",
					V->BLOCK_INDEX, V->TYPE);
		}
		break;
	case 1:
		switch (V->TYPE)
		{
		case 0: /* X3 */
			(void) fprintf(FILE, "X3(V)");
			break;
		case 1: /* X5 */
			(void) fprintf(FILE, "X5(V)");
			break;
		case 2: /* X7 */
			(void) fprintf(FILE, "X7(V)");
			break;
		case 3: /* X8 */
			(void) fprintf(FILE, "X8(·)");
		}
		break;
	case 2:
		switch (V->TYPE)
		{
		case 0: /* X4 */
			(void) fprintf(FILE, "X4(V)");
			break;
		case 1: /* X6 */
			(void) fprintf(FILE, "X6(·)");
			break;
		case 2: /* True */
			(void) fprintf(FILE, "True(·)");
			break;
		case 3: /* False */
		default:
			(void) fprintf(FILE, "False(V)");
		}
		break;
	default:
		CAESAR_ERROR(
				"Block not defined! Boolean variable not defined! Type: %d Block: %d",
				V->BLOCK_INDEX, V->TYPE);
	}
}

/*------------------------------------------------------------------------------*/
/* Calculation of the successors of a variable */
void CAESAR_VARIABLE_ITERATE(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	CAESAR_BEGIN_DEBUG(1)
	{

		printf("IT. on: ");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_1);
		printf(" =>");

	}
	CAESAR_END_DEBUG;

	switch (((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->BLOCK_INDEX)
	{
	case 0:
		switch (((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->TYPE)
		{
		case 0: /* X0 */
			CAESAR_DATALOG_COMPUTE_X0_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 1: /* X1 */
			CAESAR_DATALOG_COMPUTE_X1_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 2: /* X2 */
			CAESAR_DATALOG_COMPUTE_X2_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		default:
			CAESAR_ERROR(
					"Boolean variable not defined! Type: %d Block: %d",
					((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->BLOCK_INDEX,
					((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->TYPE);
		}
		break;
	case 1:
		switch (((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->TYPE)
		{
		case 0: /* X3 */
			CAESAR_DATALOG_COMPUTE_X3_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 1: /* X5 */
			CAESAR_DATALOG_COMPUTE_X5_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 2: /* X7 */
			CAESAR_DATALOG_COMPUTE_X7_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 3: /* X8 */
			CAESAR_DATALOG_COMPUTE_X8_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
		}
		break;
	case 2:
		switch (((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->TYPE)
		{
		case 0: /* X4 */
			CAESAR_DATALOG_COMPUTE_X4_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 1: /* X6 */
			CAESAR_DATALOG_COMPUTE_X6_SUCCESSOR(VARIABLE_1, VARIABLE_2,
					CAESAR_LOOP);
			break;
		case 2: /* True */
			(void) fprintf(stdout, " True");
			break;
		case 3: /* False */
		default:
			(void) fprintf(stdout, " False");
		}
		break;
	default:
		CAESAR_ERROR(
				"Block not defined! Boolean variable not defined! Type: %d Block: %d",
				((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->BLOCK_INDEX,
				((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->TYPE);
	}
}

/*------------------------------------------------------------------------------*/
/* Obtains the output tuples */

void STABLE_VARIABLE_ITERATE_IN_OUTPUT_RELATION_MODE(CAESAR_TYPE_SOLVE_1 BES,
		CAESAR_TYPE_NATURAL BLOCK, CAESAR_TYPE_POINTER VARIABLE,
		CAESAR_TYPE_BOOLEAN *VALUE)
{
	if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE)->TYPE != 1
			|| *VALUE == CAESAR_FALSE || BLOCK != 2)
		return;

	CAESAR_TYPE_NATURAL i;
	CAESAR_TYPE_INDEX_TABLE_1 relation_index;
	CAESAR_BODY_PREDICATE_TABLE_ENTRY cpredicate;
	CAESAR_BODY_RELATION_TABLE_ENTRY relation;
	CAESAR_BODY_COMBINATION_TABLE_ENTRY combination;

	/*      CAESAR_CREATE(CQUERY->RELATION_INDICES, sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); */
	/*      CAESAR_CREATE(CQUERY->COMBINATION_INDICES, sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); */

	PREDICATE_TABLE_GET_ENTRY_BY_INDEX(((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE)->ENTRY_INDEX,
			&cpredicate);

	relation_index = cpredicate.RELATION;

	CAESAR_DATALOG_GET_RELATION_BY_INDEX(relation_index, &relation);

	if (relation.FACTS != NULL)
	{
		return;
	}

	CAESAR_CREATE(combination.ARGUMENTS, relation.ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
	CAESAR_CREATE(combination.ARGUMENT_IS_VARIABLE, relation.ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);

	COMBINATION_TABLE_GET_ENTRY_BY_INDEX(relation.COMBINATIONS,
			cpredicate.COMBINATION, relation.ARITY, &combination);

	for (i = 0; i < combination.LENGTH; i++)
	{
		if (i != 0)
			fprintf(OUTPUT_FILES[relation_index], " ");
		fprintf(OUTPUT_FILES[relation_index], "%d", combination.ARGUMENTS[i]);
	}
	fprintf(OUTPUT_FILES[relation_index], "\n");

	CAESAR_DELETE(combination.ARGUMENTS);
	CAESAR_DELETE(combination.ARGUMENT_IS_VARIABLE);
}

/*==============================================================================*/
/* Construction of the BES with its associate data structures */

CAESAR_TYPE_BOOLEAN CAESAR_INITIALIZE_DATALOG_STRUCTURES()
{

	/* Build the domain table */
	DOMAIN_TABLE_BUILD();

	/* Build the variable table
	 * 
	 * [Restrictions]
	 * 		- Must exist before the rule table
	 */
	CAESAR_DATALOG_BUILD_VARIABLE_TABLE();
	//
	//	/* Build the relation table */
	CAESAR_DATALOG_BUILD_RELATION_TABLE();
	
    CAESAR_BEGIN_DEBUG(2) {
    	DOMAIN_TABLE_PRINT();
    } CAESAR_END_DEBUG;
	

	/* Build the concretized rule table */
	CAESAR_DATALOG_BUILD_CRULE_TABLE();

	/* Build the variable table
	 * 
	 * [Restrictions]
	 * 		- Must exist before the rule table
	 */
	CAESAR_DATALOG_BUILD_RULE_TABLE();

	PQUERY_TABLE_BUILD(); /* X1 */
	//	CAESAR_DATALOG_BUILD_CQUERY_TABLE(); /* X2 */
	PREDICATE_TABLE_BUILD(); /* X3 */
	CAESAR_DATALOG_BUILD_PRULE_TABLE(); /* X4*/

	return CAESAR_TRUE;
}

/* Outer variables defined in the scanner */
extern FILE *yyin;
extern FILE *yyout;

/* Prototype for avoiding warnings */
extern CAESAR_TYPE_BOOLEAN yyparse(void);

CAESAR_TYPE_BOOLEAN CAESAR_READ_DATALOG_1(CAESAR_TYPE_SOLVE_1 *DATALOG_BES,
		CAESAR_TYPE_STRING DATALOG_FILENAME)
{
	CAESAR_TYPE_FILE BES_FILE;
	CAESAR_TYPE_BOOLEAN SYNTAX_ERROR;

	/* Opening of the datalog file */
	if ((BES_FILE = fopen(DATALOG_FILENAME, "r")) == NULL)
	{
		(*DATALOG_BES) = NULL;
		CAESAR_ERROR("File ``%s'' is unreadable", DATALOG_FILENAME);
	}

	/* Initialization of the I/O files for BISON */
	yyin = BES_FILE;
	yyout = stdout;

	/* Lexical and syntactic analysis */
	SYNTAX_ERROR = (CAESAR_TYPE_BOOLEAN) yyparse();

	/* Closing of the datalog file */
	(void) fclose(BES_FILE);

	/* Checking of the existence of syntactic errors */
	if (SYNTAX_ERROR)
	{
		(*DATALOG_BES) = NULL;
		CAESAR_ERROR("Syntax error");
	}

	/* Initialization of the data structures for computation */
	CAESAR_INITIALIZE_DATALOG_STRUCTURES();

	/***********************/
	/* CREATION OF THE BES */
	/***********************/

	CAESAR_CREATE_SOLVE_1(DATALOG_BES, 
			caesar_output_tuples ? 3 : 1, CAESAR_BLOCK_SIGN,
			CAESAR_BLOCK_UNIQUE_RESOLUTION, CAESAR_BLOCK_SOLVE_MODE,
			CAESAR_BLOCK_VARIABLE_AREA, CAESAR_BLOCK_LIMIT_SIZE,
			CAESAR_BLOCK_HASH_SIZE, CAESAR_BLOCK_PRIME, CAESAR_VARIABLE_KIND,
			CAESAR_VARIABLE_COMPARE, CAESAR_VARIABLE_HASH,
			CAESAR_VARIABLE_PRINT, CAESAR_VARIABLE_ITERATE, NULL);

	if (*DATALOG_BES != NULL)
		return CAESAR_TRUE;
	else
		return CAESAR_FALSE;
}

CAESAR_TYPE_BOOLEAN CAESAR_COMPUTE_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES,
		CAESAR_TYPE_BOOLEAN *SOLUTION_EXISTS)
{
	CAESAR_TYPE_BOOLEAN_VARIABLE X0;
	CAESAR_TYPE_BOOLEAN ANSWER;
	CAESAR_TYPE_ERROR_SOLVE_1 ERROR;

	/* Build the initial variable */

	X0.BLOCK_INDEX = 0;
	X0.TYPE = 0;
	X0.ENTRY_INDEX = 0;

	/* Compute the solution to the initial variable */

	CAESAR_BEGIN_DEBUG(1)
	{
		printf("<< DEBUGGING >>\n");
		printf("Execution Trace [(V) -> Disjunctive, (·) -> Conjunctive]\n");
		printf("--------------------------------------------------------\n");
	}CAESAR_END_DEBUG;

	ANSWER = CAESAR_COMPUTE_SOLVE_1(DATALOG_BES, /* BES */
	0, /* BLOCK */
	(CAESAR_TYPE_POINTER) &X0 /* initial variable */
	);

	/* Check the presence of errors in the computation of the BES */

	ERROR = CAESAR_STATUS_COMPUTE_SOLVE_1(DATALOG_BES, 0);

	switch (ERROR)
	{
	case CAESAR_NONE_SOLVE_1:
		break;
	default:
		printf("RESOLUTION INCOMPLETE: ");
		switch (ERROR)
		{
		case CAESAR_MULTIPLE_RESOLUTION_SOLVE_1:
			printf("MULTIPLE RESOLUTION ERROR");
			break;
		case CAESAR_MEMORY_SHORTAGE_SOLVE_1:
			printf("NOT ENOUGH MEMORY");
			break;
		case CAESAR_RECURSIVE_BLOCK_SOLVE_1:
			printf("I THINK I'M GETTING RECURSIVE");
			break;
		default:
			printf("NOT CONTEMPLED (CHECK CAESAR_SOLVE)");
		}

		/* Return: something bad has happened */

		return CAESAR_FALSE;

		break;
	}

	CAESAR_BEGIN_DEBUG(1)
	{
		printf("<< /DEBUGGING >>\n");
	}CAESAR_END_DEBUG;

	/* Check the answer of the BES and return its value */

	if (ANSWER)
	{
		*SOLUTION_EXISTS = CAESAR_TRUE;
		printf("TRUE\n");
	}
	else
	{
		*SOLUTION_EXISTS = CAESAR_FALSE;
		printf("FALSE\n");
	}

	/* Return: everything is ok */

	return CAESAR_TRUE;
}

CAESAR_TYPE_BOOLEAN CAESAR_ITERATE_SOLUTION_DATALOG_1(
		CAESAR_TYPE_SOLVE_1 DATALOG_BES)
{
	CAESAR_ERROR("Function CAESAR_ITERATE_SOLUTION_DATALOG_1 not implemented");
	return CAESAR_FALSE;
}

CAESAR_TYPE_BOOLEAN CAESAR_WRITE_SOLUTION_DATALOG_1(
		CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_FILE FILE)
{
	CAESAR_TYPE_NATURAL relation_count;
	relation_count = CAESAR_DATALOG_GET_RELATION_COUNT();

	CAESAR_TYPE_NATURAL *domain_counter;
	CAESAR_TYPE_NATURAL domain_count;

	domain_count = DOMAIN_TABLE_ENTRY_COUNT();

	/* Allocate memory */

	CAESAR_CREATE(OUTPUT_FILES, relation_count * sizeof (CAESAR_TYPE_FILE), CAESAR_TYPE_FILE *);

	CAESAR_CREATE(OUTPUT_FILES_NAMES, relation_count * sizeof (CAESAR_TYPE_STRING), CAESAR_TYPE_STRING *);

	CAESAR_CREATE(domain_counter, domain_count * sizeof (CAESAR_TYPE_NATURAL), CAESAR_TYPE_NATURAL *);

	/***************************************************************/
	/**/
	/***************************************************************/

	if (FILE != NULL)
	{
		CAESAR_ERROR("Function not implemented in CAESAR_WRITE_SOLUTION_DATALOG_1");
		return CAESAR_FALSE;
	}

	/***************************************************************/
	/**/
	/***************************************************************/

	CAESAR_TYPE_STRING FILENAME;
	CAESAR_TYPE_NATURAL ENCODING_BITS;
	CAESAR_TYPE_NATURAL i;
	CAESAR_TYPE_NATURAL k;
	CAESAR_TYPE_NATURAL output_relations_number;

	CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
	CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
	RELATION = &RELATION_BODY;

	CAESAR_BODY_DOMAIN_TABLE_ENTRY DOMAIN_BODY;
	CAESAR_TYPE_DOMAIN_TABLE_ENTRY DOMAIN;
	DOMAIN = &DOMAIN_BODY;

	output_relations_number = 0;

	for (i = 0; i < relation_count; i++)
	{

		/* Get relation info */

		CAESAR_DATALOG_GET_RELATION_BY_INDEX(i, RELATION);

		/* If it is an output relation we open its file */

		if (RELATION->FACTS == NULL)
		{

			++output_relations_number;

			/* Initialize domain counter */

			for (k = 0; k < domain_count; k++)
				domain_counter[k] = 0;

			/* Allocate memory and build the new filename */

			CAESAR_CREATE(FILENAME, strlen(RELATION->NAME) + strlen(DATALOG_SOLVE_OUTPUT_TUPLES_SUFIX) + 1, CAESAR_TYPE_STRING);
			strcpy(FILENAME, RELATION->NAME);
			strcat(FILENAME, DATALOG_SOLVE_OUTPUT_TUPLES_SUFIX);

			/* Open the new output file */

			if ((OUTPUT_FILES[i] = fopen(FILENAME, "w")) == NULL)
			{
				CAESAR_ERROR(
						"File ``%s'' (output tuples) is imposible to create",
						FILENAME);
			}

			/* Write the header of the output tuples file */

			fprintf(OUTPUT_FILES[i], "# ");

			for (k = 0; k < RELATION->ARITY; k++)
			{
				if (k != 0)
					fprintf(OUTPUT_FILES[i], " ");
				DOMAIN_TABLE_GET_ENTRY_BY_INDEX(RELATION->DOMAINS[k], DOMAIN);

				ENCODING_BITS
						= CAESAR_BITS_TO_ENCODE_ELEMENTS(DOMAIN->CARDINAL);

				fprintf(OUTPUT_FILES[i], "%s%d:%d", DOMAIN->NAME,
						domain_counter[k], ENCODING_BITS);
				domain_counter[k]++;
			}

			fprintf(OUTPUT_FILES[i], "\n");

			OUTPUT_FILES_NAMES[i] = FILENAME;

			/* TODO: At the end of the method Deallocate memory */

			/* CAESAR_DELETE(FILENAME); */

		}
		else
		{

			OUTPUT_FILES[i] = NULL;

			OUTPUT_FILES_NAMES[i] = NULL;

		}
	}

	/***************************************************************/
	/**/
	/***************************************************************/

	CAESAR_TYPE_BOOLEAN_VARIABLE X0;
	CAESAR_TYPE_BOOLEAN VALUE;

	/* Build the initial variable */

	X0.BLOCK_INDEX = 0;
	X0.TYPE = 0;
	X0.ENTRY_INDEX = 0;

	CAESAR_ITERATE_STABLE_VARIABLE_SOLVE_1(DATALOG_BES, 2,
			(CAESAR_TYPE_POINTER) &X0, &VALUE,
			STABLE_VARIABLE_ITERATE_IN_OUTPUT_RELATION_MODE);

	/***************************************************************/
	/**/
	/***************************************************************/

	/* Close file descriptors */

	for (i = 0; i < relation_count; i++)
	{

		if (OUTPUT_FILES[i] != NULL)
			fclose(OUTPUT_FILES[i]);

	}

	/* Print messages to the output informing of the solution */

	if (output_relations_number == 0)
		return CAESAR_FALSE; /* TODOD: CHECK WHERE TO PUT THIS PROPERLY*/

	fprintf(stdout, "(consult solution in file%s ",
			output_relations_number > 1 ? "s" : "");

	for (i = 0; i < relation_count; i++)
	{
		if (OUTPUT_FILES_NAMES[i] != NULL)
		{
			--output_relations_number;
			fprintf(stdout, "``%s''%s", OUTPUT_FILES_NAMES[i],
					output_relations_number != 0 ? ", " : ")\n");
		}
	}

	/* Free memory */

	CAESAR_DELETE(OUTPUT_FILES);

	for (i = 0; i < relation_count; i++)
	{

		if (OUTPUT_FILES_NAMES[i] != NULL)
			CAESAR_DELETE(OUTPUT_FILES_NAMES[i]);

	}

	CAESAR_DELETE(OUTPUT_FILES_NAMES);

	CAESAR_DELETE(domain_counter);

	/* Return: everything is ok */

	return CAESAR_TRUE;
}

/*------------------------------------------------------------------------------*/
/* Write the BES explored - during the resolution - into a file */
void CAESAR_WRITE_BES_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES,
		CAESAR_TYPE_FILE FILE)
{
	CAESAR_TYPE_BOOLEAN_VARIABLE X0;

	/* Build the initial variable */

	X0.BLOCK_INDEX = 0;
	X0.TYPE = 0;
	X0.ENTRY_INDEX = 0;

	CAESAR_WRITE_SOLVE_1(DATALOG_BES, 0, (CAESAR_TYPE_POINTER) &X0, FILE, 
	DATALOG_SOLVE_DIAGNOSTIC_BES);
}

/*------------------------------------------------------------------------------*/
/* Write statistical information about the BES resolution into a file */
void CAESAR_WRITE_STATISTICS_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES,
		CAESAR_TYPE_FILE FILE)
{
	fprintf(FILE, "\n"
		"Statistical information of the underlying BES\n"
		"---------------------------------------------\n");

	CAESAR_FORMAT_SOLVE_1(DATALOG_BES, 1);
	CAESAR_PRINT_SOLVE_1(FILE, DATALOG_BES);

	fprintf(FILE, "\n");
}
