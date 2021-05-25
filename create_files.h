#ifndef MAX_FILE_NAME_LENGTH
#define MAX_FILE_NAME_LENGTH 1000
#endif
#ifndef IC_INIT_VAL
#define IC_INIT_VAL 100
#endif

bool create_ob_file(machine_word_struct *object_img, char *asm_file_name, int icf, int dcf); /*Receives a pointer to the beginning of the array that
includes the entire machine code, the name of the input file, the final ic and the final dc. It creates a file with the same name as the input file
with the suffix ".ob". The first line in this file will be the instructions image size and data image size. The rest of the lines will contain the
machine code: for every machine word, it includes the address of the word in memory, the word and the attribute "A,R,E". Returns TRUE if the file
was created and closed succesfuly and FALSE otherwise.*/

bool create_ext_file(linked_list ext_list, char *asm_file_name); /*Receives the externals linked list and the name of the input file. It creates a
file with the same name as the input file with the suffix ".ext". This file will contain a line for every address in the code that contains a machine
word of an external symbol. Every line includes the name of the external symbol and the address of the data word that reffers to this symbol. Returns
TRUE if the file was created and closed succesfuly and FALSE otherwise.*/

bool create_ent_file(linked_list ent_list, char *asm_file_name); /*Receives the entries linked list and the name of the input file. It creates a file
with the same name as the input file with the suffix ".ent". This file will contain a line for every entry symbol in the symbols table. Every line
includes the name of the symbol and its value in the symbols table. Returns TRUE if the file was created and closed succesfuly and FALSE otherwise.*/