#ifndef _DOC_STRUCT_HEADER_
#define _DOC_STRUCT_HEADER_
    #include <stddef.h>
    #include "doc.h"

    #define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
    #define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
    #define STRINGFY(x) #x
#endif

#if defined(STRUCT_MEMBERS) && defined(STRUCT_NAME)

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct{
    #define X(type, member) type member;
    STRUCT_MEMBERS
    #undef X
}STRUCT_NAME;

size_t CAT(STRUCT_NAME, _offsets)[] = (size_t[]){
    #define X(type, member) offsetof(struct_ex_t, member),
    STRUCT_MEMBERS
    #undef X
};

char *CAT(STRUCT_NAME, _names)[] = (char*[]){
    #define X(type, member) #member,
    STRUCT_MEMBERS
    #undef X
};

size_t CAT(STRUCT_NAME, _members) = (
    #define X(type, member) 1 +
    STRUCT_MEMBERS
    #undef X
    0
);

// void printf_values(struct_ex_t struct_ex){
//     printf("Struct:\n");
//     for(size_t i = 0; i < struct_members; i++){
//         printf("%s : %u\n", names[i], offsets[i]);
//     }
// }

doc *CAT(doc_struct_new_, STRUCT_NAME)(STRUCT_NAME struct_instance){
    doc *doc_struct = doc_new(STRINGFY(STRUCT_NAME), dt_obj, ";");
    #define X(type, member) doc_add(doc_struct, ".", #member, dt_##type, struct_instance.member); 
    STRUCT_MEMBERS
    #undef X

    return doc_struct;
}

void CAT(doc_struct_set_, STRUCT_NAME)(STRUCT_NAME struct_instance, doc *doc_struct){
    #define X(type, member) doc_set(doc_struct, #member, type, struct_instance.member); 
    STRUCT_MEMBERS
    #undef X
}

void CAT(doc_struct_get_, STRUCT_NAME)(STRUCT_NAME *struct_instance, doc *doc_struct){
    #define X(type, member) struct_instance->member = doc_get(doc_struct, #member, type); 
    STRUCT_MEMBERS
    #undef X
}

#ifdef __cplusplus 
}
#endif

#endif