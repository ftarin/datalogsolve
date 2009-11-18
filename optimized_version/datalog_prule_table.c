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

#include "datalog_prule_table.h"
#include "datalog_rule_table.h"
#include "datalog_predicate_table.h"

/*============================================================================*/
/* Types */

/*==============================================================================*/
/* Global variables */

/* TODO: Avoid passing this global table as a parameter. Checking staticness. */
CAESAR_TYPE_TABLE_1 PRULE_TABLE;

/*==============================================================================*/
/* cpredicate functions */

/*------------------------------------------------------------------------------*/
/* Construction of the cquery table */

void CAESAR_DATALOG_BUILD_PRULE_TABLE()
{
     CAESAR_TYPE_TABLE_1 AUX_PRULE_TABLE;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_PRULE_TABLE_ENTRY));
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     CAESAR_CREATE_TABLE_1(
	  &AUX_PRULE_TABLE,
	  BASE_AREA,
	  MARK_AREA,
	  0, /* LIMIT_SIZE by default */
	  0, /* HASH_SIZE by default */
	  CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	  NULL, /* COMPARE_FUNCTION */
	  NULL, /* HASH_FUNCTION */ /* TODO: PUT THE CAESAR_7_HASH */
	  NULL, /* PRINT_FUNCTION */
	  CAESAR_OVERFLOW_ABORT_TABLE_1 /* OVERFLOW_FUNCTION */
	  );

     PRULE_TABLE = AUX_PRULE_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Addition of an entry to the cpredicate table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_PRULE_TABLE_ENTRY(CAESAR_TYPE_PRULE_TABLE_ENTRY PRULE, CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
     CAESAR_TYPE_PRULE_TABLE_ENTRY PRULE_ENTRY;
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;

     PRULE_ENTRY = (CAESAR_TYPE_PRULE_TABLE_ENTRY) CAESAR_PUT_BASE_TABLE_1(PRULE_TABLE);
     PRULE_ENTRY->CPREDICATE_INDEX = PRULE->CPREDICATE_INDEX;
     PRULE_ENTRY->RULE_INDEX = PRULE->RULE_INDEX;

     return !CAESAR_SEARCH_AND_PUT_TABLE_1(PRULE_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Get a prule entry by index */

void CAESAR_DATALOG_GET_PRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_PRULE_TABLE_ENTRY *ENTRY_POINTER)
{
     CAESAR_RETRIEVE_I_B_TABLE_1(
	  PRULE_TABLE,
	  INDEX,
	  (CAESAR_TYPE_POINTER *) ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Get a prule entry by index */

void CAESAR_DATALOG_GET_PRULE_BY_INDEX_NEW(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_PRULE_TABLE_ENTRY ENTRY)
{
     CAESAR_TYPE_POINTER BASE;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  PRULE_TABLE,
	  INDEX,
	  &BASE);

     ENTRY->CPREDICATE_INDEX = ((CAESAR_TYPE_PRULE_TABLE_ENTRY) BASE)->CPREDICATE_INDEX;
     ENTRY->RULE_INDEX = ((CAESAR_TYPE_PRULE_TABLE_ENTRY) BASE)->RULE_INDEX;
}

/*------------------------------------------------------------------------------*/
/* Prule the prule table */

void CAESAR_DATALOG_PRINT_PRULE_TABLE()
{
     CAESAR_FORMAT_TABLE_1(PRULE_TABLE, 2);
     CAESAR_PRINT_TABLE_1(stdout, PRULE_TABLE);
}

/*------------------------------------------------------------------------------*/
/* Printing of a X4 variable */

void CAESAR_DATALOG_PRINT_PRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
     CAESAR_BODY_PRULE_TABLE_ENTRY PRULE_BODY;
     CAESAR_TYPE_PRULE_TABLE_ENTRY PRULE;
     PRULE = &PRULE_BODY;

     CAESAR_BODY_PREDICATE_TABLE_ENTRY CPREDICATE_BODY;
     CAESAR_TYPE_PREDICATE_TABLE_ENTRY CPREDICATE;
     CPREDICATE = &CPREDICATE_BODY;

     CAESAR_DATALOG_GET_PRULE_BY_INDEX_NEW(INDEX, PRULE);
     PREDICATE_TABLE_GET_ENTRY_BY_INDEX(PRULE->CPREDICATE_INDEX, CPREDICATE);

     printf("X4_");

     CAESAR_DATALOG_PRINT_RULE(PRULE->RULE_INDEX);

     printf("{");
     COMBINATION_PRINT(CPREDICATE->RELATION, CPREDICATE->COMBINATION);
     printf("}");

}
