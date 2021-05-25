/*This file contains functions that helps setting machine words. All the functions in this file are called directly or indirectly from first_pass and
second_pass functions.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "global_structures.h"
#include "machine_words.h"
#include "text_functions.h"


/*Receives a decimal integer, a pointer to the destination machine_word_struct, the number of the current line and the file name. It checks that the int is
not too long for representing it as signed number by 12 bits and converts the int to 12 bits word. If so, it sets num_word_ptr fields:
machine_word.data_word_value to num, ARE to A, is_true to TRUE and returns TRUE. If not, it returns FALSE. Handles negatives by 2's complement.*/
bool int_to_word(int num, machine_word_struct *num_word_ptr, int lines_counter, char *file_name) {
    int max_int = (1 << (WORD_SIZE - 1)) - 1;
    int min_int = -1 << (WORD_SIZE - 1);

    if ((num > max_int) || (num < min_int)) {
            fprintf (stdout, "%s: In line %d: Data overflow: There is a number that is too long for a machine word\n", file_name, lines_counter);
            return FALSE;
    }
    num_word_ptr -> machine_word.data_word_value = (unsigned int) num;
    num_word_ptr -> ARE = 'A';
    num_word_ptr -> is_set = TRUE;
    return TRUE;
}


/*Receives a char c and a pointer to the destination machine_word_struct and sets the machine word struct fields: machine_word.data_word_value according to
c, ARE to 'A' and is_set to TRUE.*/
bool char_to_word(char c, machine_word_struct *char_word_ptr) {
    char_word_ptr -> machine_word.data_word_value = (unsigned int) c;
    char_word_ptr -> ARE = 'A';
    char_word_ptr -> is_set = TRUE;
    return TRUE;
}


/*Receives a register number and a pointer to the destination machine_word_struct and sets the machine word struct fields: machine_word.data_word_value
according to the register number, ARE to 'A', addressing_mode to REGISTER_DIRECT and is_set to TRUE.*/
void register_to_word(int register_num, machine_word_struct *machine_word_operand) {
    int val = 1 << register_num; /*turn on the bit in index register_num*/
    machine_word_operand -> machine_word.data_word_value = (unsigned int) val;
    machine_word_operand -> ARE = 'A';
    machine_word_operand -> is_set = TRUE;
    machine_word_operand -> addressing_mode = REGISTER_DIRECT;
}


/*Receives an operand as a string, a pointer to the destination machine_word_struct, 3 arrays of strings that together includes all the saved words in the
program, the line number and the name of the file. It updates the addressing mode according to the string syntax, checks syntax validity and amount of parameters
validity. If the addressing mode is immediate or register direct, it sets machine_word_operand. Else, the field is_set will be set to FALSE and the line_number
and symbol_operand_name fields will also be set. Returns TRUE if the syntax is valid, the amount of parameters is valid and immediate operands lengths are valid.*/
bool str_operand_to_word_and_addressing_mode(char *str_operand, machine_word_struct *machine_word_operand, char ** registers, char **instructions_names,\
char **directives, int lines_counter, char *file_name) {
    int i;

    if (*str_operand == '#') { /*Immediate operand*/
        int operand_val;

        str_operand++; /*skip the '#'*/
        if (*str_operand == '\0') {
            fprintf(stdout, "%s: In line %d: Missing operand after #\n", file_name, lines_counter);
            return FALSE;
        }
        if(!(isnum(str_operand))) {
            fprintf(stdout, "%s: In line %d: %s is not a legal number\n", file_name, lines_counter, str_operand);
            return FALSE;
        }
        operand_val = atoi(str_operand);
        if (!(int_to_word(operand_val, machine_word_operand, lines_counter, file_name)))
            return FALSE;
        machine_word_operand -> addressing_mode = IMMEDIATE;
        return TRUE;
    }
    else {
        if (*str_operand == '%') { /*Relative operand*/
            str_operand++; /*skip the '%'*/
            if (*str_operand == '\0') {
                fprintf(stdout, "%s: In line %d: Missing operand after %%\n", file_name, lines_counter);
                return FALSE;
            }
            machine_word_operand -> addressing_mode = RELATIVE;
        }
        else {
            for (i = 0; i < NUM_OF_REGISTERS; i++) { /*Iterates registers names and checks if str_operand is one of them.*/
                if (strcmp(str_operand, registers[i]) == 0) { /*i is the register number*/
                    register_to_word(i, machine_word_operand);
                    break;
                }
            }
            if (machine_word_operand -> addressing_mode != REGISTER_DIRECT)
                machine_word_operand -> addressing_mode = DIRECT;
        }
        if (machine_word_operand -> addressing_mode == RELATIVE || machine_word_operand -> addressing_mode == DIRECT) { /*machine_word will be set at the
        second pass*/
            machine_word_operand -> is_set = FALSE;
            strcpy(machine_word_operand -> symbol_operand_name, str_operand); /*Will be used in the second pass*/
            machine_word_operand -> line_number = lines_counter; /*Will be used for errors messages in the second pass*/
            if (!(is_valid_symbol_operand(str_operand, registers, instructions_names, directives, lines_counter, file_name))) /*Checks symbols syntax validity*/
                return FALSE;
        }
    }
    return TRUE;
}


 /*Receives the current instruction and a pointer to the destination machine_word_struct and sets the machine word struct fields: opcode and funct according
 to curr_instruction and source_add and dest_add to zero, ARE to A and is_set to TRUE.*/
void instruction_to_instruction_word(instruction curr_instruction, machine_word_struct *instruction_word_ptr) {
    instruction_word_ptr -> machine_word.instruction_word.opcode = (unsigned int) curr_instruction.opcode;
    instruction_word_ptr -> machine_word.instruction_word.funct = (unsigned int) curr_instruction.funct;
    instruction_word_ptr -> machine_word.instruction_word.source_add = (unsigned int) 0;
    instruction_word_ptr -> machine_word.instruction_word.dest_add = (unsigned int) 0;
    instruction_word_ptr -> ARE = 'A';
    instruction_word_ptr -> is_set = TRUE;
}


/*Receives a pointer to the current instruction machine word, the current instruction, a bool that indicates if the current machine word is the dest or the source
of the instruction, the addressing mode of the operand as detected earlier,  the line number and the file name. It checks that the addressing modes are valid for
the current instruction and returns FALSE if not. If the addressing modes are valid, it sets them in the instruction machine word and returns TRUE.*/
bool set_addressing_mode_in_word(machine_word_struct *instruction_word_ptr, instruction curr_instruction, bool is_dest, addressing_mode operand_addressing_mode,\
int lines_counter, char *file_name) {
    int i, num_of_valid_addressing_modes;
    addressing_mode *valid_addressing_modes_arr = NULL; /*A pointer to an array that holds all the valid addressing modes for the operand*/

    if (is_dest) {
        num_of_valid_addressing_modes = curr_instruction.num_of_valid_dest_modes;
        valid_addressing_modes_arr = curr_instruction.valid_dest_modes;
    }
    else {
        num_of_valid_addressing_modes = curr_instruction.num_of_valid_source_modes;
        valid_addressing_modes_arr = curr_instruction.valid_source_modes;
    }

    for (i = 0; i < num_of_valid_addressing_modes; i++) { /*Iterates all the valid addressing modes for the operand and checks if the current addressing mode is
    equal to one of them.*/
        if (operand_addressing_mode == valid_addressing_modes_arr[i]) {
            if (is_dest)
                instruction_word_ptr -> machine_word.instruction_word.dest_add = (unsigned int) valid_addressing_modes_arr[i];
            else
                instruction_word_ptr -> machine_word.instruction_word.source_add = (unsigned int) valid_addressing_modes_arr[i];
            return TRUE;
        }
    }
    /*reaches here only if the current addressing mode is invalid for the instruction operand*/
    fprintf(stdout, "%s: In line %d: Invalid addresing mode for this instruction\n", file_name, lines_counter);
    return FALSE;
}


/*Receives an integer val, a char ARE and a pointer to the destination machine_word_struct and sets the machine word struct fields: machine_word.data_word_value
to val, ARE to ARE and is_set to TRUE.*/
void set_data_word(int val, char ARE, machine_word_struct *curr_data_word) {
    curr_data_word -> machine_word.data_word_value = (unsigned int) val;
    curr_data_word -> ARE = ARE;
    curr_data_word -> is_set = TRUE;
}