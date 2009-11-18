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

#include "datalog_common.h"
#include "string.h"

/*------------------------------------------------------------------------------*/
/* String copy */

CAESAR_TYPE_STRING CAESAR_COPY_STRING(CAESAR_TYPE_STRING CAESAR_STRING)
{
  CAESAR_TYPE_STRING CAESAR_STRING_COPY;

  /* Allocation of memory for the copied string */
  CAESAR_CREATE(CAESAR_STRING_COPY, strlen(CAESAR_STRING) + 1, CAESAR_TYPE_STRING);
  if (CAESAR_STRING_COPY == NULL)
    CAESAR_ERROR("Memory shortage for string");

  /* Copy of the string */
  CAESAR_STRING_COPY = strcpy(CAESAR_STRING_COPY, CAESAR_STRING);

  return CAESAR_STRING_COPY;
}

/*------------------------------------------------------------------------------*/
/* Calculates the number of bits needed to encode each element in a
 * particularly sized set of elements */

CAESAR_TYPE_NATURAL CAESAR_BITS_TO_ENCODE_ELEMENTS(CAESAR_TYPE_NATURAL NUMBER_OF_ELEMENTS)
{
     CAESAR_TYPE_NATURAL FLOOR_LOG2;
     CAESAR_TYPE_NATURAL AUX;

     FLOOR_LOG2 = 0;
     AUX = NUMBER_OF_ELEMENTS;
     
     for (FLOOR_LOG2 = 0; AUX > 1; FLOOR_LOG2++)
     {
	  AUX >>= 1;
     }

     if ( (1 << FLOOR_LOG2) < (NUMBER_OF_ELEMENTS) )
     {
	  return FLOOR_LOG2 + 1;
     }
     else
     {
	  return FLOOR_LOG2;
     }     
}

/*------------------------------------------------------------------------------*/
/* Counts the number of lines in a file
 * 
 * - If the files doesn't end with a newline, that nonexistent newline is counted 
 */
CAESAR_TYPE_NATURAL COUNT_NUMBER_OF_LINES(CAESAR_TYPE_FILE FILE)
{
	int READ_CHAR;
	CAESAR_TYPE_BOOLEAN NEWLINE_FOUND;
	CAESAR_TYPE_NATURAL NUMBER_OF_LINES;
	
	NEWLINE_FOUND = CAESAR_FALSE;
	NUMBER_OF_LINES = 0;
	
	do {
		READ_CHAR = getc(FILE);

		switch (READ_CHAR) {
		case '\n':
			NUMBER_OF_LINES++;
			NEWLINE_FOUND = CAESAR_TRUE;
			break;
		case EOF:
			if (!NEWLINE_FOUND)
				NUMBER_OF_LINES++;
			break;
		default:
			NEWLINE_FOUND = CAESAR_FALSE;				
		}
			
	} while (READ_CHAR != EOF);
	
	return NUMBER_OF_LINES;
}

/*------------------------------------------------------------------------------*/
/* TODO: Function for debugging the creation of a table */

void DEBUG_CREATE_TABLE(CAESAR_TYPE_STRING NAME, CAESAR_TYPE_AREA_1 BASE, CAESAR_TYPE_AREA_1 MARK)
{
     CAESAR_ASSERT(BASE != ((CAESAR_TYPE_AREA_1) NULL));
  
  fprintf
    (
     stdout,
     "[DBG] CREATION OF A TABLE NAMED %s:\n"
     "[DBG] \tBASE\n"
     "[DBG] \t\tAREA = @%x\n"
     "[DBG] \t\tALIGNMENT = %d\n"
     "[DBG] \t\tSIZE = %d\n"
     "[DBG] \tMARK\n"
     "[DBG] \t\tAREA = @%x\n"
     "[DBG] \t\tALIGNMENT = %d\n"
     "[DBG] \t\tSIZE = %d \n",
     NAME,
     BASE, CAESAR_ALIGNMENT_AREA_1(BASE), CAESAR_SIZE_AREA_1(BASE),
     MARK, CAESAR_ALIGNMENT_AREA_1(MARK), CAESAR_SIZE_AREA_1(MARK)
     );
}
