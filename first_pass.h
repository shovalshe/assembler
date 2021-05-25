
#define MAX_LINE_LENGTH 80
#define MAX_DIRECTIVE_LENGTH 7
#define TWO 2
#ifndef NUM_OF_REGISTERS
#define NUM_OF_REGISTERS 8
#endif
#ifndef NUM_OF_ACTIONS
#define NUM_OF_ACTIONS 16
#endif
#ifndef NUM_OF_DIRECTIVES
#define NUM_OF_DIRECTIVES 4
#endif
#ifndef MEMORY_SIZE
#define MEMORY_SIZE 4096
#endif
#ifndef IC_INIT_VAL
#define IC_INIT_VAL 100
#endif


bool first_pass(char *file_name, machine_word_struct *code_img, machine_word_struct *data_img, linked_list *symbols_table_ptr,\
linked_list *data_list_ptr, linked_list *ent_list_ptr, int *p_icf, int *p_dcf); /*Receives the file name, a pointer to the code image, a pointer to the
data image, a pointer to the symbols table, to the data list, to the ent lins, a pointer to an int that will hold the final ic and a pointer to an int
that will hold the final dc. The function executes a loop that iterates lines in the input file and process them. It prints errors mesages if necessary
- at most one for every line in the input file. It identify all symbols and insert a part of the machine words to code_img and data_img. Returns TRUE
if the entire process was succesfully completed and FALSE otherwise.*/

bool get_symbol(char *symbol_name, char *p_start, char *p_end, char **registers, char **action_names, char **attributes, linked_list symbols_table,\
int lines_counter, char *file_name); /*Receives a pointer to the destination string, a pointer to the beginning of a string, a pointer to a char in this
string, 3 arrays of strings that together includes all the saved words in the program, the symbols table, the line number and the name of the file. It
copies the string pointed by p_start and ends at p_end to symbol_name. It is also checks that the string that has been copied has a valid symbol syntax
and returns TRUE if so and FALSE otherwise.*/

bool get_directive(directive *dest, char *p_start, char **p_end, int lines_counter, char *file_name); /*Receives a pointer to the destination directive,
a pointer to the beginning of a string, a pointer to an address of a char in this string,the line number and the name of the file. It checks if the word
that starts at p_start is a defined directive and set the directive accordingly. It increment p_end to point at the first char after the current word and
returns TRUE if the directive is defined and FALSE otherwise.*/


bool get_and_insert_data_parameters(char **p_char, machine_word_struct *data_img, int *dc, int lines_counter, char *file_name); /*Receives a pointer to
the address of the current char in the statement, a pointer to the beginning of the data image, the current dc, the line number and the name of the file.
It inserts the integers that are written cotrrectly in p_char to data_img and increment dc by the amount of words that was inserted. It also increments
p_char to point at the next char after all chars that has been analyzed. Returns FALSE if there was a problem and TRUE otherwise*/

bool get_and_insert_string_parameters(char **p_char, machine_word_struct *data_img, int *dc, int lines_counter, char *file_name); /*Receives a pointer
to the address of the current char in the statement, a pointer to the beginning of the data image, the current dc, the line number and the name of the
file. If p_char holds a valid string, Its chars will be inserted to data_img (and also '\0' in the end of the string) and dc will be incremented by the
amount of machine words that was inserted. It also increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if
there was a problem and TRUE otherwise*/

bool get_extern_or_entry_parameter(char *ext_or_ent_parameter_name, directive curr_directive, char **p_char, char **registers, char **action_names,\
char **attributes, linked_list *symbols_table_ptr, int lines_counter, char *file_name); /*Receives a pointer to the destination strig, the current directive,
a pointer to the address of the current char in the statement, 3 arrays of strings that together includes all the saved words in the program, a pointer to
the symbols table, the line number and the name of the file. It checks that the next word in p_char is a valid symbol name by calling get_symbol and copies
it to ext_or_ent_parameter_name. It also increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if there was a
problem and TRUE otherwise*/

bool get_instruction(instruction *curr_action, char **p_char, instruction *actions_table, char **action_names, int lines_counter, char *file_name); /*Receives
a pointer to the current instruction, a pointer to the address of the current char in the statement, a pointer to the instructions table, the line number and
the file name. It checks that the next word in p_char is a valid instruction name by calling get_instruction_name and sets curr_instruction according to the
name of the instruction. It also increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if there was a problem and
TRUE otherwise*/

bool get_instruction_operands_and_insert_words(instruction curr_instruction, char **p_char, machine_word_struct *code_img, int *ic, char **registers,\
char **instructions_names, char **directives, int lines_counter, char *file_name); /*Receives the current instruction, a pointer to the address of the current
char in the statement, a pointer to the beginning of the code image, the current ic, 3 arrays of strings that together includes all the saved words in the
program, the line number and the file name. It checks that there is the right amount of operand for the current instruction, that they are valid for this
instruction, inserts the instruction word and its operands to code image, increments ic by the number of words inserted and increments p_char to point at the
next char after all chars that has been analyzed. Returns FALSE if there was a problem and TRUE otherwise*/

void add_icf_to_every_data_symbol_val(linked_list data_list, int icf); /*Receives the data linked list and the final ic, iterates all symbols in data list and
add icf to every symbol value in this list and also to the value of this symbol that stores in the symbols table*/