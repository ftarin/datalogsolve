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

#include "datalog_rule_table.h"
#include "datalog_crule_table.h"
#include "datalog_variable_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_relation_table.h"


/* TODO: Remove when a combination.h file is created */
extern CAESAR_TYPE_NATURAL CAESAR_DATALOG_COMBINATION_TABLE_ENTRY_SIZE(CAESAR_TYPE_NATURAL ARITY);


/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_RULE_TABLE_BASE {
     /* Rule address */
     CAESAR_TYPE_RULE RULE;
} CAESAR_BODY_RULE_TABLE_BASE, *CAESAR_TYPE_RULE_TABLE_BASE;

typedef struct CAESAR_STRUCT_RULE_TABLE_MARK {
     /* Size of the rule tail */
     CAESAR_TYPE_NATURAL LENGTH;

     /* Relation head  index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;

     /* crule table */
     CAESAR_TYPE_TABLE_1 CRULE_TABLE;

     /* List of maps for free variables */
     CAESAR_TYPE_VARIABLE_MAP_LIST FREE_VARIABLE_MAP_LIST;

     CAESAR_TYPE_VARIABLE_MAP *FREE_VARIABLE_MAP_ARRAY;

     CAESAR_TYPE_NATURAL FREE_VARIABLE_NUMBER;

     /* List of maps for linked variables */
     CAESAR_TYPE_VARIABLE_MAP_LIST LINKED_VARIABLE_MAP_LIST;

     CAESAR_TYPE_POINTER TAIL_TO_CONCRETIZE;

     CAESAR_TYPE_NATURAL *TAIL_TO_CONCRETIZE_DISPLAY;

     CAESAR_TYPE_INDEX_TABLE_1 *TAIL_TO_CONCRETIZE_RELATION_INDICES;
} CAESAR_BODY_RULE_TABLE_MARK, *CAESAR_TYPE_RULE_TABLE_MARK;

typedef struct CAESAR_STRUCT_RULE_I_TABLE_BASE {
     /* rule index */
     CAESAR_TYPE_INDEX_TABLE_1 RULE_INDEX;
} CAESAR_BODY_RULE_I_TABLE_BASE, *CAESAR_TYPE_RULE_I_TABLE_BASE;


/*==============================================================================*/
/* Global variables */

/* Global rule table */
static CAESAR_TYPE_TABLE_1 RULE_TABLE;

/*==============================================================================*/
/* Rule Table Management */

/*------------------------------------------------------------------------------*/
/* Construction of the rule table */

void CAESAR_DATALOG_BUILD_RULE_TABLE()
{
     CAESAR_TYPE_TABLE_1 RULE_TABLE_AUX;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_RULE_TABLE_BASE));
     MARK_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_RULE_TABLE_MARK));

     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("RULE", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;

     /*==============================*/
     /* Build a new empty rule table */
     /*==============================*/
     
     CAESAR_CREATE_TABLE_1(
	  &RULE_TABLE_AUX,
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

     /*=============================*/
     /* Return the built rule table */
     /*=============================*/
     
     RULE_TABLE = RULE_TABLE_AUX;

     /*==================================================*/
     /* Search the rules applied to the current relation */
     /*==================================================*/

     CAESAR_TYPE_RULE_LIST RLCELL;
     CAESAR_TYPE_INDEX_TABLE_1 INDEX;

     CAESAR_BODY_RELATION_TABLE_ENTRY R_ENTRY_BODY;
     CAESAR_TYPE_RELATION_TABLE_ENTRY R_ENTRY;
     R_ENTRY = &R_ENTRY_BODY;

     CAESAR_TYPE_INDEX_TABLE_1 RULE_I_INDEX;
     CAESAR_BODY_RULE_TABLE_ENTRY RULE_ENTRY;
     CAESAR_BODY_RULE_I_TABLE_ENTRY RULE_I;
     
     for (RLCELL = CAESAR_DATALOG_RULE_LIST; RLCELL != NULL; RLCELL = RLCELL->NEXT) {
	  
	  INDEX = CAESAR_DATALOG_GET_RELATION_INDEX_BY_NAME(RLCELL->RULE->HEAD->RELATION->NAME);

	  CAESAR_DATALOG_GET_RELATION_BY_INDEX(INDEX, R_ENTRY);
	  
	  RULE_ENTRY.RULE = RLCELL->RULE;
	  RULE_ENTRY.LENGTH = CAESAR_LITERAL_LIST_LENGTH(RLCELL->RULE->TAIL);
	  RULE_ENTRY.RELATION_INDEX = INDEX;
	  CAESAR_DATALOG_BUILD_CRULE_I_TABLE(&(RULE_ENTRY.CRULE_TABLE), RULE_ENTRY.LENGTH);
	  CAESAR_DATALOG_ADD_RULE_TABLE_ENTRY(&RULE_ENTRY, &(RULE_I.RULE_INDEX));
	  CAESAR_DATALOG_ADD_RULE_I_TABLE_ENTRY(R_ENTRY->RULES, &RULE_I, &RULE_I_INDEX);
	  
     }

}

/*------------------------------------------------------------------------------*/
/* Add an entry to the rule table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_RULE_TABLE_ENTRY(CAESAR_TYPE_RULE_TABLE_ENTRY RULE, CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
     CAESAR_TYPE_RULE_TABLE_BASE RULE_BASE;
     CAESAR_TYPE_RULE_TABLE_MARK RULE_MARK;
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;

     RULE_BASE = (CAESAR_TYPE_RULE_TABLE_BASE) CAESAR_PUT_BASE_TABLE_1(RULE_TABLE);
     RULE_BASE->RULE = RULE->RULE;

     RULE_MARK = (CAESAR_TYPE_RULE_TABLE_MARK) CAESAR_PUT_MARK_TABLE_1(RULE_TABLE);
     RULE_MARK->LENGTH = RULE->LENGTH;
     RULE_MARK->RELATION_INDEX = RULE->RELATION_INDEX;
     RULE_MARK->CRULE_TABLE = RULE->CRULE_TABLE;

     /* CALCULATE THE FREE AND LINKED VARIABLES */

     /* FIRST WE CALCULATE THOSE OF THE HEAD */

     CAESAR_TYPE_RULE_VARIABLE_LIST FREE_VARIABLES;
     CAESAR_TYPE_RULE_VARIABLE_LIST LINKED_VARIABLES;
     CAESAR_TYPE_LITERAL_LIST LCELL;
     CAESAR_TYPE_RULE_VARIABLE_LIST VCELL2;
     CAESAR_TYPE_RULE_VARIABLE_LIST VCELL;
     CAESAR_TYPE_NATURAL ARGUMENT_INDEX;
     CAESAR_TYPE_NATURAL LITERAL_INDEX;
     CAESAR_TYPE_NATURAL TAIL_TO_CONCRETIZE_SIZE;
     CAESAR_TYPE_NATURAL *TAIL_TO_CONCRETIZE_DISPLAY;
     CAESAR_TYPE_POINTER TAIL_TO_CONCRETIZE;
     CAESAR_TYPE_INDEX_TABLE_1 *TAIL_TO_CONCRETIZE_RELATION_INDICES;
     CAESAR_TYPE_NATURAL FREE_VARIABLE_NUMBER;
     
     FREE_VARIABLES = NULL;
     LINKED_VARIABLES = NULL;

     /**********************************************/
     /* Find the linked variables of the rule tail */
     /**********************************************/

     for (ARGUMENT_INDEX = 0, VCELL2 = RULE->RULE->HEAD->RULE_VARIABLE_LIST;
	  VCELL2 != NULL;
	  ARGUMENT_INDEX++, VCELL2 = VCELL2->NEXT) {

	  LINKED_VARIABLES = CAESAR_CONSTRUCT_RULE_VARIABLE_LIST(VCELL2->RULE_VARIABLE, LINKED_VARIABLES);
	  

     }

     /*********************************************************************/
     /* Find the free variables and calculate properties of the rule tail */
     /*********************************************************************/

     /* Create the concretized tail display */
     CAESAR_CREATE(TAIL_TO_CONCRETIZE_DISPLAY, RULE->LENGTH * sizeof(CAESAR_TYPE_NATURAL), CAESAR_TYPE_NATURAL *);

     /* Create the concretized tail relation indexes */
     CAESAR_CREATE(TAIL_TO_CONCRETIZE_RELATION_INDICES, RULE->LENGTH * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);

     /* Initialize concretized tail size acumulator */
     TAIL_TO_CONCRETIZE_SIZE = 0;

     /* Initialize the free variable counter */
     FREE_VARIABLE_NUMBER = 0;

     /*---------------------------------------------------------------------*/
     /* Add the variable to the free variables if is not in the linked ones */
     /*---------------------------------------------------------------------*/
     
/* TODO: think it's repeated      CAESAR_CREATE(TAIL_TO_CONCRETIZE_DISPLAY, RULE->LENGTH * sizeof(CAESAR_TYPE_NATURAL), CAESAR_TYPE_NATURAL *); */

     RULE_MARK->RELATION_INDEX = RULE->RELATION_INDEX;
          
     for (LITERAL_INDEX = 0, LCELL = RULE->RULE->TAIL;
	  LCELL != NULL;
	  LITERAL_INDEX++, LCELL = LCELL->NEXT) {

	  /*=======================================================*/
	  /* Set the tail relation indexes for the current literal */
	  /*=======================================================*/

	  CAESAR_BEGIN_DEBUG(4) {
	       
	       CAESAR_DATALOG_PRINT_RELATION_TABLE();
	       
	  } CAESAR_END_DEBUG;

	  TAIL_TO_CONCRETIZE_RELATION_INDICES[LITERAL_INDEX] = CAESAR_DATALOG_GET_RELATION_INDEX_BY_NAME(LCELL->LITERAL->RELATION->NAME);
	  
	  /*==============================================*/
	  /* Set the tail display for the current literal */
	  /*==============================================*/

	  TAIL_TO_CONCRETIZE_DISPLAY[LITERAL_INDEX] = TAIL_TO_CONCRETIZE_SIZE;
	  
	  /*===============================================*/
	  /* Set the tail length until the current literal */
	  /*===============================================*/

	  TAIL_TO_CONCRETIZE_SIZE += CAESAR_DATALOG_COMBINATION_TABLE_ENTRY_SIZE(LCELL->LITERAL->RELATION->ARITY);

	  /*======================================================*/
	  /* Iterate through the arguments of the current literal */
	  /*======================================================*/	  

	  for (ARGUMENT_INDEX = 0, VCELL2 = LCELL->LITERAL->RULE_VARIABLE_LIST;
	       VCELL2 != NULL;
	       ARGUMENT_INDEX++, VCELL2 = VCELL2->NEXT) {

	       /*---------------------------------------------------------------------*/
	       /* Add the variable to the free variables if is not in the linked ones */
	       /*---------------------------------------------------------------------*/

	       if (CAESAR_RULE_VARIABLE_IN_LIST(VCELL2->RULE_VARIABLE, LINKED_VARIABLES) == CAESAR_FALSE && CAESAR_RULE_VARIABLE_IN_LIST(VCELL2->RULE_VARIABLE, FREE_VARIABLES) == CAESAR_FALSE) {

		    FREE_VARIABLES = CAESAR_CONSTRUCT_RULE_VARIABLE_LIST(VCELL2->RULE_VARIABLE, FREE_VARIABLES);
		    FREE_VARIABLE_NUMBER++;
		    
	       }		   

	  }

     }

     /****************************************************/
     /* Create the concretized tail pattern and store it */
     /****************************************************/

     CAESAR_CREATE(TAIL_TO_CONCRETIZE, TAIL_TO_CONCRETIZE_SIZE, CAESAR_TYPE_POINTER);

     RULE_MARK->TAIL_TO_CONCRETIZE = TAIL_TO_CONCRETIZE;

     /**************************************/
     /* Store the concretized tail display */
     /**************************************/

     RULE_MARK->TAIL_TO_CONCRETIZE_DISPLAY = TAIL_TO_CONCRETIZE_DISPLAY;

     /***********************************************/
     /* Store the concretized tail relation indices */
     /***********************************************/

     RULE_MARK->TAIL_TO_CONCRETIZE_RELATION_INDICES = TAIL_TO_CONCRETIZE_RELATION_INDICES;

     /**************************************/
     /* Store the number of free variables */
     /**************************************/

     RULE_MARK->FREE_VARIABLE_NUMBER = FREE_VARIABLE_NUMBER;

     LINKED_VARIABLES = CAESAR_REVERSE_RULE_VARIABLE_LIST(LINKED_VARIABLES);
     FREE_VARIABLES = CAESAR_REVERSE_RULE_VARIABLE_LIST(FREE_VARIABLES);

     CAESAR_TYPE_VARIABLE_POSITION HEAD_POSITION;
     CAESAR_TYPE_VARIABLE_POSITION TAIL_POSITION;
     CAESAR_TYPE_VARIABLE_POSITION_LIST TAIL_POSITION_LIST;

     CAESAR_TYPE_INDEX_TABLE_1 VINDEX;

     CAESAR_TYPE_VARIABLE_MAP MAP;
     CAESAR_TYPE_VARIABLE_MAP_LIST LV_MAP_LIST;
     CAESAR_TYPE_VARIABLE_MAP_LIST FV_MAP_LIST;

     LV_MAP_LIST = NULL;

     for (VCELL = LINKED_VARIABLES;
	  VCELL != NULL;
	  VCELL = VCELL->NEXT) {

	  TAIL_POSITION_LIST = NULL;

	  for (VCELL2 = RULE->RULE->HEAD->RULE_VARIABLE_LIST, ARGUMENT_INDEX = 0;
	       VCELL2 != NULL;
	       VCELL2 = VCELL2->NEXT, ARGUMENT_INDEX++) {
	       if (0 == strcmp(VCELL2->RULE_VARIABLE, VCELL->RULE_VARIABLE)) {
		    HEAD_POSITION = CAESAR_CREATE_VARIABLE_POSITION(
			 0, /* There is only one atom in the head */
			 ARGUMENT_INDEX
			 );
	       }
	  }
	  for (LCELL = RULE->RULE->TAIL, LITERAL_INDEX = 0;
	       LCELL != NULL;
	       LCELL = LCELL->NEXT, LITERAL_INDEX++) {
	       for (VCELL2 = LCELL->LITERAL->RULE_VARIABLE_LIST, ARGUMENT_INDEX = 0;
		    VCELL2 != NULL;
		    VCELL2 = VCELL2->NEXT, ARGUMENT_INDEX++) {
		    if (0 == strcmp(VCELL2->RULE_VARIABLE, VCELL->RULE_VARIABLE)) {
			 TAIL_POSITION = CAESAR_CREATE_VARIABLE_POSITION(
			      LITERAL_INDEX,
			      ARGUMENT_INDEX); 
			 TAIL_POSITION_LIST = CAESAR_CONSTRUCT_VARIABLE_POSITION_LIST(
			      TAIL_POSITION,
			      TAIL_POSITION_LIST
			      );
		    }
	       }
	  }
	  
	  /* We invert the list in order to have the elements in the correct order */
	  TAIL_POSITION_LIST = CAESAR_REVERSE_VARIABLE_POSITION_LIST(TAIL_POSITION_LIST);
	  
	  VINDEX = CAESAR_DATALOG_GET_INDEX_BY_VARIABLE(VCELL->RULE_VARIABLE);

	  MAP = CAESAR_CREATE_VARIABLE_MAP(
	       VINDEX, /* Variable index */
	       0, /* Not necessary */
	       HEAD_POSITION, /* Head position */
	       TAIL_POSITION_LIST /* Tail positions */
	       );
	  
	  LV_MAP_LIST = CAESAR_CONSTRUCT_VARIABLE_MAP_LIST(MAP, LV_MAP_LIST);
     }
     
     LV_MAP_LIST = CAESAR_REVERSE_VARIABLE_MAP_LIST(LV_MAP_LIST);

     CAESAR_BEGIN_DEBUG(4) {
	  
	  CAESAR_PRINT_VARIABLE_MAP_LIST(LV_MAP_LIST, "LINKED");
	  
     } CAESAR_END_DEBUG;	  

     RULE_MARK->LINKED_VARIABLE_MAP_LIST = LV_MAP_LIST;
     
     /* FREE VARIABLES*/

     CAESAR_TYPE_NATURAL j;
     CAESAR_TYPE_NATURAL k;
     CAESAR_TYPE_NATURAL CARDINAL;
     CAESAR_TYPE_DOMAIN_LIST DCELL;

     FV_MAP_LIST = NULL; /* Just in case there are no free variables */

     for (VCELL = FREE_VARIABLES;
	  VCELL != NULL;
	  VCELL = VCELL->NEXT) {

	  TAIL_POSITION_LIST = NULL;

	  for (VCELL2 = RULE->RULE->HEAD->RULE_VARIABLE_LIST, ARGUMENT_INDEX = 0;
	       VCELL2 != NULL;
	       VCELL2 = VCELL2->NEXT, ARGUMENT_INDEX++) {
	       if (0 == strcmp(VCELL2->RULE_VARIABLE, VCELL->RULE_VARIABLE)) {
		    HEAD_POSITION = CAESAR_CREATE_VARIABLE_POSITION(
			 0, /* There is only one atom in the head */
			 ARGUMENT_INDEX
			 );
	       }
	  }
	  for (LCELL = RULE->RULE->TAIL, LITERAL_INDEX = 0;
	       LCELL != NULL;
	       LCELL = LCELL->NEXT, LITERAL_INDEX++) {
	       for (VCELL2 = LCELL->LITERAL->RULE_VARIABLE_LIST, ARGUMENT_INDEX = 0;
		    VCELL2 != NULL;
		    VCELL2 = VCELL2->NEXT, ARGUMENT_INDEX++) {
		    if (0 == strcmp(VCELL2->RULE_VARIABLE, VCELL->RULE_VARIABLE)) {
			 TAIL_POSITION = CAESAR_CREATE_VARIABLE_POSITION(
			      LITERAL_INDEX,
			      ARGUMENT_INDEX); 
			 TAIL_POSITION_LIST = CAESAR_CONSTRUCT_VARIABLE_POSITION_LIST(
			      TAIL_POSITION,
			      TAIL_POSITION_LIST
			      );
		    }
	       }
	  }
	  
	  /* We invert the list in order to have the elements in the correct order */
	  TAIL_POSITION_LIST = CAESAR_REVERSE_VARIABLE_POSITION_LIST(TAIL_POSITION_LIST);
	
	  VINDEX = CAESAR_DATALOG_GET_INDEX_BY_VARIABLE(VCELL->RULE_VARIABLE);

	  /* We chose the domain of the first ocurrence of the
	   * variable as its global domain which should be ok if the
	   datalog program is correct */

	  for (LCELL = RULE->RULE->TAIL, j = 0;
	       LCELL != NULL && j <= TAIL_POSITION_LIST->POSITION->LITERAL_INDEX;
	       LCELL = LCELL->NEXT, j++) {

	       /* IMPROVE THIS CODE */

	       if (j == TAIL_POSITION_LIST->POSITION->LITERAL_INDEX) {

		    for (DCELL = LCELL->LITERAL->RELATION->DOMAIN_LIST, k = 0;
			 DCELL != NULL && k <= TAIL_POSITION_LIST->POSITION->ARGUMENT_INDEX;
			 DCELL = DCELL->NEXT, k++) {

			 if (k == TAIL_POSITION_LIST->POSITION->ARGUMENT_INDEX) {
			      
			      CARDINAL = DCELL->DOMAIN->CARDINAL;
			      
			 }

		    }

	       }

	  }
		   
	  MAP = CAESAR_CREATE_VARIABLE_MAP(
	       VINDEX, /* Variable index */
	       CARDINAL,
	       HEAD_POSITION, /* Head position */
	       TAIL_POSITION_LIST /* Tail positions */
	       );
	  
	  FV_MAP_LIST = CAESAR_CONSTRUCT_VARIABLE_MAP_LIST(MAP, FV_MAP_LIST);
     }
 
     /* NO TIENE NADA FVMAP-LIST */
    
     FV_MAP_LIST = CAESAR_REVERSE_VARIABLE_MAP_LIST(FV_MAP_LIST);
     
     CAESAR_BEGIN_DEBUG(4) {
	  
	  CAESAR_PRINT_VARIABLE_MAP_LIST(FV_MAP_LIST, "FREE");
	  
     } CAESAR_END_DEBUG;

     RULE_MARK->FREE_VARIABLE_MAP_LIST = FV_MAP_LIST;

     CAESAR_VARIABLE_MAP_LIST_TO_ARRAY(
	  RULE_MARK->FREE_VARIABLE_MAP_LIST,
	  &(RULE_MARK->FREE_VARIABLE_NUMBER),
	  &(RULE_MARK->FREE_VARIABLE_MAP_ARRAY)
	  );

     /* /CALCULATE THE FREE AND LINKED VARIABLES */

     return !CAESAR_SEARCH_AND_PUT_TABLE_1(RULE_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Get a rule entry by index */

void CAESAR_DATALOG_GET_RULE_BY_INDEX(
     CAESAR_TYPE_INDEX_TABLE_1 INDEX,
     CAESAR_TYPE_RULE_TABLE_ENTRY ENTRY
     )
{
     CAESAR_TYPE_RULE *BASE; 
     CAESAR_TYPE_RULE_TABLE_MARK MARK;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  RULE_TABLE,
	  INDEX,
	  (CAESAR_TYPE_POINTER *) &BASE);

     ENTRY->RULE = *BASE; /* TODO: CHECK THIS POINTERS */

     CAESAR_RETRIEVE_I_M_TABLE_1(
	  RULE_TABLE,
	  INDEX,
	  (CAESAR_TYPE_POINTER *) &MARK);

     ENTRY->LENGTH = MARK->LENGTH;
     ENTRY->RELATION_INDEX = MARK->RELATION_INDEX;
     ENTRY->CRULE_TABLE = MARK->CRULE_TABLE;
     ENTRY->FREE_VARIABLE_MAP_LIST = MARK->FREE_VARIABLE_MAP_LIST;
     ENTRY->FREE_VARIABLE_MAP_ARRAY = MARK->FREE_VARIABLE_MAP_ARRAY;
     ENTRY->FREE_VARIABLE_NUMBER = MARK->FREE_VARIABLE_NUMBER;
     ENTRY->LINKED_VARIABLE_MAP_LIST = MARK->LINKED_VARIABLE_MAP_LIST;
     ENTRY->TAIL_TO_CONCRETIZE = MARK->TAIL_TO_CONCRETIZE;
     ENTRY->TAIL_TO_CONCRETIZE_DISPLAY = MARK->TAIL_TO_CONCRETIZE_DISPLAY;
     ENTRY->TAIL_TO_CONCRETIZE_RELATION_INDICES = MARK->TAIL_TO_CONCRETIZE_RELATION_INDICES;

}

/*------------------------------------------------------------------------------*/
/* Construction of a rule_i table */

void CAESAR_DATALOG_BUILD_RULE_I_TABLE(CAESAR_TYPE_TABLE_1 *RULE_I_TABLE_POINTER)
{
     CAESAR_TYPE_TABLE_1 RULE_I_TABLE_AUX;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_RULE_I_TABLE_ENTRY));
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     /*==============================*/
     /* Build a new empty rule table */
     /*==============================*/
     
     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("RULE_I", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;

     CAESAR_CREATE_TABLE_1(
	  &RULE_I_TABLE_AUX,
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

     /*=============================*/
     /* Return the built rule table */
     /*=============================*/
     
     *RULE_I_TABLE_POINTER = RULE_I_TABLE_AUX;
}

/*------------------------------------------------------------------------------*/
/* Add an entry to the rule_i table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_RULE_I_TABLE_ENTRY(CAESAR_TYPE_TABLE_1 RULE_I_TABLE, CAESAR_TYPE_RULE_I_TABLE_ENTRY RULE_I, CAESAR_TYPE_INDEX_TABLE_1 *INDEX)
{
     CAESAR_TYPE_RULE_I_TABLE_BASE RULE_I_BASE;
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;

     RULE_I_BASE = (CAESAR_TYPE_RULE_I_TABLE_BASE) CAESAR_PUT_BASE_TABLE_1(RULE_I_TABLE);
     RULE_I_BASE->RULE_INDEX = RULE_I->RULE_INDEX;

     return !CAESAR_SEARCH_AND_PUT_TABLE_1(RULE_I_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
}

/*------------------------------------------------------------------------------*/
/* Get a rule entry by index */

void CAESAR_DATALOG_GET_RULE_I_BY_INDEX(
     CAESAR_TYPE_TABLE_1 RULE_I_TABLE,
     CAESAR_TYPE_INDEX_TABLE_1 INDEX,
     CAESAR_TYPE_RULE_I_TABLE_ENTRY ENTRY
     )
{
     CAESAR_TYPE_RULE_I_TABLE_BASE BASE;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  RULE_I_TABLE,
	  INDEX,
	  (CAESAR_TYPE_POINTER *) &BASE);

     ENTRY->RULE_INDEX = BASE->RULE_INDEX;
}

/*------------------------------------------------------------------------------*/
/* Count number of rule-i entries */
CAESAR_TYPE_NATURAL CAESAR_DATALOG_COUNT_RULE_I(CAESAR_TYPE_TABLE_1 TABLE)
{
     return (CAESAR_TYPE_NATURAL) CAESAR_PUT_INDEX_TABLE_1(TABLE);
}

/*------------------------------------------------------------------------------*/
/* Print rule */

void CAESAR_DATALOG_PRINT_RULE(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
     CAESAR_BODY_RULE_TABLE_ENTRY RULE_ENTRY_BODY;
     CAESAR_TYPE_RULE_TABLE_ENTRY RULE_ENTRY;
     RULE_ENTRY = &RULE_ENTRY_BODY;

     CAESAR_DATALOG_GET_RULE_BY_INDEX(INDEX, RULE_ENTRY);

     CAESAR_TYPE_RULE RULE;
     RULE = RULE_ENTRY->RULE;

     printf("%s", RULE->HEAD->RELATION->NAME);
     CAESAR_PRINT_RULE_VARIABLE_LIST_NAMES(RULE->HEAD->RULE_VARIABLE_LIST);

     printf(":-");

     CAESAR_TYPE_LITERAL_LIST LCELL;
     
     LCELL = RULE->TAIL;
     if (LCELL != NULL) {
	  printf("%s", LCELL->LITERAL->RELATION->NAME);
	  CAESAR_PRINT_RULE_VARIABLE_LIST_NAMES(LCELL->LITERAL->RULE_VARIABLE_LIST);
     }
     for (LCELL = LCELL->NEXT; LCELL != NULL; LCELL = LCELL->NEXT) {
	  printf(",%s", LCELL->LITERAL->RELATION->NAME);
	  CAESAR_PRINT_RULE_VARIABLE_LIST_NAMES(LCELL->LITERAL->RULE_VARIABLE_LIST);
     }
}
