
#ifndef IC_INIT_VAL
#define IC_INIT_VAL 100
#endif

bool second_pass(char *file_name, machine_word_struct *object_img, linked_list symbols_table, linked_list ent_list, int icf, linked_list *ext_list);
/*Receives the file name, a pointer to the object image, the symbols table, the ent list, the final ic and a pointer to the ext list. The function executes
2 steps: 1. passing over all symbols that declared as entries and updating their visibility attribute to entry in the symbols table and their value in the
ent list according to the value in the symbols table. 2. passing over all the object image (includes instruction and data image) and updating words that are
not coded yet. It prints errors mesages if necessary - at most one for every line in the input file. Returns TRUE if the entire process was succesfully
completed and FALSE otherwise.*/

bool update_entry_attribute(linked_list ent_list, linked_list symbols_table, char *file_name); /*Receives the ent list, the symbols table and the file name
and passes all symbols in ent list and searches every symbol in the symbols table. If the symbol was found, it updates its visibility attribute to entry in
the symbols table, updates the value of the symbol in the ent list according to the value in the symbols table and returns TRUE. Else, it prints an error
message and returns FALSE.*/

bool complete_missing_words(machine_word_struct * object_img, linked_list symbols_table, int icf, linked_list *ext_list, char *file_name); /*Receives a
pointer to the object image the symbols table, the final ic, a pointer to the ext list and the file name. It passes over all the object image and updates
words that are not coded yet (words of relative and direct addresess modes). Returns FALSE if there was a problem and TRUE otherwise*/

bool add_to_ext_list(linked_list *ext_list, int oc, machine_word_struct *object_img); /*Receives a pointer to ext list, the current object counter and a
pointer to the object image and inserts the machine word in index oc in the object image into the ext list with all relevat additional data. Data that is
not relevant or unknown will be zeroed in the ext list. Return FALSE if there was a dynamic allocation failure and TRUE otherwise.*/