/*The purpose of this program is to imulate an assembler. It is activated by inserting names of assembly files in the command line.
The interaction with the user's input is managed by main function. For every input file, main calls other function in assembler.c that processes
the file, prints errors if necessary and creates output files if the entire process was succesfully completed.
Input: standart input.
Output: standart output for errors messages and files with the suffix: .ob, .ent, .ext.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "symbols.h"
#include "global_structures.h"
#include "assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "create_files.h"


/*main receives arguments from the command line: the first is argc (The number of words in the command line) and the second is an array of the
words that are written in the command line. The first element in this array is a pointer to "./assembler" and the rest are pointers to the words written
after "./assembler". These words should be names (without the suffix '.as') of assembly files that are placed in the same folder. IF so, main will execute
the assembler process which includes passing over the file, finding errors and printing them, and if there are no errors, output files will be created:
.ob file and .ext and .ent files if necessary. main returns 0 if the entire process was succesfully completed.*/
int main(int argc, char *argv[]) {
    int i;
    bool succeeded = TRUE; /*Holds the success of all processes. Will change to False if there will be any problem*/
    
    if (argc == 1) { /*No input files*/
        fprintf(stdout, "There is no input file. You hsould write a name of file in the command line\n");
        exit (0);
    }
    for (i = 1; i < argc; i++) { /*Iterates all words written in the command line after ./assembler*/
        succeeded = process_asm_file(argv[i]);
        if (!succeeded) {
            fprintf(stdout, "There was a problem while processing the file \"%s.as\"\n", argv[i]);
            fprintf(stdout, "\n"); /*separates between error messages of different files*/
        }
    }
    return 0;
}


/*Receives a string asm_file_name and if this string is a name of an assembly file in the same folder (without the suffix '.as'), it process the
file and creates output files: asm_file_name.ob and asm_file_name.ext and asm_file_name.ent if there are externals and entries symbols declared in
asm_file_name.as. If there are errors in asm_file_name, there will be no output files and errors messages will be printed to the standart output. The function
returns True if the entire process was succesfully completed.*/
bool process_asm_file(char *asm_file_name) {
    machine_word_struct code_img[MEMORY_SIZE]; /*Table of all instruction machine words and additional data*/
    machine_word_struct data_img[MEMORY_SIZE]; /*Table of all data machine words and additional data*/
    machine_word_struct *object_img = NULL; /*A pointer that later will point to the beggining of an array that will hold the entire machine code (including code
    and data.*/
    linked_list symbols_table = {SYMBOLS_TABLE, 0, NULL}; /*Table of all symbols declared in the input file*/
    linked_list data_list = {DATA_LIST, 0, NULL}; /*Table of all data symbols declared in the input file*/
    linked_list ent_list = {ENT_LIST, 0, NULL}; /*Table of all entry symbols declared in the input file*/
    linked_list ext_list = {EXT_LIST, 0, NULL}; /*Table of all addresses in the machine code of external symbols*/
    bool succeeded = TRUE; /*Holds the success of all the file process. Will change to False if there will be any problem*/
    int icf, dcf; /*Will hold the final instruction counter and the final data counter*/
    char asm_file_name_with_suffix[MAX_FILE_NAME_LENGTH + 1]; /*Will hold the name of the current input file including the .as suffix*/

    strcpy(asm_file_name_with_suffix, asm_file_name);
    strcat(asm_file_name_with_suffix, ".as"); /*adds the suffix ".as"*/

    succeeded = first_pass(asm_file_name_with_suffix, code_img, data_img, &symbols_table, &data_list, &ent_list, &icf, &dcf); /*Identify all symbols and insert
    a part of the machine words to code_img and data_img.*/
    if (!succeeded) {
        free_all_dynamic_allocations(symbols_table, data_list, ent_list, ext_list);
        return succeeded;
    }

    create_object_img(code_img, data_img, icf, dcf, &object_img); /*object_img holds the entire machine code: first instructions image and then data image.*/
    succeeded &= second_pass(asm_file_name_with_suffix, object_img, symbols_table, ent_list, icf, &ext_list); /*Complete building machine code.*/
    if (!succeeded) {
        free_all_dynamic_allocations(symbols_table, data_list, ent_list, ext_list);
        return succeeded;
    }

    succeeded &= create_ob_file(object_img, asm_file_name, icf, dcf);
    if (ext_list.list_head != NULL)
        succeeded &= create_ext_file(ext_list, asm_file_name);
    if (ent_list.list_head != NULL)
        succeeded &= create_ent_file(ent_list, asm_file_name);
    free_all_dynamic_allocations(symbols_table, data_list, ent_list, ext_list);
    return succeeded;
}


/*Receives the code image, the data image (arrays of machine_word_struct), the final ic, the final dc and a pointer to the object image pointer. In the end
of the function, the last pointer will points to the beggining of the machine code array. In the machine code array, code will apear first and data last.*/
void create_object_img(machine_word_struct *code_img, machine_word_struct * data_img, int icf, int dcf, machine_word_struct **object_img_ptr) {
    int dc; /*data counter*/
    
    *object_img_ptr = code_img;
    for (dc = 0; dc < dcf; dc++) /*Iterates data_img elements and assigns them afte all instructions words*/
        (*object_img_ptr)[icf + dc] = data_img[dc];
    return;
}

/*Receives all 4 linked list that has been created and free all dinamic allocations.*/
void free_all_dynamic_allocations(linked_list symbols_table, linked_list data_list, linked_list ent_list, linked_list ext_list) {
    destroy_list(symbols_table);
    destroy_list(data_list);
    destroy_list(ent_list);
    destroy_list(ext_list);
    return;
}