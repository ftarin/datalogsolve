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

#include "datalog_cpredicate_table.h"
#include "datalog_relation_table.h"

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_CPREDICATE_TABLE_BASE {
     /* Relation index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION;

     /* Combination index */
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION;
} CAESAR_BODY_CPREDICATE_TABLE_BASE, *CAESAR_TYPE_CPREDICATE_TABLE_BASE;

/*==============================================================================*/
/* Global variables */

/* TODO: Avoid passing this global table as a parameter. Checking staticness. */
CAESAR_TYPE_TABLE_1 CPREDICATE_TABLE;


/*==============================================================================*/
/* cpredicate functions */

/*------------------------------------------------------------------------------*/
/* Construction of the cquery table */

void CAESAR_DATALOG_BUILD_CPREDICATE_TABLE()
{
     CAESAR_TYPE_TABLE_1 AUX_CPREDICATE_TABLE;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1) * 2);
     MARK_AREA = CAESAR_NATURAL_AREA_1(1);

     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("CPREDICATE", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;

     CAESAR_CREATE_TABLE_1(
	  &AUX_CPREDICATE_TABLE,
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
     
     CPREDICATE_TABLE = AUX_CPREDICATE_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Addition of an entry to the cpredicate table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_CPREDICATE_TABLE_ENTRY(CAESAR_TYPE_CPREDICATE_TABLE_ENTRY CPREDICATE, CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
     CAESAR_TYPE_CPREDICATE_TABLE_ENTRY CPREDICATE_ENTRY;
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;

     CPREDICATE_ENTRY = (CAESAR_TYPE_CPREDICATE_TABLE_ENTRY) CAESAR_PUT_BASE_TABLE_1(CPREDICATE_TABLE);
     CPREDICATE_ENTRY->RELATION = CPREDICATE->RELATION;
     CPREDICATE_ENTRY->COMBINATION = CPREDICATE->COMBINATION;

     return !CAESAR_SEARCH_AND_PUT_TABLE_1(CPREDICATE_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Get a cpredicate entry by index */

void CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CPREDICATE_TABLE_ENTRY ENTRY)
{
     CAESAR_TYPE_POINTER BASE;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  CPREDICATE_TABLE,
	  INDEX,
	  &BASE);

     ENTRY->RELATION = ((CAESAR_TYPE_CPREDICATE_TABLE_BASE) BASE)->RELATION;
     ENTRY->COMBINATION = ((CAESAR_TYPE_CPREDICATE_TABLE_BASE) BASE)->COMBINATION;
}

/*------------------------------------------------------------------------------*/
/* Get a cpredicate entry by index */

void CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX_2(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CPREDICATE_TABLE_ENTRY *ENTRY_POINTER)
{
     CAESAR_RETRIEVE_I_B_TABLE_1(
	  CPREDICATE_TABLE,
	  INDEX,
	  (CAESAR_TYPE_POINTER *) ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Printing of a X3 variable */

void CAESAR_DATALOG_PRINT_COMBINATION(CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX, CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX);

void CAESAR_DATALOG_PRINT_CPREDICATE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
     CAESAR_BODY_CPREDICATE_TABLE_ENTRY CPREDICATE_BODY;
     CAESAR_TYPE_CPREDICATE_TABLE_ENTRY CPREDICATE;
     CPREDICATE = &CPREDICATE_BODY;

     CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX(INDEX, CPREDICATE);
     
     printf("X3_");
     
     CAESAR_DATALOG_PRINT_COMBINATION(CPREDICATE->RELATION, CPREDICATE->COMBINATION);

}
