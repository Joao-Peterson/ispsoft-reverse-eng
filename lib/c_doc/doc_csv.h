#ifndef _DOC_CSV_HEADER_
#define _DOC_CSV_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "doc.h"

/* ----------------------------------------- Enumerators ------------------------------------ */

/**
 * @brief enumerator with parse options that can be passed to doc_csv calls.
 * @note you can pass more than one options using the bitwise OR operator, since the
 * options are powers of two, single bits. Ex: (csv_parse_first_line_as_names | csv_parse_first_column_as_names)
 * will execute both function at the same time.
 */
typedef enum{
    csv_parse_normal_mode                           = 0,                            /**< Parse file as a simple matrix */
    csv_parse_first_line_as_names                   = 0x01,                         /**< Parse first line as the name for the columns */
    csv_parse_first_column_as_names                 = 0x02,                         /**< Parse first column, first cell in the line, as the name of that line */
    csv_parse_use_custom_separator                  = 0x04,                         /**< Tells the csv parse calls to accept a extra argument with the char to be used as separator */
    csv_parse_opt_max                               = 0x07                          /**< Maximum number of bitwised arguments */
}doc_csv_parse_opt_t;

/**
 * @brief enumerator with stringify options that can be passed to doc_csv calls.
 * @note you can pass more than one options using the bitwise OR operator, since the
 * options are powers of two, single bits. Ex: (stringify_csv_first_line_as_names | stringify_csv_first_column_as_names)
 * will execute both function at the same time.
 */
typedef enum{
    csv_stringify_normal_mode                       = 0,                            /**< Stringify structure as a simple matrix */
    csv_stringify_put_line_name_in_first_column     = 0x01,                         /**< If a line has a name, then put it into the first column of that line */
    csv_stringify_put_columns_names_in_first_line   = 0x02,                         /**< If at least one column has a name, insert a first line as the columns names */
    csv_stringify_use_custom_separator              = 0x04,                         /**< Tells the csv parse calls to accept a extra argument with the char to be used s */
    csv_stringify_opt_max                           = 0x07                          /**< Maximum number of bitwised arguments */
}doc_csv_stringify_opt_t;

/* ----------------------------------------- Functions -------------------------------------- */

/**
 * @brief open a csv file designated by the filename and parse it into
 * a doc data structure.
 * @note see doc_csv_parse call.
 * @param filename: path to the file
 * @param ...: optional parameter of type doc_csv_parse_opt_t
 * @return a doc data structure
 */
doc *doc_csv_open(char *filename, ...);

/**
 * @brief stringify a doc structure and save it to a file 
 * @note see doc_csv_stringify call.
 * @param csv_doc: csv doc data structure
 * @param filename: path to the file
 * @param ...: optional parameter of type doc_csv_stringify_opt_t
 */
void doc_csv_save(doc *csv_doc, char *filename, ...);

/**
 * @brief parses a csv stream into a doc data structure
 * @note an optional parameter of type doc_parse_csv_pot_t, a enumerator, can be passed,
 * these can specify if the first line and/or column should be used as names for the data,
 * since a csv table is represented by a object with objects with cells, these first objects 
 * are anonymous, acessing them is easy trought the syntax 'csv[0][0]' for example, by using names
 * they more accessible.
 * @param stream: a csv file stream
 * @param ...: optional parameter, of type doc_csv_parse_opt_t
 * @return a doc data structure
 */
doc *doc_csv_parse(char *stream, ...);

/**
 * @brief stringify a doc data structure to a csv file stream.
 * @note doc data structure must have a high level obj/array with one or more obj/array inside of it
 * representing the lines that contains cell data, each line should have the same number of cells
 * @param csv_doc: doc structure of the csv file
 * @return ASCII stream of the csv file  
 */
char *doc_csv_stringify(doc *csv_doc, ...);

#ifdef __cplusplus 
}
#endif
#endif