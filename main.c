#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "inc/ispsoft_file_def.h"
#include "inc/isp.h"
#include "doc.h"
#include "parse_utils.h"
#include "inc/pnet.h"

int main(int argc, char **argv){

    pnet_t *pnet = pnet_new(5, 4,
         "p1", "p2", "p3", "p4", "p5",
         0, 0, 1, 0, 1,
         "t1", "t2", "t3", "t4",
         -1,  0,  0,  0,
          1, -1,  0,  1,
          0,  1, -1,  0,
          0,  0,  1, -1,
          0,  0, -1,  0
    ); 

    pnet_print(pnet);

    matrix_int_t *trigger = matrix_new(
        pnet->transitions->x, 1, 
        0, 0, 1, 0
    );
    
    pnet_fire(pnet, trigger);
    pnet_sense(pnet);

    pnet_print(pnet);

    pnet_delete(pnet);

    // if(argc < 3){ printf("Few arguments, needs 2, 'in' and 'out'.\n"); return 0; }

    // char *file = fstream(argv[1]);
    // if(file == NULL){ printf("Fiel open failed.\n"); return 0; }
    
    // ISP_HEADER_t *header = (ISP_HEADER_t*)file;

    // if(header->file_identification != 0x00 || header->check_value2 != 0x0000 || header->program_data_offset > 0xFFFF){
    //     printf("Input file is not a ISPsoft file.\n");
    //     printf("[version_ispsoft:     0x%0000X]\n",header->version_ispsoft);
    //     printf("[plc_type:            0x%0000X]\n",header->plc_type);
    //     printf("[footer_offset:       0x%0000X]\n",header->footer_offset);
    //     printf("[footer_size:         0x%0000X]\n",header->footer_size);
    //     printf("[file_identification: 0x%0000X]\n",header->file_identification);
    //     printf("[check_value2:        0x%0000X]\n",header->check_value2);
    //     printf("[program_data_offset: 0x%04X]\n",header->program_data_offset);
    //     printf("[program_data_size:   0x%04X]\n",header->program_data_size);
    //     return 0;
    // }

    // // buffer the compressed data
    // uint8_t *deflated_stream = malloc(header->program_data_size);
    // memcpy(deflated_stream, header->program_data_offset + file, header->program_data_size);

    // // DEBUG
    // // printf("[version_ispsoft:     0x%04X]\n",header->version_ispsoft);
    // // printf("[plc_type:            0x%04X]\n",header->plc_type);
    // // printf("[footer_offset:       0x%04X]\n",header->footer_offset);
    // // printf("[footer_size:         0x%04X]\n",header->footer_size);
    // // printf("[file_identification: 0x%04X]\n",header->file_identification);
    // // printf("[check_value2:        0x%04X]\n",header->check_value2);
    // // printf("[program_data_offset: 0x%04X]\n",header->program_data_offset);
    // // printf("[program_data_size:   0x%04X]\n",header->program_data_size);
    // // printf("[0x%02X]...[0x%02X][0x%02X][0x%02X][0x%02X]\n",deflated_stream[0],deflated_stream[header->program_data_size-4],deflated_stream[header->program_data_size-3],deflated_stream[header->program_data_size-2],deflated_stream[header->program_data_size-1]);

    // size_t inflated_stream_size = 0;
    // uint8_t *inflated_stream = inflate_stream(deflated_stream, header->program_data_size, &inflated_stream_size); 
    // if(inflated_stream == NULL){ printf("inflated null.\n"); return 0; }

    // // DEBUG    
    // // printf("Inflated stream: \n%s\n",(char*)inflated_stream);

    // FILE *file_out = fopen(argv[2],"w+b");
    // if(file_out == NULL){ printf("Not able to create new file (%s).\n", argv[2]); return 0;}
    
    // size_t data_write = fwrite(inflated_stream, 1, inflated_stream_size, file_out);
    // if(data_write < inflated_stream_size){ printf("Write to file (%s) failed. (%u)/(%u) bytes.\n", argv[2], data_write, inflated_stream_size); return 0;}

    // fflush(file_out);
    // fclose(file_out);

    // free(deflated_stream);
    // free(inflated_stream);
    // free(file);
    
    return 0;
}