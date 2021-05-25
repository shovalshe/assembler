
#ifndef NUM_OF_REGISTERS
#define NUM_OF_REGISTERS 8
#endif
#define WORD_SIZE 12


bool int_to_word (int num, machine_word_struct *num_word_ptr, int lines_counter, char *file_name); /*Receives a decimal integer, a pointer to the destination
machine_word_struct, the number of the current line and the file name. It checks that the int is not too long for representing it as signed number by 12 bits
and converts the int to 12 bits word. If so, it sets num_word_ptr fields: machine_word.data_word_value to num, ARE to A, is_true to TRUE and returns TRUE. If
not, it returns FALSE. Handles negatives by 2's complement.*/

bool char_to_word(char c, machine_word_struct * char_word_ptr); /*Receives a char c and a pointer to the destination machine_word_struct and sets the machine
word struct fields: machine_word.data_word_value according to c, ARE to 'A' and is_set to TRUE.*/

void register_to_word(int register_num, machine_word_struct *machine_word_operand); /*Receives a register number and a pointer to the destination
machine_word_struct and sets the machine word struct fields: machine_word.data_word_value according to the register number, ARE to 'A', addressing_mode to
REGISTER_DIRECT and is_set to TRUE.*/

bool str_operand_to_word_and_addressing_mode(char *str_operand, machine_word_struct *machine_word_operand, char **registers, char **instructions_names,\
char **directives, int lines_counter, char *file_name); /*Receives an operand as a string, a pointer to the destination machine_word_struct, 3 arrays of
strings that together includes all the saved words in the program, the line number and the name of the file. It updates the addressing mode according to the
string syntax, checks syntax validity and amount of parameters validity. If the addressing mode is immediate or register direct, it sets machine_word_operand.
Else, the field is_set will be set to FALSE and the line_number and symbol_operand_name fields will also be set. Returns TRUE if the syntax is valid, the amount
of parameters is valid and immediate operands lengths are valid.*/

void instruction_to_instruction_word(instruction curr_instruction, machine_word_struct *instruction_word_ptr); /*Receives the current instruction and a pointer
to the destination machine_word_struct and sets the machine word struct fields: opcode and funct according to curr_instruction and source_add and dest_add to
zero, ARE to A and is_set to TRUE.*/

bool set_addressing_mode_in_word(machine_word_struct *instruction_word_ptr, instruction curr_instruction, bool is_dest, addressing_mode operand_addressing_mode,\
int lines_counter, char *file_name); /*Receives a pointer to the current instruction machine word, the current instruction, a bool that indicates if the current
machine word is the dest or the source of the instruction, the addressing mode of the operand as detected earlier,  the line number and the file name. It checks
that the addressing modes are valid for the current instruction and returns FALSE if not. If the addressing modes are valid, it sets them in the instruction
machine word and returns TRUE.*/

void set_data_word(int val, char ARE, machine_word_struct *curr_data_word); /*Receives an integer val, a char ARE and a pointer to the destination
machine_word_struct and sets the machine word struct fields: machine_word.data_word_value to val, ARE to ARE and is_set to TRUE.*/