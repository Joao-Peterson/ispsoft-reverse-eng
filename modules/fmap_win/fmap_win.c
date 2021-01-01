#include "fmap_win.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <windows.h>

/*--------------------------------------------------------------------------------------------------------------*/

// Get win system error as string
char *get_win_error(){
    DWORD error_code = GetLastError();

    char *error_msg = malloc(sizeof(*error_msg)*200);
    
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, // format as error from system
                    NULL, // aditional parameter
                    error_code, // DWORD error code
                    MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // language to format string
                    (LPSTR)error_msg, // string, cast before passing
                    200, // size
                    NULL // aditional parameters
                    );

    return error_msg;
}

//map file to memory using windows memory api, returns adress pointer
FMAP *fmap_win(char *filename, FMAP_MODE_enum mode){
    FMAP *file = malloc(sizeof(*file));
    file->filename = malloc(strlen(filename)+1);
    strcpy(file->filename, filename);

    DWORD FileHandle_options;
    DWORD FileObjMap_options;
    DWORD FileView_options;

    switch(mode){
        case FMAP_MODE_READ:
            FileHandle_options = GENERIC_READ;
            FileObjMap_options = PAGE_READONLY;
            FileView_options = FILE_MAP_READ;
            break;

        case FMAP_MODE_WRITE:
            FileHandle_options = GENERIC_WRITE;
            FileObjMap_options = PAGE_WRITECOPY;
            FileView_options = FILE_MAP_WRITE;
            break;

        default:
            FileHandle_options = (GENERIC_READ | GENERIC_WRITE);
            FileObjMap_options = PAGE_READWRITE;
            FileView_options = FILE_MAP_ALL_ACCESS;
            break;
    }
    
    // SECURITY_ATTRIBUTES file_acess_security;
    LPSECURITY_ATTRIBUTES file_acess_security = malloc(sizeof(*file_acess_security));
    file_acess_security->bInheritHandle=1; // file can be inherited by child processes 
    file_acess_security->lpSecurityDescriptor = NULL; // null because CreatefileA will append a default security descriptor later

    // OPENS FILE 
    file->file_handle_ptr = CreateFileA(filename, // filename
                                    FileHandle_options, // open as read 
                                    0, // no sharing with other non child processes of the system, makes the file exlusive for this aplication
                                    file_acess_security, // the struct that contains secutity data, see above SECURITY_ATTRIBUTES
                                    OPEN_ALWAYS, // always try to open a existing file
                                    FILE_ATTRIBUTE_NORMAL, // atributes and flags when opening the file, in this case normal
                                    NULL // file handle template when creating a new file, in this case it's only for read, therefore, NULL 
                                    );

    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif

    // FILE MAP
    file->file_map_obj_ptr =  CreateFileMappingA(file->file_handle_ptr, // the file handle
                                                    file_acess_security, // the struct that contains secutity data, see above SECURITY_ATTRIBUTES
                                                    FileObjMap_options, // to be read, copied and executed
                                                    0,0, // size is the same as the file
                                                    NULL // name
                                                    );

    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif
    
    file->stream = MapViewOfFile(file->file_map_obj_ptr, // filemap object
                                                    FileView_options, // acess permissions
                                                    0,0, // offset
                                                    0 // size to view, if 0 all of it
                                                    );

    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif
    
    FlushViewOfFile(file->stream,0); // flush all cached modifications to the view

    return file;
}

// closes view and handles
void fmap_close(FMAP *file){
    FlushViewOfFile(file->stream,0); // flush all cached modifications to the view before closing it
    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif

    UnmapViewOfFile(file->stream); // closes file view  
    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif

    CloseHandle(file->file_handle_ptr); // close the mapped object
    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif

    CloseHandle(file->file_map_obj_ptr); // close file 
    #ifdef _FMAP_WIN_DEBUG_
        printf("- %s",get_win_error());
    #endif

    free(file->filename);
    free(file);
}

/*--------------------------------------------------------------------------------------------------------------*/

// //grava arquivo txt na memória, uma string
// char *read_asci(const char *path){
//     FILE *file = fopen(path, "r"); //abre arquivo
//     if(file==NULL){ // verifica se ponteiro lido é nulo
//         printf("Arquivo nao encontrado ou invalido\n");
//         return NULL;
//     }

//     fseek(file,0,SEEK_END); // vai até o fim do file
//     int len = ftell(file); // pega tamanho
//     fseek(file,0,SEEK_SET); // volta ao inicio

//     char *buf = (char*)malloc(sizeof(char)*(len+1)); //aloca buffer com tamanho +1
//     fread(buf,sizeof(char),len,file); // lê para o buffer
//     buf[len]='\0'; // introduz fechamento de string

//     fclose(file);
//     return buf;
// }