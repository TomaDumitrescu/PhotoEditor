// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// changes the image matrix, according to a given surface
void crop(image *img, int magic_number, int *x1, int *x2, int *y1, int *y2)
{
	if (magic_number == -1) {
		fprintf(stdout, "No image loaded\n");
		return;
	}

	if (magic_number == 2 || magic_number == 5) {
		// y2 - y1 rows and x2 - x1 columns
		unsigned char **new_mat = malloc((*y2 - *y1) * sizeof(unsigned char *));
		DIE(!new_mat, MALLOC_ERROR);
		for (int i = 0; i < *y2 - *y1; i++) {
			new_mat[i] = malloc((*x2 - *x1) * sizeof(unsigned char));
			DIE(!new_mat[i], MALLOC_ERROR);
		}
		for (int i = *y1; i < *y2; i++)
			for (int j = *x1; j < *x2; j++)
				new_mat[i - *y1][j - *x1] = img->gray_mat[i][j];

		// pointer swap trick
		unsigned char **trash = img->gray_mat;
		img->gray_mat = new_mat;
		for (int i = 0; i < img->height; i++)
			free(trash[i]);
		free(trash);

		// updating selections and image dimensions
		img->width = *x2 - *x1;
		img->height = *y2 - *y1;
		*x1 = 0, *x2 = img->width, *y1 = 0, *y2 = img->height;
	} else if (magic_number == 3 || magic_number == 6) {
		pixel **new_mat = malloc((*y2 - *y1) * sizeof(pixel *));
		DIE(!new_mat, MALLOC_ERROR);
		for (int i = 0; i < *y2 - *y1; i++) {
			new_mat[i] = malloc((*x2 - *x1) * sizeof(pixel));
			DIE(!new_mat[i], MALLOC_ERROR);
		}
		for (int i = *y1; i < *y2; i++)
			for (int j = *x1; j < *x2; j++) {
				// new_mat starts from (0, 0), not from (x1, y1)
				new_mat[i - *y1][j - *x1].r = img->px_mat[i][j].r;
				new_mat[i - *y1][j - *x1].g = img->px_mat[i][j].g;
				new_mat[i - *y1][j - *x1].b = img->px_mat[i][j].b;
			}

		pixel **trash = img->px_mat;
		img->px_mat = new_mat;
		for (int i = 0; i < img->height; i++)
			free(trash[i]);
		free(trash);

		img->width = *x2 - *x1;
		img->height = *y2 - *y1;
		*x1 = 0, *x2 = img->width, *y1 = 0, *y2 = img->height;
	}

	fprintf(stdout, "Image cropped\n");
}
