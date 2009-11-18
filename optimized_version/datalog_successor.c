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
#include "datalog_successor.h"
#include "datalog_common.h"
#include "datalog_relation_table.h"
#include "datalog_basic_data_structures.h"
#include "datalog_pquery_table.h"
#include "datalog_predicate_table.h"
#include "datalog_combination_table.h"
#include "datalog_domain_table.h"
#include "datalog_fact_table.h"
#include "datalog_rule_table.h"
#include "datalog_prule_table.h"
#include "datalog_crule_table.h"

void CAESAR_DATALOG_COMPUTE_X0_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	CAESAR_TYPE_INDEX_TABLE_1 MAXIMUM_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 INDEX;

	/* Iterate over each parameterized query (pquery) */

	MAXIMUM_INDEX = PQUERY_TABLE_GET_ENTRY_COUNT();

	for (INDEX = 0; INDEX < MAXIMUM_INDEX; INDEX++)
	{
		/* Generate a X1 successor for each pquery */

		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 0;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 1;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= INDEX;

		CAESAR_BEGIN_DEBUG(1)
		{
			printf(" ");
			CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		}CAESAR_END_DEBUG;

		CAESAR_LOOP(
				(CAESAR_TYPE_POINTER) NULL,
				caesar_output_tuples == CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
						: 0, VARIABLE_2);
	}
	CAESAR_BEGIN_DEBUG(1)
	{
		printf("\n");
	}CAESAR_END_DEBUG;
}

void CAESAR_DATALOG_COMPUTE_X1_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	CAESAR_TYPE_INDEX_TABLE_1 PQUERY_INDEX;

	PQUERY_INDEX = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;

	/* Generate a X2 successor for each X1 */

	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			= 0;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 2;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
			= PQUERY_INDEX;

	CAESAR_BEGIN_DEBUG(1)
	{
		printf(" ");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		printf("\n");
	}CAESAR_END_DEBUG;

	CAESAR_LOOP( (CAESAR_TYPE_POINTER) NULL, caesar_output_tuples
			== CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			: 0, VARIABLE_2);
}

void CAESAR_DATALOG_COMPUTE_X2_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	/* Declarations */
	CAESAR_TYPE_NATURAL i;
	CAESAR_TYPE_INDEX_TABLE_1 PCQUERY_INDEX;
	CAESAR_TYPE_INDEX_TABLE_1 PREDICATE_INDEX;

	/* TODO: Declarations to improve */
	CAESAR_TYPE_PQUERY_TABLE_ENTRY PQUERY;
	CAESAR_BODY_PQUERY_TABLE_ENTRY PQUERY_BODY;
	PQUERY = &PQUERY_BODY;
	CAESAR_BODY_PREDICATE_TABLE_ENTRY PREDICATE_BODY;
	CAESAR_TYPE_PREDICATE_TABLE_ENTRY PREDICATE;
	PREDICATE = &PREDICATE_BODY;

	/* X1 */
	PCQUERY_INDEX = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;
	PQUERY_TABLE_GET_ENTRY_BY_INDEX(PCQUERY_INDEX, PQUERY);

	/* TODO: We should iterate for each predicate of the pcpquery */
	for (i = 0; i < 1; i++)
	{
		//		PQUERY_GET_VALUE_AT(PCQUERY, (CAESAR_TYPE_NATURAL) 0, &(PREDICATE->RELATION),
		//				&(PREDICATE->COMBINATION));
		PREDICATE->RELATION = PQUERY->RELATION_INDICES[i];
		PREDICATE->COMBINATION = PQUERY->COMBINATION_INDICES[i];

		PREDICATE_TABLE_ADD_ENTRY(PREDICATE, &PREDICATE_INDEX);

		/* Generate a X3 successor for each X2 */

		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 1;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 0;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= PREDICATE_INDEX;

		CAESAR_BEGIN_DEBUG(1)
		{
			printf(" ");
			CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		}CAESAR_END_DEBUG;

		CAESAR_LOOP(
				(CAESAR_TYPE_POINTER) NULL,
				caesar_output_tuples == CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
						: 0, VARIABLE_2);
	}

	CAESAR_BEGIN_DEBUG(1)
	{
		printf("\n");
	}CAESAR_END_DEBUG;
}

void CAESAR_DATALOG_COMPUTE_X3_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	CAESAR_TYPE_INDEX_TABLE_1 PREDICATE_INDEX;

	PREDICATE_INDEX = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;

	/* Generate a X4 successor for a X3 */

	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			= 2;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 0;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
			= PREDICATE_INDEX;

	CAESAR_LOOP( (CAESAR_TYPE_POINTER) NULL, caesar_output_tuples
			== CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			: 0, VARIABLE_2);

	CAESAR_BEGIN_DEBUG(1)
	{
		printf(" ");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
	}CAESAR_END_DEBUG;

	/* Generate a X5 successor a X3 */

	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			= 1;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 1;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
			= PREDICATE_INDEX;

	CAESAR_BEGIN_DEBUG(1)
	{
		printf(" ");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		printf("\n");
	}CAESAR_END_DEBUG;

	CAESAR_LOOP( (CAESAR_TYPE_POINTER) NULL, caesar_output_tuples
			== CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			: 0, VARIABLE_2);
}

void CAESAR_DATALOG_COMPUTE_X4_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	/* We get the pcpredicate to manipulate */
	CAESAR_TYPE_INDEX_TABLE_1 PREDICATE_INDEX;
	CAESAR_BODY_PREDICATE_TABLE_ENTRY PREDICATE_BODY;
	CAESAR_TYPE_PREDICATE_TABLE_ENTRY PREDICATE;
	PREDICATE = &PREDICATE_BODY;
	PREDICATE_INDEX = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;
	PREDICATE_TABLE_GET_ENTRY_BY_INDEX(PREDICATE_INDEX, PREDICATE);

	/* It is a fact */
	CAESAR_TYPE_RELATION_TABLE_ENTRY RELATION;
	CAESAR_BODY_RELATION_TABLE_ENTRY RELATION_BODY;
	RELATION = &RELATION_BODY;
	CAESAR_TYPE_COMBINATION_TABLE_ENTRY COMBINATION;
	CAESAR_BODY_COMBINATION_TABLE_ENTRY COMBINATION_BODY;
	COMBINATION = &COMBINATION_BODY;
	CAESAR_BODY_DOMAIN_TABLE_ENTRY DOMAIN_BODY;
	CAESAR_TYPE_DOMAIN_TABLE_ENTRY DOMAIN;
	DOMAIN = &DOMAIN_BODY;

	CAESAR_DATALOG_GET_RELATION_BY_INDEX(PREDICATE->RELATION, RELATION);
	if (RELATION->FACTS == NULL)
	{
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 2;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= 0;

		/*************************************/
		/* Add to caesar_solve the successor */
		/*************************************/

		CAESAR_BEGIN_DEBUG(1)
		{
			printf(" ");
			CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			printf("\n");
		}CAESAR_END_DEBUG;

		return;
	}

	CAESAR_TYPE_INDEX_TABLE_1 combination_index;
	CAESAR_TYPE_BOOLEAN solution_founded= CAESAR_FALSE;
	CAESAR_TYPE_NATURAL fact_counter;
	CAESAR_TYPE_NATURAL fact_table_size;
	CAESAR_TYPE_POINTER general_combination;
	CAESAR_TYPE_POINTER concrete_combination;

	COMBINATION_TABLE_GET_BINARY_ENTRY_BY_INDEX(RELATION->COMBINATIONS,
			PREDICATE->COMBINATION, &general_combination);

	fact_table_size = FACT_TABLE_GET_ENTRY_COUNT(RELATION->FACTS);

	for (fact_counter = 0; fact_counter < fact_table_size; fact_counter++)
	{
		COMBINATION_TABLE_GET_BINARY_ENTRY_BY_INDEX(RELATION->FACTS,
				fact_counter, &concrete_combination);
		if (FACT_IS_CONCRETIZATION_OF(concrete_combination,
				RELATION->ARITY, general_combination))
		{
			solution_founded = CAESAR_TRUE;
			COMBINATION_TABLE_ADD_ENTRY_COPY(RELATION->COMBINATIONS,
					concrete_combination, RELATION->ARITY, &combination_index);
			PREDICATE->RELATION = PREDICATE->RELATION; /* Just for being clear */
			PREDICATE->COMBINATION = combination_index;
			PREDICATE_TABLE_ADD_ENTRY(PREDICATE, &PREDICATE_INDEX);

			/*=========================*/
			/* Generate a X6 successor */
			/*=========================*/

			((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
					= 2;
			((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE
					= 1;
			((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
					= PREDICATE_INDEX;

			/*************************************/
			/* Add to caesar_solve the successor */
			/*************************************/

			CAESAR_BEGIN_DEBUG(1)
			{
				printf(" ");
				CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			}CAESAR_END_DEBUG;

			CAESAR_LOOP(
					(CAESAR_TYPE_POINTER) NULL,
					caesar_output_tuples == CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
							: 0, VARIABLE_2);

			if (caesar_output_tuples == CAESAR_FALSE)
			{
				CAESAR_BEGIN_DEBUG(1)
				{
					printf("\n");
				}CAESAR_END_DEBUG;
				return;
			}
		}
	}

	CAESAR_BEGIN_DEBUG(1)
	{
		if (solution_founded == CAESAR_FALSE)
		{
			((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			= 2;
			((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3;
			((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
			= 0;
			printf(" ");
			CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		}
		printf("\n");
	}CAESAR_END_DEBUG;

	return;
}

void CAESAR_DATALOG_COMPUTE_X5_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	/* We get the pcpredicate to manipulate */
	CAESAR_TYPE_INDEX_TABLE_1 predicate_index;
	CAESAR_TYPE_PREDICATE_TABLE_ENTRY predicate;
	CAESAR_BODY_PREDICATE_TABLE_ENTRY predicate_body;
	predicate = &predicate_body;
	predicate_index = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;
	PREDICATE_TABLE_GET_ENTRY_BY_INDEX(predicate_index, predicate);

	/* Get the relation info */
	CAESAR_TYPE_RELATION_TABLE_ENTRY relation;
	CAESAR_BODY_RELATION_TABLE_ENTRY relation_body;
	relation = &relation_body;
	CAESAR_DATALOG_GET_RELATION_BY_INDEX(predicate->RELATION, relation);

	CAESAR_TYPE_NATURAL matching_rules_count;

	matching_rules_count = CAESAR_DATALOG_COUNT_RULE_I(relation->RULES);

	if (matching_rules_count == 0)
	{
		/* IMPLICIT TRUE SUCCESSOR BECAUSE X6 IS A CONJUNCTIVE */

		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 2;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= 0;

		CAESAR_BEGIN_DEBUG(1)
		{
			printf(" ");
			CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
			printf("\n");
		}CAESAR_END_DEBUG;

		return;
	}

	CAESAR_BODY_RULE_I_TABLE_ENTRY rule_i;
	CAESAR_BODY_PRULE_TABLE_ENTRY prule;
	CAESAR_TYPE_INDEX_TABLE_1 prule_index;
	CAESAR_TYPE_NATURAL i;

	for (i=0; i < matching_rules_count; i++)
	{
		CAESAR_DATALOG_GET_RULE_I_BY_INDEX(relation->RULES, i, &rule_i);

		prule.CPREDICATE_INDEX = predicate_index;
		prule.RULE_INDEX = rule_i.RULE_INDEX;

		CAESAR_DATALOG_ADD_PRULE_TABLE_ENTRY(&prule, &prule_index);

		/* Generate a X7 successor */

		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 1;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 2;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= prule_index;

		CAESAR_LOOP(
				(CAESAR_TYPE_POINTER) NULL,
				caesar_output_tuples == CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
						: 0, VARIABLE_2);

		CAESAR_BEGIN_DEBUG(1)
		{
			printf(" ");
			CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		}CAESAR_END_DEBUG;
	}

	CAESAR_BEGIN_DEBUG(1)
	{
		printf("\n");
	}CAESAR_END_DEBUG;
}

void CAESAR_DATALOG_COMPUTE_X6_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	/* IMPLICIT TRUE SUCCESSOR BECAUSE X6 IS A CONJUNCTIVE */

	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
			= 2;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 2;
	((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
			= 0;

	CAESAR_BEGIN_DEBUG(1)
	{
		printf(" ");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		printf("\n");
	}CAESAR_END_DEBUG;

	return;
}

//				CAESAR_BEGIN_DEBUG(1)
//				{
//					CAESAR_TYPE_NATURAL X8_SUCCESSORS_COUNTER;
//				}CAESAR_END_DEBUG;

CAESAR_TYPE_NATURAL X8_SUCCESSORS_COUNTER;
CAESAR_TYPE_NATURAL X3_SUCCESSORS_COUNTER;

void CAESAR_DATALOG_COMPUTE_X7_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	CAESAR_BEGIN_DEBUG(1)
	{
		X8_SUCCESSORS_COUNTER = 0;
	}CAESAR_END_DEBUG;

	CAESAR_TYPE_PRULE_TABLE_ENTRY prule;
	CAESAR_BODY_RULE_TABLE_ENTRY rule;
	CAESAR_BODY_PREDICATE_TABLE_ENTRY head_predicate;
	CAESAR_BODY_RELATION_TABLE_ENTRY head_relation;
	CAESAR_TYPE_POINTER head_combination;

	CAESAR_TYPE_INDEX_TABLE_1 prule_index;

	prule_index = ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX;

	CAESAR_DATALOG_GET_PRULE_BY_INDEX(prule_index, &prule);

	CAESAR_DATALOG_GET_RULE_BY_INDEX(prule->RULE_INDEX, &rule);

	PREDICATE_TABLE_GET_ENTRY_BY_INDEX(prule->CPREDICATE_INDEX, &head_predicate);

	CAESAR_DATALOG_GET_RELATION_BY_INDEX(head_predicate.RELATION,
			&head_relation);

	COMBINATION_TABLE_GET_BINARY_ENTRY_BY_INDEX(head_relation.COMBINATIONS,
			head_predicate.COMBINATION, &head_combination);

	/* Variable Substitution */

	CAESAR_TYPE_VARIABLE_MAP_LIST vmap_cell;
	CAESAR_TYPE_VARIABLE_POSITION_LIST vposition_cell;
	CAESAR_BODY_RELATION_TABLE_ENTRY subobjective_relation;
	CAESAR_TYPE_INDEX_TABLE_1 element_index;
	CAESAR_TYPE_BOOLEAN is_variable;

	for (vmap_cell=rule.LINKED_VARIABLE_MAP_LIST; vmap_cell != NULL; vmap_cell
			= vmap_cell->NEXT)
	{
		/* We read the current argument of the rule head */
		COMBINATION_GET_VALUE_AT(head_combination,
				vmap_cell->MAP->HEAD_POSITION->ARGUMENT_INDEX,
				head_relation.ARITY, &element_index, &is_variable);

		for (vposition_cell = vmap_cell->MAP->TAIL_POSITION_LIST; vposition_cell
				!= NULL; vposition_cell = vposition_cell->NEXT)
		{
			CAESAR_DATALOG_GET_RELATION_BY_INDEX(
					rule.TAIL_TO_CONCRETIZE_RELATION_INDICES[vposition_cell->POSITION->LITERAL_INDEX],
					&subobjective_relation);

			COMBINATION_SET_VALUE_AT(
					rule.TAIL_TO_CONCRETIZE
							+ rule.TAIL_TO_CONCRETIZE_DISPLAY[vposition_cell->POSITION->LITERAL_INDEX],
					vposition_cell->POSITION->ARGUMENT_INDEX,
					subobjective_relation.ARITY, element_index, is_variable);
		}
	}

	/* Non-head variables concretization */

	CAESAR_TYPE_INDEX_TABLE_1 *counters;
	CAESAR_TYPE_INDEX_TABLE_1 *counters_limits;

	/* TODO: Confirm the deletion of these structures when unused */

	CAESAR_CREATE(counters, rule.FREE_VARIABLE_NUMBER * sizeof (CAESAR_TYPE_INDEX_TABLE_1), CAESAR_TYPE_INDEX_TABLE_1 *);
	CAESAR_CREATE(
			counters_limits,
			rule.FREE_VARIABLE_NUMBER * sizeof (CAESAR_TYPE_INDEX_TABLE_1),
			CAESAR_TYPE_INDEX_TABLE_1 *);

	/* Initialize the index arrays */

	CAESAR_TYPE_NATURAL i;

	for (i = 0; i < rule.FREE_VARIABLE_NUMBER; i++)
	{
		counters[i] = 0;
		counters_limits[i] = rule.FREE_VARIABLE_MAP_ARRAY[i]->CARDINAL;
	}

	/* Iterate over each concretized rule (crule) */

	/*
	 For each concretization
	 For each variable
	 For each position of the variable
	 Edit the combination
	 Add the new combinations  
	 */

	CAESAR_TYPE_NATURAL j;
	CAESAR_TYPE_INDEX_TABLE_1 combination_index;
	CAESAR_TYPE_BOOLEAN is_duplicated;
	CAESAR_BODY_PREDICATE_TABLE_ENTRY predicate;
	CAESAR_TYPE_INDEX_TABLE_1 predicate_index;
	CAESAR_BODY_CRULE_TABLE_ENTRY crule;
	CAESAR_TYPE_INDEX_TABLE_1 crule_index;

	/* TODO: Optimize this just doing it once for every bes resolution */

	crule.RULE_INDEX = prule->RULE_INDEX;
	CAESAR_CREATE(crule.CPREDICATE_INDICES,
			rule.LENGTH * sizeof (CAESAR_TYPE_INDEX_TABLE_1),
			CAESAR_TYPE_INDEX_TABLE_1 *);

	do
	{ /* Edition of the combinations */
		for (i = 0; i < rule.FREE_VARIABLE_NUMBER; i++)
		{
			for (vposition_cell
					= rule.FREE_VARIABLE_MAP_ARRAY[i]->TAIL_POSITION_LIST; vposition_cell
					!= NULL; vposition_cell = vposition_cell->NEXT)
			{

				CAESAR_DATALOG_GET_RELATION_BY_INDEX(
						rule.TAIL_TO_CONCRETIZE_RELATION_INDICES[vposition_cell->POSITION->LITERAL_INDEX],
						&subobjective_relation);

				COMBINATION_SET_VALUE_AT(
						rule.TAIL_TO_CONCRETIZE
								+ rule.TAIL_TO_CONCRETIZE_DISPLAY[vposition_cell->POSITION->LITERAL_INDEX],
						vposition_cell->POSITION->ARGUMENT_INDEX,
						subobjective_relation.ARITY, 
						DOMAIN_TABLE_GET_ELEMENT_BY_FILTERED_ELEMENT_INDEX(
								subobjective_relation.DOMAINS[vposition_cell->POSITION->ARGUMENT_INDEX]
								,counters[i]), 
						CAESAR_FALSE /* We are concretizing!!! */);
			}
		}
		/* Addition of the combinations */
		for (j = 0; j < rule.LENGTH; j++)
		{
			CAESAR_DATALOG_GET_RELATION_BY_INDEX(
					rule.TAIL_TO_CONCRETIZE_RELATION_INDICES[j],
					&subobjective_relation);

			is_duplicated = COMBINATION_TABLE_ADD_ENTRY_COPY(
					subobjective_relation.COMBINATIONS, rule.TAIL_TO_CONCRETIZE
							+ rule.TAIL_TO_CONCRETIZE_DISPLAY[j],
					subobjective_relation.ARITY, &combination_index);

			/* We could add some statistical info about the repetitions */

			predicate.RELATION = rule.TAIL_TO_CONCRETIZE_RELATION_INDICES[j];
			predicate.COMBINATION = combination_index;

			PREDICATE_TABLE_ADD_ENTRY( &predicate, &predicate_index);

			crule.CPREDICATE_INDICES[j] = predicate_index;
		}

		CAESAR_DATALOG_ADD_CRULE_TABLE_ENTRY( &crule, &crule_index);

		/* Generate a X8 successor for each cquery */

		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 1;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 3;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= crule_index;

		CAESAR_BEGIN_DEBUG(1)
		{
			X8_SUCCESSORS_COUNTER++;
		}CAESAR_END_DEBUG;

		CAESAR_LOOP(
				(CAESAR_TYPE_POINTER) NULL,
				caesar_output_tuples == CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
						: 0, VARIABLE_2);

	} while (CAESAR_DATALOG_ITERATE_OVER_COMBINATIONS(counters,
			counters_limits, NULL, rule.FREE_VARIABLE_NUMBER) );

	CAESAR_BEGIN_DEBUG(1)
	{
		printf(" %d successor%s of type ", X8_SUCCESSORS_COUNTER, X8_SUCCESSORS_COUNTER>1?"s":"");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		printf("\n");
	}CAESAR_END_DEBUG;

	return;
}

void CAESAR_DATALOG_COMPUTE_X8_SUCCESSOR(CAESAR_TYPE_POINTER VARIABLE_1,
		CAESAR_TYPE_POINTER VARIABLE_2, void (*CAESAR_LOOP)())
{
	CAESAR_BEGIN_DEBUG(1)
	{
		X3_SUCCESSORS_COUNTER = 0;
	}CAESAR_END_DEBUG;

	CAESAR_TYPE_NATURAL i;
	CAESAR_BODY_CRULE_TABLE_ENTRY crule;
	CAESAR_BODY_RULE_TABLE_ENTRY rule;

	CAESAR_DATALOG_GET_CRULE_BY_INDEX(((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_1)->ENTRY_INDEX,
			&crule);
	CAESAR_DATALOG_GET_RULE_BY_INDEX(crule.RULE_INDEX, &rule);

	for (i = 0; i < rule.LENGTH; i++)
	{
		/* Generate a X3  successor */
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
				= 1;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->TYPE = 0;
		((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->ENTRY_INDEX
				= crule.CPREDICATE_INDICES[i];

		CAESAR_BEGIN_DEBUG(1)
		{
			X3_SUCCESSORS_COUNTER++;
		}CAESAR_END_DEBUG;

		CAESAR_LOOP(
				(CAESAR_TYPE_POINTER) NULL,
				caesar_output_tuples == CAESAR_TRUE ? ((CAESAR_TYPE_BOOLEAN_VARIABLE *) VARIABLE_2)->BLOCK_INDEX
						: 0, VARIABLE_2);
	}

	CAESAR_BEGIN_DEBUG(1)
	{
		printf(" %d successor%s of type ", X3_SUCCESSORS_COUNTER, X3_SUCCESSORS_COUNTER>1?"s":"");
		CAESAR_VARIABLE_PRINT(stdout, VARIABLE_2);
		printf("\n");
	}CAESAR_END_DEBUG;

}

