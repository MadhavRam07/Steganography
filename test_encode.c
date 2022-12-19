/*
Name:Madhuri Yogi
Date:31-10-2022
Description:Steganography Project
Sample Execution:
1.For Encoding: ./a.out -e secret.txt beautiful.bmp

Selected encoding
Read and validate is success
Stared Encoding
Open file is a success
width = 1024
height = 768
Check capacity is a success
Copied bmp header succesfully
Magic string encoded successfully
Encoded secret file extension size
encoded secret file extension successfully
Encoded secret file size successfully
Encoded secret file data successfully
Copied remaining bytes successfully
Encoding is successfull
 
2.For Decoding: ./a.out -d stego.bmp
Selected decoding
Read and validate decode arguments is a success
Started Decoding
Opening file is a success
fptr = 54
fptr at # = 62
fptr at * = 70
Magic string decoded successfully
fptr at extn_size = 70
Deocded secret file size successfully
fptr at extn = 102
Decoded secret file extension successfully
Creating secret file successfully
fptr at file_size = 134
Decoded secret file size successfully
fptr at file_contents = 166
Decoded secret file contents successfully
Decoding is successfull
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include "common.h"

int main(int argc,char *argv[])
{
    //1.validate the CLA

    //2.check the operation type

    if(check_operation_type(argv) == e_encode)
    {
        printf("Selected encoding\n");
        //3.Read and validate the CLA
        EncodeInfo encInfo;
        if(read_and_validate_encode_args(argv,&encInfo) == e_success)
        {
            printf("Read and validate is success\n");
            //4.Started Encoding
            if(do_encoding(&encInfo) == e_success)
            {
                printf("Encoding is successfull\n");
            }
            else
            {
                printf("Encoding failed\n");
            }
        }
        else
        {
            printf("Read and validate encode failed\nPass ./a.out -e secret.txt beautiful.bmp\n");
            return 1;
        }

    }
    else if(check_operation_type(argv) == e_decode)
    {
        printf("Selected decoding\n");
        DecodeInfo decInfo;
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("Read and validate decode arguments is a success\n");
            //5.Started Decoding
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding is successfull\n");
            }
            else
            {
                printf("Decoding failed\n");
                return -1;
            }
        }
        else
        {
            printf("Read and validate decode failed\nPass ./a.out -d stego.bmp\n");
            return -1;
        }
    }
    else
    {
        printf("Invalid input\nEncoding:./a.out -e secret.txt beautiful.bmp\nDecoding:./a.out -d stego.bmp\n");
        return 0;
    }
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
        return e_encode;
    if(strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        return e_unsupported;
}

