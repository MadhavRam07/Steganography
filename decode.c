#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
       if(strcmp(strstr(argv[2], "."), ".bmp") == 0)
       {
              decInfo-> src_image_fname = argv[2];
       }
       else
       {
              return e_failure;
       }
       return e_success;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, decode Secret file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_image_file(DecodeInfo *decInfo)
{
       // Src Image file
       decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "r");
       // Do Error handling
       if (decInfo->fptr_src_image == NULL)
       {   
              perror("fopen");
              fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

              return e_failure;
       }

       // No failure return e_success
       return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{
       fseek(decInfo-> fptr_src_image, 54, SEEK_SET);
       printf("fptr = %ld\n", ftell(decInfo-> fptr_src_image));
       if(decode_magic_string_from_data(magic_string, strlen(magic_string), decInfo-> fptr_src_image, decInfo) == e_success)
              return e_success;
       else
              return e_failure;
}

Status decode_magic_string_from_data(const char *data, int size, FILE *fptr_src_image, DecodeInfo *decInfo)
{
       int i;
       char ch;
       for(i = 0 ; i < size ; i++)
       {
              fread(decInfo-> image_data, 8, 1, fptr_src_image);
              decode_bit_from_lsb(decInfo-> image_data);
              ch = decode_binary_to_equivalent(ch, decInfo-> image_data);
              printf("fptr at %c = %ld\n", ch, ftell(fptr_src_image));
              if(data[i] != ch)
                     return e_failure;
       }
       return e_success;
}

Status decode_bit_from_lsb(char *image_buffer)
{
       int i;
       for(i = 0 ; i < 8 ; i++)
       {
              image_buffer[i] = image_buffer[i] & 0x01;
              //printf("image_buffer[%d] = %d\n", i, image_buffer[i]);
       }
}

Status decode_extn_size(FILE *fptr_src_image, int size, DecodeInfo *decInfo)
{
       int i;
       decInfo-> extn_size = 0;
              printf("fptr at extn_size = %ld\n", ftell(fptr_src_image));
       for(i = 0 ; i < size ; i++)
       {
              fread(decInfo-> image_data, 8, 1, fptr_src_image);
              decode_bit_from_lsb(decInfo-> image_data);
              decInfo-> extn_size = decode_binary_to_equivalent(decInfo-> extn_size, decInfo-> image_data) + decInfo-> extn_size;
       }
       return e_success;

}

int decode_binary_to_equivalent(int size, char *image_data)
{
       int i;
       size = 0;
       for(i = 0 ; i < 8 ; i++)
       {
              uint mask = 1 << i;
              size = (image_data[(7 - i)] * mask) + size;
       }
       return size;
}

Status decode_secret_file_extn(char *extn_secret_file, FILE *fptr_src_image, int extn_size, DecodeInfo *decInfo)
{
       char str[30] = "decoded_secret";
       int i;
              printf("fptr at extn = %ld\n", ftell(fptr_src_image));
       for(i = 0 ; i < extn_size ; i++)
       {
              fread(decInfo-> image_data, 8, 1, fptr_src_image);
              decode_bit_from_lsb(decInfo-> image_data);
              extn_secret_file[i] = decode_binary_to_equivalent(extn_secret_file[i], decInfo-> image_data);
       }
       extn_secret_file[i] = '\0';
       decInfo-> fptr_decode_secret = fopen(strcat(str, extn_secret_file), "w");
       decInfo-> secret_fname = str;
       return e_success;
}

Status open_secret_file(DecodeInfo *decInfo)
{
       // Do Error handling
       if (decInfo->fptr_decode_secret == NULL)
       {
              perror("fopen");
              fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo-> secret_fname);

              return e_failure;
       }
       // No failure return e_success
       return e_success;
}

Status decode_secret_file_size(FILE *fptr_src_image, int size, DecodeInfo *decInfo)
{
       int i, j;
       decInfo-> secret_file_size = 0;
              printf("fptr at file_size = %ld\n", ftell(fptr_src_image));
       for(i = 0 ; i < size ; i++)
       {
              fread(decInfo-> image_data, 8, 1, fptr_src_image);
              decode_bit_from_lsb(decInfo-> image_data);
              decInfo-> secret_file_size = decode_binary_to_equivalent(decInfo-> secret_file_size, decInfo-> image_data) + decInfo-> secret_file_size;
       }
       return e_success;
}

Status decode_secret_file_contents(FILE *fptr_src_image, int size, FILE *fptr_decode_secret, DecodeInfo *decInfo)
{
       int i;
       char ch;
              printf("fptr at file_contents = %ld\n", ftell(fptr_src_image));
       for(i = 0 ; i < size ; i++)
       {
              fread(decInfo-> image_data, 8, 1, fptr_src_image);
              decode_bit_from_lsb(decInfo-> image_data);
              ch = decode_binary_to_equivalent(ch, decInfo-> image_data);
              fwrite(&ch, 1, 1, fptr_decode_secret);
       }
       return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
       printf("Started Decoding\n");
       //4. Open the image file
       if(open_image_file(decInfo) == e_success)
       {
              printf("Opening file is a success\n");
              //5. Decode the magic string
              if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
              {
                     printf("Magic string decoded successfully\n");
                     //6. Decode secret file extension size
                     if(decode_extn_size(decInfo-> fptr_src_image, sizeof(int), decInfo) == e_success)
                     {
                            printf("Deocded secret file size successfully\n");
                            //7. Deocode secret file extension
                            if(decode_secret_file_extn(decInfo-> extn_secret_file, decInfo-> fptr_src_image, decInfo-> extn_size, decInfo) == e_success)
                            {
                                   printf("Decoded secret file extension successfully\n");
                                   //8. Open secret file
                                   if(open_secret_file(decInfo) == e_success)
                                   {
                                          printf("Creating secret file successfully\n");
                                          //9. Decode secret file size
                                          if(decode_secret_file_size(decInfo-> fptr_src_image, sizeof(int), decInfo) == e_success)
                                          {
                                                 printf("Decoded secret file size successfully\n");
                                                 //10. Decode secret file contents
                                                 if(decode_secret_file_contents(decInfo-> fptr_src_image, decInfo-> secret_file_size, decInfo-> fptr_decode_secret, decInfo) == e_success)
                                                 {
                                                        printf("Decoded secret file contents successfully\n");
                                                 }
                                                 else
                                                 {
                                                        printf("Decoding secret file contents failed\n");
                                                        e_failure;
                                                 }
                                          }
                                          else
                                          {
                                                 printf("Decoding secret file size failed\n");
                                                 return e_failure;
                                          }
                                   }
                                   else
                                   {
                                          printf("Opening secret file failed\n");
                                          return e_failure;
                                   }
                            }
                            else
                            {
                                   printf("Decoding secret file extension failed\n");
                                   return e_failure;
                            }
                     }
                     else
                     {
                            printf("Deocding secret file size failed\n");
                            return e_failure;
                     }
              }
              else
              {
                     printf("Decoding Magic string failed\n");
                     return e_failure;
              }
       }
       else
       {
              return e_failure;
       }
       return e_success;
}
