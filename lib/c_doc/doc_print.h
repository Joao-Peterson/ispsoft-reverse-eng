#ifndef _DOC_PRINT_HEADER_
#define _DOC_PRINT_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include "doc.h"
#include <stdio.h>
#include <stdbool.h>

/* ----------------------------------------- Enum's ----------------------------------------- */

/**
 * @brief type for a function pointer to a fprintf function like
 */
typedef int (*doc_fprint_function_t) (FILE* file, const char *control, ...);

/**
 * @brief type for a function pointer to a printf function like
 */
typedef int (*doc_print_function_t)  (const char *control, ...);

/* ----------------------------------------- Functions -------------------------------------- */

/**
 * @brief sets the printf function like to print the doc structures, may this be after 
 * doc_print_file_set(), then its function will be used, the last set will be the one used
 * @param print_function_ptr: pointer to printf like function
 */
void doc_print_set(doc_print_function_t print_function_ptr);

/**
 * @brief sets the fprintf function like to print the doc structures, may this be after 
 * doc_print_set(), then its function will be used, the last set will be the one used
 * @param print_function_ptr: pointer to fprintf like function
 * @param f_out: pointer to file descriptor to be the output, may be stdout or a file
 * descriptor opened with fopen()
 */
void doc_print_file_set(doc_fprint_function_t fprint_function_ptr, FILE *f_out);

/**
 * @brief print the doc data structure, showing the name, type and value,
 * may it be a numeric value or a string, even binary data, in a indented manner
 * @param variable: pointer to doc variable to be printed
 */
void doc_print(doc *variable);

#ifdef __cplusplus 
}
#endif
#endif