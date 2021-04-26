#ifndef _PNET_HEADER_
#define _PNET_HEADER_

#include "matrix.h"
#include <stdarg.h>

typedef enum{
    end_options_pnet    = -1,
    places_name_pnet,
    places_init_pnet,
    transitions_pnet,
    arcs_pnet,
    inhibit_arcs_pnet,
    conditions_name_pnet,
    input_name_pnet,
    output_name_pnet,
    conditions_map_pnet,
    input_map_pnet,
    output_map_pnet,
    transitions_delay_pnet,
    pnet_options_max
}pnet_options_t;

typedef enum{
    pos_edge_pnet               = 0x01,
    rise_edge_pnet              = 0x01,
    neg_edge_pnet               = 0x02,
    fall_edge_pnet              = 0x02,
    any_edge_pnet               = 0x04,
    toggle_up_down_pnet         = 0x08,
    change_up_down_pnet         = 0x08,
    toggle_down_up_pnet         = 0x10,
    change_down_up_pnet         = 0x10,
    toggle_pnet                 = 0x20,
    change_pnet                 = 0x20,
    toggle_any_pnet             = 0x20,
    change_any_pnet             = 0x20
}pnet_edge_t;

typedef struct{
    matrix_string_t *places_name;
    matrix_string_t *transitions;
    matrix_string_t *conditions_name;
    matrix_string_t *input_name;
    matrix_string_t *output_name;

    matrix_int_t *init_state;
    matrix_int_t *arcs;
    matrix_int_t *arcs_inhibit;
    matrix_int_t *transitions_delay;
    matrix_int_t *input;
    matrix_int_t *input_edge;
    matrix_int_t *input_last;
    matrix_int_t *input_last_edge;
    matrix_int_t *conditions;
    matrix_int_t *output;
    matrix_int_t *input_map;
    matrix_int_t *conditions_map;
    matrix_int_t *output_map;

    matrix_int_t *w_minus;
    matrix_int_t *places;
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
    matrix_string_delete(pnet->places_name);
    matrix_string_delete(pnet->transitions);
    matrix_string_delete(pnet->conditions_name);
    matrix_string_delete(pnet->input_name);
    matrix_string_delete(pnet->output_name);
    matrix_delete(pnet->init_state);
    matrix_delete(pnet->arcs);
    matrix_delete(pnet->arcs_inhibit);
    matrix_delete(pnet->transitions_delay);
    matrix_delete(pnet->input);
    matrix_delete(pnet->conditions);
    matrix_delete(pnet->output);
    matrix_delete(pnet->input_map);
    matrix_delete(pnet->conditions_map);
    matrix_delete(pnet->output_map);
    matrix_delete(pnet->w_minus);
    matrix_delete(pnet->places);
    matrix_delete(pnet->sensitive_transitions);

    free(pnet);
}

void pnet_print(pnet_t *pnet){
    printf("\n################# Petri net #################\n");
    matrix_print(pnet->places, "state");
    printf("\n");
    matrix_print(pnet->sensitive_transitions, "sensible");
    printf("\n");
    matrix_print(pnet->input, "input");
    printf("\n");
    matrix_print(pnet->output, "output");
    printf("#############################################\n\n");
}

void pnet_sense(pnet_t *pnet){
    matrix_int_t *sense_m   = matrix_duplicate(pnet->w_minus);
    matrix_int_t *inhibit_m = matrix_duplicate(pnet->arcs_inhibit);

    for(size_t transition = 0; transition < pnet->arcs->x; transition++){
        pnet->sensitive_transitions->m[0][transition] = 1;                          // set transition to sensibilized by default
        for(size_t place = 0; place < pnet->arcs->y; place++){
            /**
             * by adding the exiting token to the required tokens, w_minus, we can compare to see
             * if the transition is firable, that is, after the sum, the transition column should
             * be zero or bigger, indicating that there are enought or more tokens to satisfy the
             * subtraction.
             * for inhibit arcs, by subtracting the number of required token by the existing ones
             * the result should be 0 or negative, thus if bigger than zero, there are not enought
             * tokens.  
             */

            sense_m->m[place][transition]   += pnet->places->m[0][place];            // sum tokens with required tokens, should be zero
            inhibit_m->m[place][transition] -= pnet->places->m[0][place];            // sub tokens with requires inhibit tokens, hould also be zero

            if(
                sense_m->m[place][transition] < 0    || 
                inhibit_m->m[place][transition] > 0   
            )                                                                       // if any missing sub token or missing inhibit token 
                pnet->sensitive_transitions->m[0][transition] = 0;                  // desensibilize
        }
    }

    matrix_int_t *conditions = matrix_duplicate(pnet->conditions_map);

    for(size_t transition = 0; transition < pnet->conditions_map->x; transition++){
        int buf = 1;
        for(size_t condition = 0; condition < pnet->conditions_map->y; condition++){
            
            /**
             * for every condition mapped to a transition, logic AND it with the condition input state,
             * if all conditions are valid then logic AND with the sensitive_transitions vector  
             */

            conditions->m[condition][transition] &= pnet->conditions->m[0][condition]; 
            if(conditions->m[condition][transition] == 0){
                buf = 0;
                break;
            }
        }

        pnet->sensitive_transitions->m[0][transition] &= buf; 
    }

    matrix_delete(conditions);
    matrix_delete(sense_m);
    matrix_delete(inhibit_m);
}

matrix_int_t *pnet_input_detection(pnet_t *pnet){
    matrix_int_t *transitions = matrix_new_zero(pnet->transitions->x, pnet->transitions->y);
    matrix_int_t *only_edges = matrix_new_zero(pnet->input->x, pnet->input->y);

    for(size_t input = 0; input < pnet->input->x; input++){
        pnet->input_edge->m[0][input] = 0;
        only_edges->m[0][input] = 0;

        if(pnet->input_last->m[0][input] == 0 && pnet->input->m[0][input] == 1){
            pnet->input_edge->m[0][input] |= pos_edge_pnet;
            pnet->input_edge->m[0][input] |= any_edge_pnet;
            only_edges->m[0][input] = pnet->input_edge->m[0][input];
        }
        else if(pnet->input_last->m[0][input] == 1 && pnet->input->m[0][input] == 0){
            pnet->input_edge->m[0][input] |= neg_edge_pnet;
            pnet->input_edge->m[0][input] |= any_edge_pnet;
            only_edges->m[0][input] = pnet->input_edge->m[0][input];
        }

        if((pnet->input_last_edge->m[0][input] & neg_edge_pnet) && (pnet->input_edge->m[0][input] & pos_edge_pnet)){
            pnet->input_edge->m[0][input] |= toggle_down_up_pnet;
            pnet->input_edge->m[0][input] |= toggle_any_pnet;
        }
        else if((pnet->input_last_edge->m[0][input] & pos_edge_pnet) && (pnet->input_edge->m[0][input] & neg_edge_pnet)){
            pnet->input_edge->m[0][input] |= toggle_up_down_pnet;
            pnet->input_edge->m[0][input] |= toggle_any_pnet;
        }

    }

    matrix_copy(pnet->input_last_edge, only_edges);
    matrix_copy(pnet->input_last, pnet->input);
    
    matrix_delete(only_edges);

    for(size_t transition = 0; transition < pnet->input_map->x; transition++){
        transitions->m[0][transition] = 0;
        for(size_t input = 0; input < pnet->input_map->y; input++){
            if(pnet->input_map->m[input][transition] & pnet->input_edge->m[0][input]){
                transitions->m[0][transition] = 1;
                break;
            }
        }
    }

    return transitions;
}

void pnet_output_set(pnet_t *pnet){
    for(size_t output = 0; output < pnet->output_map->x; output++){
        pnet->output->m[0][output] = 0;
        for(size_t place = 0; place < pnet->output_map->y; place++){
            
            if(
                (pnet->output_map->m[place][output] > 0) 
                && (pnet->places->m[0][place] >= pnet->output_map->m[place][output])
            ){
                pnet->output->m[0][output] = 1;
                break;
            }
        }
    }
}

void pnet_fire(pnet_t *pnet, matrix_int_t *input, matrix_int_t *conditions){
    if(pnet == NULL || input == NULL || conditions == NULL) return;
    if(input->x != pnet->input->x || conditions->x != pnet->conditions->x) return;

    matrix_copy(pnet->input, input);

    matrix_copy(pnet->conditions, conditions);
    
    pnet_sense(pnet);

    matrix_int_t *transitions_to_fire = pnet_input_detection(pnet);
    
    matrix_int_t *transitions_able_to_fire = matrix_and(transitions_to_fire, pnet->sensitive_transitions);
    matrix_int_t *transitions_able_to_fire_T = matrix_transpose(transitions_able_to_fire);
    matrix_int_t *buffer_mul = matrix_mul(pnet->arcs, transitions_able_to_fire_T);
    matrix_int_t *buffer_mul_T = matrix_transpose(buffer_mul);
    matrix_int_t *buffer_add = matrix_add(pnet->places, buffer_mul_T);

    matrix_copy(pnet->places, buffer_add);

    pnet_output_set(pnet);

    matrix_delete(transitions_to_fire);
    matrix_delete(transitions_able_to_fire);
    matrix_delete(transitions_able_to_fire_T);
    matrix_delete(buffer_mul);
    matrix_delete(buffer_mul_T);
    matrix_delete(buffer_add);
}

pnet_t *pnet_new(size_t places_num, size_t transitions_num, size_t conditions_num, size_t inputs_num, size_t output_num, ...){
    if(places_num == 0 || transitions_num == 0) return NULL;
    
    va_list args;
    va_start(args, output_num);

    size_t size;
    pnet_t *pnet = (pnet_t*)calloc(1, sizeof(pnet_t)); 

    bool handle_options = true;
    for(pnet_options_t options = va_arg(args, pnet_options_t); handle_options; options = va_arg(args, pnet_options_t)){
        
        switch(options){
            case places_name_pnet:
                pnet->places_name = v_matrix_string_new(places_num, 1, &args);
                if(pnet->places_name == NULL){
                    free(pnet);
                    va_end(args);
                    return NULL;
                }
            break;

            case places_init_pnet:
                pnet->init_state = v_matrix_new(places_num, 1, &args);
                if(pnet->init_state == NULL){
                    free(pnet);
                    va_end(args);
                    return NULL;
                }
            break;

            case transitions_pnet:
                pnet->transitions = v_matrix_string_new(transitions_num, 1, &args);
                if(pnet->transitions == NULL){
                    free(pnet);
                    va_end(args);
                    return NULL;
                }
            break;
            
            case arcs_pnet:
                pnet->arcs = v_matrix_new(transitions_num, places_num, &args);
                if(pnet->arcs == NULL){
                    free(pnet);
                    va_end(args);
                    return NULL;
                }
            break;
            
            case inhibit_arcs_pnet:           
                pnet->arcs_inhibit = v_matrix_new(transitions_num, places_num, &args);
                if(pnet->arcs_inhibit == NULL){
                    free(pnet);
                    va_end(args);
                    return NULL;
                }
            break;
            
            case transitions_delay_pnet:           
                pnet->transitions_delay = v_matrix_new(transitions_num, 1, &args);
                if(pnet->transitions_delay == NULL){
                    free(pnet);
                    va_end(args);
                    return NULL;
                }
            break;

            case conditions_name_pnet:
                if(conditions_num != 0){
                    pnet->conditions_name = v_matrix_string_new(conditions_num, 1, &args);
                    if(pnet->conditions_name == NULL){
                        free(pnet);
                        va_end(args);
                        return NULL;
                    }
                    pnet->conditions = matrix_new_zero(conditions_num, 1);
                }
            break;

            case input_name_pnet:
                if(inputs_num != 0){
                    pnet->input_name = v_matrix_string_new(inputs_num, 1, &args);
                    if(pnet->input_name == NULL){
                        free(pnet);
                        va_end(args);
                        return NULL;
                    }
                    pnet->input = matrix_new_zero(inputs_num, 1);
                    pnet->input_edge = matrix_new_zero(inputs_num, 1);
                    pnet->input_last = matrix_new_zero(inputs_num, 1);
                    pnet->input_last_edge = matrix_new_zero(inputs_num, 1);
                }
            break;

            case output_name_pnet:
                if(output_num != 0){
                    pnet->output_name = v_matrix_string_new(output_num, 1, &args);
                    if(pnet->output_name == NULL){
                        free(pnet);
                        va_end(args);
                        return NULL;
                    }
                    pnet->output = matrix_new_zero(output_num, 1);
                }
            break;

            case conditions_map_pnet:
                if(conditions_num != 0){
                    pnet->conditions_map = v_matrix_new(transitions_num, conditions_num, &args);
                    if(pnet->conditions_map == NULL){
                        free(pnet);
                        va_end(args);
                        return NULL;
                    }
                }
            break;

            case input_map_pnet:
                if(inputs_num != 0){
                    pnet->input_map = v_matrix_new(transitions_num, inputs_num, &args);
                    if(pnet->input_map == NULL){
                        free(pnet);
                        va_end(args);
                        return NULL;
                    }
                }
            break;

            case output_map_pnet:
                if(output_num != 0){
                    pnet->output_map = v_matrix_new(output_num, places_num, &args);
                    if(pnet->output_map == NULL){
                        free(pnet);
                        va_end(args);
                        return NULL;
                    }
                }
            break;
            
            case end_options_pnet:
                handle_options = false;
            break;

            default:
                free(pnet);
                va_end(args);
                return NULL;
            break;
        }
    }

    pnet->w_minus               = matrix_duplicate(pnet->arcs);
    pnet_w_minus_new(pnet);
    pnet->places                = matrix_duplicate(pnet->init_state);
    pnet->sensitive_transitions = matrix_new_zero(transitions_num, 1);
    pnet_sense(pnet);

    va_end(args);

    return pnet;
}

#endif