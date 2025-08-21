#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

#include "lodepng.h"

#include "main.h"

#define debug 0

int main(int argc, char *argv[]) {


  //syntax e: ./stego e file_to_encode.png text_to_encode.txt
  //syntax d: ./stego d file_to_decode.png unencoded_reference_png

  const char* operation = argc > 1 ? argv[1] : "e";

  if (!strcmp(operation, "e")) {

    const char* image_filename = argc > 2 ? argv[2] : "test_files/test.png";
    const char* text_filename = argc > 3 ? argv[3] : "test_files/test.txt";
    
    img_data_t *initial_image_data = decodeTwoSteps(image_filename);
    txt_data_t *text_data = get_text_data(text_filename, initial_image_data->data_size);

    if (initial_image_data == NULL || text_data == NULL) {
        printf("Error processing image or text data.\n");
        free(initial_image_data);
        free(text_data->data);
        free(text_data);
        return 1;
    }

    hide_text_in_image(initial_image_data, text_data);

    encodeTwoSteps("test_files/encoded_image.png", (unsigned char *)initial_image_data->data, initial_image_data->width, initial_image_data->height);
    
    free(initial_image_data);
    free(text_data->data);
    free(text_data);
  return 0;
  } else if (!strcmp(operation, "d")) {

    const char* encoded_image_filename = argc > 2 ? argv[2] : "test_files/encoded_image.png";
    const char* ref_image_filename = argc > 3 ? argv[3] : "test_files/test.png";

    //encoder.c houses the image decoder, is reused for decoding
    img_data_t *encoded_image_data = decodeTwoSteps(encoded_image_filename); 
    img_data_t *reference_image_data = decodeTwoSteps(ref_image_filename); 
    txt_data_t *text_data = decode_image(reference_image_data, encoded_image_data);

    if (debug) {
      int diff = *(int *)encoded_image_data->data ^ *(int *)reference_image_data->data;
      printf("\nDifference detected (yes if not 0): %d\n", diff);
    }

    FILE *fp = fopen("test_files/decoded_text.txt", "w");
    if (fp == NULL) {
            printf("Error opening the file for parsing decoded text");
            free(encoded_image_data->data);     //I'm really just doing this cause I'm a noob and should remember never to forget.
            free(reference_image_data->data);   //doesn't really matter cause the program is about to close anyways
            free(encoded_image_data);
            free(reference_image_data);
            free(text_data->data);
            free(text_data);
            return -1;
      }
    if (debug) {  
    fprintf(fp, "%s", text_data->data);
    }
    fclose(fp);

  } else {
    printf("Invalid operation type: '%s' -> use 'e' for encoding and 'd' for decoding", operation);
    return -1;
  }


  printf("\nDone\n");
  return 0;
}