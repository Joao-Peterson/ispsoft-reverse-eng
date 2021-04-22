/**
 * @file doc.h
 * 
 * DOC - The C dynamic object lib!
 * 
 * This library is made for C and C++ programs and implement a data structure to mimic dynamic objects
 * present in interpreted languages, parsers can be written to convert files notations such as xml, json, etc, to
 * the data structure.
 * 
 * Created by: João Peterson Scheffer - 2021.
 * 
 * This is the header file, please don't modify, except when stated in documentation.
 * 
 * Refer to this header as means to use the library and get informations about the variables and functions.
 * 
 */

#ifndef _DOC_HEADER_
#define _DOC_HEADER_
#ifdef __cplusplus 
extern "C" {
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ----------------------------------------- Definitions ------------------------------------ */

#define ERROR_MSG_LEN_DOC_HEADER    500     // max string len for error msg, used by doc_get_error_msg()

#define DOC_NAME_MAX_LEN            250     // max name length for doc instance

#define MAX_OBJ_MEMBER_QTY   UINT32_MAX     // maximum quantity of members inside a array or object  

/* ----------------------------------------- Typedef's ---------------------------------------- */

typedef uint32_t doc_size_t;                // type for looping through members inside array's or obj's  

typedef unsigned int uint_t;                // syntax sugar for getting the values with get_value() macro later

/* ----------------------------------------- Enum's ----------------------------------------- */

/**
 * @brief enumerator that contains types for use on the syntax
 */
typedef enum{
    dt_null,                                /**< Denotes a null value with no 'doc_' inherited structure */                                                
    dt_obj,                                 /**< Denotes a object, can hold members */                                            
    dt_array,                               /**< Denotes a array, can hold members only of the same type */                                                
    dt_double,                              /**< Denotes a double value */                                                
    dt_float,                               /**< Denotes a float value */                                                 
    dt_uint,                                /**< Denotes a uint value */                                                
    dt_uint64,                              /**< Denotes a uint64 value */                                                
    dt_uint32,                              /**< Denotes a uint32 value */                                                
    dt_uint16,                              /**< Denotes a uint16 value */                                                
    dt_uint8,                               /**< Denotes a uint8 value */                                                
    dt_int,                                 /**< Denotes a int value */                                            
    dt_int64,                               /**< Denotes a int64 value */                                                
    dt_int32,                               /**< Denotes a int32 value */                                                
    dt_int16,                               /**< Denotes a int16 value */                                                
    dt_int8,                                /**< Denotes a int8 value */                                                   
    dt_bool,                                /**< Denotes a bool value */                                                  
    dt_string,                              /**< Denotes a normal string that can be freed */                                                 
    dt_const_string,                        /**< Denotes a normal string that can't be freed */                                                                      
    dt_bindata,                             /**< Denotes binary data that can be freed */                                                
    dt_const_bindata                        /**< Denotes binary data that can't be freed */                                                       
}doc_type_t;

/**
 * @brief macros for staticaly check type
 */
#define IS_DOC_TYPE(type) \
   (((type) == dt_null)             || \
    ((type) == dt_obj)              || \
    ((type) == dt_array)            || \
    ((type) == dt_double)           || \
    ((type) == dt_float)            || \
    ((type) == dt_uint)             || \
    ((type) == dt_uint64)           || \
    ((type) == dt_uint32)           || \
    ((type) == dt_uint16)           || \
    ((type) == dt_uint8)            || \
    ((type) == dt_int)              || \
    ((type) == dt_int64)            || \
    ((type) == dt_int32)            || \
    ((type) == dt_int16)            || \
    ((type) == dt_int8)             || \
    ((type) == dt_bool)             || \
    ((type) == dt_string)           || \
    ((type) == dt_const_string)     || \
    ((type) == dt_bindata)          || \
    ((type) == dt_const_bindata))

/**
 * @brief error codes
 */
typedef enum{
    errno_doc_size_of_string_or_bindata_is_beyond_four_megabytes_Check_if_size_is_of_type_size_t_or_cast_it_to_size_t_first     =  1,
    errno_doc_ok                                                                                                                =  0,
    errno_doc_not_a_type                                                                                                        = -1,
    errno_doc_overflow_quantity_members_or_name_is_too_big                                                                      = -2,
    errno_doc_value_not_same_type_as_array                                                                                      = -3,
    errno_doc_duplicate_names                                                                                                   = -4,
    errno_doc_null_passed_doc_ptr                                                                                               = -5,
    errno_doc_value_not_found                                                                                                   = -6,
    errno_doc_name_cointains_illegal_characters_or_missing_semi_colon_terminator                                                = -7,
    errno_doc_trying_to_add_new_data_to_non_object_or_non_array                                                                 = -8,
    errno_doc_trying_to_get_data_from_non_object_or_non_array                                                                   = -9,
    errno_doc_trying_to_set_value_of_non_value_type_data_type                                                                   = -10,
    errno_doc_trying_to_set_string_of_non_string_data_type                                                                      = -11,
    errno_doc_trying_to_set_bindata_of_non_bindata_data_type                                                                    = -12,
    errno_doc_trying_to_squash_a_doc_structure_to_0__This_is_not_possible_becaue_it_needs_at_least_one_object_to_hold_data      = -13,
    errno_doc_null_passed_parameter                                                                                             = -14
}errno_doc_code_t;

/* ----------------------------------------- Structs ---------------------------------------- */

/**
 * @brief main structure of the lib
 */
typedef struct doc doc;
struct doc{
    doc *next;                              /**< pointer to next member inside a array or object */
    doc *prev;                              /**< pointer to previous member inside a array or object*/
    doc *child;                             /**< pointer to the first element of this array or object */
    doc *parent;                            /**< pointer to the parent, the instance that define the object or array */
    doc_size_t childs;                 /**< quantity of childs */
    doc_type_t type;                        /**< type that describes this instance */
    char *name;                             /**< name of the element */
};

// ------------------ individual values datatypes

#pragma pack(push,1)

/**
 * @brief structure that inherit from doc that holds 'double' data, typecasting from 'doc' to 'doc_double' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */
    union{                                      /**< anonymous union, used for serialization */
        double value;                           /**< actual value */
        uint8_t bytes[sizeof(double)];          /**< array of bytes of the value */
    };
}doc_double;

/**
 * @brief structure that inherit from doc that holds 'float' data, typecasting from 'doc' to 'doc_float' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */        
    union{                                      /**< anonymous union, used for serialization */                        
        float value;                            /**< actual value */                                    
        uint8_t bytes[sizeof(float)];           /**< array of bytes of the value */                                                
    };
}doc_float;

/**
 * @brief structure that inherit from doc that holds 'uint_t' data, typecasting from 'doc' to 'doc_uint_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                        
    union{                                      /**< anonymous union, used for serialization */                                            
        unsigned int value;                     /**< actual value */                                                            
        uint8_t bytes[sizeof(unsigned int)];    /**< array of bytes of the value */                                                            
    };
}doc_uint_t;

/**
 * @brief structure that inherit from doc that holds 'uint64_t' data, typecasting from 'doc' to 'doc_uint64_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                    
    union{                                      /**< anonymous union, used for serialization */                
        uint64_t value;                         /**< actual value */                            
        uint8_t bytes[sizeof(uint64_t)];        /**< array of bytes of the value */                                                
    };
}doc_uint64_t;

/**
 * @brief structure that inherit from doc that holds 'uint32_t' data, typecasting from 'doc' to 'doc_uint32_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                
    union{                                      /**< anonymous union, used for serialization */                            
        uint32_t value;                         /**< actual value */                                        
        uint8_t bytes[sizeof(uint32_t)];        /**< array of bytes of the value */                                                            
    };
}doc_uint32_t;

/**
 * @brief structure that inherit from doc that holds 'uint16_t' data, typecasting from 'doc' to 'doc_uint16_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                            
    union{                                      /**< anonymous union, used for serialization */                                                        
        uint16_t value;                         /**< actual value */                                                                    
        uint8_t bytes[sizeof(uint16_t)];        /**< array of bytes of the value */                                                                                        
    };
}doc_uint16_t;

/**
 * @brief structure that inherit from doc that holds 'uint8_t' data, typecasting from 'doc' to 'doc_uint8_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                    
    uint8_t value;                              /**< actual value */             
}doc_uint8_t;

/**
 * @brief structure that inherit from doc that holds 'int_t' data, typecasting from 'doc' to 'doc_int_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                            
    union{                                      /**< anonymous union, used for serialization */                                        
        int value;                              /**< actual value */                                                
        uint8_t bytes[sizeof(int)];             /**< array of bytes of the value */                                                                
    };
}doc_int;

/**
 * @brief structure that inherit from doc that holds 'int64_t' data, typecasting from 'doc' to 'doc_int64_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                        
    union{                                      /**< anonymous union, used for serialization */                                                    
        int64_t value;                          /**< actual value */                                                                
        uint8_t bytes[sizeof(int64_t)];         /**< array of bytes of the value */                                                                                
    };
}doc_int64_t;

/**
 * @brief structure that inherit from doc that holds 'int32_t' data, typecasting from 'doc' to 'doc_int32_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                        
    union{                                      /**< anonymous union, used for serialization */                                                    
        int32_t value;                          /**< actual value */                                                                
        uint8_t bytes[sizeof(int32_t)];         /**< array of bytes of the value */                                                                                
    };
}doc_int32_t;

/**
 * @brief structure that inherit from doc that holds 'int16_t' data, typecasting from 'doc' to 'doc_int16_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                
    union{                                      /**< anonymous union, used for serialization */                                            
        int16_t value;                          /**< actual value */                                                        
        uint8_t bytes[sizeof(int16_t)];         /**< array of bytes of the value */                                                                        
    };
}doc_int16_t;

/**
 * @brief structure that inherit from doc that holds 'int8_t' data, typecasting from 'doc' to 'doc_int8_t' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                    
    int8_t value;                               /**< actual value */                                           
}doc_int8_t;

/**
 * @brief structure that inherit from doc that holds 'bool' data, typecasting from 'doc' to 'doc_bool' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                                                
    bool value;                                 /**< actual value */                                                                                       
}doc_bool;

/**
 * @brief structure that inherit from doc that holds 'char *' data, typecasting from 'doc' to 'doc_string' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                     
    char *string;                               /**< pointer to string data */                                                         
    size_t len;                                 /**< length of the string */                                                            
}doc_string;

/**
 * @brief structure that inherit from doc that holds 'uint8_t *' data, typecasting from 'doc' to 'doc_bindata' makes the data visible
 */
typedef struct{
    doc header;                                 /**< struct inherited of */                                                    
    uint8_t *data;                              /**< pointer to binary data */                                                         
    size_t len;                                 /**< length of the binary data */                                                   
}doc_bindata;

#pragma (pop)

/* ----------------------------------------- Prototypes ------------------------------------- */


// Macro checking ----------------------------------

/**
 * @brief check to see if is a string or binary data type
 * @param variable: doc value to check
 * @return true if string or bindata, false otherwise
 */
bool __check_string_bindata(doc *variable);

/**
 * @brief internal function, check if obj is null and if it is a value type, if it is, returns a dummy doc 
 * to be written to by some macros that would segfault on a null pointer
 * @param variable: pointer to doc
 * @return the object itself or a dummy in case obj is null or non value type
 */
doc *__check_obj_is_value(doc *variable);

/**
 * @brief internal function, visible only for macro porpouses
 * @return error code defined by 'errno_doc_code_t' in 'doc.c'
 */
int __doc_get_error_code(void);

/**
 * @brief internal function, check if obj is a object or array and if it is NULL
 * if not return its first child, else return NULL.
 */
doc *__check_obj_ite_macro(doc *object_or_array);

// Doc functions -----------------------------------

/**
 * @brief internal function, visible only for macro porpouses
 * @return string with the error message
 */
char *doc_get_error_msg(void);

/**
 * @brief creates a new object based on the lib syntax
 * @param name: name of the object
 * @param type: type defined by doc_type_t
 * @param ...: syntax for new data, name followed by the type and associeted value, if any
 * @return pointer to newly created dynamic object
 */
doc *doc_new(char *name, doc_type_t type, ...);

/**
 * @brief adds new data to existing instance
 * @param object_or_array: pointer to existing object
 * @param name_to_add_to: name of the instance inside object_or_array to add to
 * @param name: name of new data
 * @param type: the type of this data
 * @param ...: syntax for new data, name followed by the type and associeted value, if any
 */
void doc_add(doc *object_or_array, char *name_to_add_to, char *name, doc_type_t type, ...);

/**
 * @brief deletes a instance and its members recursevely, if any
 * @param variable: pointer to existing object
 * @param name: name of the data inside object_or_array to delete
 */
void doc_delete(doc *variable, char *name);

/**
 * @brief gets a pointer to element inside object_or_array
 * @param variable: pointer to existing object
 * @param name: name of the data inside object_or_array
 * @return pointer to the element
 */
doc* doc_get_ptr(doc* variable, char *name);

/**
 * @brief append an already made doc variable to a object or array
 * @param object_or_array: pointer to existing object
 * @param name: name of object/array to append
 * @param variable: pointer to doc variable to append
 */
void doc_append(doc *object_or_array, char *name, doc *variable);

/**
 * @brief allocate and make a new copy of variable
 * @param variable: pointer to value or object/array
 * @param name: the name of the variable to copy
 * @return newly copied variable
 */
doc *doc_copy(doc *variable, char *name);

/**
 * @brief rename an object
 * @note the name will copied, thus new_name can be freed freely
 * @param variable: pointer to value or object/array
 * @param name: the name of the variable to rename
 * @param new_name: the new name
 */
void doc_rename(doc *variable, char *name, char *new_name);

/**
 * @brief returns the size of a doc data type
 * @param variable: pointer to existing doc value
 * @param name: name of a value inside 'value'
 * @return if the value 'name' inside 'value' is an array or object, it returns the members inside of it,
 * if it is a string or binary data, then it will return the len or size respectively, for any other value type
 * it will return the size of that data type, and at last, NULL passed pointer and not found values will return 0 
 */
doc_size_t doc_get_size(doc *variable, char *name);

/**
 * @brief squashes an variable to a maximun nesting depth.
 * @note this will delete any object deepper than the maximum depth,
 * leaving only the non object/arrays variables at the specified depth.
 * This function is particularly useful when desiring to stringify a .ini file,
 * since most commonly ini files do not have section nesting, therefore squashing it to
 * a 1 level depth is useful.
 * @param variable: pointer to the variable of interest
 * @param name: the name of the variable to perform the squash
 * @param max_depth: the max depth at wich the process will begin 
 */
void doc_squash(doc *variable, char *name, doc_size_t max_depth);

/**
 * @brief creates a doc varaible based on a string value, this is valid
 * only to variables representation of numbers and actual strings, arrays and
 * objects will be treated as a simple whole string. When a integer value is represented,
 * the type will be a int64_t, decimal numbers will be o type double, a string with "true"
 * or "false" will be a bool and everything else will be a string.
 * @param name: the name of the variable
 * @param string: the value representation
 * @return a doc variable with automatic type
 */
doc *doc_from_string(char *name, char *string);

/* ----------------------------------------- Macros ----------------------------------------- */

// Error handling ----------------------------------

/**
 * @brief read only variable to access the error code set by the last call from the lib 
 */
#define doc_error_code (__doc_get_error_code())


// Doc functions -----------------------------------

/**
 * @brief gets the actual value from a doc instance, as a C type
 * @param variable: pointer to the value instance
 * @param name: the name of the value inside obj
 * @param type: type of the data, C keyword types
 * @return the actual value
 */
#define doc_get(variable, name, type) (*(type*)((void*)doc_get_ptr(variable, name) + sizeof(doc)))

/**
 * @brief sets the new value for a instance
 * @param variable: pointer to the data instance
 * @param name: name of the data inside obj
 * @param type: type of the data, C keyword types
 * @param new_value: value to be set
 * @param ...: as optional argument to char* and uint8_t* types, the len should be specified
 */
#define doc_set(variable, name, type, new_value, ...) \
    if(__check_string_bindata(doc_get_ptr(variable, name))){ \
        ((doc_bindata*)doc_get_ptr(variable, name))->len = strtoull(#__VA_ARGS__, NULL, 10); \
    } \
    *(type*)((void*)__check_obj_is_value(doc_get_ptr(variable,name)) + sizeof(doc)) = new_value

/**
 * @brief creates a iterator for a object or array to be used on a for loop
 * @note Eg. "for(doc_loop(member, object)){ printf("%s\n", member->name); }"
 * @param iterator: name for the iterator variable, has implicity type "doc*"
 * @param obj_or_array: object or array that contains the members to be looped
 */
#define doc_loop(iterator, obj_or_array) \
    doc* iterator = __check_obj_ite_macro(obj_or_array)->child; \
    iterator != NULL; \
    iterator = iterator->next

#ifdef __cplusplus 
}
#endif
#endif