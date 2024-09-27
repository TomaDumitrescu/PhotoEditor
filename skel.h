// Copyright 2023 <Dumitrescu Toma-Ioan>

#ifndef SKEL_H
#define SKEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <errno.h>

// symbols
#define MALLOC_ERROR "Malloc error!\n"
#define CALLOC_ERROR "Calloc error!\n"
#define REALLOC_ERROR "Realloc error!\n"
#define FILE_ERROR "File opening error!\n"
#define MAX_VAL 255
#define STRING_MAX 50
#define IMG_KER 3

// macro for defensive programming
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

// structures
typedef struct pixel pixel;
struct pixel {
	unsigned char r, g, b;
};

typedef struct image image;
struct image {
	pixel **px_mat;
	unsigned char **gray_mat;
	int width, height;
};

// functions headers
void load_image(image *img, int *magic_number, int *x1, int *x2, int *y1,
				int *y2);
void select_types(image *img, int magic_number, int *x1, int *x2, int *y1,
				  int *y2);
void histogram(image *img, int magic_number);
void equalize(image *img, int magic_number);
void crop(image *img, int magic_number, int *x1, int *x2, int *y1, int *y2);
void apply(image *img, int magic_number, int x1, int x2, int y1, int y2);
void save(image *img, int magic_number);
void deallocate(image *img, int magic_number);
int clamp(int x);

#endif
