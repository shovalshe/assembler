#ifndef MAX_SYMBOL_LENGTH
#define MAX_SYMBOL_LENGTH 31
#endif

typedef enum {
    DATA_DIRECTIVE = 0,
    STRING_DIRECTIVE = 1,
    ENTRY_DIRECTIVE = 2,
    EXTERN_DIRECTIVE = 3,
    NO_DIRECTIVE = 4
} directive;

typedef enum {
    NON = 0, /*unknown*/
    DATA = 1,
    CODE = 2
} memory_attribute;

typedef enum {
    LOCAL = 0,
    ENTRY = 1,
    EXTERN = 2
} visibility_attribute;

typedef enum {
    SYMBOLS_TABLE = 0,
    DATA_LIST = 1,
    EXT_LIST = 2,
    ENT_LIST = 3
} list_type;

/*Node of linked list*/
typedef struct symbol {
    char name[MAX_SYMBOL_LENGTH + 1];
    int val;
    memory_attribute memory;
    visibility_attribute visibility;
    struct symbol *address_in_symbols_table; /*Will be used for lists that are not the symbols list*/
    int line_number; /*Will be used only for entry symbols*/
    struct symbol *next;
} symbol;

typedef struct {
    list_type list_type;
    int len;
    symbol *list_head;
    symbol *list_tail;
} linked_list;

bool new_symbol(symbol **new_symbol_ptr_ptr, char *name, int val, memory_attribute memory, visibility_attribute visibility, symbol *address_in_symbols_table,\
int line_number, list_type list_type); /*Receives a pointer to the dest address of the new symbol, a string (will be the name of the symbol), an int (the value
of the symbol), a memory_attribute, visibility_attribute, a pointer to the symbol in the symbols table, the line number of the current apearance of the symbol
and the type of the list that the symbol will be inserted to. The function creates a new symbol with all these parameters and assigns its address to
*new_symbol_ptr_ptr. Some of the parameters are not always necessary. When they are not necessary, the function will be called with zeroed parameters. It was
written in this way in order to fit to all types of linked lists in this program. The function returns TRUE if the dynamic allocation succeeded and FALSE
otherwise.*/

bool insert_to_list(symbol **new_symbol_ptr_ptr, char *name, int val, memory_attribute memory, visibility_attribute visibility, symbol *address_in_symbols_table,\
int line_number, linked_list *list_ptr); /*Receives a pointer to the dest address of the new symbol, a string (the name of the symbol that will be inserted),
an int (the value of the symbol), a memory_attribute, visibility_attribute, a pointer to the symbol in the symbols table, the line number of the current
apearance of the symbol and a pointer to the list that the symbol will be inserted to. The function creates a new symbol with the parameters, inserts it to
the end of the list and assigns its new address to *new_symbol_ptr_ptr. When a parameter is unknown or not necessary in the current list, the function should
be called with zeroed parameters. The function returns TRUE if the dynamic allocation in new_symbol succeeded and FALSE otherwise.*/

bool add_symbol(char *symbol_name, int val, directive curr_directive, linked_list *symbols_table_ptr, linked_list *data_list_ptr, linked_list ent_list,\
int lines_counter, char *file_name); /*Receives a string (the name of the symbol that will be added), an int (the value of the symbol), the current directive,
a pointer to the symbols table, a pointer to the data list, a pointer to the entries list, the line number of the current apearance of the symbol and the name
of the current input file. The function checks if the symbol is already exists and if not, creates a new symbol structure and inserts it to the symbols table.
If the current directive is .string or .data, it inserts the symbol also to data_list. It is also prints errors if necessary. For extern directive, it checks
that the symbol is not already declared as entry.*/

symbol * search(char *symbol_name, linked_list list_name); /*Receives a name of a symbol and a linked list and searches this symbol name in the list. Returns
a pointer to the symbol if it is already in the list, or NULL if the symbol name does'nt exist in the list*/

void destroy_list(linked_list list); /*Receives a linked list and free all dinamic allocations*/