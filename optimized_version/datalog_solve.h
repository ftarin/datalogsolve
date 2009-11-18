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

#ifndef CAESAR_DATALOG_READ_WRITE_INTERFACE
#define CAESAR_DATALOG_READ_WRITE_INTERFACE

#include <string.h>

#include "caesar_standard.h"
#include "caesar_solve_1.h"
#include "caesar_area_1.h"
#include "caesar_table_1.h"
#include "caesar_hash.h"

// This file is in ../incl
#include "caesar_debug.h"

/*============================================================================*/
/* Global variable definitions */

extern CAESAR_TYPE_BOOLEAN caesar_output_tuples;
extern CAESAR_TYPE_BOOLEAN caesar_output_bes;
extern CAESAR_TYPE_BOOLEAN caesar_output_statistics;
extern CAESAR_TYPE_STRING caesar_output_bes_file_name;


/*============================================================================*/
/* Constant definitions */

#define CAESAR_MAXIMUM_RELATION_ARITY 10

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

/* Variable Type */

typedef struct CAESAR_STRUCT_BOOLEAN_VARIABLE {
	/* Block index */
	CAESAR_TYPE_NATURAL BLOCK_INDEX :2;

	/* Variable type */
	CAESAR_TYPE_NATURAL TYPE :2;

	/* Index */
	CAESAR_TYPE_NATURAL ENTRY_INDEX :28;
} CAESAR_TYPE_BOOLEAN_VARIABLE;

/*============================================================================*/
/* Exported functions TODO */

extern CAESAR_TYPE_BOOLEAN CAESAR_READ_DATALOG_1(
		CAESAR_TYPE_SOLVE_1 *DATALOG_BES, CAESAR_TYPE_STRING DATALOG_FILENAME);
extern CAESAR_TYPE_BOOLEAN CAESAR_COMPUTE_DATALOG_1(
		CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_BOOLEAN *SOLUTION_EXISTS);
extern CAESAR_TYPE_BOOLEAN CAESAR_ITERATE_SOLUTION_DATALOG_1(
		CAESAR_TYPE_SOLVE_1 DATALOG_BES);
extern CAESAR_TYPE_BOOLEAN CAESAR_WRITE_SOLUTION_DATALOG_1(
		CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_FILE FILE);
extern void CAESAR_WRITE_BES_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES,
		CAESAR_TYPE_FILE FILE);
extern void CAESAR_WRITE_STATISTICS_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES,
		CAESAR_TYPE_FILE FILE);

extern void CAESAR_VARIABLE_PRINT(CAESAR_TYPE_FILE FILE,
		CAESAR_TYPE_POINTER VARIABLE);


#endif
