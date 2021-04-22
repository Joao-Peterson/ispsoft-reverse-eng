#ifndef _DOC_JSON_HEADER_
#define _DOC_JSON_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "doc.h"
#include "../base64/base64.h"

/* ----------------------------------------- Functions -------------------------------------- */

/**
 * @brief opens and parses a json file to a doc structure
 * @param filename: the path to file
 * @return a doc data struture 
 */
doc *doc_json_open(char *filename);

/**
 * @brief stringify a json structure and save it to a file 
 * @note see doc_json_stringify call.
 * @param json_doc: json doc data structure
 * @param filename: path to the file
 */
void doc_json_save(doc *json_doc, char *filename);

/**
 * @brief parse a file stream to a 'doc' structure
 * @param file_stream: null terminated file stream
 * @return pointer to 'doc' structure
 */
doc *doc_json_parse(char *file_stream);

/**
 * @brief creates a json file out of a 'doc' structure
 * @param json_doc: pointer to 'doc' structure
 * @return json char stream, null terminated. Return NULL if json_doc is not a dt_obj 'doc' type
 * @note json_doc must be of type dt_obj, as if it represents the actual json file,
 * and the name will not be displayed, because json files start with a '{'
 */
char *doc_json_stringify(doc *json_doc);

#ifdef __cplusplus 
}
#endif
#endif