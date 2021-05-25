
typedef enum {
    FALSE = 0,
    TRUE = 1
} bool; /*Will be used as booleans*/


bool open_file_with_check(char *file_name, FILE **fp, char *mode); /*Receives a name of a file, a pointer to *FILE and a mode and opens the file
according to the mode and using the pointer as a pointer to the file. The function prints an error if needed and return FALS. Returns TRUE if the 
file was opened succesfuly.*/

bool close_file_with_check(char *file_name, FILE **fp); /*Receives a name of a file and a pointer to the file and closes this file. The function
prints an error if needed and return FALS. Returns TRUE if the file was closed succesfuly.*/