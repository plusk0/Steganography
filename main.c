#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

#include "lodepng.h"
#include "encoder.h"


int main(int argc, char *argv[]) {

  bool done = false;
  const char* image_filename = argc > 1 ? argv[1] : "test_files/test.png";
  const char* text_filename = argc > 2 ? argv[2] : "test_files/test.txt";

  img_data_t *initial_image_data = decodeTwoSteps(image_filename);
  txt_data_t *text_data = get_text_data(text_filename, initial_image_data->data_size);

    if (initial_image_data == NULL || text_data == NULL || done == true) {
        printf("Error processing image or text data.\n");
        free(initial_image_data);
        free(text_data->data);
        free(text_data);
        return 0;
    }

    hide_text_in_image(initial_image_data, text_data);

    encodeTwoSteps("encoded_image.png", (unsigned char *)initial_image_data->data, initial_image_data->width, initial_image_data->height);
    
    free(initial_image_data);
    free(text_data->data);
    free(text_data);


  return 0;
}