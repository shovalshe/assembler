
#ifndef NUM_OF_REGISTERS
#define NUM_OF_REGISTERS 8
#endif
#ifndef NUM_OF_INSTRUCTIONS
#define NUM_OF_INSTRUCTIONS 16
#endif
#ifndef NUM_OF_DIRECTIVES
#define NUM_OF_DIRECTIVES 4
#endif
#ifndef MAX_SYMBOL_LENGTH
#define MAX_SYMBOL_LENGTH 31
#endif
#ifndef MAX_LINE_LENGTH
#define MAX_LINE_LENGTH 80
#endif
#define MAX_INSTRUCTION_NAME_LENGTH 4


bool check_line_length(char *statement, int lines_counter, char *file_name); /*Receives a string, the number of the line in the input file and the file
name. It checks if the string is longer than MAX_LINE_LENGTH chars excluding '\n' and '\0'/ If so, it prints an error message and returns FALSE. Otherwise,
it returns TRUE.*/

bool check_extraneous_text(char p_char, int lines_counter, char *file_name); /*Receives a char, the number of the line in the input file and the file name.
If the char is not '\0' or '\n', prints an error message and returns FALSE. Otherwise, returns TRUE.*/

char * skip_spaces_and_tabs(char *p_char); /*Receives a pointer to char, increment it by the number of chars that exist until the
next char that is not a space or a tab and returns that pointer. Meaning the returned pointer points at the next char that is not a space or a tab.*/

void scopy_until_p(char *dest, char *str, char *p_end); /*Recieves a pointer to destination string, another pointer to a string and
a pointer to a char that points somewhere in the second string. It copies the string pointed by str to the string pointed by dest until it reaches p_end,
and doesn't return anything. This func assumes that p_end points at a char somewhere in the string pointed by str.*/


char * get_next_par(char *str_parameter, char *p_char); /*Receives a pointer to destination string and a pointert to the beggining of a string. It skips
spaces, copies the next word (until a space or ',' into and returns a pointer to the next char after this word*/

bool get_instruction_name(char *str_instruction, char **p_char, int lines_counter, char *file_name); /*Receives a pointer to destination string, a pointert
to the address of a string, the number of the line in the input file and the file name. It skips spaces, checks the next word is not longer than
MAX_INSTRUCTION_NAME_LENGTH (If not, prints an error message and returns FALSE), copies it into str_instruction and returns TRUE. The function also increments
*p_char to points at the next non space char after the current word.*/

bool isnum(char *str); /*Receives a string, checks if it is a legal decimal integer and returns TRUE if so and FALSE otherwise.*/

bool is_valid_symbol(char *symbol_name, char **registers, char **instruction_names, char **attributes); /*Receives a string and 3 arrays of strings that
together includes all the saved words in the program. It checks that the string is not equal to any saved word (by calling is_saved_word)and that its first
char is a letter and all others are letters or digits. It returns TRUE if so and FALSE otherwise.*/

bool is_saved_word(char *symbol_name, char **registers, char **instruction_names, char **attributes); /*Receives a string and 3 arrays of strings that together
includes all the saved words in the program. It checks that the string is not equal to any saved word and returns TRUE if so and FALSE otherwise.*/

bool is_valid_symbol_operand(char *str_operand, char **registers, char **instruction_names, char **attributes, int lines_counter, char *file_name); /*Receives
a string, 3 arrays of strings that together includes all the saved words in the program, the current line number and the file name. It checks that the string
is a valid symbol (by callig to is_valid_symbol) and prints an error message if not. This function will be called when we expect to recieve a symbol operand.
It returns TRUE if str_operand is a valid symbol and FALSE otherwise.*/