#ifndef _BASE64_HEADER_
#define _BASE64_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdlib.h>

/* ----------------------------------------- Prototypes ------------------------------------- */

/**
 * @brief encodes an array of data into a base64 null terminated string
 * @param data: opaque pointer to data
 * @param data_len: the length to operate on
 * @return null terminated ASCII string 
 */
char *base64_encode(void *data, size_t data_len);

/**
 * @brief decode an array of base64 null terminated string
 * @param data: base64 string
 * @param data_len: the length of the string
 * @param output_data_length: pointer to size_t where the output data size will be written. Can be set to NULL if not desired
 * @return opaque pointer to data
 */
void *base64_decode(char *data, size_t data_len, size_t *output_data_length);


#ifdef __cplusplus 
}
#endif
#endif