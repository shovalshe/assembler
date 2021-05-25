/*This file contains functions that creates the output files of the program and prints into them the relevant data. All the functions in this file are
called directly from main  function.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "symbols.h"
#include "global_structures.h"
#include "create_files.h"


/*Receives a pointer to the beginning of the array that includes the entire machine code, the name of the input file, the final ic and the final dc.
It creates a file with the same name as the input file with the suffix ".ob". The first line in this file will be the instructions image
(code image) size (amount of machine words) and data image size. The rest of the lines will contain the machine code: for every machine word, it includes
the address of the word in memory, the word and the attribute "A,R,E". Returns TRUE if the file was created and closed succesfuly and FALSE otherwise.*/
bool create_ob_file(machine_word_struct *object_img, char *asm_file_name, int icf, int dcf) {
    FILE *fp_ob; /*pointer to the output ob file*/
    int oc; /*object counter*/
    char ob_file_name[MAX_FILE_NAME_LENGTH + 1];

    strcpy(ob_file_name, asm_file_name);
    strcat(ob_file_name, ".ob"); /*adds the suffix ".ob"*/
    
    if (!(open_file_with_check(ob_file_name, &fp_ob, "w"))) /*a message will be printed if the file did not opened*/
        return FALSE;
    fprintf(fp_ob, "%d %d\n", icf - 100, dcf); /*first line of output file: instructions image size and data image size*/
    
    for (oc = IC_INIT_VAL; oc < icf + dcf; oc++) { /*Iterates elements in object_img*/
        fprintf(fp_ob, "%04d %03X %c\n", oc, *((unsigned int*) &(object_img[oc].machine_word)), object_img[oc].ARE);
    }
    
    if (!(close_file_with_check(ob_file_name, &fp_ob))) /*a message will be printed if the file did not closed*/
        return FALSE;
    return TRUE;
}


/*Receives the externals linked list and the name of the input file. It creates a file with the same name as the input file with the suffix ".ext".
This file will contain a line for every address in the code that contains a machine word of an external symbol. Every line includes the name of the
external symbol and the address of the data word that reffers to this symbol. Returns TRUE if the file was created and closed succesfuly and FALSE
otherwise.*/
bool create_ext_file(linked_list ext_list, char *asm_file_name) {
    FILE *fp_ext; /*pointer to the output ext file*/
    char ext_file_name[MAX_FILE_NAME_LENGTH + 1];
    
    symbol * ext_list_head = ext_list.list_head;
    strcpy(ext_file_name, asm_file_name);
    strcat(ext_file_name, ".ext"); /*adds the suffix ".ext"*/
    
    if (!(open_file_with_check(ext_file_name, &fp_ext, "w"))) /*a message will be printed if the file did not opened*/
        return FALSE;

    
    while (ext_list_head != NULL) { /*Iterates nodes ext_list*/
        fprintf(fp_ext, "%s %04d\n", ext_list_head->name, ext_list_head->val);
        ext_list_head = ext_list_head -> next;
    }
    
    if (!(close_file_with_check(ext_file_name, &fp_ext))) /*a message will be printed if the file did not closed*/
        return FALSE;
    return TRUE;
}


/*Receives the entries linked list and the name of the input file. It creates a file with the same name as the input file with the suffix ".ent".
This file will contain a line for every entry symbol in the symbols table. Every line includes the name of the symbol and its value in the symbols
table. Returns TRUE if the file was created and closed succesfuly and FALSE otherwise.*/
bool create_ent_file(linked_list ent_list, char *asm_file_name) {
    FILE *fp_ent; /*pointer to the output ext file*/
    char ent_file_name[MAX_FILE_NAME_LENGTH + 1];
    symbol *head;
    
    strcpy(ent_file_name, asm_file_name);
    strcat(ent_file_name, ".ent"); /*adds the suffix ".ent"*/
    
    if (!(open_file_with_check(ent_file_name, &fp_ent, "w"))) /*a message will be printed if the file did not opened*/
        return FALSE;
    head = ent_list.list_head;
    
    while (head != NULL) { /*Iterates nodes ent_list*/
        fprintf(fp_ent, "%s %04d\n", head ->name, head ->val);
        head = head -> next;
    }
    
    if (!(close_file_with_check(ent_file_name, &fp_ent))) /*a message will be printed if the file did not closed*/
        return FALSE;
    return TRUE;
}