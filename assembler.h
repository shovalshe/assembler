
#define MEMORY_SIZE 4096
#define MAX_FILE_NAME_LENGTH 1000
#define IC_INIT_VAL 100


bool process_asm_file(char *asm_file_name); /*Receives a string asm_file_name and if this string is a name of an assembly file in the same folder
(without the suffix '.as'), it process the file and creates output files: asm_file_name.ob and asm_file_name.ext and asm_file_name.ent if there are
externals and entries symbols declared in asm_file_name.as. If there are errors in asm_file_name, there will be no output files and errors messages
will be printed to the standart output. The function returns True if the entire process was succesfully completed.*/

void create_object_img(machine_word_struct *code_img, machine_word_struct *data_img, int icf, int dcf, machine_word_struct **object_img_ptr);
/*Receives the code image, the data image (arrays of machine_word_struct), the final ic, the final dc and a pointer to the object image pointer.
In the end of the function, the last pointer will points to the beggining of the machine code array. In the machine code array, code will apear
first and data last.*/

void free_all_dynamic_allocations(linked_list symbols_table, linked_list data_list, linked_list ent_list, linked_list ext_list); /*Receives all
4 linked list that has been created and free all dinamic allocations.*/