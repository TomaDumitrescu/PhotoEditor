// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// copies the image in a dest file in text or binary format
void save(image *img, int magic_number)
{
	char P = 'P', *params = malloc(STRING_MAX);
	DIE(!params, MALLOC_ERROR);
	char *file = malloc(STRING_MAX);
	DIE(!file, MALLOC_ERROR);
	fgets(params, STRING_MAX, stdin);

	if (magic_number == -1) {
		fprintf(stdout, "No image loaded\n");
		free(params), free(file);
		return;
	}

	// Reading the whole line to detect the parameters
	if (strcmp(params, "\n") == 0) {
		fprintf(stdout, "Invalid command\n");
		free(params), free(file);
		return;
	}
	params[strlen(params) - 1] = '\0';

	char *token = strtok(params, " ");
	strcpy(file, token);
	token = strtok(NULL, " ");
	if (token) {
		FILE *out = fopen(file, "wt");
		DIE(!out, FILE_ERROR);
		if (magic_number == 2 || magic_number == 5)
			fprintf(out, "P2\n");
		else
			fprintf(out, "P3\n");
		fprintf(out, "%d %d\n255\n", img->width, img->height);

		if (magic_number == 2 || magic_number == 5)
			for (int i = 0; i < img->height; i++) {
				for (int j = 0; j < img->width; j++)
					fprintf(out, "%hhu ", img->gray_mat[i][j]);
				fprintf(out, "\n");
			}
		else
			for (int i = 0; i < img->height; i++) {
				for (int j = 0; j < img->width; j++) {
					fprintf(out, "%hhu ", img->px_mat[i][j].r);
					fprintf(out, "%hhu ", img->px_mat[i][j].g);
					fprintf(out, "%hhu ", img->px_mat[i][j].b);
				}
				fprintf(out, "\n");
			}
		fclose(out);
	} else {
		// opening the out_file in binary format
		FILE *out = fopen(file, "wb");
		DIE(!out, FILE_ERROR);
		unsigned char buff_magic, max_val = 255;
		if (magic_number == 2 || magic_number == 5)
			buff_magic = 5;
		else
			buff_magic = 6;
		fprintf(out, "%c%hhu\n", P, buff_magic);
		fprintf(out, "%d %d\n%hhu\n", img->width, img->height, max_val);

		if (magic_number == 2 || magic_number == 5)
			for (int i = 0; i < img->height; i++)
				for (int j = 0; j < img->width; j++)
					fwrite(&img->gray_mat[i][j], sizeof(unsigned char), 1, out);
		else
			for (int i = 0; i < img->height; i++)
				for (int j = 0; j < img->width; j++) {
					fwrite(&img->px_mat[i][j].r, sizeof(unsigned char), 1, out);
					fwrite(&img->px_mat[i][j].g, sizeof(unsigned char), 1, out);
					fwrite(&img->px_mat[i][j].b, sizeof(unsigned char), 1, out);
				}
		fclose(out);
	}

	fprintf(stdout, "Saved %s\n", file);
	free(file), free(params);
}
