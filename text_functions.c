/*This file contains functions that helps parsing text. All the functions in this file are called directly or indirectly from first_pass function.*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "text_functions.h"


/*Receives a string, the number of the line in the input file and the file name. It checks if the string is longer than MAX_LINE_LENGTH chars
excluding '\n' and '\0'/ If so, it prints an error message and returns FALSE. Otherwise, it returns TRUE.*/
bool check_line_length(char *statement, int lines_counter, char *file_name) {
    int i;

    for(i = 0; statement[i] != '\0' && statement[i] != '\n'; i++); /*finds the end of the string*/
    
    if (i > MAX_LINE_LENGTH && statement[i] != '\n') { /*There are more than MAX_LINE_LENGTH chars excluding '\n'*/
        fprintf(stdout, "%s: In line %d: The line is too long\n", file_name, lines_counter);
        return FALSE;
    }
    return TRUE;
}


/*Receives a char, the number of the line in the input file and the file name. If the char is not '\0' or '\n', prints an error message and returns
FALSE. Otherwise, returns TRUE.*/
bool check_extraneous_text(char p_char, int lines_counter, char *file_name) {
    if (p_char != '\0' && p_char != '\n') {
        fprintf(stdout, "%s: In line %d: Extraneous text after end of statement\n", file_name, lines_counter);
        return FALSE;
    }
    return TRUE;
}


/*Receives a pointer to char, increments it by the number of chars that exist until the next char that is not a space or a tab
and returns that pointer. Meaning the returned pointer points at the next char that is not a space or a tab.*/
char * skip_spaces_and_tabs (char *p_char) {
    while (p_char[0] != '\n' && p_char[0] != '\0' && (p_char[0] == ' ' || p_char[0] == '\t')) /*Iterates elements in the array that p_char points
    at until it reaches a char that is not a space/tab or until it reaches '\0' or '\n' and increment p_char by the number of iterations it does*/
        p_char++;
    return p_char;
}


/*Receives a pointer to destination string, another pointer to a string and a pointer to a char that points somewhere in the second string.
It copies the string pointed by str to the string pointed by dest until it reaches p_end (not included), and doesn't return anything. This func assumes
that p_end points at a char somewhere in the string pointed by str.*/
void scopy_until_p (char *dest, char *str, char *p_end) {
    int num_of_chars = 0; /*Will hold the number of chars that will be coppied to dest*/
    
    while (str + num_of_chars != p_end) { /*Iterates chars until it reaches the char that p_end points at*/
        num_of_chars++;
    };
    strncpy (dest, str, num_of_chars); /*Copies the string pointed by str to the string pointed by dest until the char that p_end points at*/
}


/*Receives a pointer to destination string and a pointert to the beggining of a string. It skips spaces, copies the next word (until a space or ','
into and returns a pointer to the next char after this word*/
char * get_next_par(char *str_parameter, char *p_char) {
    char * p_end; /*Will point at the first char after the current word*/
    int i=0;

    p_char = skip_spaces_and_tabs(p_char);
    for (i = 0; p_char[i] != '\0' && p_char[i] != '\n' && p_char[i] != ' ' && p_char[i] != '\t' && p_char[i] != ','; i++)  /*finds the end of the word*/
        ;
    p_end = p_char + i;
    scopy_until_p(str_parameter, p_char, p_end);
    return p_end;
}


/*Receives a pointer to destination string, a pointert to the address of a string, the number of the line in the input file and the file name. It skips
spaces, checks the next word is not longer than MAX_INSTRUCTION_NAME_LENGTH (If not, prints an error message and returns FALSE), copies it into
str_instruction and returns TRUE. The function also increments *p_char to points at the next non space char after the current word.*/
bool get_instruction_name(char *str_instruction, char **p_char, int lines_counter, char *file_name) {
    char *p_end;
    int i;

    *p_char = skip_spaces_and_tabs(*p_char);
    for (i = 0; (*p_char)[i] != '\0' && (*p_char)[i] != '\n' && (*p_char)[i] != ' ' && (*p_char)[i] != '\t'; i++) /*find end of the instruction name*/
        ;
    p_end = *p_char + i;
    if (p_end > *p_char + MAX_INSTRUCTION_NAME_LENGTH) { /*The word is longer than any defined instruction name.*/
        fprintf(stdout, "%s: In line %d: Undefined instruction name\n", file_name, lines_counter);
        return FALSE;
    } 
    scopy_until_p(str_instruction, *p_char, p_end);
    *p_char = p_end; /**p_char will point at the next non space char after the instruction name*/
    return TRUE;
}


/*Receives a string, checks if it is a legal decimal integer and returns TRUE if so and FALSE otherwise.*/
bool isnum(char * str) {
    int i;

    if (str[0] == '-' || str[0] == '+') /*increments str by 1 if it is pointing at a sign char*/
        str++;
    for (i = 0; str[i] != '\0'; i++) { /*Iterates chars and checks that they are digits*/
        if (!(isdigit(str[i])))
            return FALSE;
    }
    return TRUE;
}


/*Receives a string and 3 arrays of strings that together includes all the saved words in the program. It checks that the string is not equal to any
saved word (by calling is_saved_word)and that its first char is a letter and all others are letters or digits. It returns TRUE if so and FALSE otherwise.*/
bool is_valid_symbol(char *symbol_name, char **registers, char **instruction_names, char **directives) {
    int i = 0;

    if (!(isalpha(symbol_name[i])))
        return FALSE;
    for (i = 0; symbol_name[i] != '\0'; i++) { /*Iterates all chars in symbol_name*/
        if (!(isalnum(symbol_name[i])))
            return FALSE;
    }
    if (is_saved_word(symbol_name, registers, instruction_names, directives))
        return FALSE;
    return TRUE;
}


/*Receives a string and 3 arrays of strings that together includes all the saved words in the program. It checks that the string is not equal to any
saved word and returns TRUE if so and FALSE otherwise.*/
bool is_saved_word(char *symbol_name, char **registers, char **instruction_names, char **directives) {
    int i;

    for (i = 0; i < NUM_OF_INSTRUCTIONS; i++) { /*Iterates all instructions names*/
        if (strcmp(symbol_name, instruction_names[i]) == 0)
            return TRUE;
    }
    for (i = 0; i < NUM_OF_REGISTERS; i++) { /*Iterates all registers names*/
        if (strcmp(symbol_name, registers[i]) == 0)
            return TRUE;
    }
    for (i = 0; i < NUM_OF_DIRECTIVES; i++) { /*Iterates all directives names*/
        if (strcmp(symbol_name, directives[i]) == 0)
            return TRUE;
    }
    return FALSE;
}


/*Receives a string, 3 arrays of strings that together includes all the saved words in the program, the current line number and the file name. It checks
that the string is a valid symbol (by callig to is_valid_symbol) and prints an error message if not. This function will be called when we expect to recieve
a symbol operand. It returns TRUE if str_operand is a valid symbol and FALSE otherwise.*/
bool is_valid_symbol_operand(char *str_operand, char **registers, char **instruction_names, char **directives, int lines_counter, char *file_name) {
    int i;
    int str_operand_len = 0;

    for (i = 0; str_operand[i] != '\0'; i++) { /*calculates str_operand length*/
        str_operand_len++;
    }
    if (str_operand_len > MAX_SYMBOL_LENGTH) {  /*The word is too long.*/
        fprintf(stdout, "%s: In line %d: Invalid operand\n", file_name, lines_counter);
        return FALSE;
    }
    if (!(is_valid_symbol(str_operand, registers, instruction_names, directives))) {
        fprintf(stdout, "%s: In line %d: %s is invalid operand\n", file_name, lines_counter, str_operand);
        return FALSE;
    }
    return TRUE;
}