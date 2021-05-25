all: assembler

assembler: assembler.o utils.o symbols.o text_functions.o machine_words.o first_pass.o second_pass.o create_files.o assembler.h utils.h symbols.h\
text_functions.h machine_words.h first_pass.h second_pass.h create_files.h global_structures.h instructions_table.h
	gcc -ansi -Wall -pedantic -o assembler assembler.o utils.o symbols.o text_functions.o  machine_words.o first_pass.o second_pass.o create_files.o

assembler.o: assembler.c assembler.h utils.h symbols.h first_pass.h second_pass.h create_files.h global_structures.h
	gcc -ansi -Wall -pedantic -c assembler.c

utils.o: utils.c utils.h
	gcc -ansi -Wall -pedantic -c utils.c

symbols.o: symbols.c symbols.h utils.h
	gcc -ansi -Wall -pedantic -c symbols.c

text_functions.o: text_functions.c text_functions.h utils.h
	gcc -ansi -Wall -pedantic -c text_functions.c

machine_words.o: machine_words.c machine_words.h global_structures.h text_functions.h utils.h
	gcc -ansi -Wall -pedantic -c machine_words.c

first_pass.o: first_pass.c first_pass.h utils.h symbols.h machine_words.h global_structures.h text_functions.h instructions_table.h
	gcc -ansi -Wall -pedantic -c first_pass.c

second_pass.o: second_pass.c second_pass.h utils.h symbols.h global_structures.h
	gcc -ansi -Wall -pedantic -c second_pass.c

create_files.o: create_files.c create_files.h utils.h symbols.h global_structures.h
	gcc -ansi -Wall -pedantic -c create_files.c

clean:
	rm *.o