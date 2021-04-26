#ifndef _MATRIX_HEADER_
#define _MATRIX_HEADER_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

typedef struct{
    size_t x;
    size_t y;
    int **m;
}matrix_int_t;

typedef struct{
    size_t x;
    size_t y;
    char ***m;
}matrix_string_t;



matrix_string_t *v_matrix_string_new(size_t x, size_t y, va_list *args){
    if(x == 0 || y == 0) return NULL;
    
    matrix_string_t *matrix = (matrix_string_t*)calloc(1, sizeof(matrix_string_t));
    matrix->x = x;
    matrix->y = y;
    matrix->m = (char***)calloc(y, sizeof(char**));

    for(size_t i = 0; i < y; i++){
        matrix->m[i] = (char**)calloc(x, sizeof(char*));
        for(size_t j = 0; j < x; j++){
            matrix->m[i][j] = va_arg(*args, char*); 
        }
    }

    return matrix;
}

matrix_string_t *matrix_string_new(size_t x, size_t y, ...){
    va_list args;
    va_start(args, y);

    matrix_string_t *matrix = v_matrix_string_new(x, y, &args);

    return matrix;
}

void matrix_string_delete(matrix_string_t *matrix){
    for(size_t i = 0; i < matrix->y; i++)
        free(matrix->m[i]);

    free(matrix->m);
    free(matrix);    
}

void matrix_string_print(matrix_string_t *matrix, char *name){
    printf("\"%s\" =\n", name);
    for(size_t i = 0; i < matrix->y; i++){
        printf("[");
        for(size_t j = 0; j < matrix->x; j++){
            printf(" \"%s\"", matrix->m[i][j]);        
        }
        printf(" ]\n");
    }
}



matrix_int_t *v_matrix_new(size_t x, size_t y, va_list *args){
    matrix_int_t *matrix = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    matrix->x = x;
    matrix->y = y;
    matrix->m = (int**)calloc(y, sizeof(int*));

    for(size_t i = 0; i < y; i++){
        matrix->m[i] = (int*)calloc(x, sizeof(int));
        for(size_t j = 0; j < x; j++){
            matrix->m[i][j] = va_arg(*args, int); 
        }
    }

    return matrix;
}

matrix_int_t *matrix_new(size_t x, size_t y, ...){
    va_list args;
    va_start(args, y);

    matrix_int_t *matrix = v_matrix_new(x, y, &args);

    return matrix;
}

matrix_int_t *matrix_new_zero(size_t x, size_t y){
    matrix_int_t *matrix = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    matrix->x = x;
    matrix->y = y;
    matrix->m = (int**)calloc(y, sizeof(int*));

    for(size_t i = 0; i < y; i++){
        matrix->m[i] = (int*)calloc(x, sizeof(int));
        for(size_t j = 0; j < x; j++){
            matrix->m[i][j] = 0; 
        }
    }

    return matrix;
}

void matrix_delete(matrix_int_t *matrix){
    if(matrix == NULL) return;
    
    for(size_t i = 0; i < matrix->y; i++)
        free(matrix->m[i]);

    free(matrix->m);
    free(matrix);
}

void matrix_print(matrix_int_t *matrix, char *name){
    printf("\"%s\" =\n", name);
    for(size_t i = 0; i < matrix->y; i++){
        printf("[");
        for(size_t j = 0; j < matrix->x; j++){
            printf(" %i", matrix->m[i][j]);        
        }
        printf(" ]\n");
    }
}

matrix_int_t *matrix_mul(matrix_int_t *a, matrix_int_t *b){
    if(a->x != b->y)
        return NULL;

    matrix_int_t *m = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    m->x = b->x;
    m->y = a->y;
    m->m = (int**)calloc(m->y, sizeof(int*));

    for(size_t i = 0; i < m->y; i++){
        m->m[i] = (int*)calloc(m->x, sizeof(int));
        for(size_t j = 0; j < m->x; j++){
            int acc = 0;
            
            for(size_t x = 0; x < a->x; x++){
                acc += a->m[i][x] * b->m[x][j];
            }

            m->m[i][j] = acc; 
        }
    }

    return m;
}

matrix_int_t *matrix_mul_scalar(matrix_int_t *a, int c){
    matrix_int_t *m = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    m->x = a->x;
    m->y = a->y;
    m->m = (int**)calloc(m->y, sizeof(int*));

    for(size_t i = 0; i < m->y; i++){
        m->m[i] = (int*)calloc(m->x, sizeof(int));
        for(size_t j = 0; j < m->x; j++){
            m->m[i][j] = m->m[i][j] * c; 
        }
    }

    return m;
}

matrix_int_t *matrix_add(matrix_int_t *a, matrix_int_t *b){
    if(a->x != b->x || a->y != b->y) return NULL;

    matrix_int_t *m = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    m->x = a->x;
    m->y = a->y;
    m->m = (int**)calloc(m->y, sizeof(int*));

    for (size_t i = 0; i < m->y; i++){
        m->m[i] = (int*)calloc(m->x, sizeof(int));
        for (size_t j = 0; j < m->x; j++){
            m->m[i][j] = a->m[i][j] + b->m[i][j];
        }
    }
    
    return m;
}

matrix_int_t *matrix_and(matrix_int_t *a, matrix_int_t *b){
    if(a->x != b->x || a->y != b->y) return NULL;

    matrix_int_t *m = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    m->x = a->x;
    m->y = a->y;
    m->m = (int**)calloc(m->y, sizeof(int*));

    for (size_t i = 0; i < m->y; i++){
        m->m[i] = (int*)calloc(m->x, sizeof(int));
        for (size_t j = 0; j < m->x; j++){
            m->m[i][j] = a->m[i][j] & b->m[i][j];
        }
    }
    
    return m;
}

matrix_int_t *matrix_duplicate(matrix_int_t *a){
    matrix_int_t *m = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    m->x = a->x;
    m->y = a->y;
    m->m = (int**)calloc(m->y, sizeof(int*));

    for (size_t i = 0; i < m->y; i++){
        m->m[i] = (int*)calloc(m->x, sizeof(int));
        for (size_t j = 0; j < m->x; j++){
            m->m[i][j] = a->m[i][j];
        }
    }
    
    return m;
}

void matrix_copy(matrix_int_t *dest, matrix_int_t *src){
    if(dest->x != src->x || dest->y != src->y) return;
    
    for (size_t i = 0; i < dest->y; i++){
        for (size_t j = 0; j < dest->x; j++){
            dest->m[i][j] = src->m[i][j];
        }
    }
}

matrix_int_t *matrix_transpose(matrix_int_t *matrix){
    matrix_int_t *m = (matrix_int_t*)calloc(1, sizeof(matrix_int_t));
    m->x = matrix->y;
    m->y = matrix->x;
    m->m = (int**)calloc(m->y, sizeof(int*));

    for(size_t i = 0; i < m->y; i++){
        m->m[i] = (int*)calloc(m->x, sizeof(int));
        for(size_t j = 0; j < m->x; j++){
            m->m[i][j] = matrix->m[j][i]; 
        }
    }

    return m;
}

#endif