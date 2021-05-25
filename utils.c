/*This file contains functions that open and close files and prints error messages if necessary.*/

#include <stdio.h>
#include "utils.h"


/*Receives a name of a file, a pointer to *FILE and a mode and opens the file according to the mode and using the pointer as a pointer to the file.
The function prints an error if needed and return FALS. Returns TRUE if the file was opened succesfuly.*/
bool open_file_with_check(char *file_name, FILE** fp, char *mode) {
    if (!(*fp = fopen(file_name, mode))) { /*Checks that the file named file_name created and opened succesfuly*/
        fprintf (stdout, "Cannot open %s\n", file_name);
        return FALSE;
    };
    return TRUE;
}


/*Receives a name of a file and a pointer to the file and closes this file. The function prints an error if needed and return FALS. Returns TRUE if
the file was closed succesfuly.*/
bool close_file_with_check(char *file_name, FILE **fp) {
    if (fclose (*fp) != 0) { /*fclose failed*/
        fprintf (stdout, "Cannot close %s\n", file_name);
        return FALSE;
    }
    return TRUE;
}