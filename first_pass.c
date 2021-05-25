/*This file contains the function first_pass who passes over the input file and process it and other functions that are called from first_pass and helps
processing the file.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "symbols.h"
#include "global_structures.h"
#include "machine_words.h"
#include "text_functions.h"
#include "instructions_table.h"
#include "first_pass.h"


/*Receives the file name, a pointer to the code image, a pointer to the data image, a pointer to the symbols table, to the data list, to the ent lins, a pointer
to an int that will hold the final ic and a pointer to an int that will hold the final dc. The function executes a loop that iterates lines in the input file and
process them. It prints errors mesages if necessary - at most one for every line in the input file. It identify all symbols and insert a part of the machine words
to code_img and data_img. Returns TRUE if the entire process was succesfully completed and FALSE otherwise.*/
bool first_pass(char *file_name, machine_word_struct *code_img, machine_word_struct *data_img, linked_list *symbols_table_ptr, linked_list *data_list_ptr,\
linked_list *ent_list_ptr, int *p_icf, int *p_dcf) {
    FILE *fp_asm_file; /*A pointer to the input assembly file*/
    bool succeeded = TRUE; /*Holds the success of all the first pass processes. Will be changed to False if there will be any problem*/
    int ic = IC_INIT_VAL; /*instructions counter*/
    int dc = 0; /*data counter*/
    int lines_counter = 0;
    bool line_too_long = FALSE; /*Will turned to TRUE while processing a line longer than MAX_LINE_LENGTH*/
    
    /* Arrays of combinations of addressing modes. Will be used to build the instructions table.*/
    static addressing_mode addressing_modes_immediate_direct_register[] = {IMMEDIATE, DIRECT, REGISTER_DIRECT};
    static addressing_mode addressing_modes_direct_register[] = {DIRECT, REGISTER_DIRECT};
    static addressing_mode addressing_modes_direct[] = {DIRECT};
    static addressing_mode addressing_modes_direct_relative[] = {DIRECT, RELATIVE};

    static instruction instructions_table[] = {
        {MOV_OPCODE, MOV_FUNCT, MOV_NUM_OF_OPERANDS, MOV_NUM_OF_VALID_SOURCE_MODES, addressing_modes_immediate_direct_register, MOV_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*mov*/
        {CMP_OPCODE, CMP_FUNCT, CMP_NUM_OF_OPERANDS, CMP_NUM_OF_VALID_SOURCE_MODES, addressing_modes_immediate_direct_register, CMP_NUM_OF_VALID_DEST_MODES, addressing_modes_immediate_direct_register}, /*cmp*/
        {ADD_OPCODE, ADD_FUNCT, ADD_NUM_OF_OPERANDS, ADD_NUM_OF_VALID_SOURCE_MODES, addressing_modes_immediate_direct_register, ADD_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*add*/
        {SUB_OPCODE, SUB_FUNCT, SUB_NUM_OF_OPERANDS, SUB_NUM_OF_VALID_SOURCE_MODES, addressing_modes_immediate_direct_register, SUB_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*sub*/
        {LEA_OPCODE, LEA_FUNCT, LEA_NUM_OF_OPERANDS, LEA_NUM_OF_VALID_SOURCE_MODES, addressing_modes_direct, LEA_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*lea*/
        {CLR_OPCODE, CLR_FUNCT, CLR_NUM_OF_OPERANDS, CLR_NUM_OF_VALID_SOURCE_MODES, NULL, CLR_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*clr*/
        {NOT_OPCODE, NOT_FUNCT, NOT_NUM_OF_OPERANDS, NOT_NUM_OF_VALID_SOURCE_MODES, NULL, NOT_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*not*/
        {INC_OPCODE, INC_FUNCT, INC_NUM_OF_OPERANDS, INC_NUM_OF_VALID_SOURCE_MODES, NULL, INC_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*inc*/
        {DEC_OPCODE, DEC_FUNCT, DEC_NUM_OF_OPERANDS, DEC_NUM_OF_VALID_SOURCE_MODES, NULL, DEC_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*dec*/
        {JMP_OPCODE, JMP_FUNCT, JMP_NUM_OF_OPERANDS, JMP_NUM_OF_VALID_SOURCE_MODES, NULL, JMP_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_relative}, /*jmp*/
        {BNE_OPCODE, BNE_FUNCT, BNE_NUM_OF_OPERANDS, BNE_NUM_OF_VALID_SOURCE_MODES, NULL, BNE_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_relative}, /*bne*/
        {JSR_OPCODE, JSR_FUNCT, JSR_NUM_OF_OPERANDS, JSR_NUM_OF_VALID_SOURCE_MODES, NULL, JSR_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_relative}, /*jsr*/
        {RED_OPCODE, RED_FUNCT, RED_NUM_OF_OPERANDS, RED_NUM_OF_VALID_SOURCE_MODES, NULL, RED_NUM_OF_VALID_DEST_MODES, addressing_modes_direct_register}, /*red*/
        {PRN_OPCODE, PRN_FUNCT, PRN_NUM_OF_OPERANDS, PRN_NUM_OF_VALID_SOURCE_MODES, NULL, PRN_NUM_OF_VALID_DEST_MODES, addressing_modes_immediate_direct_register}, /*prn*/
        {RTS_OPCODE, RTS_FUNCT, RTS_NUM_OF_OPERANDS, RTS_NUM_OF_VALID_SOURCE_MODES, NULL, RTS_NUM_OF_VALID_DEST_MODES, NULL}, /*rts*/
        {STOP_OPCODE, STOP_FUNCT, STOP_NUM_OF_OPERANDS, STOP_NUM_OF_VALID_SOURCE_MODES, NULL, STOP_NUM_OF_VALID_DEST_MODES, NULL} /*stop*/
    }; /*This table holds all the relevant properties of all the instructions.*/
    static char *instructions_names[NUM_OF_INSTRUCTIONS] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec",\
    "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};
    static char *registers[NUM_OF_REGISTERS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    static char *directives[NUM_OF_DIRECTIVES] = {"data", "string", "entry", "extern"};
    
    if (!(open_file_with_check(file_name, &fp_asm_file, "r"))) {
        return FALSE;
    }
    
    while (!feof (fp_asm_file)) { /*Iterates lines in the input file until reaches EOF*/
        char *p_char = NULL; /*A pointer to the current char in statement*/
        char *p_end = NULL; /*A pointer to the next char after the current word in statement*/
        char *fgets_result = NULL; /*Holds the return val of fgets*/
        char statement[MAX_LINE_LENGTH + TWO] = ""; /*Holds one line from input file*/
        char symbol_name[MAX_SYMBOL_LENGTH + 1] = ""; /*a pointer to the current symbol_name in statement*/
        directive curr_directive = NO_DIRECTIVE;
        bool is_there_a_symbol = FALSE;
        char ext_parameter_name[MAX_SYMBOL_LENGTH + 1] = ""; /*Will hold external parameter if necessary*/
        char ent_parameter_name[MAX_SYMBOL_LENGTH + 1] = ""; /*Will hold entry parameter if necessary*/
        bool curr_line_succeeded = TRUE; /*Holds the success of the current line. Will change to False if there will be any problem.*/
        instruction curr_instruction; /*Will hold the current instruction if necessary*/

        if (!line_too_long) /*increment line_counter by 1 if the current line is not longer than MAX_LINE_LENGTH*/
            lines_counter += 1;

        fgets_result = fgets(statement, MAX_LINE_LENGTH + TWO, fp_asm_file); /*reads a line from the input file and stores it in the string
        pointed by statement. It returns NULL if EOF is encountered and no characters have been read*/
        if (!fgets_result) { /*EOF is encountered and no characters have been read*/
            break;
        }

        if (line_too_long) { /*If the current line is longer than MAX_LINE_LENGTH, and the current string in statement doesn't include '\n' yet,
        keep iterating*/
            if (strstr(statement, "\n"))
                line_too_long = FALSE;
            continue;
        }
        if (!(check_line_length(statement, lines_counter, file_name))) { /*Checks if the current line is longer than MAX_LINE_LENGTH*/
            succeeded = FALSE;
            line_too_long = TRUE;
            continue;
        }

        p_char = statement; /*p_char will point at the beginning of the current statement*/
        p_char = skip_spaces_and_tabs(p_char); /*p_char will points at the first non space char in statement*/
        
        if (*p_char != ';' && *p_char != '\n' && *p_char != '\0') { /*The current statement is not a comment*/
            
            /**GET SYMBOL**/
            if ((p_end = strstr(statement, ":"))) { /*There is ':' in this statementand p_end points at it*/
                is_there_a_symbol = TRUE;
                if (!(get_symbol(symbol_name, p_char, p_end, registers, instructions_names, directives, *symbols_table_ptr, lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If the symbol is legal, it is now stored in symbol_name*/
                p_char = p_end + 1; /*p_char will point to the first char after the ':'*/
                if (*p_char != '\t' && *p_char != ' ') {
                    fprintf(stdout, "%s: In line %d: There is no space after the \":\"\n", file_name, lines_counter);
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                p_char = skip_spaces_and_tabs(p_char); /*p_char will point to the next non space char in statement*/
                if (*p_char == ';') {
                    fprintf(stdout, "%s: In line %d: Symbol in the beginning of a comment statement is forbidden\n", file_name, lines_counter);
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                if (*p_char == '\n' || *p_char == '\0') {
                    fprintf(stdout, "%s: In line %d: Symbol in the beginning of an empty statement is forbidden\n", file_name, lines_counter);
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
            };

            p_char = skip_spaces_and_tabs(p_char); /*p_char will point to the next non space char in statement*/
            
            /**GET DIRECTIVE**/
            if (*p_char == '.') { /*The next non-white-space char in this statement is ".". Expecting for a directive*/
                if (!(get_directive(&curr_directive, p_char, &p_end, lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If the directive is legal, it is now stored in curr_directive*/
                p_char = p_end; /*p_char will point to the first char after the directive*/
                if (*p_char != ' ' && *p_char != '\t') {
                    fprintf(stdout, "%s: In line %d: There is no space after the directive\n", file_name, lines_counter);
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
            };

            /**GET DIRECTIVE PARAMETERS**/
            if (curr_directive == DATA_DIRECTIVE || curr_directive == STRING_DIRECTIVE) {
                if (is_there_a_symbol) {
                    if (!(add_symbol(symbol_name, dc, curr_directive, symbols_table_ptr, data_list_ptr, *ent_list_ptr, lines_counter, file_name))) {
                        succeeded = FALSE;
                        continue; /*No more errors messages according to this line will be printed*/
                    }
                    /*If there was a symbol, it is now stored in the symbols table and in data list.*/
                }

                if (curr_directive == DATA_DIRECTIVE) {
                    curr_line_succeeded &= get_and_insert_data_parameters(&p_char, data_img, &dc, lines_counter, file_name);
                }
                /*If this is a data directive with legal parameters, they have been coded to data image*/
                
                else { /*curr_directive == STRING_DIRECTIVE*/
                    curr_line_succeeded &= get_and_insert_string_parameters(&p_char, data_img, &dc, lines_counter, file_name);
                }
                /*If this is a string directive with legal parameter, it has been coded to data image*/

                succeeded &= curr_line_succeeded;
                if (!curr_line_succeeded)
                    continue; /*No more errors messages according to this line will be printed*/
            }

            else if (curr_directive == ENTRY_DIRECTIVE) {
                symbol *symbol_found_in_table_ptr = NULL; /*If the entry parameter already exists in the symbols table, this pointer will point to this symbol
                in symbols table*/
                symbol *symbol_in_ent_list_ptr = NULL;

                if (!(get_extern_or_entry_parameter(ent_parameter_name, curr_directive, &p_char, registers, instructions_names, directives, symbols_table_ptr,\
                lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If this is an entry directive with legal parameter, it is now stored as a string in ent_parameter_name*/
                symbol_found_in_table_ptr = search(ent_parameter_name, *symbols_table_ptr);
                if (symbol_found_in_table_ptr != NULL && symbol_found_in_table_ptr -> visibility == EXTERN) {
                    fprintf(stdout, "%s: In line %d: The symbol %s is already declared as external\n", file_name, lines_counter, ent_parameter_name);
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                symbol_in_ent_list_ptr = search(ent_parameter_name, *ent_list_ptr);
                if (symbol_in_ent_list_ptr == NULL) { /*The symbol was not found in ent list*/
                    if (!(insert_to_list(&symbol_in_ent_list_ptr, ent_parameter_name, 0, NON, ENTRY, NULL, lines_counter, ent_list_ptr)))
                        return FALSE; /*Dynamic allocation failure*/
                }
                /*If this is an entry directive with legal parameter, it is now stored in ent list*/
            }

            else if (curr_directive == EXTERN_DIRECTIVE) {
                if (!(get_extern_or_entry_parameter(ext_parameter_name, curr_directive, &p_char, registers, instructions_names, directives, symbols_table_ptr,\
                lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If this is an extern directive with legal parameter, it is now stored as a string in ext_parameter_name*/
                if (!(add_symbol(ext_parameter_name, 0, curr_directive, symbols_table_ptr, data_list_ptr, *ent_list_ptr, lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If this is an extern directive with legal parameter, it is now stored in symbols table*/
            }

            /**GET INSTRUCTION**/
            else {
                if (is_there_a_symbol) {
                    if (!(add_symbol(symbol_name, ic, curr_directive, symbols_table_ptr, data_list_ptr, *ent_list_ptr, lines_counter, file_name))) {
                        succeeded = FALSE;
                        continue; /*No more errors messages according to this line will be printed*/
                    }
                    /*If the symbol is legal, it is now stored in symbol_name*/
                }
                if (!(get_instruction(&curr_instruction, &p_char, instructions_table, instructions_names, lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If this is a valid instruction, it is now stored in curr_instruction*/
                if (curr_instruction.num_of_operands != 0) { /*The current instruction should get at least one operand*/
                    if (curr_line_succeeded && *p_char != '\t' && *p_char != ' ') {
                        fprintf(stdout, "%s: In line %d: There is no space after the instruction\n", file_name, lines_counter);
                        succeeded = FALSE;
                        continue; /*No more errors messages according to this line will be printed*/
                    }
                }

                p_char = skip_spaces_and_tabs(p_char); /*p_char will point to the next non space char in statement*/

                /**GET INSTRUCTION OPERANDS**/
                if (!(get_instruction_operands_and_insert_words(curr_instruction, &p_char, code_img, &ic, registers, instructions_names, directives,\
                lines_counter, file_name))) {
                    succeeded = FALSE;
                    continue; /*No more errors messages according to this line will be printed*/
                }
                /*If this is an instruction with valid operands, the instruction and the operands has been coded to code image*/
            }

            if (curr_line_succeeded) /*Checks if there is extraneous text after end of statement*/
                curr_line_succeeded &= check_extraneous_text(*p_char, lines_counter, file_name);
            
            if (ic + dc > MEMORY_SIZE) { /*The file exceeds the memory that this program reserves*/
                fprintf(stdout, "%s: Memory leak: This file exceeds permitted memory space\n", file_name);
                return FALSE;
            }

            succeeded &= curr_line_succeeded;
        };
    };

    if (!(close_file_with_check(file_name, &fp_asm_file))) { /*a message will be printed if the file did not closed*/
        succeeded = FALSE;
        return succeeded;
    }
    if (!succeeded)
        return succeeded;
    
    *p_icf = ic;
    *p_dcf = dc;
    
    add_icf_to_every_data_symbol_val(*data_list_ptr, *p_icf); /*adds icf to every data symbol in data_list and symbols_table*/
    return TRUE;
}


/*Receives a pointer to the destination string, a pointer to the beginning of a string, a pointer to a char in this string, 3 arrays of strings that together
includes all the saved words in the program, the symbols table, the line number and the name of the file. It copies the string pointed by p_start and ends at
p_end to symbol_name. It is also checks that the string that has been copied has a valid symbol syntax and returns TRUE if so and FALSE otherwise.*/
bool get_symbol(char *symbol_name, char *p_start, char *p_end, char **registers, char **instructions_names, char **directives, linked_list symbols_table,\
int lines_counter, char *file_name) {
    if (p_end > p_start + MAX_SYMBOL_LENGTH) {
        fprintf(stdout, "%s: In line %d: The symbol is too long\n", file_name, lines_counter);
        return FALSE;
    }
    scopy_until_p(symbol_name, p_start, p_end); /*Copies the text between p_char (included) and p_end (not included) into symbol_name*/
    if (!(is_valid_symbol(symbol_name, registers, instructions_names, directives))) {
        fprintf(stdout, "%s: In line %d: The symbol %s is invalid\n", file_name, lines_counter, symbol_name);
        return FALSE;
    }
    return TRUE;
}


/*Receives a pointer to the destination directive, a pointer to the beginning of a string, a pointer to an address of a char in this string,the line number and
the name of the file. It checks if the word that starts at p_start is a defined directive and set the directive accordingly. It increment p_end to point at the
first char after the current word and returns TRUE if the directive is defined and FALSE otherwise.*/
bool get_directive(directive *dest, char *p_start, char **p_end, int lines_counter, char *file_name) {
    char directive_str[MAX_DIRECTIVE_LENGTH + 1] = "";
    int i;

    for (i = 0; p_start[i] != '\0' && p_start[i] != '\n' && p_start[i] != ' ' && p_start[i] != '\t'; i++) /*find end of instruction name*/
        ;
    *p_end = p_start + i;
    if (*p_end > p_start + MAX_DIRECTIVE_LENGTH) { /*There is no legal directive that has this length*/
        fprintf(stdout, "%s: In line %d: Undefined directive\n", file_name, lines_counter);
        return FALSE;
    }
    scopy_until_p(directive_str, p_start, *p_end);

    if (strcmp(directive_str, ".data") == 0) {
        *dest = DATA_DIRECTIVE;
        return TRUE;
    }
    else if (strcmp(directive_str, ".string") == 0) {
        *dest = STRING_DIRECTIVE;
        return TRUE;
    }
    else if (strcmp(directive_str, ".entry") == 0) {
        *dest = ENTRY_DIRECTIVE;
        return TRUE;
    }
    else if (strcmp(directive_str, ".extern") == 0) {
        *dest = EXTERN_DIRECTIVE;
        return TRUE;
    }
    else {
        fprintf(stdout, "%s: In line %d: The directive %s is undefined\n", file_name, lines_counter, directive_str);
        return FALSE;
    }
}


/*Receives a pointer to the address of the current char in the statement, a pointer to the beginning of the data image, the current dc, the line number and
the name of the file. It inserts the integers that are written cotrrectly in p_char to data_img and increment dc by the amount of words that was inserted.
It also increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if there was a problem and TRUE otherwise*/
bool get_and_insert_data_parameters(char **p_char, machine_word_struct *data_img, int *dc, int lines_counter, char *file_name) {
    bool are_there_more_parameters = TRUE;

    *p_char = skip_spaces_and_tabs(*p_char); /*p_char will point to the next non space char in statement*/
    if (**p_char == ',') {
        fprintf(stdout, "%s: In line %d: There is a comma before first parameter\n", file_name, lines_counter);
        return FALSE;
    }
    if (**p_char == '\n' || **p_char == '\0') {
        fprintf(stdout, "%s: In line %d: Expecting to receive a parameter after .data\n", file_name, lines_counter);
        return FALSE;
    }

    while (are_there_more_parameters) { /*Keeps iterating as long as there is a comma after the current parameter*/
        char str_parameter[MAX_LINE_LENGTH + 1] = "";
        int parameter;
        machine_word_struct machine_word_parameter = {0};

        *p_char = get_next_par(str_parameter, *p_char); /*str_parameter will hold the next parameter and p_char will point at the next char after this parameter*/
        *p_char = skip_spaces_and_tabs(*p_char); /*p_char will point at the next char after the last space or tab after the parameter that stored in str_parameter*/
        
        if (*str_parameter == '\0') {
            fprintf(stdout, "%s: In line %d: Expecting to receive a parameter after a comma\n", file_name, lines_counter);
            return FALSE;
        }
        if(!(isnum(str_parameter))) {
            fprintf(stdout, "%s: In line %d: The parameter %s is not a legal number\n", file_name, lines_counter, str_parameter);
            return FALSE;
        }
        parameter = atoi(str_parameter);
        if (!(int_to_word(parameter, &machine_word_parameter, lines_counter, file_name))) { /*machine_word_parameter will hold the parameter*/
            return FALSE;
        }

        data_img[*dc] = machine_word_parameter; /*Inserts the parameter as a machine word to data image*/
        
        (*dc)++; /*increment dc by 1*/
        
        if (**p_char == ',') { /*if the next char is ',', increment *p_char by 1 and expect to receive more parameters.*/
            (*p_char)++;
        }
        else
            are_there_more_parameters = FALSE;
    }

    return TRUE;
}


/*Receives a pointer to the address of the current char in the statement, a pointer to the beginning of the data image, the current dc, the line number and
the name of the file. If p_char holds a valid string, Its chars will be inserted to data_img (and also '\0' in the end of the string) and dc will be incremented
by the amount of machine words that was inserted. It also increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if
there was a problem and TRUE otherwise*/
bool get_and_insert_string_parameters(char **p_char, machine_word_struct *data_img, int *dc, int lines_counter, char *file_name) {
    char string_par[MAX_LINE_LENGTH] = "";
    char *p_end_of_string = NULL;
    int i;
    bool is_zero = FALSE; /*Will be used as a flag for the loop at the end of the function*/

    *p_char = skip_spaces_and_tabs(*p_char); /**p_char will point to the next non space char in statement*/
    if (**p_char != '\"') {
        fprintf(stdout, "%s: In line %d: Expecting to receive a string after .string\n", file_name, lines_counter);
        return FALSE;
    }
    p_end_of_string = strrchr(*p_char, '\"'); /*p_end_of_string will point to the last '\"' in p_char*/
    if (p_end_of_string == *p_char) {
        fprintf(stdout, "%s: In line %d: Missing closing quotation marks\n", file_name, lines_counter);
        return FALSE;
    }

    (*p_char)++; /**p_char will point at the next char after the first '\"'*/
    scopy_until_p(string_par, *p_char, p_end_of_string); /*string_par will hold the string between the quotations marks*/
    *p_char = p_end_of_string + 1; /**p_char will point at the next char after the last '\"'*/
    *p_char = skip_spaces_and_tabs(*p_char); /**p_char will point at the next non space char after the last '\"'*/
    if (**p_char != '\0' && **p_char != '\n') {
        fprintf(stdout, "%s: In line %d: Extraneous text after string\n", file_name, lines_counter);
        return FALSE;
    }

    for (i = 0; !is_zero; i++) { /*Iterates chars in string_par, convert every char to a machine word, inserts it to data image and increment dc by 1*/
        machine_word_struct machine_word_parameter = {0};

        char_to_word(string_par[i], &machine_word_parameter); /*convert the char string_par[i] to machine word*/
        data_img[*dc] = machine_word_parameter; /*insertss the machine wordin data image*/
        (*dc)++;
        if (string_par[i] == '\0') /*reached end of string. Will execute one more iteration to insert '\0' to data image*/
            is_zero = TRUE;
    }
    return TRUE;
}


/*Receives a pointer to the destination strig, the current directive, a pointer to the address of the current char in the statement, 3 arrays of strings
that together includes all the saved words in the program, a pointer to the symbols table, the line number and the name of the file. It checks that the next
word in p_char is a valid symbol name by calling get_symbol and copies it to ext_or_ent_parameter_name. It also increments p_char to point at the next char
after all chars that has been analyzed. Returns FALSE if there was a problem and TRUE otherwise*/
bool get_extern_or_entry_parameter(char *ext_or_ent_parameter_name, directive curr_directive, char **p_char, char **registers, char **instructions_names,\
char **directives, linked_list *symbols_table_ptr, int lines_counter, char *file_name) {
    char * p_end = NULL;
    int i;
    bool succeeded = TRUE;

    *p_char = skip_spaces_and_tabs(*p_char); /**p_char will point to the next non space char in statement*/
    for (i = 0; (*p_char)[i] != '\0' && (*p_char)[i] != '\n' && (*p_char)[i] != ' ' && (*p_char)[i] != '\t'; i++) /*finds the end of external symbol name*/
        ;
    p_end = (*p_char) + i; /*p_end will point to the next char after the extern/entry parameter*/
    if (p_end == *p_char) {
        if (curr_directive == EXTERN_DIRECTIVE)
            fprintf(stdout, "%s: In line %d: Expecting to receive a symbol name after .extern\n", file_name, lines_counter);
        else 
            fprintf(stdout, "%s: In line %d: Expecting to receive a symbol name after .entry\n", file_name, lines_counter);
        return FALSE;
    }

    succeeded = get_symbol(ext_or_ent_parameter_name, *p_char, p_end, registers, instructions_names, directives, *symbols_table_ptr, lines_counter, file_name);
    /*If the first word in p_char was a valid symbol name, it is now stored in ext_parameter_name*/
    
    *p_char = p_end; /**p_char will point at the next char after the extern/entry parameter*/
    *p_char = skip_spaces_and_tabs(*p_char); /**p_char will point at the next non space char after the extern/entry parameter*/
    return succeeded;
}


/*Receives a pointer to the current instruction, a pointer to the address of the current char in the statement, a pointer to the instructions table, the line
number and the file name. It checks that the next word in p_char is a valid instruction name by calling get_instruction_name and sets curr_instruction
according to the name of the instruction. It also increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if there
was a problem and TRUE otherwise*/
bool get_instruction(instruction *curr_instruction, char **p_char, instruction *instructions_table, char **instructions_names, int lines_counter,\
char *file_name) {
    char str_instruction[MAX_INSTRUCTION_NAME_LENGTH + 1] = "";

    if (!(get_instruction_name(str_instruction, p_char, lines_counter, file_name))) {
        return FALSE;
    }
    /*If the first word in p_char was a valid instruction word, it is now stored in str_instruction*/

    if (strcmp (str_instruction, instructions_names[MOV]) == 0) {
        *curr_instruction = instructions_table[MOV];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[CMP]) == 0) {
        *curr_instruction = instructions_table[CMP];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[ADD]) == 0) {
        *curr_instruction = instructions_table[ADD];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[SUB]) == 0) {
        *curr_instruction =  instructions_table[SUB];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[LEA]) == 0) {
        *curr_instruction = instructions_table[LEA];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[CLR]) == 0) {
        *curr_instruction = instructions_table[CLR];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[NOT]) == 0) {
        *curr_instruction = instructions_table[NOT];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[INC]) == 0) {
        *curr_instruction = instructions_table[INC];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[DEC]) == 0) {
        *curr_instruction = instructions_table[DEC];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[JMP]) == 0) {
        *curr_instruction = instructions_table[JMP];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[BNE]) == 0) {
        *curr_instruction = instructions_table[BNE];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[JSR]) == 0) {
        *curr_instruction = instructions_table[JSR];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[RED]) == 0) {
        *curr_instruction = instructions_table[RED];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[PRN]) == 0) {
        *curr_instruction = instructions_table[PRN];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[RTS]) == 0) {
        *curr_instruction = instructions_table[RTS];
        return TRUE;
    }
    else if (strcmp(str_instruction, instructions_names[STOP]) == 0) {
        *curr_instruction = instructions_table[STOP];
        return TRUE;
    }
    else {
        fprintf(stdout, "%s: In line %d: Undefined instruction name\n", file_name, lines_counter);
        return FALSE;
    }
}


/*Receives the current instruction, a pointer to the address of the current char in the statement, a pointer to the beginning of the code image, the current
ic, 3 arrays of strings that together includes all the saved words in the program, the line number and the file name. It checks that there is the right amount
of operand for the current instruction, that they are valid for this instruction, inserts the instruction word and its operands to code image, increments ic by
the number of words inserted and increments p_char to point at the next char after all chars that has been analyzed. Returns FALSE if there was a problem and
TRUE otherwise*/
bool get_instruction_operands_and_insert_words(instruction curr_instruction, char **p_char, machine_word_struct *code_img, int *ic, char **registers,\
char **instructions_names, char **directives, int lines_counter, char *file_name) {
    int curr_instruction_ic = *ic;
    machine_word_struct curr_instruction_word = {0};

    instruction_to_instruction_word(curr_instruction, &curr_instruction_word); /*Converts the current instruction into an instruction word and stores it in
    curr_instruction_word.*/
    code_img[curr_instruction_ic] = curr_instruction_word; /*Inserts the instruction word to code image*/
    (*ic)++; /*ic will point to the next index in code image*/

    if (curr_instruction.num_of_operands > 0) { /*The current instruction should get at least 1 operand*/
        int i; /*operand index*/
        
        *p_char = skip_spaces_and_tabs(*p_char); /**p_char will point to the next non space char in statement*/
        if (**p_char == ',') {
            fprintf(stdout, "%s: In line %d: There is a comma before first operand\n", file_name, lines_counter);
            return FALSE;
        }
        if (**p_char == '\n' || **p_char == '\0') {
            fprintf(stdout, "%s: In line %d: Missing opperands\n", file_name, lines_counter);
            return FALSE;
        }

        for (i = 0; i < curr_instruction.num_of_operands; i++) { /*Iterates operands*/
            char str_operand[MAX_LINE_LENGTH + 1] = "";
            machine_word_struct operand_machine_word = {0};
            bool is_dest; /*TRUE if the current machine word represents a dest operand and FALSE if it is source operand*/

            *p_char = get_next_par(str_operand, *p_char); /*str_operand will hold the next operand as a string and *p_char will point at the next char after
            this operand*/
            *p_char = skip_spaces_and_tabs(*p_char); /*p_char will point at the next char after the last space or tab after the operand that stored in
            str_operand*/
            if (*str_operand == '\0') {
                if (**p_char == ',')  /*reaches here if there was another comma before the current one and there was'nt a parameter between them*/
                fprintf(stdout, "%s: In line %d: Multiple consecutive commas\n", file_name, lines_counter);
                else
                    fprintf(stdout, "%s: In line %d: Missing opperands\n", file_name, lines_counter);
                return FALSE;
            }
            if (!(str_operand_to_word_and_addressing_mode(str_operand, &operand_machine_word, registers, instructions_names, directives, lines_counter,\
            file_name))) {
                return FALSE;
            }
            /*If the syntax of the operand is valid, the addresing mode field in operand_machine_word was updated and if possible, also the machine_word. If
            not, the is_set field was updated to FALSE.*/

            if ((i == curr_instruction.num_of_operands - 1)) /*last operand is always dest*/
                is_dest = TRUE;
            else
                is_dest = FALSE;

            if (!(set_addressing_mode_in_word(&(code_img[curr_instruction_ic]), curr_instruction, is_dest, operand_machine_word.addressing_mode, lines_counter,\
            file_name))) {
                return FALSE;
            }
            /*If the addressing mode is valid for the current instruction, the instruction machine word was updated according to the addressing mode.*/

            code_img[*ic] = operand_machine_word; /*inserts the operand machine word to code image*/
            (*ic)++; /*ic will point to the next index in code image*/

            if (i == 0 && curr_instruction.num_of_operands == 2) { /*Expecting to receive additional operand*/
                if (**p_char != ',') {
                    if (**p_char == '\n' || **p_char == '\0')
                        fprintf(stdout, "%s: In line %d: Missing opperand\n", file_name, lines_counter);
                    else
                        fprintf(stdout, "%s: In line %d: Missing comma after the first operand\n", file_name, lines_counter);
                    return FALSE;
                }
                else {
                    (*p_char)++; /**p_char will point to the next char after the comma*/
                }
            }   
        }
    }
    return TRUE;
}


/*Receives the data linked list and the final ic, iterates all symbols in data list and add icf to every symbol value in this list and also to the value
of this symbol that stores in the symbols table*/
void add_icf_to_every_data_symbol_val(linked_list data_list, int icf) {
    symbol *head = data_list.list_head;

    while (head != NULL) { /*Iterates all symbols in data list*/
        (head->val) += icf;
        (head->address_in_symbols_table -> val) += icf; /*Using the pointer from the symbol stored in data_list to the same symbol name stored in symbols_table*/
        head = head -> next;
    }
}