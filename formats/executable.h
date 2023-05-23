#ifndef EXECUTABLE_H
#define EXECUTABLE_H

typedef struct eFileFormat {
    char hdr[4]; // EXEC
    int file_sz; // Size of file
} E_FILE_FORMAT;

#endif