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

#ifndef CAESAR_DATALOG_COMMON_INTERFACE
#define CAESAR_DATALOG_COMMON_INTERFACE

#include <string.h>

#include "caesar_solve_1.h"
#include "caesar_area_1.h"
#include "caesar_table_1.h"
#include "caesar_hash.h"

// This file is in ../incl
#include "caesar_debug.h"

/*============================================================================*/
/* Constant definitions */

#define CAESAR_MAXIMUM_RELATION_ARITY 10

#define DATALOG_SOLVE_EXECUTABLE_NAME "datalog_solve"

#define DATALOG_SOLVE_DATALOG_EXTENSION ".datalog"

#define DATALOG_SOLVE_OUTPUT_TUPLES_SUFIX ".tuples"

#define DATALOG_SOLVE_BES_SUFIX ".bes"

/*----------------------------------------------------------------------------*/
/* datalog_solve options */

#define DATALOG_SOLVE_DIAGNOSTIC_BES CAESAR_TRUE

/*============================================================================*/
/* datalog_solve options */

extern CAESAR_TYPE_BOOLEAN caesar_output_tuples;

/*============================================================================*/
/* Macro definitions */

/* Nature of a predicate: input/output and format */
typedef enum CAESAR_ENUM_IONATURE
{	CAESAR_INPUT_TUPLES, CAESAR_OUTPUT_TUPLES, CAESAR_OUTPUT} CAESAR_TYPE_IONATURE;

/*============================================================================*/
/* Common functionality */

extern CAESAR_TYPE_STRING CAESAR_COPY_STRING(CAESAR_TYPE_STRING CAESAR_STRING);
extern CAESAR_TYPE_NATURAL CAESAR_BITS_TO_ENCODE_ELEMENTS(
		CAESAR_TYPE_NATURAL NUMBER_OF_ELEMENTS);
extern CAESAR_TYPE_NATURAL COUNT_NUMBER_OF_LINES(CAESAR_TYPE_FILE FILE);
extern CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ITERATE_OVER_COMBINATIONS(
		CAESAR_TYPE_NATURAL *COUNTERS, CAESAR_TYPE_NATURAL *LIMITS,
		CAESAR_TYPE_BOOLEAN *MASK, CAESAR_TYPE_NATURAL SIZE);

/*============================================================================*/
/* Debugging functions */

void DEBUG_CREATE_TABLE(CAESAR_TYPE_STRING NAME, CAESAR_TYPE_AREA_1 BASE,
		CAESAR_TYPE_AREA_1 MARK);

#endif
