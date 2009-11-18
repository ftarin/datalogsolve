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

#ifndef CAESAR_DATALOG_1_INTERFACE
#define CAESAR_DATALOG_1_INTERFACE

#include "caesar_standard.h"
#include "caesar_solve_1.h"

/*============================================================================*/
/* Exported functions by the CAESAR_DATALOG_1 library */

void CAESAR_DATALOG_READ(
			 CAESAR_TYPE_SOLVE_1 *DATALOG_BES,
			 CAESAR_TYPE_STRING DATALOG_FILENAME
			 );

#endif
