#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

#include "lodepng.h"

#include "main.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

#define debug 0



img_data_t *decodeTwoSteps(const char* filename) {
  unsigned error;
  unsigned char *image = 0;
  unsigned width, height;
  unsigned char *png = 0;
  size_t pngsize;

  error = lodepng_load_file(&png, &pngsize, filename);
  if(!error) error = lodepng_decode32(&image, &width, &height, png, pngsize);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  free(png);

  img_data_t *img_data = (img_data_t *)malloc(sizeof(img_data_t));
    if (img_data == NULL) {
    printf("Error allocating memory for img_data\n");
    free(image);
    return NULL;
  }

  img_data->data = image;
  img_data->data_size = width * height;
  img_data->width = width;
  img_data->height = height;

    if (debug) {
    for (int i = 0; i < 10 && i < img_data->data_size; ++i) {
        unsigned char *pixel = (unsigned char *)&img_data->data[i * 4];
        printf("Pixel %d: R=%u G=%u B=%u A=%u\n",
            i,
            pixel[0],
            pixel[1],
            pixel[2],
            pixel[3]);
    }

    printf("Decoded image name: %s\n Pixels processed: %u\n", filename, width * height);
  }

  return img_data;
  // Note: Caller is responsible for freeing the image memory
}



int pixel_changer(char *pixel, int hex) {
  if (pixel == NULL) {
    printf("Error: pixel pointer is NULL\n");
    return -1;
  }

  if (*pixel < 3) {hex = -hex;}
 
  switch (hex) {
    case 0:
      *pixel -= 0;
      break;

    case 1: 

      if (debug) {printf("Changing pixel %u\n", *pixel);}
      *pixel -= 1; 
      if (debug) {printf("Changed pixel %u\n", *pixel);} 
      break;

    case 2: 
      if (debug) {printf("Changing pixel %u\n", *pixel);}
      *pixel -= 2; 
      if (debug) {printf("Changed pixel %u\n", *pixel);} 
      break;

    case 3:
      if (debug) {printf("Changing pixel %u\n", *pixel);}
      *pixel -= 3; 
      if (debug) {printf("Changed pixel %u\n", *pixel);} 
      break;

    default:
      printf("Error: Invalid value for pixel change\n");
      return -1;
  }
  return 0;
}

txt_data_t *get_text_data(const char *filename, int img_size) {
  FILE* fp = 0;
  txt_data_t *text_data = (txt_data_t *)malloc(sizeof(txt_data_t));
    if (text_data == NULL) {
        printf("Error allocating memory for text_data\n");
        return NULL;
    }

  if ( filename == NULL ) {
    printf("Error- No filename provided for text data\n");
    free(text_data);
    return NULL;
  }

  if( ( fp = fopen( filename, "r" ) ) == NULL ) {
		printf("Error- Unable to open %s\n", filename );
        free(text_data);
		exit( -1 );
	} 

  text_data->data = (char *)malloc(strlen(text_data->data) + 1);
  fgets(text_data->data, img_size, fp);

  fclose(fp);

  text_data->data_size = strlen(text_data->data);
  if (debug) {printf("Decoded text: %s\n", text_data->data);}

  return text_data;
    // Note: Caller is responsible for freeing the text_data memory
}

int hide_text_in_image(img_data_t *img_data, txt_data_t *text_data) {
  if (img_data == NULL || text_data == NULL) {
    printf("Error: img_data or text_data is NULL\n");
    return 0;
  }

  unsigned char *image = (unsigned char *)img_data->data;
  int img_size = img_data->data_size;
  int text_size = text_data->data_size;

  if (text_size > img_size) {
    printf("Error: Text data is too large to fit in the image\n");
    return 0;
  }

  for (int i = 0; i < text_size; i++) {
    if (debug) {printf("Found Text Bytes:"BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(text_data->data[i]));}

    unsigned char byte = text_data->data[i];
    unsigned char part1 = (byte >> 6) & 0x03; // bits 7-6
    unsigned char part2 = (byte >> 4) & 0x03; // bits 5-4
    unsigned char part3 = (byte >> 2) & 0x03; // bits 3-2
    unsigned char part4 = byte & 0x03; 

    if (debug) {printf("Parts: %u %u %u %u\n", part1, part2, part3, part4);}

    image[i * 4 + 0] = pixel_changer(&image[i * 4 + 0], part1);
    image[i * 4 + 1] = pixel_changer(&image[i * 4 + 1], part2);
    image[i * 4 + 2] = pixel_changer(&image[i * 4 + 2], part3);
    image[i * 4 + 3] = pixel_changer(&image[i * 4 + 3], part4);

    if (debug) {
      printf("Stored image data after hiding text:\n");
      for (int j = 0; j < 10 && j < img_size; ++j) {
        printf("Pixel %d: R=%u G=%u B=%u A=%u\n",
            j,
            image[j * 4 + 0],
            image[j * 4 + 1],
            image[j * 4 + 2],
            image[j * 4 + 3]);
      }
    }
  }


  return 0;
}


void encodeTwoSteps(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
  unsigned char* png;
  size_t pngsize;

  unsigned error = lodepng_encode32(&png, &pngsize, image, width, height);
  if(!error) lodepng_save_file(png, pngsize, filename);

  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  free(png);
}



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

  return 0;
}