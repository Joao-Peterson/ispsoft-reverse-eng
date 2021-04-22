#ifndef _DOC_INI_HEADER_
#define _DOC_INI_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "doc.h"
#include "../base64/base64.h"

/* ----------------------------------------- Functions -------------------------------------- */

/**
 * @brief opens and parses a ini/cfg file to a doc structure
 * @param filename: the path to file
 * @return a doc data struture 
 */
doc *doc_ini_open(char *filename);

/**
 * @brief stringify a ini/cfg structure and save it to a file 
 * @note see doc_ini_stringify call.
 * @param ini_doc: ini doc data structure
 * @param filename: path to the file
 */
void doc_ini_save(doc *ini_doc, char *filename);

/**
 * @brief parses a ini/cfg text file into a doc structure
 * @note supports non nesting sections, line break sequence '\', string literals like: "string",
 * simple variables, empty variables like: 'var=' and 'var ' without the '=' sign, anonymous variables, 
 * ex: '{anonymous_variable}', have to be surrounded by curly brackets and will have a "" empty name string,
 * 'doc_get()' and 'doc_get_ptr()' with a syntax like: 'anonymous_variables[1]' must be used, getting the value by index.
 * Comments use the '#' or ';' characters.     
 * Variables must terminate with a line break
 * @param ini_file_stream: a string of ASCII chars in memory
 * @return the doc data structure representing the file 
 */
doc *doc_ini_parse(char *ini_file_stream);

/**
 * @brief generates a ini/cfg file based on a doc data structure
 * @note each varaible inside will be a global variable inside the ini file,
 * variables insides objects or arrays will be placed below the section with same name
 * as the obj/array. Values with '#' and ';' will be placed as string literals, surrounded
 * by double quotes. Empty variables will be place with the name and a equal sign. Anonymous
 * values will be placed surrounded by curly brackets. 
 * Before the call to the stringify the data, a doc_squash is made, to be sure that the
 * data strucure is at most 2 levels deep, that is because this lib doesn't implement
 * nested sections, so every value must be at most inside a global object/array, that will
 * become a section.
 * @param doc_ini: Pointer to doc data structure
 * @return ASCII stream with the file
 */
char *doc_ini_stringify(doc *doc_ini);

#ifdef __cplusplus 
}
#endif
#endif