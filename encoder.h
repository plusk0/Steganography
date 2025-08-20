#pragma once

typedef struct img_data {
  char *data;
  int data_size;
  int width;
  int height;
} img_data_t;

typedef struct txt_data {
  char *data;
  int data_size;
} txt_data_t;


img_data_t *decodeTwoSteps(const char* filename);
txt_data_t *get_text_data(const char *filename, int img_size);
int hide_text_in_image(img_data_t *img_data, txt_data_t *text_data);
void encodeTwoSteps(const char* filename, const unsigned char* image, unsigned width, unsigned height);
