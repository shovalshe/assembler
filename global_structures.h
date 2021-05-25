
#ifndef MAX_SYMBOL_LENGTH
#define MAX_SYMBOL_LENGTH 31
#endif



typedef struct {
    unsigned int dest_add : 2;
    unsigned int source_add : 2;
    unsigned int funct : 4;
    unsigned int opcode : 4;
} instruction_word;

typedef union {
    instruction_word instruction_word;
    unsigned int data_word_value : 12; /*Will be used either for data words that are additional to an instruction word or for a data/string words*/
} machine_word; /*This struct will be used either for an istruction word or for a data_word*/

typedef enum {
    IMMEDIATE = 0,
    DIRECT = 1,
    RELATIVE = 2,
    REGISTER_DIRECT = 3
} addressing_mode;

typedef struct {
    machine_word machine_word;
    char ARE;
    bool is_set; /*Will hold TRUE after the machine_word_struct instance is set*/
    addressing_mode addressing_mode; /*Sets only in case that this machine word is a direct or relative operand*/
    char symbol_operand_name[MAX_SYMBOL_LENGTH + 1]; /*Sets only in case that this machine word is a direct or relative operand*/
    int line_number; /*Sets only in case that this machine word is a direct or relative operand*/
} machine_word_struct; /*A structures that hold the 12 bits machine words and additional data*/

typedef struct {
    unsigned char opcode; /*using unsigned char instead of int for memory saving*/
    unsigned char funct; /*using unsigned char instead of int for memory saving*/
    unsigned char num_of_operands; /*using unsigned char instead of int for memory saving. This is the number of operands that the instruction should get*/
    unsigned char num_of_valid_source_modes; /*using unsigned char instead of int for memory saving. This is the number of valid source addressing modes options
    for the instruction*/
    addressing_mode *valid_source_modes; /*An array of the valid source addresing modes of the instruction*/
    unsigned char num_of_valid_dest_modes; /*using unsigned char instead of int for memory saving. This is the number of valid dest addressing modes options
    for the instruction*/
    addressing_mode *valid_dest_modes; /*An array of the valid dest addresing modes of the instruction*/
} instruction;