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

#ifndef CAESAR_DATALOG_FACT_TABLE_INTERFACE
#define CAESAR_DATALOG_FACT_TABLE_INTERFACE

#include "caesar_standard.h"
#include "caesar_table_1.h"

/*============================================================================*/
/* Constant definitions */

/*============================================================================*/
/* Macro definitions */

/*============================================================================*/
/* Types */

/*============================================================================*/
/* Exported functions */

void CAESAR_DATALOG_BUILD_FACT_TABLE(CAESAR_TYPE_TABLE_1 *,
		CAESAR_TYPE_NATURAL, CAESAR_TYPE_STRING);
CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_IS_FACT(CAESAR_TYPE_TABLE_1 FACT_TABLE,
		CAESAR_TYPE_POINTER FACT_TABLE_ENTRY);
CAESAR_TYPE_NATURAL FACT_TABLE_GET_ENTRY_COUNT(CAESAR_TYPE_TABLE_1 FACT_TABLE);

#endif
