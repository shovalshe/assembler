/*This file contains functions that implements a linked list structure of symbols (nodes). All the functions in this file are called directly or indirectly
from main. Output: standart output*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "symbols.h"


/*Receives a pointer to the dest address of the new symbol, a string (will be the name of the symbol), an int (the value of the symbol), a memory_attribute,
visibility_attribute, a pointer to the symbol in the symbols table, the line number of the current apearance of the symbol and the type of the list that the
symbol will be inserted to. The function creates a new symbol with all these parameters and assigns its address to *new_symbol_ptr_ptr. Some of the parameters
are not always necessary. When they are not necessary, the function will be called with zeroed parameters. It was written in this way in order to fit to all
types of linked lists in this program. The function returns TRUE if the dynamic allocation succeeded and FALSE otherwise.*/
bool new_symbol(symbol **new_symbol_ptr_ptr, char *name, int val, memory_attribute memory, visibility_attribute visibility, symbol *address_in_symbols_table,\
int line_number, list_type list_type) {
    
    *new_symbol_ptr_ptr = (symbol *) malloc(sizeof(symbol));
    if (new_symbol_ptr_ptr == NULL) {
        fprintf(stdout, "out of memory\n");
        return FALSE; /*Dynamic allocation failure*/
    }

    strcpy((*new_symbol_ptr_ptr) -> name, name);
    (*new_symbol_ptr_ptr) -> val = val;
    (*new_symbol_ptr_ptr) -> memory = memory;
    (*new_symbol_ptr_ptr) -> visibility = visibility;
    (*new_symbol_ptr_ptr) -> line_number = line_number; /*Will be used only for entry symbols*/
    (*new_symbol_ptr_ptr) -> next = NULL;
    if (list_type == SYMBOLS_TABLE)
        (*new_symbol_ptr_ptr) -> address_in_symbols_table = *new_symbol_ptr_ptr;
    else
        (*new_symbol_ptr_ptr) -> address_in_symbols_table = address_in_symbols_table; /*Will be used for lists that are not the symbols list*/
    return TRUE;
}


/*Receives a pointer to the dest address of the new symbol, a string (the name of the symbol that will be inserted), an int (the value of the symbol), a
memory_attribute, visibility_attribute, a pointer to the symbol in the symbols table, the line number of the current apearance of the symbol and a pointer
to the list that the symbol will be inserted to. The function creates a new symbol with the parameters, inserts it to the end of the list and assigns its
new address to *new_symbol_ptr_ptr. When a parameter is unknown or not necessary in the current list, the function should be called with zeroed parameters.
The function returns TRUE if the dynamic allocation in new_symbol succeeded and FALSE otherwise.*/
bool insert_to_list(symbol **new_symbol_ptr_ptr, char *name, int val, memory_attribute memory, visibility_attribute visibility, symbol *address_in_symbols_table,\
int line_number, linked_list *list_ptr) {
    symbol *new_symbol_ptr = NULL;
    
    if (new_symbol(&new_symbol_ptr, name, val, memory, visibility, address_in_symbols_table, line_number, list_ptr -> list_type)) {
        if (list_ptr -> list_tail == NULL) /*The list is empty*/
            list_ptr -> list_head = new_symbol_ptr;
        else
            list_ptr -> list_tail -> next = new_symbol_ptr;
        list_ptr -> list_tail = new_symbol_ptr;
        list_ptr -> len += 1;
        (*new_symbol_ptr_ptr) = list_ptr -> list_tail;
    }
    else
        return FALSE; /*Dynamic allocation failure*/
    return TRUE;
}


/*Receives a string (the name of the symbol that will be added), an int (the value of the symbol), the current directive, a pointer to the symbols table, a
pointer to the data list, a pointer to the entries list, the line number of the current apearance of the symbol and the name of the current input file. The
function checks if the symbol is already exists and if not, creates a new symbol structure and inserts it to the symbols table. If the current directive is
.string or .data, it inserts the symbol also to data_list. It is also prints errors if necessary. For extern directive, it checks that the symbol is not
already declared as entry.*/
bool add_symbol(char *symbol_name, int val, directive curr_directive, linked_list *symbols_table_ptr, linked_list *data_list_ptr, linked_list ent_list,\
int lines_counter, char *file_name) {
    memory_attribute memory;
    visibility_attribute visibility;
    symbol *symbol_already_in_symbols_table_ptr; /*A pointer to the symbol with the same name in symbols_table if exists.*/
    symbol *address_in_symbols_table = NULL; /*Will hold the address of the symbol in symbols_table after it will be inserted.*/
    symbol *address_in_data_list = NULL; /*Will hold the address of the symbol in data_list if it will be inserted.*/

    if (curr_directive == DATA_DIRECTIVE || curr_directive == STRING_DIRECTIVE) {
        memory = DATA;
        visibility = LOCAL;
    }
    else if (curr_directive == EXTERN_DIRECTIVE) {
        symbol *symbol_already_in_ent_list_ptr; /*A pointer to the symbol with the same name in ent_list if exists.*/
        memory = NON;
        visibility = EXTERN;
        symbol_already_in_ent_list_ptr = search(symbol_name, ent_list);
        if (symbol_already_in_ent_list_ptr != NULL) { /*The symbol is already declared as entry*/
            fprintf(stdout, "%s: In line %d: The symbol %s is already declared as entry\n", file_name, lines_counter, symbol_name);
            return FALSE;
        }
    }
    else if (curr_directive == NO_DIRECTIVE) { /*This is a symbol of a code statement*/
        memory = CODE;
        visibility = LOCAL; /*later can be changed to entry*/
    }
    
    symbol_already_in_symbols_table_ptr = search(symbol_name, *symbols_table_ptr);
    if (symbol_already_in_symbols_table_ptr != NULL) { /*The symbol already exists in symbols_table*/
        if (visibility == EXTERN && symbol_already_in_symbols_table_ptr -> visibility != EXTERN) {/*This symbol already exists as local*/
            fprintf(stdout, "%s: In line %d: Local symbol cannot be declared as external\n", file_name, lines_counter);
            return FALSE;
        }
        else if (visibility != EXTERN || symbol_already_in_symbols_table_ptr -> visibility != EXTERN) { /*This is not a repetetive declaration of an
        external symbol.*/
            fprintf(stdout, "%s: In line %d: The symbol %s already exists\n", file_name, lines_counter, symbol_name);
            return FALSE;
        }
    }
    else if (!(insert_to_list(&address_in_symbols_table, symbol_name, val, memory, visibility, NULL, lines_counter, symbols_table_ptr))) /*Inserts the
    symbol to symbols_table*/
        return FALSE; /*Dynamic allocation failure*/

    if (memory == DATA) /*If this is a data symbol, insert it also to data_list.*/
        if (!(insert_to_list(&address_in_data_list, symbol_name, val, memory, visibility, address_in_symbols_table, lines_counter, data_list_ptr)))
            return FALSE; /*Dynamic allocation failure*/
    return TRUE;
}

/*Receuves a name of a symbol and a linked list and searches this symbol name in the list. Returns a pointer to the symbol if it is already in the list,
or NULL if the symbol name does'nt exist in the list*/
symbol * search(char *symbol_name, linked_list list_name) {
    symbol * head = list_name.list_head;
    
    while (head != NULL) { /*Iterates nodes in the list*/
        if (strcmp(head -> name, symbol_name) == 0)
            return head;
        head = head -> next;
    }
    return NULL;
}

/*Receives a linked list and free all dinamic allocations*/
void destroy_list(linked_list list) {
    if (list.list_head == NULL)
        return;
    while (list.list_head != NULL) {
        symbol *tmp = list.list_head;
        list.list_head = list.list_head -> next;
        free(tmp);
    }
}