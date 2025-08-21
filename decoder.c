#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

#include "lodepng.h"
#include "decoder.h"

#include "main.h"

txt_data_t *decode_image(img_data_t *initial_image_data, img_data_t *encoded_image_data) {

    unsigned char *image = (unsigned char *)initial_image_data->data;
    unsigned char *enc_image = (unsigned char *)encoded_image_data->data;
if (initial_image_data->data_size != encoded_image_data->data_size) {
    printf("Error: pictures have different Data size");

}
    txt_data_t *decoded_text = malloc(sizeof(txt_data_t));
if (decoded_text == NULL) {
    printf("Allocating text memory failed");
    exit( -1 );
}

    unsigned char *text = malloc(initial_image_data->data_size + 1);
    printf("Expecting max. %u text\n", initial_image_data->data_size);
    

for (int i = 0; i < initial_image_data->data_size; ++i) {
    unsigned char diff1 = image[i * 4 + 0] - enc_image[i * 4 + 0];
    unsigned char diff2 = image[i * 4 + 1] - enc_image[i * 4 + 1];
    unsigned char diff3 = image[i * 4 + 2] - enc_image[i * 4 + 2];
    unsigned char diff4 = image[i * 4 + 3] - enc_image[i * 4 + 3];

    unsigned char diffbyte = (diff1 << 6) ^ (diff2 << 4) ^ (diff3 << 2) ^ diff4;

    //text[i]
    text[i] = diffbyte;
    printf("%d ", diffbyte);
}
    decoded_text->data = text;
    decoded_text->data_size = strlen(text);

    printf("\nDecoded text: '%s' | With length: '%u'\n", decoded_text->data, decoded_text->data_size);
}