// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// matrix in heap space
double **alloc_matrix(int r, int c)
{
	double **mat = calloc(r, sizeof(double *));
	DIE(!mat, CALLOC_ERROR);
	for (int i = 0; i < r; i++) {
		mat[i] = calloc(c, sizeof(double));
		DIE(!mat[i], CALLOC_ERROR);
	}

	return mat;
}

// delete the matrix from the heap
void free_matrix(double **mat, int size)
{
	for (int i = 0; i < size; i++)
		free(mat[i]);

	free(mat);
}

// performs the arithmetical operations on pixels using ker matrix
void change_pixels(image *img, int xs, int xf, int ys, int yf, double **ker,
				   const double coeff)
{
	int dy[] = {-1, 0, 1}, dx[] = {-1, 0, 1}, xv, yv;
	double sum = 0;

	// more code, but less memory allocation
	double **img_mat = alloc_matrix(img->height + 1, img->width + 1);

	// algorithm should be applied for all color channels
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			img_mat[i][j] = 1.0 * img->px_mat[i][j].r;

	for (int i = ys; i < yf; i++) {
		for (int j = xs; j < xf; j++) {
			sum = 0;
			// convolution, not being confused with matrices product
			for (int iv = 0; iv < IMG_KER; iv++)
				for (int jv = 0; jv < IMG_KER; jv++) {
					yv = i + dy[iv];
					xv = j + dx[jv];
					sum += img->px_mat[yv][xv].r * ker[iv][jv];
				}
			img_mat[i][j] = sum * coeff;
		}
	}

	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			img->px_mat[i][j].r = clamp(round(img_mat[i][j]));

	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			img_mat[i][j] = 1.0 * img->px_mat[i][j].g;

	for (int i = ys; i < yf; i++) {
		for (int j = xs; j < xf; j++) {
			sum = 0;
			for (int iv = 0; iv < 3; iv++)
				for (int jv = 0; jv < 3; jv++) {
					yv = i + dy[iv];
					xv = j + dx[jv];
					sum += img->px_mat[yv][xv].g * ker[iv][jv];
				}
			img_mat[i][j] = sum * coeff;
		}
	}

	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			img->px_mat[i][j].g = clamp(round(img_mat[i][j]));

	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			img_mat[i][j] = 1.0 * img->px_mat[i][j].b;

	for (int i = ys; i < yf; i++) {
		for (int j = xs; j < xf; j++) {
			sum = 0;
			for (int iv = 0; iv < 3; iv++)
				for (int jv = 0; jv < 3; jv++) {
					yv = i + dy[iv];
					xv = j + dx[jv];
					sum += img->px_mat[yv][xv].b * ker[iv][jv];
				}
			img_mat[i][j] = sum * coeff;
		}
	}

	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			img->px_mat[i][j].b = clamp(round(img_mat[i][j]));

	free_matrix(img_mat, img->height + 1);
}

// defines kernel for edge
void apply_edge(image *img, int xs, int xf, int ys, int yf)
{
	double **edge_ker = alloc_matrix(IMG_KER, IMG_KER);

	for (int i = 0; i < IMG_KER; i++)
		edge_ker[0][i] = -1, edge_ker[2][i] = -1;
	edge_ker[1][0] = -1, edge_ker[1][2] = -1, edge_ker[1][1] = 8;

	change_pixels(img, xs, xf, ys, yf, edge_ker, 1);

	free_matrix(edge_ker, IMG_KER);
}

// defines kernel for blur
void apply_blur(image *img, int xs, int xf, int ys, int yf)
{
	const double blur_coeff = 1.0 / 9;
	double **blur_ker = alloc_matrix(IMG_KER, IMG_KER);
	for (int i = 0; i < IMG_KER; i++)
		for (int j = 0; j < IMG_KER; j++)
			blur_ker[i][j] = 1;

	change_pixels(img, xs, xf, ys, yf, blur_ker, blur_coeff);

	free_matrix(blur_ker, IMG_KER);
}

// defines kernel for sharpen
void apply_sharpen(image *img, int xs, int xf, int ys, int yf)
{
	double **sharp_ker = alloc_matrix(IMG_KER, IMG_KER);
	for (int i = 0; i < IMG_KER; i++) {
		sharp_ker[0][i] = 0, sharp_ker[2][i] = 0;
		if (i % 2)
			sharp_ker[0][i] = -1, sharp_ker[2][i] = -1;
	}
	sharp_ker[1][0] = -1, sharp_ker[1][2] = -1, sharp_ker[1][1] = 5;

	change_pixels(img, xs, xf, ys, yf, sharp_ker, 1);

	free_matrix(sharp_ker, IMG_KER);
}

// defines kernel for gaussian blur
void apply_gaussian_blur(image *img, int xs, int xf, int ys, int yf)
{
	const double gaussian_blur_coeff = 1.0 / 16;
	double **gauss_ker = alloc_matrix(IMG_KER, IMG_KER);
	for (int i = 0; i < IMG_KER; i++) {
		gauss_ker[0][i] = 1, gauss_ker[2][i] = 1;
		if (i % 2)
			gauss_ker[0][i] = 2, gauss_ker[2][i] = 2;
	}
	gauss_ker[1][0] = 2, gauss_ker[1][2] = 2, gauss_ker[1][1] = 4;

	change_pixels(img, xs, xf, ys, yf, gauss_ker, gaussian_blur_coeff);

	free_matrix(gauss_ker, IMG_KER);
}

// main apply where the logic is centered
void apply(image *img, int magic_number, int x1, int x2, int y1, int y2)
{
	char *cmd_line = malloc(STRING_MAX);
	DIE(!cmd_line, MALLOC_ERROR);
	fgets(cmd_line, STRING_MAX, stdin);

	if (magic_number == -1) {
		printf("No image loaded\n");
		free(cmd_line);
		return;
	}

	if (strcmp(cmd_line, "\n") == 0) {
		fprintf(stdout, "Invalid command\n");
		free(cmd_line);
		return;
	}

	if (magic_number == 2 || magic_number == 5) {
		printf("Easy, Charlie Chaplin\n");
		free(cmd_line);
		return;
	}

	size_t len = strlen(cmd_line);
	cmd_line[len - 1] = '\0';

	char *verify_type = malloc(STRING_MAX);
	DIE(!verify_type, MALLOC_ERROR);
	strcpy(verify_type, cmd_line + 1);
	free(cmd_line);
	int xs = x1, xf = x2, ys = y1, yf = y2;

	/* border pixels will remain unchanges this way
	   another solution is to use the same x1, x2, y1, y2 */
	if (x1 == 0)
		xs = 1;

	if (x2 == img->width)
		xf = img->width - 1;

	if (y1 == 0)
		ys = 1;

	if (y2 == img->height)
		yf = img->height - 1;

	if (strcmp(verify_type, "EDGE") == 0) {
		apply_edge(img, xs, xf, ys, yf);
	} else if (strcmp(verify_type, "SHARPEN") == 0) {
		apply_sharpen(img, xs, xf, ys, yf);
	} else if (strcmp(verify_type, "BLUR") == 0) {
		apply_blur(img, xs, xf, ys, yf);
	} else if (strcmp(verify_type, "GAUSSIAN_BLUR") == 0) {
		apply_gaussian_blur(img, xs, xf, ys, yf);
	} else {
		fprintf(stdout, "APPLY parameter invalid\n");
		free(verify_type);
		return;
	}

	fprintf(stdout, "APPLY %s done\n", verify_type);
	free(verify_type);
}
