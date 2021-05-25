
/*This file contains coded data of all instructions and will be used to build the instructions table in first_pass which is an array of instruction structs*/


typedef enum {
    MOV = 0,
    CMP = 1,
    ADD = 2,
    SUB = 3,
    LEA = 4,
    CLR = 5,
    NOT = 6,
    INC = 7,
    DEC = 8, 
    JMP = 9,
    BNE = 10,
    JSR = 11,
    RED = 12,
    PRN = 13,
    RTS = 14,
    STOP = 15
} instruction_name; /*Will be used as indexes for the instructions table*/

typedef enum {
    MOV_OPCODE = 0,
    CMP_OPCODE = 1,
    ADD_OPCODE = 2,
    SUB_OPCODE = 2,
    LEA_OPCODE = 4,
    CLR_OPCODE = 5,
    NOT_OPCODE = 5,
    INC_OPCODE = 5,
    DEC_OPCODE = 5, 
    JMP_OPCODE = 9,
    BNE_OPCODE = 9,
    JSR_OPCODE = 9,
    RED_OPCODE = 12,
    PRN_OPCODE = 13,
    RTS_OPCODE = 14,
    STOP_OPCODE = 15
} opcode;

typedef enum {
    MOV_FUNCT = 0,
    CMP_FUNCT = 0,
    ADD_FUNCT = 10,
    SUB_FUNCT = 11,
    LEA_FUNCT = 0,
    CLR_FUNCT = 10,
    NOT_FUNCT = 11,
    INC_FUNCT = 12,
    DEC_FUNCT = 13, 
    JMP_FUNCT = 10,
    BNE_FUNCT = 11,
    JSR_FUNCT = 12,
    RED_FUNCT = 0,
    PRN_FUNCT = 0,
    RTS_FUNCT = 0,
    STOP_FUNCT = 0
} funct;

typedef enum {
    MOV_NUM_OF_OPERANDS = 2,
    CMP_NUM_OF_OPERANDS = 2,
    ADD_NUM_OF_OPERANDS = 2,
    SUB_NUM_OF_OPERANDS = 2,
    LEA_NUM_OF_OPERANDS = 2,
    CLR_NUM_OF_OPERANDS = 1,
    NOT_NUM_OF_OPERANDS = 1,
    INC_NUM_OF_OPERANDS = 1,
    DEC_NUM_OF_OPERANDS = 1, 
    JMP_NUM_OF_OPERANDS = 1,
    BNE_NUM_OF_OPERANDS = 1,
    JSR_NUM_OF_OPERANDS = 1,
    RED_NUM_OF_OPERANDS = 1,
    PRN_NUM_OF_OPERANDS = 1,
    RTS_NUM_OF_OPERANDS = 0,
    STOP_NUM_OF_OPERANDS = 0
} num_of_operands;

typedef enum {
    MOV_NUM_OF_VALID_SOURCE_MODES = 3,
    CMP_NUM_OF_VALID_SOURCE_MODES = 3,
    ADD_NUM_OF_VALID_SOURCE_MODES = 3,
    SUB_NUM_OF_VALID_SOURCE_MODES = 3,
    LEA_NUM_OF_VALID_SOURCE_MODES = 1,
    CLR_NUM_OF_VALID_SOURCE_MODES = 0,
    NOT_NUM_OF_VALID_SOURCE_MODES = 0,
    INC_NUM_OF_VALID_SOURCE_MODES = 0,
    DEC_NUM_OF_VALID_SOURCE_MODES = 0, 
    JMP_NUM_OF_VALID_SOURCE_MODES = 0,
    BNE_NUM_OF_VALID_SOURCE_MODES = 0,
    JSR_NUM_OF_VALID_SOURCE_MODES = 0,
    RED_NUM_OF_VALID_SOURCE_MODES = 0,
    PRN_NUM_OF_VALID_SOURCE_MODES = 0,
    RTS_NUM_OF_VALID_SOURCE_MODES = 0,
    STOP_NUM_OF_VALID_SOURCE_MODES = 0
} num_of_valid_source_modes;


typedef enum {
    MOV_NUM_OF_VALID_DEST_MODES = 2,
    CMP_NUM_OF_VALID_DEST_MODES = 3,
    ADD_NUM_OF_VALID_DEST_MODES = 2,
    SUB_NUM_OF_VALID_DEST_MODES = 2,
    LEA_NUM_OF_VALID_DEST_MODES = 2,
    CLR_NUM_OF_VALID_DEST_MODES = 2,
    NOT_NUM_OF_VALID_DEST_MODES = 2,
    INC_NUM_OF_VALID_DEST_MODES = 2,
    DEC_NUM_OF_VALID_DEST_MODES = 2, 
    JMP_NUM_OF_VALID_DEST_MODES = 2,
    BNE_NUM_OF_VALID_DEST_MODES = 2,
    JSR_NUM_OF_VALID_DEST_MODES = 2,
    RED_NUM_OF_VALID_DEST_MODES = 2,
    PRN_NUM_OF_VALID_DEST_MODES = 3,
    RTS_NUM_OF_VALID_DEST_MODES = 0,
    STOP_NUM_OF_VALID_DEST_MODES = 0
} num_of_valid_dest_modes;