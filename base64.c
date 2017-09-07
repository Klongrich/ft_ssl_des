#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

char        *base64_encode(const unsigned char *src, size_t input_length) {

    unsigned char       *encoded_data;
    size_t              output_length;
    int                 i;
    int                 j;

    output_length = 4 * ((input_length + 2) / 3);
    encoded_data = (unsigned char *)malloc(sizeof(unsigned char) * output_length);
    if (encoded_data == NULL) 
        return NULL;
    i = 0;
    j = 0;
    while (i < input_length)
    {
        uint32_t octet_a = i < input_length ? (unsigned char)src[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)src[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)src[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    i = 0;
    while (i++ < mod_table[input_length % 3])
        encoded_data[output_length - 1 - i] = '=';

    return encoded_data;
}

void        build_decoding_table() {
    int i;

    decoding_table = malloc(256);
    i = 0;
    while (i++ < 64)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}
    
    
void        base64_cleanup() {
     free(decoding_table);
}


unsigned char *base64_decode(const char *src, size_t input_length) {

    unsigned char   *decoded_data;
    size_t          output_length;
    int             i;
    int             j;

    if (decoding_table == NULL) 
        build_decoding_table();

    if (input_length % 4 != 0) 
        return NULL;

    output_length = input_length / 4 * 3;

    if (src[input_length - 1] == '=') 
        output_length--;
    if (src[input_length - 2] == '=') 
        output_length--;

    decoded_data = (unsigned char *)malloc(sizeof(unsigned char) * output_length);
    if (decoded_data == NULL) 
        return NULL;

    i = 0;
    j = 0;
    while (i < input_length)
    {
        uint32_t sextet_a = src[i] == '=' ? 0 & i++ : decoding_table[src[i++]];
        uint32_t sextet_b = src[i] == '=' ? 0 & i++ : decoding_table[src[i++]];
        uint32_t sextet_c = src[i] == '=' ? 0 & i++ : decoding_table[src[i++]];
        uint32_t sextet_d = src[i] == '=' ? 0 & i++ : decoding_table[src[i++]];

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    base64_cleanup();
    return decoded_data;
}


int     main()
{
    char *name = "ft_base64";
    char *encoded = base64_encode(name, strlen(name));
    char *decoded = base64_decode(encoded, strlen(encoded));

    printf("Name: %s\nEncoded: %s\nDecoded: %s\n", name, encoded, decoded);

}