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
#include "datalog_variable_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_relation_table.h"
#include "datalog_fact_table.h"
#include "datalog_cpredicate_table.h"
#include "datalog_prule_table.h"
#include "datalog_rule_table.h"
#include "datalog_crule_table.h"
#include "string.h"

CAESAR_TYPE_NATURAL GLOBAL_BLOCK_0_VARIABLE_COUNTER = 0;

/* TODO: TO MOVE TO A PROPER FILE */
void CAESAR_DATALOG_PRINT_PQUERY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX);
void CAESAR_DATALOG_PRINT_CQUERY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX);
void CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDEX(CAESAR_TYPE_TABLE_1 TABLE, CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_STRING *STRING);

/*==============================================================================*/
/* Global variables */

/* TODO: To place and comment properly */

static CAESAR_TYPE_FILE *OUTPUT_FILES;

static CAESAR_TYPE_STRING *OUTPUT_FILES_NAMES;

/* TODO: Avoid passing this global table as a parameter. Checking staticness. */
CAESAR_TYPE_TABLE_1 PQUERY_TABLE;

/* TODO: Avoid passing this global table as a parameter. Checking staticness. */
CAESAR_TYPE_TABLE_1 CQUERY_TABLE;

/* TODO: Avoid passing this global table as a parameter. Checking staticness. */
CAESAR_TYPE_TABLE_1 DOMAIN_TABLE;

/*==============================================================================*/
/* BES Management */

/*------------------------------------------------------------------------------*/
/* OPTIONS FOR SATISFABILITY CHECK MODE */

/*------------------------------------------------------------------------------*/
/* Block unique resolution calculation */
static CAESAR_TYPE_BOOLEAN CAESAR_SATISFABILITY_BLOCK_UNIQUE_RESOLUTION(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
  return CAESAR_FALSE;
/*   if (BLOCK_INDEX == 0) */
/*     return CAESAR_TRUE; */
/*   else */
/*     return CAESAR_FALSE; */
}

/*------------------------------------------------------------------------------*/
/* Block sign calculation */
static CAESAR_TYPE_BOOLEAN CAESAR_SATISFABILITY_BLOCK_SIGN(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
     return CAESAR_MINIMAL_FIXED_POINT_SOLVE_1;
} 

/*------------------------------------------------------------------------------*/
/* Resolution mode of a block */
static CAESAR_TYPE_NATURAL CAESAR_SATISFABILITY_BLOCK_SOLVE_MODE(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
 /* DFS Optimized for alternating disjunctive conjuctive
  * variables / Otherwise use DFS General Mode 0 */
  return 5;
}

/*------------------------------------------------------------------------------*/
/* OPTIONS FOR NORMAL MODE */

/*------------------------------------------------------------------------------*/
/* Block sign calculation */
static CAESAR_TYPE_BOOLEAN CAESAR_BLOCK_SIGN(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
     return CAESAR_MINIMAL_FIXED_POINT_SOLVE_1;
} 

/*------------------------------------------------------------------------------*/
/* Block unique resolution calculation */
static CAESAR_TYPE_BOOLEAN CAESAR_BLOCK_UNIQUE_RESOLUTION(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
/*      return (BLOCK_INDEX == 0) ? CAESAR_FALSE : CAESAR_TRUE; */
     return CAESAR_FALSE;
}

/*------------------------------------------------------------------------------*/
/* Resolution mode of a block */
static CAESAR_TYPE_NATURAL CAESAR_BLOCK_SOLVE_MODE(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
     return (BLOCK_INDEX == 0) ? 
	  1 /* BFS General */ 
	  :
	  5 /* DFS Optimized for alternating disjunctive conjuctive
	     * variables / Otherwise use DFS General Mode 0 */;
}

/*------------------------------------------------------------------------------*/
/* COMMON OPTIONS */

/*------------------------------------------------------------------------------*/
/* Area of a block variable */
static CAESAR_TYPE_AREA_1 CAESAR_BLOCK_VARIABLE_AREA(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
     return CAESAR_NATURAL_AREA_1 (sizeof (CAESAR_TYPE_BOOLEAN_VARIABLE)); /* TODO */
}

/*------------------------------------------------------------------------------*/
/* CAESAR_LIMIT_SIZE of a bloc's variable table */
static CAESAR_TYPE_NATURAL CAESAR_BLOCK_LIMIT_SIZE(CAESAR_TYPE_NATURAL BLOCK_INDEX)
{
     return 0;
}

/*------------------------------------------------------------------------------*/
/* CAESAR_HASH_SIZE of a bloc's variable table */
static CAESAR_TYPE_NATURAL CAESAR_BLOCK_HASH_SIZE(CAESAR_TYPE_NATURAL BLOCK_INDEX)
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

     if (V->BLOCK_INDEX == 0)
	  switch (V->TYPE) {
	  case 0: /* X0 */
	  case 1: /* X1 */
	       return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1; /* Returns an OR */
	  case 2: /* X2 */
	       return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1; /* Debugging */
	       return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1; /* Returns an AND */
	  case 3: /* FALSE */
	  default:
	       return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1; 
	  }
     else
	  switch (V->TYPE) {
	  case 0: /* TRUE */
	       return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1;
	  case 1: /* X3 */
	  case 2: /* X4 */
	       return CAESAR_DISJUNCTIVE_VARIABLE_SOLVE_1;
	  case 3: /* X5 */
	  default:
	       return CAESAR_CONJUNCTIVE_VARIABLE_SOLVE_1;
	  }
}

/*------------------------------------------------------------------------------*/
/* Comparison between a bloc's variables */
static CAESAR_TYPE_BOOLEAN CAESAR_VARIABLE_COMPARE(
     CAESAR_TYPE_POINTER VARIABLE_1,
     CAESAR_TYPE_POINTER VARIABLE_2
     )
{
     if (*VARIABLE_1 == *VARIABLE_2)
	  return CAESAR_TRUE;
     else
	  return CAESAR_FALSE;
}

/*------------------------------------------------------------------------------*/
/* Hash-code calculation of a bloc's variables */
static CAESAR_TYPE_NATURAL CAESAR_VARIABLE_HASH(
     CAESAR_TYPE_POINTER VARIABLE,
     CAESAR_TYPE_NATURAL MODULUS
     )
{
     /* TODO : Choose better hash functions for the case of a natural */
     return CAESAR_0_HASH(VARIABLE, sizeof (CAESAR_TYPE_BOOLEAN_VARIABLE), MODULUS);
}

/*------------------------------------------------------------------------------*/
/* Printing of a variable */
static void CAESAR_VARIABLE_PRINT(
     CAESAR_TYPE_FILE FILE,
     CAESAR_TYPE_POINTER VARIABLE
     )
{
     CAESAR_TYPE_BOOLEAN_VARIABLE *V;
     V = (CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE;

     if (V->BLOCK_INDEX == 0)
	  switch (V->TYPE) {
	  case 0: /* X0 */
	       (void) fprintf(FILE, "X0");
	       break;
	  case 1: /* X1 */
	       CAESAR_DATALOG_PRINT_PQUERY_BY_INDEX(V->ENTRY_INDEX);
	       break;
	  case 2: /* X2 */
	       CAESAR_DATALOG_PRINT_CQUERY_BY_INDEX(V->ENTRY_INDEX);
	       break;
	  case 3: /* FALSE */
	       (void) fprintf(FILE, "F");
	       break;
	  }
     else
	  switch (V->TYPE) {
	  case 0: /* TRUE */
	       (void) fprintf(FILE, "T");
	       break;
	  case 1: /* X3 */
	       CAESAR_DATALOG_PRINT_CPREDICATE_BY_INDEX(V->ENTRY_INDEX);
	       break;
	  case 2: /* X4 */
	       CAESAR_DATALOG_PRINT_PRULE_BY_INDEX(V->ENTRY_INDEX);
	       break;
	  case 3: /* X5 */
	       CAESAR_DATALOG_PRINT_CRULE_BY_INDEX(V->ENTRY_INDEX);
	       break;
	  }     
}

/*------------------------------------------------------------------------------*/
/* Count the number of entries in the domain table */

CAESAR_TYPE_NATURAL CAESAR_DATALOG_GET_DOMAIN_COUNT()
{
     return (CAESAR_TYPE_NATURAL) CAESAR_PUT_INDEX_TABLE_1(DOMAIN_TABLE);
}

/*------------------------------------------------------------------------------*/
/* Construction of the domain elements tables */

typedef struct CAESAR_STRUCT_DOMAIN_ELEMENTS_TABLE_BASE {
     /* Element */
     CAESAR_TYPE_STRING ELEMENT;
} CAESAR_BODY_DOMAIN_ELEMENTS_TABLE_BASE, *CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE;

void CAESAR_DATALOG_PRINT_DOMAIN_ELEMENTS_TABLE(
     CAESAR_TYPE_FILE FILE,
     CAESAR_TYPE_TABLE_1 TABLE
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM;
     CAESAR_TYPE_POINTER ENTRY;

     fprintf(FILE, "\nDomain elements Table:");
     
     /* We get the maximum index */
     MAXIMUM = CAESAR_PUT_INDEX_TABLE_1(TABLE);

     /* We iterate through all the indexes */
     for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM; TABLE_INDEX++) {
	  /* We get the address of each index's entry */
	  CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, TABLE_INDEX, &ENTRY);

	  fprintf(FILE, "\n\t%d\t%s", TABLE_INDEX, ((CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE) ENTRY)->ELEMENT);
     }
}

CAESAR_TYPE_NATURAL CAESAR_DATALOG_BUILD_DOMAIN_ELEMENTS_TABLE(CAESAR_TYPE_TABLE_1 *DOMAIN_ELEMENTS_TABLE_POINTER, CAESAR_TYPE_STRING DOMAIN_FILENAME)
{
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;
     CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE BASE;

/* TODO:
   HERE I MUST TO CHOOSE BETWEEN STORING ALL THE INFO IN THE TABLES OR
   DISPERSED ON MEMORY. FROM THAT DECISION I'LL CHOOSE TO USE ONE OF
   THE 2 TYPES TYPES *BODY* OR *TYPE*  */

     /* Creation of the table of domains */

     CAESAR_TYPE_TABLE_1 DOMAIN_ELEMENTS_TABLE;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_DOMAIN_ELEMENTS_TABLE_BASE));
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     CAESAR_CREATE_TABLE_1(
	  &DOMAIN_ELEMENTS_TABLE,
	  BASE_AREA,
	  MARK_AREA,
	  0, /* LIMIT_SIZE by default */
	  0, /* HASH_SIZE by default */
	  CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	  (CAESAR_TYPE_COMPARE_FUNCTION) CAESAR_COMPARE_STRING_AREA_1, /* COMPARE_FUNCTION */
	  NULL, /* HASH_FUNCTION */
	  NULL, /* PRINT_FUNCTION */
	  CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	  );
     
     /* TODO: Read the elements from the domain files and save them
      * under the correct table */

     CAESAR_TYPE_FILE DOMAIN_FILE;

     /* Opening of the datalog file */
     if ((DOMAIN_FILE = fopen(DOMAIN_FILENAME, "r")) == NULL) {
	  CAESAR_ERROR("File ``%s'' (domain) is unreadable ", DOMAIN_FILENAME);
     }
     
     const CAESAR_TYPE_INTEGER STRING_BUFFER_SIZE = 256;

     char STRING_BUFFER[STRING_BUFFER_SIZE];
     
     CAESAR_TYPE_POINTER SUCCESS;

     /* TODO: need a getline function */
     /*  fgetc(DOMAIN_FILE); */

     CAESAR_TYPE_NATURAL CARDINAL;

     CARDINAL = 0;
     SUCCESS = fgets(STRING_BUFFER, STRING_BUFFER_SIZE, DOMAIN_FILE);
     while (SUCCESS != NULL) {
	  BASE = (CAESAR_TYPE_DOMAIN_ELEMENTS_TABLE_BASE) CAESAR_PUT_BASE_TABLE_1(DOMAIN_ELEMENTS_TABLE);

	  if (STRING_BUFFER[strlen(STRING_BUFFER) - 1] == '\n')
	       STRING_BUFFER[strlen(STRING_BUFFER) - 1] = '\0';
	  BASE->ELEMENT = CAESAR_COPY_STRING(STRING_BUFFER);
	  CAESAR_PUT_TABLE_1(DOMAIN_ELEMENTS_TABLE);
	  CARDINAL++;
	  
	  SUCCESS = fgets(STRING_BUFFER, STRING_BUFFER_SIZE, DOMAIN_FILE);
     }

     /* TODO: REMOVE BECAUSE WE ARE FILLING THE TABLE ALREADY WITH THE
      * FILE CONTENTS BASE->ELEMENT = "BES"; */
     
     
     /* TODO: TABLE DEBUGGING */
     /* CAESAR_FORMAT_TABLE_1(DOMAIN_ELEMENTS_TABLE, 2); */
     /* CAESAR_PRINT_TABLE_1(stdout, DOMAIN_ELEMENTS_TABLE); */

     CAESAR_BEGIN_DEBUG(4) {

	  CAESAR_DATALOG_PRINT_DOMAIN_ELEMENTS_TABLE(stdout, DOMAIN_ELEMENTS_TABLE);

     } CAESAR_END_DEBUG;

     /* Return the table */
     *DOMAIN_ELEMENTS_TABLE_POINTER = DOMAIN_ELEMENTS_TABLE;

     return CARDINAL;
}

typedef struct CAESAR_STRUCT_DOMAIN_TABLE_MARK {
     /* Domain cardinal */
     CAESAR_TYPE_NATURAL CARDINAL;

     /* Domain elements table */
     CAESAR_TYPE_TABLE_1 ELEMENTS;     
} CAESAR_BODY_DOMAIN_TABLE_MARK, *CAESAR_TYPE_DOMAIN_TABLE_MARK;

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_DOMAIN_TABLE_ENTRY {

     CAESAR_TYPE_STRING NAME;

     CAESAR_TYPE_NATURAL CARDINAL;

     CAESAR_TYPE_TABLE_1 ELEMENTS;
} CAESAR_BODY_DOMAIN_TABLE_ENTRY, *CAESAR_TYPE_DOMAIN_TABLE_ENTRY;

/*------------------------------------------------------------------------------*/
/* Get the a domain by its index */

void CAESAR_DATALOG_GET_DOMAIN_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_DOMAIN_TABLE_ENTRY ENTRY)
{
     CAESAR_TYPE_POINTER BASE;
     CAESAR_TYPE_POINTER MARK;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  DOMAIN_TABLE,
	  INDEX,
	  &BASE
	  );

     CAESAR_RETRIEVE_I_M_TABLE_1(
	  DOMAIN_TABLE,
	  INDEX,
	  &MARK
	  );

     ENTRY->NAME = *((CAESAR_TYPE_STRING *) BASE);
     ENTRY->CARDINAL = ((CAESAR_TYPE_DOMAIN_TABLE_MARK) MARK)->CARDINAL;
     ENTRY->ELEMENTS = ((CAESAR_TYPE_DOMAIN_TABLE_MARK) MARK)->ELEMENTS;
}

/*------------------------------------------------------------------------------*/
/* Get the a domain element by its index and the domain index */

void CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(CAESAR_TYPE_INDEX_TABLE_1 DOMAIN_INDEX, CAESAR_TYPE_INDEX_TABLE_1 ELEMENT_INDEX, CAESAR_TYPE_STRING *STRING)
{
     CAESAR_BODY_DOMAIN_TABLE_ENTRY DOMAIN_BODY;
     CAESAR_TYPE_DOMAIN_TABLE_ENTRY DOMAIN;
     DOMAIN = &DOMAIN_BODY;

     CAESAR_DATALOG_GET_DOMAIN_BY_INDEX(DOMAIN_INDEX, DOMAIN);
     CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDEX(DOMAIN->ELEMENTS, ELEMENT_INDEX, STRING);
}

/*------------------------------------------------------------------------------*/
/* Get the a domain element by its index and table */

void CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDEX(CAESAR_TYPE_TABLE_1 TABLE, CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_STRING *STRING)
{
     CAESAR_TYPE_POINTER BASE;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  TABLE,
	  INDEX,
	  &BASE
	  );

     CAESAR_BEGIN_DEBUG(5) {

     fprintf(stdout, "TABLA DE DOMINIOS [%d elementos] -> acceso al %i\n", CAESAR_PUT_INDEX_TABLE_1(TABLE), INDEX);

     } CAESAR_END_DEBUG;


     *STRING = *((CAESAR_TYPE_STRING *) BASE);
}


void CAESAR_DATALOG_BUILD_DOMAIN_TABLE()
{
     /* Creation of TABLE STRUCTURES to memorize the DATALOG-BES */

     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;
     CAESAR_TYPE_POINTER BASE;
     CAESAR_TYPE_DOMAIN_TABLE_MARK MARK;
     CAESAR_TYPE_TABLE_1 DOMAIN_ELEMENTS_TABLE;

     /* Creation of the table of domains */

     CAESAR_TYPE_TABLE_1 AUX_DOMAIN_TABLE;

     BASE_AREA = CAESAR_STRING_AREA_1();
     MARK_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_BODY_DOMAIN_TABLE_MARK));

     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("DOMAIN", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;

     CAESAR_CREATE_TABLE_1(
	  &AUX_DOMAIN_TABLE,
	  BASE_AREA,
	  MARK_AREA,
	  0, /* LIMIT_SIZE by default */
	  0, /* HASH_SIZE by default */
	  CAESAR_TRUE, /* Sets HASH_SIZE to its nearest prime number  */
	  (CAESAR_TYPE_COMPARE_FUNCTION) CAESAR_COMPARE_STRING_AREA_1, /* COMPARE_FUNCTION */
	  NULL, /* HASH_FUNCTION */
	  NULL, /* PRINT_FUNCTION */
	  CAESAR_OVERFLOW_ABORT_TABLE_1/* OVERFLOW_FUNCTION */
	  );


     CAESAR_TYPE_DOMAIN_LIST CELL;

     for (CELL = CAESAR_DATALOG_DOMAIN_LIST;
	  CELL != NULL;
	  CELL = CELL->NEXT) {

	  /* Search where it's gonna be hold the new entry */
 	  BASE = CAESAR_PUT_BASE_TABLE_1(AUX_DOMAIN_TABLE);
	  MARK = (CAESAR_TYPE_DOMAIN_TABLE_MARK) CAESAR_PUT_MARK_TABLE_1(AUX_DOMAIN_TABLE);

	  /* Add the information to the new entry */
	  /*
	    CAESAR_COPY_AREA_1(BASE,&(CELL->CAESAR_DOMAIN->CAESAR_NAME), sizeof (CAESAR_TYPE_STRING));*/
	  *((CAESAR_TYPE_STRING *)BASE) = CELL->DOMAIN->NAME;

	  /* We should tell the user that the cardinal provided by him is incorrect if it is */
	  /* TODO: Decidir esto MARK->CARDINAL = CELL->DOMAIN->CARDINAL;*/
	  MARK->CARDINAL = CAESAR_DATALOG_BUILD_DOMAIN_ELEMENTS_TABLE(&DOMAIN_ELEMENTS_TABLE, CELL->DOMAIN->FILENAME);
	  /* 	  MARK->CARDINAL = COUNT_NUMBER_OF_LINES(CELL->DOMAIN->FILENAME); */
	  MARK->ELEMENTS = DOMAIN_ELEMENTS_TABLE;
	  
	  /* TODO: DEFINIR ESTRUCTURAS STRUCT_MARK_DOMAIN */
	  /* Encapsulation on table creation functions INIT_AUX_DOMAIN_TABLE */

	  /* Add the new entry to the table */
   	  CAESAR_PUT_TABLE_1(AUX_DOMAIN_TABLE);
     }
     
     CAESAR_BEGIN_DEBUG(4) {

	  CAESAR_FORMAT_TABLE_1(AUX_DOMAIN_TABLE, 2);
	  CAESAR_PRINT_TABLE_1(stdout, AUX_DOMAIN_TABLE);

     } CAESAR_END_DEBUG;

 
     /* Return the table */
     DOMAIN_TABLE = AUX_DOMAIN_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Construction of the combinations tables */

CAESAR_TYPE_NATURAL CAESAR_DATALOG_COMBINATION_TABLE_ENTRY_SIZE(CAESAR_TYPE_NATURAL ARITY)
{
     CAESAR_TYPE_NATURAL INDEXES_SIZE;
     CAESAR_TYPE_NATURAL FLAGS_SIZE;
     
     /* Size of the indexes */
     INDEXES_SIZE = sizeof (CAESAR_TYPE_INDEX_TABLE_1) * ARITY;
     
     /* Size of the IS_VARIABLE bit flags for all the indexes */
     if ((ARITY  % 8) == 0)
	  /* The flags perfectly fit in a number of fully used bytes */
	  FLAGS_SIZE = ARITY / 8;
     else
	  /* We have to add a not fully used byte */
	  FLAGS_SIZE = ARITY / 8 + 1;
	  
     /* The size of the entry will be the sum of those of the indexes
      * and flags */
     return INDEXES_SIZE + FLAGS_SIZE;
}

void CAESAR_DATALOG_BUILD_COMBINATION_TABLE(CAESAR_TYPE_TABLE_1 *COMBINATION_TABLE_POINTER, CAESAR_TYPE_NATURAL ARITY)
{
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;
     CAESAR_TYPE_TABLE_1 COMBINATION_TABLE;

     BASE_AREA = CAESAR_BYTE_AREA_1(CAESAR_DATALOG_COMBINATION_TABLE_ENTRY_SIZE(ARITY));
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("COMBINATION", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;
     
     CAESAR_CREATE_TABLE_1(
	  &COMBINATION_TABLE,
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
     
     /* Return the table */
     *COMBINATION_TABLE_POINTER = COMBINATION_TABLE;
}

void CAESAR_DATALOG_EDIT_COMBINATION_TABLE_ENTRY(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_NATURAL ARITY,
     CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX,
     CAESAR_TYPE_BOOLEAN IS_VARIABLE
     )
{
     /* If INDEX == ARITY then we are changing the TRUTH VALUE of the
      * combination */
     CAESAR_TYPE_NATURAL END_OF_INDEXES_OFFSET;
     CAESAR_TYPE_NATURAL INDEX_OFFSET;
     CAESAR_TYPE_POINTER BYTE_BASE_POINTER;

     /* The index must always be lower than the arity */
     CAESAR_ASSERT(INDEX < ARITY);

     /* TODO : Here we must provide a way to change the truth value of
      * the combination or decide to do something else. The fact is that we
      * have decided to store the facts in other place */

     INDEX_OFFSET = sizeof (CAESAR_TYPE_INDEX_TABLE_1) * INDEX;
     END_OF_INDEXES_OFFSET = sizeof (CAESAR_TYPE_INDEX_TABLE_1) * ARITY;
     
     /* Change of an entry's index */
     *( (CAESAR_TYPE_INDEX_TABLE_1 *) (COMBINATION_TABLE_ENTRY + INDEX_OFFSET) ) = TABLE_INDEX;
     
     BYTE_BASE_POINTER = COMBINATION_TABLE_ENTRY + END_OF_INDEXES_OFFSET;
     BYTE_BASE_POINTER += INDEX / 8;
     if (IS_VARIABLE == CAESAR_TRUE) {
       /* Sets the variable flag */
       (*BYTE_BASE_POINTER) |= ((CAESAR_TYPE_BYTE) 1 << (INDEX % 8));
     } else {
       /* Unsets the variable flag */
       (*BYTE_BASE_POINTER) &= (((CAESAR_TYPE_BYTE) 255) - ((CAESAR_TYPE_BYTE) 1 << (INDEX % 8)));
     }
}

void CAESAR_DATALOG_GET_COMBINATION_TABLE_ENTRY(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_NATURAL ARITY,
     CAESAR_TYPE_INDEX_TABLE_1 *TABLE_INDEX,
     CAESAR_TYPE_BOOLEAN *IS_VARIABLE
     )
{
     CAESAR_TYPE_NATURAL END_OF_INDEXES_OFFSET;
     CAESAR_TYPE_NATURAL INDEX_OFFSET;
     CAESAR_TYPE_POINTER BYTE_BASE_POINTER;
     
     /* The index must always be lower than the arity */
     CAESAR_ASSERT(INDEX < ARITY);

     INDEX_OFFSET = sizeof (CAESAR_TYPE_INDEX_TABLE_1) * INDEX;
     END_OF_INDEXES_OFFSET = sizeof (CAESAR_TYPE_INDEX_TABLE_1) * ARITY;

     /* Looking-up of an entry's index */
     *TABLE_INDEX = *( (CAESAR_TYPE_INDEX_TABLE_1 *) (COMBINATION_TABLE_ENTRY + INDEX_OFFSET) );
     
     BYTE_BASE_POINTER = COMBINATION_TABLE_ENTRY + END_OF_INDEXES_OFFSET;
     BYTE_BASE_POINTER += INDEX / 8;
     if (
	 (
	  ((CAESAR_TYPE_BYTE) *BYTE_BASE_POINTER) & 
	  ((CAESAR_TYPE_BYTE) (1 << (INDEX % 8)))
	  ) 
	 > ((CAESAR_TYPE_BYTE) 0)
	 )
       *IS_VARIABLE = CAESAR_TRUE;
     else
       *IS_VARIABLE = CAESAR_FALSE;	  
}

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_COMBINATION_TABLE_ENTRY { 
     /* cpredicate index */
     CAESAR_TYPE_INDEX_TABLE_1 *ARGUMENTS;
     
     /* Relation index */
     CAESAR_TYPE_BOOLEAN *ARGUMENT_IS_VARIABLE;

     /* Query length */
     CAESAR_TYPE_NATURAL LENGTH;
} CAESAR_BODY_COMBINATION_TABLE_ENTRY, *CAESAR_TYPE_COMBINATION_TABLE_ENTRY;

/*------------------------------------------------------------------------------*/
/* Get a combination entry by index */



void CAESAR_DATALOG_GET_COMBINATION_BY_INDEX(
     CAESAR_TYPE_TABLE_1 COMBINATION_TABLE,
     CAESAR_TYPE_INDEX_TABLE_1 INDEX,
     CAESAR_TYPE_NATURAL ARITY,
     CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION
     )
{
     CAESAR_TYPE_POINTER BASE;
     CAESAR_TYPE_NATURAL i;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  COMBINATION_TABLE,
	  INDEX,
	  &BASE
	  );

     COMBINATION->LENGTH = ARITY;

     for (i = 0; i < ARITY; i++) {
	  CAESAR_DATALOG_GET_COMBINATION_TABLE_ENTRY(
	       BASE,
	       i,
	       ARITY,
	       &(COMBINATION->ARGUMENTS[i]),
	       &(COMBINATION->ARGUMENT_IS_VARIABLE[i])
	       );
     }
}

void CAESAR_DATALOG_PRINT_COMBINATION_TABLE(
     CAESAR_TYPE_FILE FILE,
     CAESAR_TYPE_TABLE_1 TABLE,
     CAESAR_TYPE_NATURAL ARITY
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 ENTRY_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 ENTRY_MEMBER_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM;
     CAESAR_TYPE_BOOLEAN IS_VARIABLE;
     CAESAR_TYPE_POINTER ENTRY;

     fprintf(FILE, "Combination Table:\n");
     
     /* We get the maximum index */
     MAXIMUM = CAESAR_PUT_INDEX_TABLE_1(TABLE);

     /* We iterate through all the indexes */
     for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM; TABLE_INDEX++) {
	  fprintf(FILE, "\n[BASE %02d]", TABLE_INDEX);

	  /* We get the address of each index's entry */
	  CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, TABLE_INDEX, &ENTRY);

	  /* We iterate through each index's member */
	  for (ENTRY_INDEX = 0; ENTRY_INDEX < ARITY; ENTRY_INDEX++) {

	       CAESAR_DATALOG_GET_COMBINATION_TABLE_ENTRY(
		    ENTRY,
		    ENTRY_INDEX,
		    ARITY,
		    &ENTRY_MEMBER_INDEX,
		    &IS_VARIABLE
		    );

	       if (IS_VARIABLE == CAESAR_TRUE)
		    fprintf(FILE, "\tVar. %d", ENTRY_MEMBER_INDEX);
	       else
		    fprintf(FILE, "\tCnt. %d", ENTRY_MEMBER_INDEX);
	  }
     }

     fprintf(FILE, "\n\n");
}

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_COMBINATION_TABLE_ENTRY(
     CAESAR_TYPE_TABLE_1 COMBINATION_TABLE,
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX
     )
{
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;
     
     return CAESAR_SEARCH_AND_PUT_TABLE_1(COMBINATION_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
}

void CAESAR_DATALOG_CREATE_COMBINATION_TABLE_ENTRY(
     CAESAR_TYPE_TABLE_1 COMBINATION_TABLE,
     CAESAR_TYPE_POINTER *COMBINATION_TABLE_ENTRY)
{
     *COMBINATION_TABLE_ENTRY = CAESAR_PUT_BASE_TABLE_1(COMBINATION_TABLE);
}

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_COMBINATION_TABLE_ENTRY2(
     CAESAR_TYPE_TABLE_1 COMBINATION_TABLE,
     CAESAR_TYPE_POINTER OUTTER_ENTRY,
     CAESAR_TYPE_NATURAL ARITY,
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX
     )
{
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;
     CAESAR_TYPE_POINTER INNER_ENTRY;

     CAESAR_DATALOG_CREATE_COMBINATION_TABLE_ENTRY(
	  COMBINATION_TABLE,
	  &INNER_ENTRY
	  );
     memcpy(
	  INNER_ENTRY,
	  OUTTER_ENTRY, 
	  CAESAR_DATALOG_COMBINATION_TABLE_ENTRY_SIZE(ARITY)
	  );
     
     return CAESAR_SEARCH_AND_PUT_TABLE_1(COMBINATION_TABLE, INDEX, &EXISTING_ENTRY_POINTER);
}

/*==============================================================================*/
/* Query table Management */

/*------------------------------------------------------------------------------*/
/* Creation of a query table entry */

void CAESAR_DATALOG_CREATE_PQUERY_TABLE_ENTRY(
     CAESAR_TYPE_TABLE_1 QUERY_TABLE,
     CAESAR_TYPE_POINTER *QUERY_TABLE_ENTRY
     )
{
     *QUERY_TABLE_ENTRY = CAESAR_PUT_BASE_TABLE_1(QUERY_TABLE);
}

void CAESAR_DATALOG_CREATE_CQUERY_TABLE_ENTRY(   
     CAESAR_TYPE_TABLE_1 QUERY_TABLE,
     CAESAR_TYPE_POINTER *QUERY_TABLE_ENTRY
     ) 
{
     CAESAR_DATALOG_CREATE_PQUERY_TABLE_ENTRY(QUERY_TABLE, QUERY_TABLE_ENTRY);
}

/* void CAESAR_DATALOG_CREATE_CQUERY_TABLE_ENTRY(    */
/*      CAESAR_TYPE_TABLE_1 QUERY_TABLE, */
/*      CAESAR_TYPE_POINTER *QUERY_TABLE_ENTRY */
/*      ) CAESAR_DATALOG_CREATE_PQUERY_TABLE_ENTRY; */

/*------------------------------------------------------------------------------*/
/* Edition of a query table entry */

void CAESAR_DATALOG_EDIT_PQUERY_TABLE_ENTRY(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;
     
     /* The index must be reasonable */
     CAESAR_ASSERT(INDEX < 1); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */
 
     /* We see the entry as an array of table indexes */
     INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;
     
     /* We update the values */
     INDEX_ARRAY[INDEX * 2] = RELATION_INDEX;
     INDEX_ARRAY[INDEX * 2 + 1] = COMBINATION_INDEX;
}

void CAESAR_DATALOG_EDIT_CQUERY_TABLE_ENTRY(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX
     )
{
     CAESAR_DATALOG_EDIT_PQUERY_TABLE_ENTRY(COMBINATION_TABLE_ENTRY, INDEX, RELATION_INDEX, COMBINATION_INDEX);
}

void CAESAR_DATALOG_EDIT_PQUERY_TABLE_ENTRY_MARK(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;
     
     /* The index must be reasonable */
     CAESAR_ASSERT(INDEX < CAESAR_DATALOG_MAX_RULE_BODY_SIZE); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */
 
     /* We see the entry as an array of table indexes */
     INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;
     
     /* We update the values */
     INDEX_ARRAY[INDEX * 2] = RELATION_INDEX;
     INDEX_ARRAY[INDEX * 2 + 1] = COMBINATION_INDEX;
}


void CAESAR_DATALOG_GET_CQUERY_TABLE_ENTRY(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 *RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 *COMBINATION_INDEX
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;
     
     /* The index must be reasonable */
     CAESAR_ASSERT(INDEX < 1); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */
 
     /* We see the entry as an array of table indexes */
     INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;
     
     /* We update the values */
     *RELATION_INDEX = INDEX_ARRAY[INDEX * 2];
     *COMBINATION_INDEX = INDEX_ARRAY[INDEX * 2 + 1];
}

/*------------------------------------------------------------------------------*/
/* Read a query table entry */

void CAESAR_DATALOG_GET_PQUERY_TABLE_ENTRY(
     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY,
     CAESAR_TYPE_NATURAL INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 *RELATION_INDEX,
     CAESAR_TYPE_INDEX_TABLE_1 *COMBINATION_INDEX
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX_ARRAY;
     
     /* The index must be reasonable */
     CAESAR_ASSERT(INDEX < 1); /* TODO: For queries this must be CAESAR_DATALOG_MAX_QUERY_SIZE */
 
     /* We see the entry as an array of table indexes */
     INDEX_ARRAY = (CAESAR_TYPE_INDEX_TABLE_1 *) COMBINATION_TABLE_ENTRY;
     
     /* We update the values */
     *RELATION_INDEX = INDEX_ARRAY[INDEX * 2];
     *COMBINATION_INDEX = INDEX_ARRAY[INDEX * 2 + 1];
}

/*------------------------------------------------------------------------------*/
/* Addition of an entry to a query table */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_PQUERY_TABLE_ENTRY(
     CAESAR_TYPE_TABLE_1 QUERY_TABLE,
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX
     )
{
     CAESAR_TYPE_POINTER EXISTING_ENTRY_POINTER;
     CAESAR_TYPE_BOOLEAN SUCCESS;
          
     SUCCESS = CAESAR_SEARCH_AND_PUT_TABLE_1(QUERY_TABLE, INDEX, &EXISTING_ENTRY_POINTER);

     return SUCCESS;
}

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ADD_CQUERY_TABLE_ENTRY(
     CAESAR_TYPE_TABLE_1 QUERY_TABLE,
     CAESAR_TYPE_INDEX_TABLE_1 *INDEX
     )
{
     return CAESAR_DATALOG_ADD_PQUERY_TABLE_ENTRY(QUERY_TABLE, INDEX);
}

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_PQUERY_TABLE_ENTRY { 
/* TODO: THIS SHOULD BE IMPROVED IN FUTURE VERSIONS FOR ANY LENGTH ARRAYS */

     /* cpredicate index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDICES[1];
     
     /* Relation index */
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDICES[1];

     /* Query length */
     CAESAR_TYPE_NATURAL LENGTH;
} CAESAR_BODY_PQUERY_TABLE_ENTRY, *CAESAR_TYPE_PQUERY_TABLE_ENTRY;

/*------------------------------------------------------------------------------*/
/* Get a pquery entry by index */

void CAESAR_DATALOG_GET_PQUERY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_PQUERY_TABLE_ENTRY PQUERY)
{
     CAESAR_TYPE_POINTER BASE;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  PQUERY_TABLE,
	  INDEX,
	  &BASE
	  );

     CAESAR_DATALOG_GET_PQUERY_TABLE_ENTRY(
	  BASE,
	  0,
	  PQUERY->RELATION_INDICES,
	  PQUERY->COMBINATION_INDICES
	  );

     PQUERY->LENGTH = 1;
}

/*============================================================================*/
/* Types */

typedef struct CAESAR_STRUCT_CQUERY_TABLE_ENTRY { 
/* TODO: THIS SHOULD BE IMPROVED IN FUTURE VERSIONS FOR ANY LENGTH ARRAYS */

     /* cpredicate index */
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDICES[1];
     
     /* Relation index */
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDICES[1];

     /* Query length */
     CAESAR_TYPE_NATURAL LENGTH;
} CAESAR_BODY_CQUERY_TABLE_ENTRY, *CAESAR_TYPE_CQUERY_TABLE_ENTRY;

/*------------------------------------------------------------------------------*/
/* Get a pquery entry by index */

void CAESAR_DATALOG_GET_CQUERY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX, CAESAR_TYPE_CQUERY_TABLE_ENTRY CQUERY)
{
     CAESAR_TYPE_POINTER BASE;

     CAESAR_RETRIEVE_I_B_TABLE_1(
	  CQUERY_TABLE,
	  INDEX,
	  &BASE
	  );

     CAESAR_DATALOG_GET_CQUERY_TABLE_ENTRY(
	  BASE,
	  0,
	  CQUERY->RELATION_INDICES,
	  CQUERY->COMBINATION_INDICES
	  );

     CQUERY->LENGTH = 1;
}

/*------------------------------------------------------------------------------*/
/* Printing of a cquery entry */

void CAESAR_DATALOG_PRINT_COMBINATION(CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX, CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX)
{
     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_STRING STRING;

     CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
     CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
     RELATION = &RELATION_BODY;

     CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
     CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
     COMBINATION = &COMBINATION_BODY;

     CAESAR_DATALOG_GET_RELATION_BY_INDEX(RELATION_INDEX, RELATION);

     CAESAR_CREATE(COMBINATION->ARGUMENTS, RELATION->ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
     CAESAR_CREATE(COMBINATION->ARGUMENT_IS_VARIABLE, RELATION->ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);
     CAESAR_DATALOG_GET_COMBINATION_BY_INDEX(RELATION->COMBINATIONS, COMBINATION_INDEX, RELATION->ARITY, COMBINATION);
     
     printf("%s(", RELATION->NAME);
     i = 0;
     if (i < RELATION->ARITY) {
	  if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE) {
	       CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i], &STRING);
	       printf("%s", STRING);
	  }
	  else {
 	       CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);
	       printf("%s", STRING);
	  }
     }
     for (i = 1; i < RELATION->ARITY; i++)
	  if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE) {
	       CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i], &STRING);
	       printf(",%s", STRING);
	  }
	  else {
 	       CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);
	       printf(",%s", STRING);
	  }
     
     printf(")");
     CAESAR_DELETE(COMBINATION->ARGUMENTS);
     CAESAR_DELETE(COMBINATION->ARGUMENT_IS_VARIABLE);     
}

void CAESAR_DATALOG_PRINT_CQUERY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_STRING STRING;

     CAESAR_BODY_CQUERY_TABLE_ENTRY CQUERY_BODY;
     CAESAR_TYPE_CQUERY_TABLE_ENTRY CQUERY;
     CQUERY = &CQUERY_BODY;

     CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
     CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
     RELATION = &RELATION_BODY;

     CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
     CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
     COMBINATION = &COMBINATION_BODY;

     CAESAR_DATALOG_GET_CQUERY_BY_INDEX(INDEX, CQUERY);
     CAESAR_DATALOG_GET_RELATION_BY_INDEX(CQUERY->RELATION_INDICES[0], RELATION);

     CAESAR_CREATE(COMBINATION->ARGUMENTS, RELATION->ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
     CAESAR_CREATE(COMBINATION->ARGUMENT_IS_VARIABLE, RELATION->ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);
     CAESAR_DATALOG_GET_COMBINATION_BY_INDEX(RELATION->COMBINATIONS, CQUERY->COMBINATION_INDICES[0], RELATION->ARITY, COMBINATION);
     
     printf("X2_%s(", RELATION->NAME);
     
     /* TODO: Throw this prinf away */
     printf("[[CQUERYINDEX %d]]", INDEX);

     i = 0;
     if (i < RELATION->ARITY) {
	  if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE) {
	       CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i], &STRING);
	       printf("%s", STRING);
	  }
	  else {
 	       CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);
	       printf("%s", STRING);
	  }
     }
     for (i = 1; i < RELATION->ARITY; i++)
	  if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE) {
	       CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i], &STRING);
	       printf(",%s", STRING);
	  }
	  else {
 	       CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);
	       printf(",%s", STRING);
	  }
     
     printf(")");
     CAESAR_DELETE(COMBINATION->ARGUMENTS);
     CAESAR_DELETE(COMBINATION->ARGUMENT_IS_VARIABLE);     
}

/*------------------------------------------------------------------------------*/
/* Printing of a pquery entry */

void CAESAR_DATALOG_PRINT_PQUERY_BY_INDEX(CAESAR_TYPE_INDEX_TABLE_1 INDEX)
{
     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_STRING STRING;

     CAESAR_BODY_PQUERY_TABLE_ENTRY PQUERY_BODY;
     CAESAR_TYPE_PQUERY_TABLE_ENTRY PQUERY;
     PQUERY = &PQUERY_BODY;

     CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
     CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
     RELATION = &RELATION_BODY;

     CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
     CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
     COMBINATION = &COMBINATION_BODY;

     CAESAR_DATALOG_GET_PQUERY_BY_INDEX(INDEX, PQUERY);
     CAESAR_DATALOG_GET_RELATION_BY_INDEX(PQUERY->RELATION_INDICES[0], RELATION);

     CAESAR_CREATE(COMBINATION->ARGUMENTS, RELATION->ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
     CAESAR_CREATE(COMBINATION->ARGUMENT_IS_VARIABLE, RELATION->ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);
     CAESAR_DATALOG_GET_COMBINATION_BY_INDEX(RELATION->COMBINATIONS, PQUERY->COMBINATION_INDICES[0], RELATION->ARITY, COMBINATION);
     
     printf("X1_%s(", RELATION->NAME);
     
     /* TODO: Throw this prinf away */
     printf("[[PQUERYINDEX %d]]", INDEX);

     i = 0;
     if (i < RELATION->ARITY) {
	  if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE) {
	       CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i], &STRING);
	       printf("%s", STRING);
	  }
	  else {
 	       CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);
	       printf("%s", STRING);
	  }
     }
     for (i = 1; i < RELATION->ARITY; i++)
	  if (COMBINATION->ARGUMENT_IS_VARIABLE[i] == CAESAR_TRUE) {
	       CAESAR_DATALOG_GET_VARIABLE_BY_INDEX(COMBINATION->ARGUMENTS[i], &STRING);
	       printf(",%s", STRING);
	  }
	  else {
 	       CAESAR_DATALOG_GET_DOMAIN_ELEMENT_BY_INDICES(RELATION->DOMAINS[i], COMBINATION->ARGUMENTS[i], &STRING);
	       printf(",%s", STRING);
	  }
     
     printf(")");
     CAESAR_DELETE(COMBINATION->ARGUMENTS);
     CAESAR_DELETE(COMBINATION->ARGUMENT_IS_VARIABLE);
}

/*------------------------------------------------------------------------------*/
/* Printing of the query table */

void CAESAR_DATALOG_PRINT_PQUERY_TABLE(
     CAESAR_TYPE_FILE FILE,
     CAESAR_TYPE_TABLE_1 TABLE
     )
{
     CAESAR_TYPE_INDEX_TABLE_1 ENTRY_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 ENTRY_RELATION_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 ENTRY_COMBINATION_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM;
     CAESAR_TYPE_POINTER ENTRY;

     fprintf(FILE, "Query table:\n");
     
     /* We get the maximum index */
     MAXIMUM = CAESAR_PUT_INDEX_TABLE_1(TABLE);

     /* We iterate through all the indexes */
     for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM; TABLE_INDEX++) {
	  fprintf(FILE, "\n[BASE %02d]", TABLE_INDEX);

	  /* We get the address of each index's entry */
	  CAESAR_RETRIEVE_I_B_TABLE_1(TABLE, TABLE_INDEX, &ENTRY);

	  /* TODO: BIG DEAL HERE: We need the size of the query */
	  /* We iterate through each index's member */
	  for (ENTRY_INDEX = 0; ENTRY_INDEX < 1; ENTRY_INDEX++) {
	       
	       CAESAR_DATALOG_GET_PQUERY_TABLE_ENTRY(
		    ENTRY,
		    ENTRY_INDEX,
		    &ENTRY_RELATION_INDEX,
		    &ENTRY_COMBINATION_INDEX
		    );
	       
	       fprintf(FILE, "\tRelation: %02d", ENTRY_RELATION_INDEX);
	       fprintf(FILE, "\tCombination: %02d", ENTRY_COMBINATION_INDEX);

	  }
     }

     fprintf(FILE, "\n\n");
}

/*------------------------------------------------------------------------------*/
/* Construction of the pquery table */

void CAESAR_DATALOG_BUILD_PQUERY_TABLE()
{
     CAESAR_TYPE_TABLE_1 AUX_QUERY_TABLE;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     /* TODO: CHECK THIS MARK AREA ¿IT'S USE */

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1) * 2);
     MARK_AREA = CAESAR_NATURAL_AREA_1(1); /* TODO: WHO KNOWS */

     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("PQUERY", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;
     
     CAESAR_CREATE_TABLE_1(
	  &AUX_QUERY_TABLE,
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
     
     /* TODO: In the future the queries will be explicit and these
      * code should be delete */

     /* For iterating through the list */
     CAESAR_TYPE_RELATION_LIST RELATION_CELL;
     CAESAR_TYPE_RELATION RELATION;

     /* For searching on the tables */
     CAESAR_TYPE_TABLE_1 COMBINATION_TABLE;
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX;
     CAESAR_TYPE_INDEX_TABLE_1 QUERY_INDEX;
     CAESAR_TYPE_POINTER AUX_QUERY_TABLE_ENTRY;
/*      //CAESAR_TYPE_POINTER RELATION_TABLE_ENTRY; */

     CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_TABLE_ENTRY_BODY;
     CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION_TABLE_ENTRY;
     RELATION_TABLE_ENTRY = &RELATION_TABLE_ENTRY_BODY;

     CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY;
     CAESAR_TYPE_NATURAL i;

     for (RELATION_CELL = CAESAR_DATALOG_RELATION_LIST;
	  RELATION_CELL != NULL;
	  RELATION_CELL = RELATION_CELL->NEXT) {

	  RELATION = RELATION_CELL->RELATION;

	  if (RELATION->IONATURE == CAESAR_OUTPUT_TUPLES) {
	       
	       /* We search the relation entry */
	       

	       CAESAR_BEGIN_DEBUG(4) {
		    
		    fprintf(stdout, "\n\nWe are looking a %s in:\n", RELATION->NAME);
		    CAESAR_DATALOG_PRINT_RELATION_TABLE();
		    
	       } CAESAR_END_DEBUG;


	       RELATION_INDEX = CAESAR_DATALOG_GET_RELATION_INDEX_BY_NAME(RELATION->NAME);
	       
	       CAESAR_DATALOG_GET_RELATION_BY_INDEX(RELATION_INDEX, RELATION_TABLE_ENTRY);

/* 		    CAESAR_SEARCH_TABLE_1(RELATION_TABLE, */
/* 					       (CAESAR_TYPE_POINTER) &(RELATION->NAME), */
/* 					       &RELATION_INDEX, */
/* 					       &RELATION_TABLE_ENTRY); */
/* 	       CAESAR_ASSERT(SUCCESS == CAESAR_TRUE); */
	       
/* 	       /\* We search the combination table to be used *\/ */
/* 	       CAESAR_RETRIEVE_I_M_TABLE_1(RELATION_TABLE, */
/* 					   RELATION_INDEX, */
/* 					   (CAESAR_TYPE_POINTER *) &RELATION_TABLE_ENTRY); */
	       COMBINATION_TABLE = RELATION_TABLE_ENTRY->COMBINATIONS;
	       
	       for (i = 0; i < RELATION->ARITY; i++) {

		    /* We edit the new combination to be added */
		    CAESAR_DATALOG_CREATE_COMBINATION_TABLE_ENTRY(COMBINATION_TABLE, &COMBINATION_TABLE_ENTRY);
		    
		    /* TODO: As an example we add a variable with the
		     * i  index */
		    CAESAR_DATALOG_EDIT_COMBINATION_TABLE_ENTRY(COMBINATION_TABLE_ENTRY, i, RELATION->ARITY, i, CAESAR_TRUE);

		    /* We add the variable to be used in the query */
		    /* TODO: CRUD on THE VARIABLE TABLE */

	       }

	       /* We store the new combination */
	       CAESAR_DATALOG_ADD_COMBINATION_TABLE_ENTRY(COMBINATION_TABLE, &COMBINATION_INDEX);


	       /* We create the query entry */
	       CAESAR_DATALOG_CREATE_PQUERY_TABLE_ENTRY(AUX_QUERY_TABLE, &AUX_QUERY_TABLE_ENTRY);
	       
	       /* We put the values in the entry*/
	       CAESAR_DATALOG_EDIT_PQUERY_TABLE_ENTRY(AUX_QUERY_TABLE_ENTRY, 0, RELATION_INDEX, COMBINATION_INDEX);
	       
	       /* We add the new entry to the table */
	       CAESAR_DATALOG_ADD_PQUERY_TABLE_ENTRY(AUX_QUERY_TABLE, &QUERY_INDEX);
	  }
     }
     
     PQUERY_TABLE = AUX_QUERY_TABLE;
}

/*------------------------------------------------------------------------------*/
/* Construction of the cquery table */

void CAESAR_DATALOG_BUILD_CQUERY_TABLE()
{
     CAESAR_TYPE_TABLE_1 AUX_CQUERY_TABLE;
     CAESAR_TYPE_AREA_1 BASE_AREA;
     CAESAR_TYPE_AREA_1 MARK_AREA;

     BASE_AREA = CAESAR_BYTE_AREA_1(sizeof (CAESAR_TYPE_INDEX_TABLE_1) * 2);
     MARK_AREA = CAESAR_NATURAL_AREA_1(1);
     MARK_AREA = CAESAR_EMPTY_AREA_1();

     CAESAR_BEGIN_DEBUG(2) {
       DEBUG_CREATE_TABLE("CQUERY", BASE_AREA, MARK_AREA);
     } CAESAR_END_DEBUG;
     
     CAESAR_CREATE_TABLE_1(
	  &AUX_CQUERY_TABLE,
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
     
     CQUERY_TABLE = AUX_CQUERY_TABLE;
}

/*==============================================================================*/
/* BES Management */

/*------------------------------------------------------------------------------*/
/* Function which lets you iterate over combinations of table indexes */

CAESAR_TYPE_BOOLEAN CAESAR_DATALOG_ITERATE_OVER_COMBINATIONS(
    CAESAR_TYPE_INDEX_TABLE_1 *COUNTERS,
    CAESAR_TYPE_INDEX_TABLE_1 *LIMITS,
    CAESAR_TYPE_BOOLEAN *MASK,
    CAESAR_TYPE_NATURAL SIZE
    )
{
     CAESAR_TYPE_NATURAL i, j;
     CAESAR_TYPE_BOOLEAN CARRY;

     CARRY = CAESAR_TRUE;

     /* TODO: BIG DEAL */

     for (i = SIZE - 1; i >= 0 && i < SIZE; i--)
	  if (/* MASK[i] == CAESAR_FALSE && */ (COUNTERS[i] + 1) < LIMITS[i]) {
	       COUNTERS[i]++;
	       for (j = i + 1; j < SIZE; j++)
		    COUNTERS[j] = 0;
	       return CAESAR_TRUE;
	  }
     return CAESAR_FALSE;
}

/*------------------------------------------------------------------------------*/
/* Obtains the output tuples */

void STABLE_VARIABLE_ITERATE_IN_OUTPUT_RELATION_MODE(
     CAESAR_TYPE_SOLVE_1 BES,
     CAESAR_TYPE_NATURAL BLOCK,
     CAESAR_TYPE_POINTER VARIABLE,
     CAESAR_TYPE_BOOLEAN *VALUE)
{
     if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE)->TYPE != 2 || *VALUE == CAESAR_FALSE || BLOCK != 0)
	  return;

     CAESAR_TYPE_NATURAL i;
     CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;

     CAESAR_BODY_CQUERY_TABLE_ENTRY CQUERY_BODY;
     CAESAR_TYPE_CQUERY_TABLE_ENTRY CQUERY;
     CQUERY = &CQUERY_BODY;

     CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
     CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
     RELATION = &RELATION_BODY;

     CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
     CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
     COMBINATION = &COMBINATION_BODY;

/*      CAESAR_CREATE(CQUERY->RELATION_INDICES, sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); */
/*      CAESAR_CREATE(CQUERY->COMBINATION_INDICES, sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); */

     CAESAR_DATALOG_GET_CQUERY_BY_INDEX(
	  ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE)->ENTRY_INDEX,
	  CQUERY);

     RELATION_INDEX = CQUERY->RELATION_INDICES[0];

     CAESAR_DATALOG_GET_RELATION_BY_INDEX(
	  RELATION_INDEX,
	  RELATION);

     CAESAR_CREATE(COMBINATION->ARGUMENTS, RELATION->ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
     CAESAR_CREATE(COMBINATION->ARGUMENT_IS_VARIABLE, RELATION->ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);

     CAESAR_DATALOG_GET_COMBINATION_BY_INDEX(
	  RELATION->COMBINATIONS,
	  CQUERY->COMBINATION_INDICES[0],
	  RELATION->ARITY,
	  COMBINATION
	  );



     for (i = 0; i < COMBINATION->LENGTH; i++) {
	  if (i != 0)
	       fprintf(OUTPUT_FILES[RELATION_INDEX], " ");
	  fprintf(OUTPUT_FILES[RELATION_INDEX], "%d", COMBINATION->ARGUMENTS[i]);
     }
     fprintf(OUTPUT_FILES[RELATION_INDEX], "\n");

     CAESAR_DELETE(COMBINATION->ARGUMENTS);
     CAESAR_DELETE(COMBINATION->ARGUMENT_IS_VARIABLE);     

}

/*------------------------------------------------------------------------------*/
/* Calculation of the successors of a variable */

void CAESAR_VARIABLE_ITERATE(
     CAESAR_TYPE_POINTER VARIABLE_1,
     CAESAR_TYPE_POINTER VARIABLE_2,
     void (*CAESAR_LOOP)()
     )
{
     CAESAR_BEGIN_DEBUG(0) {

	  printf("IT. on: ");CAESAR_VARIABLE_PRINT(stdout, VARIABLE_1);printf(", produces:\n");
	  
     } CAESAR_END_DEBUG;

     /******************************************************************************/
     /* Create the successor variables depending on the type of the given variable */
     /******************************************************************************/

     if (((CAESAR_TYPE_BOOLEAN_VARIABLE * ) VARIABLE_1)->BLOCK_INDEX == 0) {
	  
	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 3) {

	       /*========================================*/
	       /* FALSE doesn't have a successor variabe */
	       /*========================================*/

	       CAESAR_BEGIN_DEBUG(2) {
		    
		    printf("\t-> THIS IS AN ERROR BECAUSE WE DON'T GENERATE EXPLICIT FALSE VARIABLES <-\n");
		    
	       } CAESAR_END_DEBUG;
	       
	       return;

	  }
	  
	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 0) {

	       /*============================*/
	       /* X0 is the initial variable */
	       /*============================*/
	       
	       CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM_INDEX;
	       CAESAR_TYPE_INDEX_TABLE_1 TABLE_INDEX;

	       /* TODO: Here we possibly should check the limits of
		* entry_index, but the query table will always be
		short */
	       /* We iterate through all the indexes */

	       /*------------------------------------------------*/
	       /* Iterate over each parameterized query (pquery) */
	       /*------------------------------------------------*/

	       MAXIMUM_INDEX = CAESAR_PUT_INDEX_TABLE_1(PQUERY_TABLE);

	       for (TABLE_INDEX = 0; TABLE_INDEX < MAXIMUM_INDEX; TABLE_INDEX++) {
		    
		    /*******************************************/
		    /* Generate a X1 successor for each pquery */
		    /*******************************************/

		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 0;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 1;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = TABLE_INDEX;

		    /*************************************/
		    /* Add to caesar_solve the successor */
		    /*************************************/

		    CAESAR_BEGIN_DEBUG(2) {
			 
			 printf("\t");
			 CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			 printf("\n");
			 
		    } CAESAR_END_DEBUG;
		    
		    CAESAR_LOOP(
			 (CAESAR_TYPE_POINTER) NULL,
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
			 VARIABLE_2
			 );

		    GLOBAL_BLOCK_0_VARIABLE_COUNTER++;
	       }

	       return;
	  }

	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 1) {

	       
	       /*======================================*/
	       /* X1 is a parameterized query (pquery) */
	       /*======================================*/
	       
	       /* TODO: This will only work for simple 1-relation queries */

	       CAESAR_TYPE_POINTER ENTRY;
	       CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;
	       CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX;
	       CAESAR_TYPE_INDEX_TABLE_1 QUERY_INDEX;
	       CAESAR_TYPE_NATURAL i;
	       CAESAR_TYPE_TABLE_1 COMBINATION_TABLE;
	       CAESAR_TYPE_POINTER COMBINATION_TABLE_ENTRY;

	       CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_ENTRY_BODY;
	       CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION_ENTRY;
	       RELATION_ENTRY = &RELATION_ENTRY_BODY;

	       /*------------------------------------------------------*/
	       /* Get the relation index of the pquery to be evaluated */
	       /*------------------------------------------------------*/

	       CAESAR_RETRIEVE_I_B_TABLE_1(
		    PQUERY_TABLE,
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX,
		    &ENTRY);

	       CAESAR_DATALOG_GET_PQUERY_TABLE_ENTRY(
		    ENTRY,
		    0,
		    &RELATION_INDEX,
		    &COMBINATION_INDEX); /* For this version the combination index it's not important as we use on only output relations as queries */
	       
	       /*------------------------------------------------------*/
	       /* Get the relation entry of the pquery to be evaluated */
	       /*------------------------------------------------------*/

	       CAESAR_DATALOG_GET_RELATION_BY_INDEX(RELATION_INDEX, RELATION_ENTRY);
	       
	       /*---------------------------------------------------------*/
	       /* Get the combination table of the pquery to be evaluated */
	       /*--------------------------------...----------------------*/

	       COMBINATION_TABLE = RELATION_ENTRY->COMBINATIONS;

	       /*-----------------------*/
	       /* Concretize the pquery */
	       /*-----------------------*/
	       
	       { /* This bloc is for documenting purposes */

		    /***************************************************************************/
		    /* Create the index arrays to iterate over combinations of domain elements */
		    /***************************************************************************/

		    CAESAR_TYPE_INDEX_TABLE_1 *INDEXES;
		    CAESAR_TYPE_INDEX_TABLE_1 *INDEXES_LIMITS;
		    CAESAR_TYPE_BOOLEAN *INDEXES_MASK;
		    CAESAR_TYPE_POINTER CQUERY_TABLE_ENTRY;

		    /* TODO: Confirm the deletion of these structures when unused */
		    
		    CAESAR_CREATE(INDEXES, RELATION_ENTRY->ARITY * sizeof (CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); 
		    CAESAR_CREATE(INDEXES_LIMITS, RELATION_ENTRY->ARITY * sizeof (CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); 
		    CAESAR_CREATE(INDEXES_MASK, RELATION_ENTRY->ARITY * sizeof (CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *); 

		    /*******************************/
		    /* Initialize the index arrays */
		    /*******************************/

		    CAESAR_TYPE_DOMAIN_TABLE_MARK DOMAIN_MARK;

		    for (i = 0; i < RELATION_ENTRY->ARITY; i++) {

			 /* We start by the index 0 */
			 INDEXES[i] = 0;

			 /* With trivial queries there's no need for a mask */
			 INDEXES_MASK[i] = CAESAR_FALSE;

			 /* We search the domain table for the domain's cardinal */
			 CAESAR_RETRIEVE_I_M_TABLE_1(DOMAIN_TABLE, RELATION_ENTRY->DOMAINS[i], (CAESAR_TYPE_POINTER *) &DOMAIN_MARK);
			 INDEXES_LIMITS[i] = DOMAIN_MARK->CARDINAL;
		    }
		    
		    /************************************************/
		    /* Iterate over each concretized query (cquery) */
		    /************************************************/
		    
		    do {

			 /*===========================================*/
			 /* Store the information of the new variable */
			 /*===========================================*/

			 { /* This block is for documenting purposes */

			      /*-----------------------------------------------------*/
			      /* Edit the new combination for the current the cquery */
			      /*-----------------------------------------------------*/

			      CAESAR_DATALOG_CREATE_COMBINATION_TABLE_ENTRY(COMBINATION_TABLE, &COMBINATION_TABLE_ENTRY);

			      for (i = 0; i < RELATION_ENTRY->ARITY; i++) {
			      
				   CAESAR_DATALOG_EDIT_COMBINATION_TABLE_ENTRY(COMBINATION_TABLE_ENTRY, i, RELATION_ENTRY->ARITY, INDEXES[i], CAESAR_FALSE);
				   
			      }
			      
			      /*------------------------------------------------------*/
			      /* Store the new combination for the current the cquery */
			      /*------------------------------------------------------*/

			      CAESAR_DATALOG_ADD_COMBINATION_TABLE_ENTRY(COMBINATION_TABLE, &COMBINATION_INDEX);
			      
			      /*-------------------------------*/
			      /* Edit and store the new cquery */
			      /*-------------------------------*/

			      CAESAR_DATALOG_CREATE_CQUERY_TABLE_ENTRY(CQUERY_TABLE, &CQUERY_TABLE_ENTRY);
			      
			      CAESAR_DATALOG_EDIT_CQUERY_TABLE_ENTRY(CQUERY_TABLE_ENTRY, 0, RELATION_INDEX, COMBINATION_INDEX);
			      
			      CAESAR_DATALOG_ADD_CQUERY_TABLE_ENTRY(CQUERY_TABLE, &QUERY_INDEX);
			      
			 } /* This block is for documenting purposes */
			      
			 /*=========================================*/
			 /* Generate a X2 successor for each cquery */
			 /*=========================================*/

			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 0;
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 2;
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = QUERY_INDEX;

			 /*=========================================*/
			 /* Add to caesar_solve the successor */
			 /*=========================================*/
			 
			 CAESAR_BEGIN_DEBUG(2) {
			      
			      printf("\t");
			      CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			      printf("\n");
			      
			 } CAESAR_END_DEBUG;

			 CAESAR_LOOP(
			      (CAESAR_TYPE_POINTER) NULL,
			      ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
			      VARIABLE_2
			      );

			 GLOBAL_BLOCK_0_VARIABLE_COUNTER++;			 
		       			 
		    } while (
			     CAESAR_DATALOG_ITERATE_OVER_COMBINATIONS(
								      INDEXES,
								      INDEXES_LIMITS,
								      INDEXES_MASK,
								      RELATION_ENTRY->ARITY)
			     );
	       }
	       
	       return;
	  }

	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 2) {

	       /* TODO: COMMENT WELL THIS STUFF */

	       /*====================================*/
	       /* X2 is a concretized query (cquery) */
	       /*====================================*/

	       /* TODO: This will only work for simple 1-relation queries */

	       CAESAR_TYPE_POINTER ENTRY;
	       CAESAR_TYPE_INDEX_TABLE_1 RELATION_INDEX;
	       CAESAR_TYPE_INDEX_TABLE_1 COMBINATION_INDEX;
	       CAESAR_BODY_CPREDICATE_TABLE_ENTRY NEW_ENTRY;
	       CAESAR_TYPE_INDEX_TABLE_1 NEW_ENTRY_INDEX;
	       
	       /*------------------------------------------------------*/
	       /* Get the relation index of the cquery to be evaluated */
	       /*------------------------------------------------------*/

	       CAESAR_RETRIEVE_I_B_TABLE_1(
		    CQUERY_TABLE,
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX,
		    &ENTRY);

	       CAESAR_DATALOG_GET_CQUERY_TABLE_ENTRY(
		    ENTRY,
		    0,
		    &RELATION_INDEX,
		    &COMBINATION_INDEX);

	       /* TODO: Decide correctly the structure of the new_entry variable types and use it coherently */
	       
	       NEW_ENTRY.RELATION = RELATION_INDEX;
	       NEW_ENTRY.COMBINATION = COMBINATION_INDEX;
	       
	       CAESAR_DATALOG_ADD_CPREDICATE_TABLE_ENTRY(&NEW_ENTRY ,&NEW_ENTRY_INDEX);
	       
	       /*=========================================*/
	       /* Generate a X3 successor for each cquery */
	       /*=========================================*/
	       
	       ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 1;
	       ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 1;
	       ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = NEW_ENTRY_INDEX;
	       
	       /*=========================================*/
	       /* Add to caesar_solve the successor */
	       /*=========================================*/

	       CAESAR_BEGIN_DEBUG(2) {
		    
		    printf("\t");
		    CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		    printf("\n");
		    
	       } CAESAR_END_DEBUG;
	       
	       CAESAR_LOOP(
		    (CAESAR_TYPE_POINTER) NULL,
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
		    VARIABLE_2
		    );
	      
	       return;
	  }

	  CAESAR_ASSERT(CAESAR_FALSE);

     } else {
	  
	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 0) {

	       /*=======================================*/
	       /* TRUE doesn't have a successor variabe */
	       /*=======================================*/

	       CAESAR_BEGIN_DEBUG(2) {
		    
		    printf("\t-> END OF BRANCH <-\n");
		    
	       } CAESAR_END_DEBUG;
	       
	       return;
	       
	  }

	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 1) {

	       /*============================================*/
	       /* X3 is a concretized predicate (cpredicate) */
	       /*============================================*/

	       CAESAR_BODY_CPREDICATE_TABLE_ENTRY CPREDICATE_BODY;
	       CAESAR_TYPE_CPREDICATE_TABLE_ENTRY ENTRY;
	       ENTRY = &CPREDICATE_BODY;

	       CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX(
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX,
		    ENTRY);

	       /* It's a fact? */
	       
	       /*------------------------------------------------------*/
	       /* Get the relation entry of the pquery to be evaluated */
	       /*------------------------------------------------------*/

	       CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_ENTRY_BODY;
	       CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION_ENTRY;
	       RELATION_ENTRY = &RELATION_ENTRY_BODY;

	       CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
	       CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
	       COMBINATION = &COMBINATION_BODY;

	       CAESAR_DATALOG_GET_RELATION_BY_INDEX(ENTRY->RELATION, RELATION_ENTRY);

	       if (RELATION_ENTRY->FACTS != NULL) {

		    CAESAR_CREATE(COMBINATION->ARGUMENTS, RELATION_ENTRY->ARITY * sizeof(CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
		    CAESAR_CREATE(COMBINATION->ARGUMENT_IS_VARIABLE, RELATION_ENTRY->ARITY * sizeof(CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *);
		    CAESAR_DATALOG_GET_COMBINATION_BY_INDEX(
			 RELATION_ENTRY->COMBINATIONS,
			 ENTRY->COMBINATION,
			 RELATION_ENTRY->ARITY, COMBINATION);

		    /*********************************/
		    /* The predicate could be a fact */
		    /*********************************/

		    if (CAESAR_DATALOG_IS_FACT(RELATION_ENTRY->FACTS, (CAESAR_TYPE_POINTER) COMBINATION->ARGUMENTS)) {

			/*===========================================*/
			/* Generate a TRUE successor if it is a fact */
			/*===========================================*/

			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 1;
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 0;
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = 0;

			 /*************************************/
			 /* Add to caesar_solve the successor */
			 /*************************************/
			 
			 CAESAR_BEGIN_DEBUG(2) {
			      
			      printf("\t");
			      CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			      printf("\n");
			      
			 } CAESAR_END_DEBUG;
			 
			 CAESAR_LOOP(
			      (CAESAR_TYPE_POINTER) NULL,
			      ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
			      VARIABLE_2
			      );

			 /* This has to be free before returning */
			 CAESAR_DELETE(COMBINATION->ARGUMENTS);
			 CAESAR_DELETE(COMBINATION->ARGUMENT_IS_VARIABLE);     
			 
			 return;
			 
		    } 

/* 		    else { */

/* 			/\*===================================================*\/ */
/* 			/\* Generate a IMPLICITLY a FALSE successor otherwise *\/ */
/* 			/\*===================================================*\/ */

/* 			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 0; */
/* 			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3; */
/* 			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = 0; */

/* 			 CAESAR_BEGIN_DEBUG(2) { */
			      
/* 			      printf("\t"); */
/* 			      CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2); */
/* 			      printf("\n"); */
			      
/* 			 } CAESAR_END_DEBUG; */
			 
/* 		    } */
		    
	       }

	       /* It's extracted from a rule? */
	       
	       CAESAR_TYPE_NATURAL INDEX;
	       CAESAR_TYPE_NATURAL MATCHING_RULES_COUNT;
	       CAESAR_BODY_RULE_I_TABLE_ENTRY RULE_I;
	       CAESAR_BODY_PRULE_TABLE_ENTRY NEW_PRULE;
	       CAESAR_TYPE_INDEX_TABLE_1 NEW_PRULE_INDEX;
	       CAESAR_TYPE_BOOLEAN SUCCESS;

	       MATCHING_RULES_COUNT = CAESAR_DATALOG_COUNT_RULE_I(RELATION_ENTRY->RULES);
	       
	       /* If it has no matching rules (and it wasn't a fact), it is false */
	       if (MATCHING_RULES_COUNT == 0) {

			/*===================================================*/
			/* Generate a IMPLICITLY a FALSE successor otherwise */
			/*===================================================*/

			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 0;
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3;
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = 0;

			 CAESAR_BEGIN_DEBUG(2) {
			      
			      printf("\t");
			      CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			      printf("\n");
			      
			 } CAESAR_END_DEBUG;
		 
	       }

	       for (INDEX = 0; INDEX < MATCHING_RULES_COUNT; INDEX++) {
		    
		    CAESAR_DATALOG_GET_RULE_I_BY_INDEX(RELATION_ENTRY->RULES, INDEX, &RULE_I);

		    NEW_PRULE.CPREDICATE_INDEX = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;
		    NEW_PRULE.RULE_INDEX = RULE_I.RULE_INDEX;

		    SUCCESS = CAESAR_DATALOG_ADD_PRULE_TABLE_ENTRY(&NEW_PRULE, &NEW_PRULE_INDEX);
		    
		    CAESAR_BEGIN_DEBUG(3) {
			 
			 if (!SUCCESS) CAESAR_ASSERT(CAESAR_FALSE);
			 
		    } CAESAR_END_DEBUG;

		    /*=========================*/
		    /* Generate a X4 successor */
		    /*=========================*/

		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 1;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 2;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = NEW_PRULE_INDEX;

		    CAESAR_BEGIN_DEBUG(2) {
			 
			 printf("\t");
			 CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			 printf("\n");
			 
		    } CAESAR_END_DEBUG;
		    
		    CAESAR_LOOP(
			 (CAESAR_TYPE_POINTER) NULL,
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
			 VARIABLE_2
			 );
		    
	       }
	       
	       return;

	  }
	  
	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 2) {

	       /*====================================*/
	       /* X4 is a parameterized rule (prule) */
	       /*====================================*/
	       
	       CAESAR_TYPE_PRULE_TABLE_ENTRY PR_ENTRY;
	       CAESAR_BODY_RULE_TABLE_ENTRY RL_ENTRY_BODY;
	       CAESAR_TYPE_RULE_TABLE_ENTRY RL_ENTRY;

	       
	       CAESAR_BODY_RELATION_TABLE_ENTRY R_ENTRY_BODY;
	       CAESAR_TYPE_RELATION_TABLE_ENTRY R_ENTRY;
	       R_ENTRY = &R_ENTRY_BODY;

	       CAESAR_BODY_RELATION_TABLE_ENTRY TAIL_R_ENTRY_BODY;
	       CAESAR_TYPE_RELATION_TABLE_ENTRY TAIL_R_ENTRY;
	       TAIL_R_ENTRY = &TAIL_R_ENTRY_BODY;

	       CAESAR_BODY_CPREDICATE_TABLE_ENTRY CP_ENTRY_BODY;
	       CAESAR_TYPE_CPREDICATE_TABLE_ENTRY CP_ENTRY;
	       CP_ENTRY = &CP_ENTRY_BODY;

	       CAESAR_TYPE_POINTER HEAD_COMBINATION;

	       RL_ENTRY = &RL_ENTRY_BODY;

	       /* GET PRULE INFO */
	       CAESAR_DATALOG_GET_PRULE_BY_INDEX(
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX,
		    &PR_ENTRY
		    );
	       /* GET RULE INFO */
	       CAESAR_DATALOG_GET_RULE_BY_INDEX(
		    PR_ENTRY->RULE_INDEX,
		    RL_ENTRY
		    );

	       /* GET CPREDICATE INFO */
	       CAESAR_DATALOG_GET_CPREDICATE_BY_INDEX(
		    PR_ENTRY->CPREDICATE_INDEX,
		    CP_ENTRY
		    );

	       /* GET RELATION INFO FOR RULE */
	       CAESAR_DATALOG_GET_RELATION_BY_INDEX(CP_ENTRY->RELATION, R_ENTRY);

	       /* GET COMBINATION INFO */
	       CAESAR_RETRIEVE_I_B_TABLE_1(
		    R_ENTRY->COMBINATIONS,
		    CP_ENTRY->COMBINATION,
		    &HEAD_COMBINATION
		    );

	       /****************/
	       /* Substitution */
	       /****************/
	       
	       CAESAR_TYPE_VARIABLE_MAP_LIST VMAP_CELL;
	       CAESAR_TYPE_VARIABLE_POSITION_LIST VPOSITION_CELL;
	       CAESAR_TYPE_INDEX_TABLE_1 DOMAIN_ELEMENT_INDEX;
	       CAESAR_TYPE_BOOLEAN IS_VARIABLE;
	       
	       for (VMAP_CELL = RL_ENTRY->LINKED_VARIABLE_MAP_LIST;
		    VMAP_CELL != NULL;
		    VMAP_CELL = VMAP_CELL->NEXT
		    ) {
		    
		    CAESAR_DATALOG_GET_COMBINATION_TABLE_ENTRY(
			 HEAD_COMBINATION,
			 VMAP_CELL->MAP->HEAD_POSITION->ARGUMENT_INDEX,
			 R_ENTRY->ARITY,
			 &DOMAIN_ELEMENT_INDEX,
			 &IS_VARIABLE);

		    CAESAR_ASSERT(IS_VARIABLE == CAESAR_FALSE);
		    
		    for (VPOSITION_CELL = VMAP_CELL->MAP->TAIL_POSITION_LIST;
			 VPOSITION_CELL != NULL;
			 VPOSITION_CELL = VPOSITION_CELL->NEXT
			 ) {

			 /* GET RELATION INFO FOR RULE LITERAL */
			 CAESAR_DATALOG_GET_RELATION_BY_INDEX(
			      RL_ENTRY->TAIL_TO_CONCRETIZE_RELATION_INDICES[VPOSITION_CELL->POSITION->LITERAL_INDEX],
			      TAIL_R_ENTRY
			      );

			 CAESAR_DATALOG_EDIT_COMBINATION_TABLE_ENTRY(
			      RL_ENTRY->TAIL_TO_CONCRETIZE + RL_ENTRY->TAIL_TO_CONCRETIZE_DISPLAY[VPOSITION_CELL->POSITION->LITERAL_INDEX],
			      VPOSITION_CELL->POSITION->ARGUMENT_INDEX,
			      TAIL_R_ENTRY->ARITY,
			      DOMAIN_ELEMENT_INDEX /* TODO
				 VPOSITION_CELL->POSITION->LITERAL_INDEX */,
			      CAESAR_FALSE /* It is not a variable */
			      );
			 
			 
		    }

	       }

	       /******************/
	       /* Concretization */
	       /******************/

	       CAESAR_TYPE_INDEX_TABLE_1 *INDEXES;
	       CAESAR_TYPE_INDEX_TABLE_1 *INDEXES_LIMITS;
	       CAESAR_TYPE_BOOLEAN *INDEXES_MASK;
	       
	       /* TODO: Confirm the deletion of these structures when unused */
	       
	       CAESAR_CREATE(INDEXES, RL_ENTRY->FREE_VARIABLE_NUMBER * sizeof (CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); 
	       CAESAR_CREATE(INDEXES_LIMITS, RL_ENTRY->FREE_VARIABLE_NUMBER * sizeof (CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *); 
	       CAESAR_CREATE(INDEXES_MASK, RL_ENTRY->FREE_VARIABLE_NUMBER * sizeof (CAESAR_TYPE_BOOLEAN), CAESAR_TYPE_BOOLEAN *); 
	       
	       /*******************************/
	       /* Initialize the index arrays */
	       /*******************************/

	       CAESAR_TYPE_NATURAL i;

	       for (i = 0; i < RL_ENTRY->FREE_VARIABLE_NUMBER; i++) {
		    
		    /* We start by the index 0 */
		    INDEXES[i] = 0;
		    
		    /* With trivial queries there's no need for a mask */
		    INDEXES_MASK[i] = CAESAR_FALSE;
		    
		    INDEXES_LIMITS[i] = RL_ENTRY->FREE_VARIABLE_MAP_ARRAY[i]->CARDINAL;
	       }
	       
	       /**********************************************/
	       /* Iterate over each concretized rule (crule) */
	       /**********************************************/

	       /*

	       For each concretization
	        For each variable
	         For each position of the variable
	          Edit the combination
		Add the new combinations  

	       */

	       CAESAR_TYPE_NATURAL j;
	       CAESAR_TYPE_INDEX_TABLE_1 NEW_COMBINATION_INDEX;
	       CAESAR_TYPE_BOOLEAN DUPLICATED;

	       CAESAR_BODY_CPREDICATE_TABLE_ENTRY NEW_CPREDICATE;
	       CAESAR_TYPE_INDEX_TABLE_1 NEW_CPREDICATE_INDEX;

	       CAESAR_BODY_CRULE_TABLE_ENTRY NEW_CRULE;
	       CAESAR_TYPE_INDEX_TABLE_1 NEW_CRULE_INDEX;

/* TODO: Optimize this just doing it once for every bes resolution */
	       

	       
	       NEW_CRULE.RULE_INDEX = PR_ENTRY->RULE_INDEX;
	       CAESAR_CREATE(NEW_CRULE.CPREDICATE_INDICES,
			     RL_ENTRY->LENGTH * sizeof (CAESAR_TYPE_INDEX_TABLE_1),
			     CAESAR_TYPE_INDEX_TABLE_1 *); 

	       do {
		    for (i = 0; i < RL_ENTRY->FREE_VARIABLE_NUMBER; i++){
			 for (
			      VPOSITION_CELL = RL_ENTRY->FREE_VARIABLE_MAP_ARRAY[i]->TAIL_POSITION_LIST;
			      VPOSITION_CELL != NULL;
			      VPOSITION_CELL = VPOSITION_CELL->NEXT
			      ) {
			      
			      CAESAR_DATALOG_GET_RELATION_BY_INDEX(
				   RL_ENTRY->TAIL_TO_CONCRETIZE_RELATION_INDICES[VPOSITION_CELL->POSITION->LITERAL_INDEX],
				   TAIL_R_ENTRY
				   );

			      CAESAR_DATALOG_EDIT_COMBINATION_TABLE_ENTRY(
				   RL_ENTRY->TAIL_TO_CONCRETIZE + RL_ENTRY->TAIL_TO_CONCRETIZE_DISPLAY[VPOSITION_CELL->POSITION->LITERAL_INDEX],
				   VPOSITION_CELL->POSITION->ARGUMENT_INDEX,
				   TAIL_R_ENTRY->ARITY,
				   INDEXES[i],
				   CAESAR_FALSE
				   );
			 }
		    }
		    
		    /************/
		    /* Addition */
		    /************/
		    
		    CAESAR_BEGIN_DEBUG(4) {
			 
			 printf("[DEBUGGING] RL_ENTRY->LENGTH = %d", RL_ENTRY->LENGTH);
			 
		    } CAESAR_END_DEBUG;

		    for (j = 0; j < RL_ENTRY->LENGTH; j++) {
			 /* GET RELATION INFO FOR RULE LITERAL */

			 CAESAR_DATALOG_GET_RELATION_BY_INDEX(
			      RL_ENTRY->TAIL_TO_CONCRETIZE_RELATION_INDICES[j],
			      TAIL_R_ENTRY
			      );
			 
			 DUPLICATED = CAESAR_DATALOG_ADD_COMBINATION_TABLE_ENTRY2(
			      TAIL_R_ENTRY->COMBINATIONS,
			      RL_ENTRY->TAIL_TO_CONCRETIZE + RL_ENTRY->TAIL_TO_CONCRETIZE_DISPLAY[j],
			      TAIL_R_ENTRY->ARITY,
			      &NEW_COMBINATION_INDEX
			      );
			 
			 NEW_CPREDICATE.RELATION = RL_ENTRY->TAIL_TO_CONCRETIZE_RELATION_INDICES[j];
			 NEW_CPREDICATE.COMBINATION = NEW_COMBINATION_INDEX;
			 
			 CAESAR_DATALOG_ADD_CPREDICATE_TABLE_ENTRY(&NEW_CPREDICATE, &NEW_CPREDICATE_INDEX);

			 NEW_CRULE.CPREDICATE_INDICES[j] = NEW_CPREDICATE_INDEX;
			 
			 CAESAR_BEGIN_DEBUG(4) {

			      printf("[DEBUGGING] NEW COMBINATION %s\n", DUPLICATED ? "DUPLICATED!!!!" : "OK");
			      printf("[DEBUGGING] IN CONCRETIZATION OF RULE %d\n", PR_ENTRY->RULE_INDEX);
			      printf("[DEBUGGING] %s COMBINATIONS\n", TAIL_R_ENTRY->NAME);
			      CAESAR_DATALOG_PRINT_COMBINATION_TABLE(stdout, TAIL_R_ENTRY->COMBINATIONS, TAIL_R_ENTRY->ARITY);
			 
			 } CAESAR_END_DEBUG;
		    }

		    CAESAR_DATALOG_ADD_CRULE_TABLE_ENTRY(&NEW_CRULE, &NEW_CRULE_INDEX);
		    
		    /*=========================================*/
		    /* Generate a X5 successor for each cquery */
		    /*=========================================*/
		    
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 1;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = NEW_CRULE_INDEX;
		    
		    /*===================================*/
		    /* Add to caesar_solve the successor */
		    /*===================================*/

		    CAESAR_BEGIN_DEBUG(2) {
			 
			 printf("\t");
			 CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			 printf("\n");
			 
		    } CAESAR_END_DEBUG;

		    CAESAR_LOOP(
			 (CAESAR_TYPE_POINTER) NULL,
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
			 VARIABLE_2
			 );

	       } while (CAESAR_DATALOG_ITERATE_OVER_COMBINATIONS(
			     INDEXES,
			     INDEXES_LIMITS,
			     INDEXES_MASK,
			     RL_ENTRY->FREE_VARIABLE_NUMBER)
		    );

	       return;

	  }

	  if (((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->TYPE == 3) {

	       /*==================================*/
	       /* X5 is a concretized rule (crule) */
	       /*==================================*/

	       CAESAR_TYPE_NATURAL i;

	       /*-----------------*/
	       /* Memory dealing  */
	       /*-----------------*/
	       
	       CAESAR_BODY_CRULE_TABLE_ENTRY CRL_ENTRY_BODY;
	       CAESAR_TYPE_CRULE_TABLE_ENTRY CRL_ENTRY;
	       CRL_ENTRY = &CRL_ENTRY_BODY;

	       CAESAR_BODY_RULE_TABLE_ENTRY RL_ENTRY_BODY;
	       CAESAR_TYPE_RULE_TABLE_ENTRY RL_ENTRY;
	       RL_ENTRY = &RL_ENTRY_BODY;

	       /*-------------*/
	       /* Get X5 info */
	       /*-------------*/

	       CAESAR_DATALOG_GET_CRULE_BY_INDEX(
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX,
		    CRL_ENTRY
		    );

	       CAESAR_DATALOG_GET_RULE_BY_INDEX(
		    CRL_ENTRY->RULE_INDEX,
		    RL_ENTRY
		    );
	       
	       /*-------------------------------*/
	       /* Generate X3 successors for X5 */
	       /*-------------------------------*/
	       
	       for (i = 0; i < RL_ENTRY->LENGTH; i++) {

		    /*************************/
		    /* Build an X3 successor */
		    /*************************/
	       

		    /*********  TODO : CHECK X3 GENERATED  ***********/

		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX = 1;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 1;
		    ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX = CRL_ENTRY->CPREDICATE_INDICES[i];
		    
		    /*************************************/
		    /* Add to caesar_solve the successor */
		    /*************************************/
	       
		    CAESAR_BEGIN_DEBUG(2) {
			 
			 printf("\t");
			 CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			 printf("\n");
			 
		    } CAESAR_END_DEBUG;

		    CAESAR_LOOP(
			 (CAESAR_TYPE_POINTER) NULL,
			 ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX,
			 VARIABLE_2
			 );
	       }

	       return;
	  }
	
	  CAESAR_ASSERT(CAESAR_FALSE);
     }
     
     CAESAR_ASSERT(CAESAR_FALSE);
}

/*==============================================================================*/
/* Construction of the BES with its associate data structures */

/* Outer variables defined in the scanner */
extern FILE *yyin;
extern FILE *yyout;

CAESAR_TYPE_BOOLEAN CAESAR_READ_DATALOG_1(CAESAR_TYPE_SOLVE_1 *DATALOG_BES, CAESAR_TYPE_BOOLEAN SATISFABILITY_MODE, CAESAR_TYPE_STRING DATALOG_FILENAME)
{
     CAESAR_TYPE_FILE BES_FILE;
     CAESAR_TYPE_BOOLEAN SYNTAX_ERROR;

     /* Opening of the datalog file */
     if ((BES_FILE = fopen(DATALOG_FILENAME, "r")) == NULL) {
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
     if (SYNTAX_ERROR) {
	  (*DATALOG_BES) = NULL;
	  CAESAR_ERROR("Syntax error");
     }

     CAESAR_DATALOG_BUILD_DOMAIN_TABLE();
     CAESAR_DATALOG_BUILD_VARIABLE_TABLE(); /* Must exist before
					     * building the rule table */

     CAESAR_DATALOG_BUILD_RELATION_TABLE();

     CAESAR_DATALOG_BUILD_CRULE_TABLE();
     CAESAR_DATALOG_BUILD_RULE_TABLE();
     
     CAESAR_DATALOG_BUILD_PQUERY_TABLE(); /* X1 */
     CAESAR_DATALOG_BUILD_CQUERY_TABLE(); /* X2 */
     CAESAR_DATALOG_BUILD_CPREDICATE_TABLE(); /* X3 */
     CAESAR_DATALOG_BUILD_PRULE_TABLE(); /* X4*/

     CAESAR_BEGIN_DEBUG(4) {
	  
	  /* TODO: TABLE DEBUGGING */
	  CAESAR_DATALOG_PRINT_PQUERY_TABLE(stdout, PQUERY_TABLE);
	  /* TODO: DEBUGGING CODE OF CAESAR_DATALOG_MAX_RULE_BODY_SIZE */
	  printf("\nMax. Size of a rule body: %d\n", CAESAR_DATALOG_MAX_RULE_BODY_SIZE);
	  
     } CAESAR_END_DEBUG;

     /***********************/
     /* CREATION OF THE BES */
     /***********************/

     if (SATISFABILITY_MODE)
       {
	 CAESAR_CREATE_SOLVE_1(
			       DATALOG_BES,
			       2, /* Number of blocks */
			       CAESAR_SATISFABILITY_BLOCK_SIGN,
			       CAESAR_SATISFABILITY_BLOCK_UNIQUE_RESOLUTION,
			       CAESAR_SATISFABILITY_BLOCK_SOLVE_MODE,
			       CAESAR_BLOCK_VARIABLE_AREA,
			       CAESAR_BLOCK_LIMIT_SIZE,
			       CAESAR_BLOCK_HASH_SIZE,
			       CAESAR_BLOCK_PRIME,
			       CAESAR_VARIABLE_KIND,
			       CAESAR_VARIABLE_COMPARE,
			       CAESAR_VARIABLE_HASH,
			       CAESAR_VARIABLE_PRINT,
			       CAESAR_VARIABLE_ITERATE,
			       NULL /* For future extensions of the
				       solve_1 library */
			       );
       }
     else
       {
	 CAESAR_CREATE_SOLVE_1(
			       DATALOG_BES,
			       2, /* Number of blocks */
			       CAESAR_BLOCK_SIGN,
			       CAESAR_BLOCK_UNIQUE_RESOLUTION,
			       CAESAR_BLOCK_SOLVE_MODE,
			       CAESAR_BLOCK_VARIABLE_AREA,
			       CAESAR_BLOCK_LIMIT_SIZE,
			       CAESAR_BLOCK_HASH_SIZE,
			       CAESAR_BLOCK_PRIME,
			       CAESAR_VARIABLE_KIND,
			       CAESAR_VARIABLE_COMPARE,
			       CAESAR_VARIABLE_HASH,
			       CAESAR_VARIABLE_PRINT,
			       CAESAR_VARIABLE_ITERATE,
			       NULL /* For future extensions of the
				       solve_1 library */
			       );
       }

     if (*DATALOG_BES != NULL)
	  return CAESAR_TRUE;
     else
	  return CAESAR_FALSE;
}

CAESAR_TYPE_BOOLEAN CAESAR_COMPUTE_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_BOOLEAN *SOLUTION_EXISTS)
{
     CAESAR_TYPE_BOOLEAN_VARIABLE X0;
     CAESAR_TYPE_BOOLEAN ANSWER;
     CAESAR_TYPE_ERROR_SOLVE_1 ERROR;

     /* Build the initial variable */

     X0.BLOCK_INDEX = 0;
     X0.TYPE = 0;
     X0.ENTRY_INDEX = 0;
     
     /* Compute the solution to the initial variable */

     GLOBAL_BLOCK_0_VARIABLE_COUNTER = 1;

     ANSWER = CAESAR_COMPUTE_SOLVE_1(
	  DATALOG_BES, /* BES */
	  0, /* BLOCK */
	  (CAESAR_TYPE_POINTER) &X0 /* initial variable */
	  );

     /* Check the presence of errors in the computation of the BES */
     
     ERROR = CAESAR_STATUS_COMPUTE_SOLVE_1(DATALOG_BES, 0);
     
     switch (ERROR) {
     case CAESAR_NONE_SOLVE_1:
	  break;
     default:
	  printf("RESOLUTION INCOMPLETE: ");
	  switch (ERROR) {
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
     
     /* Check the answer of the BES and return its value */
     
     if (ANSWER) {
	  *SOLUTION_EXISTS = CAESAR_TRUE;
	  printf("TRUE\n");
     } else {
	  *SOLUTION_EXISTS = CAESAR_FALSE;
	  printf("FALSE\n");
     }

     CAESAR_BEGIN_DEBUG(1) {

     fprintf(stdout, "Número de variables del bloque 1: %d\n",GLOBAL_BLOCK_0_VARIABLE_COUNTER);

     } CAESAR_END_DEBUG;

     /* Return: everything is ok */ 
     
     return CAESAR_TRUE;
}

CAESAR_TYPE_BOOLEAN CAESAR_ITERATE_SOLUTION_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES)
{
     CAESAR_ERROR("Function CAESAR_ITERATE_SOLUTION_DATALOG_1 not implemented");
     return CAESAR_FALSE;
}


CAESAR_TYPE_BOOLEAN CAESAR_WRITE_SOLUTION_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_FILE FILE)
{
     /***************************************************************/
     /* */
     /***************************************************************/

     CAESAR_TYPE_NATURAL RELATION_COUNT;
     
     RELATION_COUNT = CAESAR_DATALOG_GET_RELATION_COUNT();

     CAESAR_TYPE_NATURAL *DOMAIN_COUNTER;
     CAESAR_TYPE_NATURAL DOMAIN_COUNT;

     DOMAIN_COUNT = CAESAR_DATALOG_GET_DOMAIN_COUNT();

     /* Allocate memory */

     CAESAR_CREATE(OUTPUT_FILES, RELATION_COUNT * sizeof (CAESAR_TYPE_FILE), CAESAR_TYPE_FILE *);

     CAESAR_CREATE(OUTPUT_FILES_NAMES, RELATION_COUNT * sizeof (CAESAR_TYPE_STRING), CAESAR_TYPE_STRING *);

     CAESAR_CREATE(DOMAIN_COUNTER, DOMAIN_COUNT * sizeof (CAESAR_TYPE_NATURAL), CAESAR_TYPE_NATURAL *);

     /***************************************************************/
     /**/
     /***************************************************************/

     if (FILE != NULL) {
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

     for (i = 0; i < RELATION_COUNT; i++) {

	  /* Get relation info */

	  CAESAR_DATALOG_GET_RELATION_BY_INDEX(i, RELATION);
	  
	  /* If it is an output relation we open its file */
	  
	  if (RELATION->FACTS == NULL) {
	    
	    ++output_relations_number;
	    	    
	    /* Initialize domain counter */
	    
	    for (k = 0; k < DOMAIN_COUNT; k++)
	      DOMAIN_COUNTER[k] = 0;
	    
	    /* Allocate memory and build the new filename */
	    
	    CAESAR_CREATE(FILENAME, strlen(RELATION->NAME) + strlen(DATALOG_SOLVE_OUTPUT_TUPLES_SUFIX) + 1, CAESAR_TYPE_STRING);
	    strcpy(FILENAME, RELATION->NAME);
	    strcat(FILENAME, DATALOG_SOLVE_OUTPUT_TUPLES_SUFIX);
	    
	    /* Open the new output file */
	    
	    if ((OUTPUT_FILES[i] = fopen(FILENAME, "w")) == NULL) {
	      CAESAR_ERROR("File ``%s'' (output tuples) is imposible to create", FILENAME);
	    }
	    
	    /* Write the header of the output tuples file */
	    
	    fprintf(OUTPUT_FILES[i], "# ");
	    
	    for (k = 0; k < RELATION->ARITY; k++) {
	      if (k != 0)
		fprintf(OUTPUT_FILES[i], " ");
	      CAESAR_DATALOG_GET_DOMAIN_BY_INDEX(RELATION->DOMAINS[k], DOMAIN);
	      
	      ENCODING_BITS = CAESAR_BITS_TO_ENCODE_ELEMENTS(DOMAIN->CARDINAL);
	      
	      fprintf(OUTPUT_FILES[i], "%s%d:%d", DOMAIN->NAME, DOMAIN_COUNTER[k], ENCODING_BITS);
	      DOMAIN_COUNTER[k]++;
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

     CAESAR_ITERATE_STABLE_VARIABLE_SOLVE_1(
	  DATALOG_BES,
	  0,
	  (CAESAR_TYPE_POINTER) &X0,
	  &VALUE,
	  STABLE_VARIABLE_ITERATE_IN_OUTPUT_RELATION_MODE);

     /***************************************************************/
     /**/
     /***************************************************************/

     /* Close file descriptors */

     for (i = 0; i < RELATION_COUNT; i++) {

	  if (OUTPUT_FILES[i] != NULL)
	       fclose(OUTPUT_FILES[i]);

     }

     /* Print messages to the output informing of the solution */
     
     if (output_relations_number == 0)
       return CAESAR_FALSE; /* TODOD: CHECK WHERE TO PUT THIS PROPERLY*/

     fprintf(stdout, "(consult solution in file%s ", output_relations_number > 1 ? "s" : "");

     for (i = 0; i < RELATION_COUNT; i++)
       {
	 if (OUTPUT_FILES_NAMES[i] != NULL)
	   {
	     --output_relations_number;
	     fprintf(stdout, "``%s''%s", OUTPUT_FILES_NAMES[i], output_relations_number != 0 ? ", " : ")\n");
	   }
       }     

     /* Free memory */

     CAESAR_DELETE(OUTPUT_FILES);

     for (i = 0; i < RELATION_COUNT; i++)
       {

	 if (OUTPUT_FILES_NAMES[i] != NULL)
	   CAESAR_DELETE(OUTPUT_FILES_NAMES[i]);
	 
       }
     
     CAESAR_DELETE(OUTPUT_FILES_NAMES);
     
     CAESAR_DELETE(DOMAIN_COUNTER);
     
     /* Return: everything is ok */
     
     return CAESAR_TRUE;
}

/*------------------------------------------------------------------------------*/
/* Write the BES explored - during the resolution - into a file */

void CAESAR_WRITE_BES_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_FILE FILE)
{
     CAESAR_TYPE_BOOLEAN_VARIABLE X0;

     /* Build the initial variable */
     
     X0.BLOCK_INDEX = 0;
     X0.TYPE = 0;
     X0.ENTRY_INDEX = 0;

     CAESAR_WRITE_SOLVE_1(
	  DATALOG_BES,
	  0,
	  (CAESAR_TYPE_POINTER) &X0,
	  FILE,
	  DATALOG_SOLVE_DIAGNOSTIC_BES);
}

/*------------------------------------------------------------------------------*/
/* Write statistical information about the BES resolution into a file */

void CAESAR_WRITE_STATISTICS_DATALOG_1(CAESAR_TYPE_SOLVE_1 DATALOG_BES, CAESAR_TYPE_FILE FILE)
{
     fprintf(FILE, 
	     "\n"
	     "Statistical information of the underlying BES\n"
	     "---------------------------------------------\n");

     CAESAR_FORMAT_SOLVE_1(DATALOG_BES, 1);
     CAESAR_PRINT_SOLVE_1(FILE, DATALOG_BES);

     fprintf(FILE, 
	     "\n");
}
