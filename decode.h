#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding secret file to secret text
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
       /* Source Image info */
       char *src_image_fname;
       FILE *fptr_src_image;
       char image_data[MAX_IMAGE_BUF_SIZE];

       /* Decode secret info */
       char *secret_fname;
       int extn_size;
       int secret_file_size;
       FILE *fptr_decode_secret;
       char extn_secret_file[MAX_FILE_SUFFIX];

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get file pointers for i/p file */
Status open_image_file(DecodeInfo *decInfo);

/* Decode magic string from data */
Status decode_magic_string_from_data(const char *data, int size, FILE *fptr_src_image, DecodeInfo *decInfo);

/* Decode bit from lsb */
Status decode_bit_from_lsb(char *image_buffer);

/* Decode secret file extension size */
Status decode_extn_size(FILE *fptr_src_image, int size, DecodeInfo *decInfo);

/* Decode binary to its equivalent */
int decode_binary_to_equivalent(int size, char *image_data);

/* Decode secret file extension */
Status decode_secret_file_extn(char* extn_secret_file, FILE *fptr_src_image, int extn_size, DecodeInfo *decInfo);

/* Get file pointers for o/p file */
Status open_secret_file(DecodeInfo *decInfo);

/* Decode secret file size*/
Status decode_secret_file_size(FILE *fptr_src_image, int size, DecodeInfo *decInfo);

/* Decode secret file contents */
Status decode_secret_file_contents(FILE *fptr_src_image, int size, FILE *fptr_decode_secret, DecodeInfo *decInfo);

#endif
