BINARY_DIR = bin
LEX_YACC_DIR = lexyacc-code
LIBRARY_DIR = lib
OBJECT_DIR = objects
SOURCE_DIR = src

OBJECTS = $(OBJECT_DIR)/y.tab.o $(OBJECT_DIR)/lex.yy.o
EXECUTABLES = $(BINARY_DIR)/calc3a $(BINARY_DIR)/calc3i $(BINARY_DIR)/calc3g

.PHONY: all clean

all: $(EXECUTABLES) library
	@echo "All the programs have been built."

$(EXECUTABLES): $(OBJECTS)
	gcc $^ $(LEX_YACC_DIR)/$(notdir $@).c -o $@

$(OBJECT_DIR)/%.o: $(LEX_YACC_DIR)/%.c
	gcc -c $< -o $@

library: $(OBJECT_DIR)/functions.o
	ar -rc $(LIBRARY_DIR)/libfunctions.a $^
	ranlib $(LIBRARY_DIR)/libfunctions.a

$(OBJECT_DIR)/functions.o: $(SOURCE_DIR)/functions.s
	gcc -c $< -o $@

$(LEX_YACC_DIR)/%.c: $(LEX_YACC_DIR)/%.y $(LEX_YACC_DIR)/%.l
	bison -y -d -o $(LEX_YACC_DIR)/y.tab.c $(LEX_YACC_DIR)/calc3.y
	flex -o $(LEX_YACC_DIR)/lex.yy.c $(LEX_YACC_DIR)/calc3.l

clean:
	rm -f $(LIBRARY_DIR)/*
	rm -f $(BINARY_DIR)/*
	rm -f $(OBJECT_DIR)/*
	rm -f $(LEX_YACC_DIR)/y.tab.*
	rm -f $(LEX_YACC_DIR)/lex.yy.*
	@echo "All the programs have been cleaned."