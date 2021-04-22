#ifndef _ISP_HEADER_
#define _ISP_HEADER_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>

#define COMPRESSED_DATA_CHUNK 16384

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
            stream_output_base = (uint8_t *)realloc(stream_output_base, (*stream_output_size));
            stream_output = stream_output_base + (*stream_output_size) - have;
            memcpy(stream_output, buffer_out, have);

        }while(stream_compression.avail_out == 0);

    }while(code != Z_STREAM_END);

    inflateEnd(&stream_compression);
    
    return stream_output_base;
}



#endif