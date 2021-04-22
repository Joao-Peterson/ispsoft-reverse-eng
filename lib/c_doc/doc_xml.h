#ifndef _DOC_XML_HEADER_
#define _DOC_XML_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "doc.h"
#include "../base64/base64.h"

/* ----------------------------------------- Functions -------------------------------------- */

/**
 * @brief opens and parses a xml file to a doc structure
 * @param filename: the path to file
 * @return a doc data struture 
 */
doc *doc_xml_open(char *filename);

/**
 * @brief stringify a xml structure and save it to a file 
 * @note see doc_xml_stringify call.
 * @param xml_doc: xml doc data structure
 * @param filename: path to the file
 */
void doc_xml_save(doc *xml_doc, char *filename);

/**
 * @brief parse a xml file stream into a doc structure
 * @param xml_stream: the stream of the file in memory
 * @return a doc data structure, where each tag that contains other tags will be a
 * dt_obj in wich the sub tags will be it's childs, tags that have values will be 
 * a dt_obj varaible with a child named value with type dt_string, 
 * even number and other data types will follow this format.
 * Each tag also has atributes, this atributes will be packed inside a variable named atributes,
 * of type dt_obj, inside the tag object where its childs will be dt_string with the atributes.
 */
doc *doc_xml_parse(char *xml_stream);


/**
 * @brief generate a xml text stream of the data structure
 * @note each value that will become a xml tag must be a object with two members,
 * with one being a object with name 'atributes' that holds the atributes of the tag,
 * and other object with name 'value', that holds the actual value of the tag.
 * @param xml_doc: pointer to a doc structure
 * @return char string with the xml file
 */
char *doc_xml_stringify(doc *xml_doc);

#ifdef __cplusplus 
}
#endif
#endif