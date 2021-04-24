#ifndef _PNET_HEADER_
#define _PNET_HEADER_

#include "matrix.h"
#include <stdarg.h>

typedef enum{
    places_pnet,
    places_init_pnet,
    transitions_pnet,
    arcs_pnet,
    inhibit_arcs_pnet
}pnet_options_t;

#define IS_PNET_OPTION(option) \
( \
    (option == places_pnet) || \
    (option == places_init_pnet) || \
    (option == transitions_pnet) || \
    (option == arcs_pnet) || \
    (option == inhibit_arcs_pnet) \
)

typedef struct{
    matrix_string_t *places;
    matrix_string_t *transitions;
    matrix_int_t *init_state;
    matrix_int_t *arcs;
    matrix_int_t *arcs_inhibit;

    matrix_int_t *w_minus;
    matrix_int_t *state;
    matrix_int_t *sensitive_transitions;
}pnet_t;

void pnet_w_minus_new(pnet_t *pnet){
    for(size_t i = 0; i < pnet->arcs->y; i++){
        for(size_t j = 0; j < pnet->arcs->x; j++){
            if(pnet->w_minus->m[i][j] >= 0)
                pnet->w_minus->m[i][j] = 0;
        }
    }
}

void pnet_delete(pnet_t *pnet){
    matrix_delete(pnet->arcs);
    matrix_delete(pnet->init_state);
    matrix_delete(pnet->sensitive_transitions);
    matrix_delete(pnet->state);
    matrix_delete(pnet->w_minus);
    matrix_string_delete(pnet->places);
    matrix_string_delete(pnet->transitions);

    free(pnet);
}

void pnet_print(pnet_t *pnet){
    printf("\n################# Petri net #################\n");
    matrix_print(pnet->state, "state");
    printf("\n");
    matrix_print(pnet->sensitive_transitions, "sensible");
    printf("#############################################\n\n");
}

void pnet_sense(pnet_t *pnet){
    matrix_int_t *sense_m   = matrix_duplicate(pnet->w_minus);
    matrix_int_t *inhibit_m = matrix_duplicate(pnet->arcs_inhibit);

    for(size_t j = 0; j < pnet->arcs->x; j++){
        pnet->sensitive_transitions->m[0][j] = 1;                                   // set transition to sensibilized by default
        for(size_t i = 0; i < pnet->arcs->y; i++){
            /**
             * by adding the exiting token to the required tokens, w_minus, we can compare to see
             * if the transition is firable, that is, after the sum, the transition column should
             * be zero or bigger, indicating that there are enought or more tokens to satisfy the
             * subtraction.
             * for inhibit arcs, by subtracting the number of required token by the existing ones
             * the result should be 0 or negative, thus if bigger than zero, there are not enought
             * tokens.  
             */

            sense_m->m[i][j]    += pnet->state->m[0][i];                            // sum tokens with required tokens, should be zero
            inhibit_m->m[i][j]  -= pnet->state->m[0][i];                            // sub tokens with requires inhibit tokens, hould also be zero

            if(sense_m->m[i][j] < 0 || inhibit_m->m[i][j] > 0)                      // if any missing sub token or missing inhibit token 
                pnet->sensitive_transitions->m[0][j] = 0;                           // desensibilize
        }
    }

    matrix_delete(sense_m);
    matrix_delete(inhibit_m);
}

void pnet_fire(pnet_t *pnet, matrix_int_t *transitions_to_fire){
    pnet_sense(pnet);
    
    matrix_int_t *transitions_able_to_fire = matrix_and(transitions_to_fire, pnet->sensitive_transitions);
    matrix_int_t *transitions_able_to_fire_T = matrix_transpose(transitions_able_to_fire);
    matrix_int_t *buffer_mul = matrix_mul(pnet->arcs, transitions_able_to_fire_T);
    matrix_int_t *buffer_mul_T = matrix_transpose(buffer_mul);
    matrix_int_t *buffer_add = matrix_add(pnet->state, buffer_mul_T);

    // matrix_print(transitions_able_to_fire, "1");
    // matrix_print(transitions_able_to_fire_T, "2");
    // matrix_print(buffer_mul, "3");
    // matrix_print(buffer_mul_T, "4");
    // matrix_print(buffer_add, "5");
    
    matrix_copy(pnet->state, buffer_add);

    matrix_delete(transitions_able_to_fire);
    matrix_delete(transitions_able_to_fire_T);
    matrix_delete(buffer_mul);
    matrix_delete(buffer_mul_T);
    matrix_delete(buffer_add);
}

pnet_t *pnet_new(size_t places_num, size_t transitions_num, ...){
    va_list args;
    va_start(args, transitions_num);
    
    pnet_t *pnet = (pnet_t*)calloc(1, sizeof(pnet_t)); 

    for(pnet_options_t options = va_arg(args, pnet_options_t); IS_PNET_OPTION(options); options = va_arg(args, pnet_options_t)){
        switch(options){
            case places_pnet:
                pnet->places                = v_matrix_string_new(places_num, 1, &args);
            break;

            case places_init_pnet:
                pnet->init_state            = v_matrix_new(places_num, 1, &args);
            break;

            case transitions_pnet:
                pnet->transitions           = v_matrix_string_new(transitions_num, 1, &args);
            break;
            
            case arcs_pnet:
                pnet->arcs                  = v_matrix_new(transitions_num, places_num, &args);
            break;
            
            case inhibit_arcs_pnet:           
                pnet->arcs_inhibit          = v_matrix_new(transitions_num, places_num, &args);
            break;

            default:
                return NULL;
            break;
        }
    }

    if(
        pnet->places == NULL ||
        pnet->init_state == NULL ||
        pnet->transitions == NULL ||
        pnet->arcs_inhibit == NULL ||
        pnet->arcs == NULL
    ){
        va_end(args);
        return NULL;
    }

    pnet->w_minus               = matrix_duplicate(pnet->arcs);
    pnet_w_minus_new(pnet);
    pnet->state                 = matrix_duplicate(pnet->init_state);
    pnet->sensitive_transitions = matrix_new_zero(transitions_num, 1);
    pnet_sense(pnet);

    va_end(args);

    return pnet;
}

#endif