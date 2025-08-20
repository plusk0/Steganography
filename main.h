#pragma once

struct rgba_color {
  int r;
  int g;
  int b;
  int a;
};

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
