// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// constraint for x to be in [0, 255] interval
int clamp(int x)
{
	if (x >= 0 && x <= MAX_VAL)
		return x;
	if (x < 0)
		return 0;
	return MAX_VAL;
}

// enhances the contrast of the image, using equalization
void equalize(image *img, int magic_number)
{
	if (magic_number == -1) {
		fprintf(stdout, "No image loaded\n");
		return;
	}

	if (magic_number == 3 || magic_number == 6) {
		fprintf(stdout, "Black and white image needed\n");
		return;
	}

	int area = img->width * img->height;
	int *frequency = calloc(MAX_VAL + 1, sizeof(int));
	DIE(!frequency, CALLOC_ERROR);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			frequency[img->gray_mat[i][j]]++;

	// partial sum array to reduce algorithm complexity
	for (int i = 1; i <= MAX_VAL; i++)
		frequency[i] += frequency[i - 1];

	double eq_result = 0;
	int new_pixel;
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			// equalize formula for new pixels values
			eq_result = (1.0 * frequency[img->gray_mat[i][j]] / area) * MAX_VAL;
			new_pixel = clamp(round(eq_result));
			img->gray_mat[i][j] = new_pixel;
		}
	}

	fprintf(stdout, "Equalize done\n");
	free(frequency);
}
