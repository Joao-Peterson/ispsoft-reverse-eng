#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "modules/fmap_win/fmap_win.h"
#include "inc/ispsoft_file_def.h"

#define COMPRESSED_DATA_CHUNK 16384

uint8_t *inflate_stream(uint8_t *stream_data, size_t stream_data_size, size_t *stream_output_size);

int main(int argc, char **argv){

    if(argc < 3){ printf("Few arguments, needs 2, 'in' and 'out'.\n"); return 0; }

    // mapp file to memory
    FMAP *file_in = fmap_win(argv[1], FMAP_MODE_READ);
    if(file_in == NULL){ printf("File map failed.\n"); return 0; }
    
    ISP_HEADER_t *header = (ISP_HEADER_t*)file_in->stream;

    if(header->file_identification != 0x00 || header->check_value2 != 0x0000 || header->program_data_offset > 0xFFFF){
        printf("Input file is not a ISPsoft file.\n");
        printf("[version_ispsoft:     0x%0000X]\n",header->version_ispsoft);
        printf("[plc_type:            0x%0000X]\n",header->plc_type);
        printf("[footer_offset:       0x%0000X]\n",header->footer_offset);
        printf("[footer_size:         0x%0000X]\n",header->footer_size);
        printf("[file_identification: 0x%0000X]\n",header->file_identification);
        printf("[check_value2:        0x%0000X]\n",header->check_value2);
        printf("[program_data_offset: 0x%04X]\n",header->program_data_offset);
        printf("[program_data_size:   0x%04X]\n",header->program_data_size);
        return 0;
    }

    // buffer the compressed data
    uint8_t *deflated_stream = malloc(header->program_data_size);
    memcpy(deflated_stream, header->program_data_offset + file_in->stream, header->program_data_size);

    // DEBUG
    // printf("[version_ispsoft:     0x%04X]\n",header->version_ispsoft);
    // printf("[plc_type:            0x%04X]\n",header->plc_type);
    // printf("[footer_offset:       0x%04X]\n",header->footer_offset);
    // printf("[footer_size:         0x%04X]\n",header->footer_size);
    // printf("[file_identification: 0x%04X]\n",header->file_identification);
    // printf("[check_value2:        0x%04X]\n",header->check_value2);
    // printf("[program_data_offset: 0x%04X]\n",header->program_data_offset);
    // printf("[program_data_size:   0x%04X]\n",header->program_data_size);
    // printf("[0x%02X]...[0x%02X][0x%02X][0x%02X][0x%02X]\n",deflated_stream[0],deflated_stream[header->program_data_size-4],deflated_stream[header->program_data_size-3],deflated_stream[header->program_data_size-2],deflated_stream[header->program_data_size-1]);

    size_t inflated_stream_size = 0;
    uint8_t *inflated_stream = inflate_stream(deflated_stream, header->program_data_size, &inflated_stream_size); 
    if(inflated_stream == NULL){ printf("inflated null.\n"); return 0; }

    // DEBUG    
    // printf("Inflated stream: \n%s\n",(char*)inflated_stream);

    FILE *file_out = fopen(argv[2],"w+b");
    if(file_out == NULL){ printf("Not able to create new file (%s).\n", argv[2]); return 0;}
    
    size_t data_write = fwrite(inflated_stream, 1, inflated_stream_size, file_out);
    if(data_write < inflated_stream_size){ printf("Write to file (%s) failed. (%u)/(%u) bytes.\n", argv[2], data_write, inflated_stream_size); return 0;}

    fflush(file_out);
    fclose(file_out);

    free(deflated_stream);
    free(inflated_stream);
    fmap_close(file_in);
    
    return 0;
}

uint8_t *inflate_stream(uint8_t *stream_data, size_t stream_data_size, size_t *stream_output_size){

    int code;
    size_t have;
    uint8_t *stream_output_base = NULL; 
    uint8_t *stream_output = stream_output_base; 
    (*stream_output_size) = 0;

    uint8_t buffer_in [COMPRESSED_DATA_CHUNK];
    uint8_t buffer_out[COMPRESSED_DATA_CHUNK];

    z_stream stream_compression;
    stream_compression.zalloc = Z_NULL;
    stream_compression.zfree = Z_NULL;
    stream_compression.opaque = Z_NULL;

    code = inflateInit2(&stream_compression, -MAX_WBITS); // - to avoid header detection
    if(code != Z_OK){ printf("InflateInit failed. Msg: %s\n", stream_compression.msg); return 0; }

    stream_data += 6; // jump over the header and ID bytes and start directly at the deflated data. The ID bytes have been causing trouble
    stream_data_size -= 6;
    size_t remaining_data = stream_data_size; 

    do{ // loop until all deflated data is read

        if(remaining_data >= COMPRESSED_DATA_CHUNK){
            memcpy(buffer_in, stream_data + (stream_data_size - remaining_data), COMPRESSED_DATA_CHUNK);
            stream_compression.avail_in = COMPRESSED_DATA_CHUNK;
            remaining_data -= COMPRESSED_DATA_CHUNK;
        }
        else if(remaining_data == 0){
            break; // exit while loop
        }
        else{
            memcpy(buffer_in, stream_data + (stream_data_size - remaining_data), remaining_data);
            stream_compression.avail_in = remaining_data;
            remaining_data -= remaining_data;
        }

        //DEBUG
        // printf("[BUFF_IN] | size_data(%u) | size_buffer(%u):\n[ ", stream_data_size, stream_compression.avail_in);
        // for(int z = 0; z < stream_compression.avail_in; z++)
        //     printf("%02X ",buffer_in[z]);
        // printf("]\n");

        stream_compression.next_in = buffer_in;

        do{ // loop until "n" "COMPRESSED_DATA_CHUNK's" have been inflated from a single "COMPRESSED_DATA_CHUNK" input
            stream_compression.avail_out = COMPRESSED_DATA_CHUNK;
            stream_compression.next_out = buffer_out;
            
            code = inflate(&stream_compression, Z_FULL_FLUSH);

            if(code != Z_OK && code != Z_STREAM_END){
                printf("error during inflation. Code: (%i). Msg: %s\n", code, stream_compression.msg); 
                return NULL; 
            }else{
                // printf("[DEBUG] (BP1)\n");
            }

            have = COMPRESSED_DATA_CHUNK - stream_compression.avail_out;

            (*stream_output_size) += have;
            stream_output_base = realloc(stream_output_base, (*stream_output_size));
            stream_output = stream_output_base + (*stream_output_size) - have;
            memcpy(stream_output, buffer_out, have);

        }while(stream_compression.avail_out == 0);

    }while(code != Z_STREAM_END);

    inflateEnd(&stream_compression);
    
    return stream_output_base;
}