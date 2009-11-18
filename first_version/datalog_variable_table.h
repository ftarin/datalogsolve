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

#ifndef CAESAR_DATALOG_TABLES_INTERFACE
#define CAESAR_DATALOG_TABLES_INTERFACE

#include <string.h>

#include "caesar_standard.h"
#include "caesar_solve_1.h"
#include "caesar_area_1.h"
#include "caesar_table_1.h"
#include "caesar_hash.h"

// This file is in ../incl/
#include "caesar_debug.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

/*============================================================================*/
/* Exported functions */

void CAESAR_DATALOG_BUILD_VARIABLE_TABLE();
CAESAR_TYPE_INDEX_TABLE_1 CAESAR_DATALOG_GET_INDEX_BY_VARIABLE(CAESAR_TYPE_STRING VARIABLE);
void CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_STRING *ENTRY_POINTER);

#endif
