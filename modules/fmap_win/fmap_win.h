#ifndef _FMAP_WIN_
#define _FMAP_WIN_

// #define _FMAP_WIN_DEBUG_

#include <stdint.h>
#include <stdbool.h>

// File mapping
typedef enum{
    FMAP_MODE_READ  = 0x1,
    FMAP_MODE_WRITE = 0x2
}FMAP_MODE_enum;

typedef struct FMAP{
    char *filename;
    void *file_handle_ptr;
    void *file_map_obj_ptr;
    char *stream;
}
FMAP;

/* ----------------------------------------------------- Windows File handling ------------------------------------------------------ */

// Get win system error as string
char *get_win_error();

//map file to memory using windows memory api, needs two pointers, to file and to map object, returns adress pointer
FMAP *fmap_win(char *filename, FMAP_MODE_enum mode);

// closes view, file handle and map object handle
void fmap_close(FMAP *file);

/* ----------------------------------------------------- Extra functions ------------------------------------------------------------ */

// retorna ponteiro para vetor contendo arquivo de texto desejado
char *read_asci(const char *path);

#endif