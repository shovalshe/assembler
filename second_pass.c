/*This file contains the function second_pass that includes 2 steps:
1. passing over all symbols that declared as entries and updating their visibility attribute to entry in the symbols table and their value in the ent list
according to the value in the symbols table.
2. passing over all the object image (includes instruction and data image) and updating words that are not coded yet.
It is also includes functions that executes actions in the second pass.
Since the second pass is separated to 2 steps, the errors messages of the second pass will also be separated to 2 ascending serieses (It was approved
in the Forum)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "symbols.h"
#include "global_structures.h"
#include "machine_words.h"
#include "second_pass.h"


/*Receives the file name, a pointer to the object image, the symbols table, the ent list, the final ic and a pointer to the ext list. The function executes
2 steps: 1. passing over all symbols that declared as entries and updating their visibility attribute to entry in the symbols table and their value in the
ent list according to the value in the symbols table. 2. passing over all the object image (includes instruction and data image) and updating words that are
not coded yet. It prints errors mesages if necessary - at most one for every line in the input file. Returns TRUE if the entire process was succesfully
completed and FALSE otherwise.*/
bool second_pass(char *file_name, machine_word_struct *object_img, linked_list symbols_table, linked_list ent_list, int icf, linked_list *ext_list) {
    bool succeeded = TRUE; /*Holds the success of all the second pass processes. Will be changed to False if there will be any problem*/

    if (!(update_entry_attribute(ent_list, symbols_table, file_name))) {
        succeeded = FALSE;
    }
    if (!(complete_missing_words(object_img, symbols_table, icf, ext_list, file_name))) {
        succeeded = FALSE;
    }
    return succeeded;
}


/*Receives the ent list, the symbols table and the file name and passes all symbols in ent list and searches every symbol in the symbols table. If the symbol
was found, it updates its visibility attribute to entry in the symbols table, updates the value of the symbol in the ent list according to the value in the
symbols table and returns TRUE. Else, it prints an error message and returns FALSE.*/
bool update_entry_attribute(linked_list ent_list, linked_list symbols_table, char *file_name) {
    bool succeeded = TRUE;
    symbol *head = ent_list.list_head;

    while (head != NULL) { /*Iterates all symbols in ent_list*/
        bool curr_iteration_succeeded = TRUE;
        symbol *symbol_in_symbols_table_ptr = search(head -> name, symbols_table);
        /*If head -> name is stored in the symbols table, symbol_in_symbols_table_ptr is now pointing to the symbol in the symbols table and if not, it is
        pointing to NULL.*/

        if (symbol_in_symbols_table_ptr == NULL) {
            fprintf(stdout, "%s: In line %d: The symbol %s is undeclared\n", file_name, head -> line_number, head -> name);
            curr_iteration_succeeded = FALSE;
            succeeded &= curr_iteration_succeeded;
        }
        else { /*The symbol was found in the symbols table*/
            symbol_in_symbols_table_ptr -> visibility = ENTRY;
            head -> val = symbol_in_symbols_table_ptr -> val;
        }
        head = head -> next;
    }
    return succeeded;
}


/*Receives a pointer to the object image the symbols table, the final ic, a pointer to the ext list and the file name. It passes over all the object image
and updates words that are not coded yet (words of relative and direct addresess modes). Returns FALSE if there was a problem and TRUE otherwise*/
bool complete_missing_words(machine_word_struct *object_img, linked_list symbols_table, int icf, linked_list *ext_list, char *file_name) {
    bool succeeded = TRUE;
    int oc; /*object counter*/

    for (oc = IC_INIT_VAL; oc < icf; oc++) { /*Iterates the object image*/        
        if (!(object_img[oc].is_set)) { /*The current machine word is not set yet*/
            symbol *symbol_in_symbols_table_ptr = search(object_img[oc].symbol_operand_name, symbols_table);
            /*If object_img[oc].symbol_operand_name is stored in the symbols table, symbol_in_symbols_table_ptr is now pointing to the symbol in the
            symbols table and if not, it is pointing to NULL.*/
            machine_word_struct curr_data_word = {0};

            if (symbol_in_symbols_table_ptr == NULL) {
                fprintf(stdout, "%s: In line %d: The symbol %s is undeclared\n", file_name, object_img[oc].line_number, object_img[oc].symbol_operand_name);
                succeeded = FALSE;
            }

            else if (object_img[oc].addressing_mode == RELATIVE) { /*The symbol was found in the symbols table and it is a relative operand*/
                int diff; /*Will hold the difference between the addresses of the current machine word and the symbol operand*/
                if (symbol_in_symbols_table_ptr -> visibility == EXTERN) {
                    fprintf(stdout, "%s: In line %d: External relative operands are forbidden\n", file_name, object_img[oc].line_number);
                    succeeded = FALSE;
                }
                else if (symbol_in_symbols_table_ptr -> memory == DATA) {
                    fprintf(stdout, "%s: In line %d: Relative operands that are data symbols are forbidden\n", file_name, object_img[oc].line_number);
                    succeeded = FALSE;
                    /*******************************************************should I enable relative operands that are data symbols?*****************************************/
                }
                diff = symbol_in_symbols_table_ptr -> val - oc; /*Calculates addresses difference*/
                set_data_word(diff, 'A', &curr_data_word); /*curr_data_word will hold the machine word with the relevant data*/
            }

            else if (object_img[oc].addressing_mode == DIRECT) { /*The symbol was found in the symbols table and it is a direct operand*/
                if (symbol_in_symbols_table_ptr -> visibility == EXTERN) { /*If this symbol is external, inserts the address of the operand word to ext list*/
                    if (!(add_to_ext_list(ext_list, oc, object_img)))
                        return FALSE; /*Dynamic allocation failure*/
                    set_data_word(0, 'E', &curr_data_word); /*curr_data_word will hold the machine word with the relevant data*/
                }
                else { /*Not an external symbol*/
                    set_data_word(symbol_in_symbols_table_ptr -> val, 'R', &curr_data_word); /*curr_data_word will hold the machine word with the relevant data*/
                }
            }
            object_img[oc] = curr_data_word; /*Inserts the updated data word into the object image*/
        }
    }
    return succeeded;
}


/*Receives a pointer to ext list, the current object counter and a pointer to the object image and inserts the machine word in index oc in the object image into the
ext list with all relevat additional data. Data that is not relevant or unknown will be zeroed in the ext list. Return FALSE if there was a dynamic allocation
failure and TRUE otherwise.*/
bool add_to_ext_list(linked_list *ext_list, int oc, machine_word_struct *object_img){
    symbol *symbol_in_ext_list_ptr;
    if (!(insert_to_list(&symbol_in_ext_list_ptr, object_img[oc].symbol_operand_name, oc, NON, EXTERN, NULL, object_img[oc].line_number, ext_list)))
        return FALSE; /*Dynamic allocation failure*/
    return TRUE;
}