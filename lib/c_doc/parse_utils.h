#ifndef _PARSE_UTILS_HEADER_
#define _PARSE_UTILS_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "doc.h"

/* ----------------------------------------- Definitions ------------------------------------ */

#define UINT64_MAX_DECIMAL_CHARS_PARSE_UTILS    (20)
#define FLOAT_MAX_DECIMAL_CHARS_PARSE_UTILS     (27)

#define WHITESPACE_PARSE_UTILS  " \t\n\r\v\f"                                       // white space chars

/* ----------------------------------------- Globals ---------------------------------------- */

extern const char *NUMBER_INTEGER_ALPHABET;
extern const char *NUMBER_DECIMAL_ALPHABET;

/* ----------------------------------------- Numeric values config -------------------------- */

typedef int64_t integer_type_parse_utils;                                           // type for integer values
#define integer_dt_type_parse_utils dt_int64                                        // dt type for integer values
#define integer_doc_type_parse_utils doc_int64_t                                    // doc type for integer values

#define strto_integer_parse_utils(const_char_ptr_string, const_char_ptr_ptr_endptr) \
    atoi(const_char_ptr_string)                                                     // function to convert from string to integer



#define FLOAT_DECIMAL_PLACES_PARSE_UTILS    (5)                                     // decimal places after dot on decimals
#define decimal_print_format_parse_utils    "%#.*G"                                 // to print decimal values    

typedef double decimal_type_parse_utils;                                            // type for decimal values
#define decimal_dt_type_parse_utils dt_double                                       // dt type for decimal values
#define decimal_doc_type_parse_utils doc_double                                     // doc type for decimal values

#define strto_rational_parse_utils(const_char_ptr_string, const_char_ptr_ptr_endptr) \
    strtod(const_char_ptr_string, const_char_ptr_ptr_endptr)                        // function to convert from string to decimal


/* ----------------------------------------- Functions -------------------------------------- */

// run whitespace except for the '\n' line break
void run_space(char **stream);

// run all whitespace chars
void run_whitespace(char **stream);

// run whitespace backwards
void run_whitespace_back(char **stream);

// check if a string if formed by just members of a defined alphabet 
bool str_alphabet(char *string, char *alphabet);

// check how many times a char appears in a string
size_t strnchr(char *string, char chr);

// check if string represents a number
bool str_is_number(char *string);

// check the value of a string representation of the value
doc_type_t check_value_type(char *value);

// create a doc from a string with an appropriate value type
doc *create_doc_from_string(char *name, char *value_string);

// crop and join a space inside a string
void strcrop(char *string, char *start, char *end);

// clean a string with line breaking sequence
char *strbreak_clear(char *string);

// creates a ASCII stream from a file
char *fstream(char *filename);

#ifdef __cplusplus 
}
#endif
#endif