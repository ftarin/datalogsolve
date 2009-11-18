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

#include "datalog_combination_table.h"_
#include "datalog_crule_table.h"
#include "datalog_rule_table.h"
#include "datalog_predicate_table.h"

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_CRULE_TABLE_BASE {
     /* rule index */
     CAESAR_TYPE_INDEX_TABLE_1 RULE_INDEX;

     /* crule index */
     CAESAR_TYPE_INDEX_TABLE_1 CRULE_INDEX;
} CAESAR_BODY_CRULE_TABLE_BASE, *CAESAR_TYPE_CRULE_TABLE_BASE;

/*==============================================================================*/
/* Global variables */

/* crule table */
/* TODO: Avoid passing this global table as a parameter. Checking staticness. */
static CAESAR_TYPE_TABLE_1 CRULE_TABLE;

/*------------------------------------------------------------------------------*/
/* Construction of the i-cquery table */

void CAESAR_DATALOG_BUILD_CRULE_I_TABLE(CAESAR_TYPE_TABLE_1 *CRULE_I_TABLE_POINTER, CAESAR_TYPE_NATURAL RULE_LENGTH)
{
     CAESAR_TYPE_TABLE_1 CRULE_I_TABLE;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1) * RULE_LENGTH);
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     /*==============================*/
     /* Build a new empty rule table */
     /*==============================*/
     
     CAESAR_CREATE_TABLE_1(
	  &CRULE_I_TABLE,
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

     *CRULE_I_TABLE_POINTER = CRULE_I_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Construction of the cquery table */

void CAESAR_DATALOG_BUILD_CRULE_TABLE()
{
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;
     CAESAR_TYPE_TABLE_1 AUX_CRULE_TABLE;
     
     BASE_AREA = CAESAR_BYTE_AREA_1(2 * sizeof (CAESAR_TYPE_INDEX_TABLE_1));
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     /**************************/
     /* Create the crule table */
     /**************************/

     CAESAR_CREATE_TABLE_1(
	  &AUX_CRULE_TABLE,
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

     CRULE_TABLE = AUX_CRULE_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Addition of an entry to the crule table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_CRULE_TABLE_ENTRY(CAESAR_TYPE_CRULE_TABLE_ENTRY CRULE, CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_POINTER BASE;
     CAESAR_TYPE_INDEX_TABLE_1 *AUX_BASE;
     CAESAR_TYPE_INDEX_TABLE_1 CRULE_I_INDEX;
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;
     CAESAR_TYPE_CRULE_TABLE_BASE CRULE_BASE;

     /* Search in the rule table the rule and get the crule_i table
      * pointer and the length */
     CAESAR_BODY_RULE_TABLE_ENTRY RULE_BODY;
     CAESAR_TYPE_RULE_TABLE_ENTRY RULE;

     RULE = &RULE_BODY;

     CAESAR_DATALOG_GET_RULE_BY_INDEX(CRULE->RULE_INDEX, RULE);

     /* Add the cpredicates to the crule_i table getting the index of
      * the new entry*/

     BASE = CAESAR_PUT_BASE_TABLE_1(RULE->CRULE_TABLE);
     AUX_BASE = (CAESAR_TYPE_INDEX_TABLE_1 *) BASE;

     for (i = 0; i < RULE->LENGTH; i++) {
	  
	  *AUX_BASE = CRULE->CPREDICATE_INDICES[i];
	  AUX_BASE++;

     }

     /* Add the crule with the crule_i index and rule index in the
      * crule_table */

     CAESAR_SEARCH_AND_PUT_TABLE_1(RULE->CRULE_TABLE, &CRULE_I_INDEX, &EXISTING_ENTRY_POINTER);

     CRULE_BASE = (CAESAR_TYPE_CRULE_TABLE_BASE) CAESAR_PUT_BASE_TABLE_1(CRULE_TABLE);
     CRULE_BASE->RULE_INDEX = CRULE->RULE_INDEX;
     CRULE_BASE->CRULE_INDEX = CRULE_I_INDEX;

     return !CAESAR_SEARCH_AND_PUT_TABLE_1(CRULE_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
     /* Return the crule index*/
}

void CAESAR_DATALOG_GET_CRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CRULE_TABLE_ENTRY ENTRY)
{
     /* Search by index and rebuild (destroy and recreate the vector)
      * in the data structure because it cannot
      * be built without information... */

     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_CRULE_TABLE_BASE CRULE_BASE;
     CAESAR_TYPE_POINTER CRULE_I_BASE;
     CAESAR_TYPE_INDEX_TABLE_1 *AUX_BASE;

     CAESAR_BODY_RULE_TABLE_ENTRY RL_ENTRY_BODY;
     CAESAR_TYPE_RULE_TABLE_ENTRY RL_ENTRY;
     RL_ENTRY = &RL_ENTRY_BODY;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  CRULE_TABLE,
	  INDEX,
	  (CAESAR_TYPE_POINTER *) &CRULE_BASE
	  );

     CAESAR_DATALOG_GET_RULE_BY_INDEX(CRULE_BASE->RULE_INDEX, RL_ENTRY);
     
     CAESAR_RETRIEVE_I_B_TABLE_1(
	  RL_ENTRY->CRULE_TABLE,
	  CRULE_BASE->CRULE_INDEX,
	  (CAESAR_TYPE_POINTER *) &CRULE_I_BASE
	  );

     /* Fill the crule entry */

     AUX_BASE = (CAESAR_TYPE_INDEX_TABLE_1 *) CRULE_I_BASE;

     ENTRY->RULE_INDEX = CRULE_BASE->RULE_INDEX;

     CAESAR_CREATE(
	  ENTRY->CPREDICATE_INDICES,
	  RL_ENTRY->LENGTH * sizeof (CAESAR_TYPE_INDEX_TABLE_1),
	  CAESAR_TYPE_INDEX_TABLE_1 *
	  );
     for (i = 0; i < RL_ENTRY->LENGTH; i++) {
	  ENTRY->CPREDICATE_INDICES[i] = AUX_BASE[i];
     }
}

/*------------------------------------------------------------------------------*/
/* Printing of a X5 variable */

void CAESAR_DATALOG_PRINT_CRULE_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
     CAESAR_BODY_CRULE_TABLE_ENTRY CRULE_BODY;
     CAESAR_TYPE_CRULE_TABLE_ENTRY CRULE;
     CRULE = &CRULE_BODY;

     CAESAR_BODY_PREDICATE_TABLE_ENTRY CPREDICATE_BODY;
     CAESAR_TYPE_PREDICATE_TABLE_ENTRY CPREDICATE;
     CPREDICATE = &CPREDICATE_BODY;

     CAESAR_BODY_RULE_TABLE_ENTRY RULE_BODY;
     CAESAR_TYPE_RULE_TABLE_ENTRY RULE;
     RULE = &RULE_BODY;

     CAESAR_TYPE_NATURAL i;

     printf("X5_:-");

     CAESAR_DATALOG_GET_CRULE_BY_INDEX(INDEX, CRULE);
     CAESAR_DATALOG_GET_RULE_BY_INDEX(CRULE->RULE_INDEX, RULE);

     i = 0;
     if (i < RULE->LENGTH) {
	  PREDICATE_TABLE_GET_ENTRY_BY_INDEX(CRULE->CPREDICATE_INDICES[i], CPREDICATE);
	  COMBINATION_PRINT(CPREDICATE->RELATION, CPREDICATE->COMBINATION);
     }
	  
     for (i++; i < RULE->LENGTH; i++) {
	  printf(",");
	  PREDICATE_TABLE_GET_ENTRY_BY_INDEX(CRULE->CPREDICATE_INDICES[i], CPREDICATE);
	  COMBINATION_PRINT(CPREDICATE->RELATION, CPREDICATE->COMBINATION);
     }
}
