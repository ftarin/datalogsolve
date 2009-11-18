/******************************************************************************
 *                    C A E S A R _ D A T A L O G _ 1               
 *-----------------------------------------------------------------------------
 *   Technical University of Valencia
 *   DSIC / ELP
 *   Camino de Vera s/n
 *   46022 Valencia
 *   SPAIN
 *-----------------------------------------------------------------------------
 *   Module             :       caesar_datalog_read_write.h
 *   Authors            :       Maria ALPUENTE, Marco FELIU, Christophe JOUBERT
 *                              and Alicia VILLANUEVA
 *   Version            :       1.4
 *   Date               :       07/12/03 10:52:36
 *****************************************************************************/

#include "datalog_common.h"
#include "datalog_solve.h"

/*============================================================================*/
/* datalog_solve options */

CAESAR_TYPE_BOOLEAN caesar_output_tuples = CAESAR_FALSE;
CAESAR_TYPE_BOOLEAN caesar_output_bes = CAESAR_FALSE;
CAESAR_TYPE_BOOLEAN caesar_output_statistics = CAESAR_FALSE;
CAESAR_TYPE_STRING caesar_output_bes_file_name = NULL;

/*----------------------------------------------------------------------------*/
/* Parse a command line for options and arguments */

void caesar_read_command_line(int argc, char *argv[])
{
     --argc;
     ++argv;

     /* parsing command-line options */
     while ((argc != 0) && (argv[0][0] == '-'))
       {
	    if (strcmp (argv[0], "-tuples") == 0)
	      {
		caesar_output_tuples = CAESAR_TRUE;
		--argc;
		++argv;
	      }
	    else if (strcmp (argv[0], "-bes") == 0)
	      {
		caesar_output_bes = CAESAR_TRUE;
		--argc;
		++argv;
		if (argc > 0 && argv[0][0] != '-')
		  {
		    caesar_output_bes_file_name = argv[0];
		    --argc;
		    ++argv;
		  }
	      }
	    else if (strcmp (argv[0], "-stat") == 0)
	      {
		caesar_output_statistics = CAESAR_TRUE;
		--argc;
		++argv;
	      }
	    else
	      {
		CAESAR_ERROR ("invalid argument %s in command line",
			      argv[0]);
	      }
       }
     if (argc != 0)
	  CAESAR_ERROR ("invalid argument %s in command line", argv[0]);
}

/*----------------------------------------------------------------------------*/


int main (int argc, char ** argv)
{
     CAESAR_TYPE_SOLVE_1 DATALOG_BES;
     CAESAR_TYPE_STRING DATALOG_FILENAME;
     CAESAR_TYPE_BOOLEAN ANSWER;

     printf("\n");


     if (argc < 2) {
       CAESAR_ERROR("Usage: " DATALOG_SOLVE_EXECUTABLE_NAME " [-bes [file[.bes]]] [-tuples] file[.datalog]");
     }

     caesar_read_command_line(argc-1,argv);

     DATALOG_FILENAME = argv[argc-1];
     
     /****************************/
     /* READ THE DATALOG PROGRAM */
     /****************************/

     CAESAR_TYPE_BOOLEAN SATISFABILITY_MODE;

     SATISFABILITY_MODE = !caesar_output_tuples;

     CAESAR_READ_DATALOG_1(
			   &DATALOG_BES,
			   SATISFABILITY_MODE,
			   DATALOG_FILENAME);
     if (DATALOG_BES == NULL)
	  CAESAR_ERROR("Error while loading datalog file %s", DATALOG_FILENAME);

     /*****************************************************/
     /* DEFINITION AND RESOLUTION OF THE INITIAL VARIABLE */
     /*****************************************************/

     if (!CAESAR_COMPUTE_DATALOG_1(DATALOG_BES, &ANSWER))
	  CAESAR_ERROR("Error while computing the datalog program");
	  
     /************************/
     /* PRINT THE RESOLT BES */
     /************************/
     
     if (caesar_output_bes == CAESAR_TRUE)
       {
	 CAESAR_TYPE_FILE DATALOG_BES_FILE;
	 
	 if (caesar_output_bes_file_name == NULL)
	   {
	     CAESAR_CREATE(caesar_output_bes_file_name, strlen(DATALOG_SOLVE_BES_SUFIX) + strlen(DATALOG_FILENAME) + 1, CAESAR_TYPE_STRING);
	     strcpy(caesar_output_bes_file_name, DATALOG_FILENAME);
	     strcat(caesar_output_bes_file_name, DATALOG_SOLVE_BES_SUFIX);
	   }

	 if ((DATALOG_BES_FILE = fopen(caesar_output_bes_file_name, "w")) == NULL)
	   {
	     CAESAR_ERROR("File ``%s'' is unwritable", caesar_output_bes_file_name);
	   }
	 
	 CAESAR_WRITE_BES_DATALOG_1(
				    DATALOG_BES,
				    DATALOG_BES_FILE);
       }

     /***************************/
     /* PRINT THE OUTPUT TUPLES */
     /***************************/

     if (caesar_output_tuples == CAESAR_TRUE) 
       {
	 if (!CAESAR_WRITE_SOLUTION_DATALOG_1(DATALOG_BES, NULL))
	   {
	     CAESAR_ERROR("Error while writing the solutions");
	   }
	 
       }

     /**********************/
     /* DISPLAY STATISTICS */
     /**********************/

     if (caesar_output_statistics == CAESAR_TRUE)
       {
	 CAESAR_WRITE_STATISTICS_DATALOG_1(DATALOG_BES, stdout);
       }


     exit(0);
}
