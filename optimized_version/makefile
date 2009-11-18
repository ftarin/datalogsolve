FLAGS		= -g -ggdb -Wall -O2 -static -static-libgcc
DEBUG		= 0
ARCH		= iX86
INCL		= -I. -I$(CADP)/incl -I../incl -I/usr/X11R6/include
LIB		= -L$(CADP)/gc/bin.$(ARCH) -L$(CADP)/bin.$(ARCH) -lfl -lBCG  -lbcg_draw  -lbcg_info  -lBCG_IO  -lbcg_iodyn  -lbcg_open  -lcaesar  -lexp_open  -lseq_open  -lXTL -lm 
OUTPUT_DIR	= ../bin.iX86

all: main

MAINDEP 	= \
datalog_main.c \
$(OUTPUT_DIR)/datalog_parser.o \
$(OUTPUT_DIR)/datalog_scanner.o \
$(OUTPUT_DIR)/datalog_solve.o \
$(OUTPUT_DIR)/datalog_basic_data_structures.o \
$(OUTPUT_DIR)/datalog_common.o \
$(OUTPUT_DIR)/datalog_successor.o \
$(OUTPUT_DIR)/datalog_pquery_table.o \
$(OUTPUT_DIR)/datalog_combination_table.o \
$(OUTPUT_DIR)/datalog_relation_table.o \
$(OUTPUT_DIR)/datalog_substitution.o \
$(OUTPUT_DIR)/datalog_domain_table.o \
$(OUTPUT_DIR)/datalog_variable_table.o \
$(OUTPUT_DIR)/datalog_fact_table.o \
$(OUTPUT_DIR)/datalog_rule_table.o \
$(OUTPUT_DIR)/datalog_prule_table.o \
$(OUTPUT_DIR)/datalog_crule_table.o \
$(OUTPUT_DIR)/datalog_predicate_table.o \

main: $(MAINDEP)
	gcc $(FLAGS) -o $(OUTPUT_DIR)/datalog_solve_2 $(INCL) $(MAINDEP) $(LIB) -DCAESAR_DEBUG_LEVEL=$(DEBUG)/main

$(OUTPUT_DIR)/datalog_solve.o: datalog_solve.c datalog_solve.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_solve.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_solve.o

$(OUTPUT_DIR)/datalog_variable_table.o: datalog_variable_table.c datalog_variable_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_variable_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_variable_table.o

$(OUTPUT_DIR)/datalog_relation_table.o: datalog_relation_table.c datalog_relation_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_relation_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_relation_table.o

$(OUTPUT_DIR)/datalog_fact_table.o: datalog_fact_table.c datalog_fact_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_fact_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_fact_table.o

$(OUTPUT_DIR)/datalog_predicate_table.o: datalog_predicate_table.c datalog_predicate_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_predicate_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_predicate_table.o

$(OUTPUT_DIR)/datalog_prule_table.o: datalog_prule_table.c datalog_prule_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_prule_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_prule_table.o

$(OUTPUT_DIR)/datalog_rule_table.o: datalog_rule_table.c datalog_rule_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_rule_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_rule_table.o

$(OUTPUT_DIR)/datalog_substitution.o: datalog_substitution.c datalog_substitution.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_substitution.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_substitution.o

$(OUTPUT_DIR)/datalog_crule_table.o: datalog_crule_table.c datalog_crule_table.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_crule_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_crule_table.o

$(OUTPUT_DIR)/datalog_basic_data_structures.o: datalog_basic_data_structures.c datalog_basic_data_structures.h datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_basic_data_structures.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_basic_data_structures.o

$(OUTPUT_DIR)/datalog_common.o: datalog_common.c datalog_common.h
	gcc $(FLAGS) $(INCL) -c datalog_common.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_common.o

$(OUTPUT_DIR)/datalog_successor.o: datalog_successor.c datalog_successor.h
	gcc $(FLAGS) $(INCL) -c datalog_successor.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_successor.o

$(OUTPUT_DIR)/datalog_combination_table.o: datalog_combination_table.c datalog_combination_table.h
	gcc $(FLAGS) $(INCL) -c datalog_combination_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_combination_table.o

$(OUTPUT_DIR)/datalog_domain_table.o: datalog_domain_table.c datalog_domain_table.h
	gcc $(FLAGS) $(INCL) -c datalog_domain_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_domain_table.o

$(OUTPUT_DIR)/datalog_pquery_table.o: datalog_pquery_table.c datalog_pquery_table.h
	gcc $(FLAGS) $(INCL) -c datalog_pquery_table.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_pquery_table.o

$(OUTPUT_DIR)/datalog_parser.o: datalog_parser.c
	gcc $(FLAGS) $(INCL) -c datalog_parser.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_parser.o

$(OUTPUT_DIR)/datalog_scanner.o: datalog_scanner.c datalog_parser.h 
	gcc $(FLAGS) $(INCL) -c datalog_scanner.c -DCAESAR_DEBUG_LEVEL=$(DEBUG) -o $(OUTPUT_DIR)/datalog_scanner.o

datalog_parser.h: datalog_parser.y
	bison -d -odatalog_parser.c -t -ggdb -v datalog_parser.y 

datalog_parser.c: datalog_parser.y
	bison -odatalog_parser.c -t -ggdb -v datalog_parser.y 

datalog_scanner.c: datalog_scanner.l datalog_parser.c
	flex -odatalog_scanner.c datalog_scanner.l

clean:
	rm -f $(OUTPUT_DIR)/*
	rm -f datalog_parser.output datalog_parser.vcg datalog_parser.h datalog_solve core gdb datalog_parser.c datalog_scanner.c
	rm -f *~ \#*\# \#*
