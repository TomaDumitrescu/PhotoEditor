// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// frees an image from the memory
void clear_image(int *magic_number, image *img)
{
	if (*magic_number == -1)
		return;

	if (*magic_number == 2 || *magic_number == 5) {
		// gray photo
		for (int i = 0; i < img->height; i++)
			free(img->gray_mat[i]);
		free(img->gray_mat);
	} else if (*magic_number == 3 || *magic_number == 6) {
		// rgb photo
		for (int i = 0; i < img->height; i++)
			free(img->px_mat[i]);
		free(img->px_mat);
	}

	*magic_number = -1;
}

// retain the iamge matrix on the heap, according to image type (magic_number)
void alloc_image_matrix(image *img, int *magic_number)
{
	if (*magic_number == 2 || *magic_number == 5) {
		img->gray_mat = malloc(img->height * sizeof(unsigned char *));
		DIE(!img->gray_mat, MALLOC_ERROR);
		for (int i = 0; i < img->height; i++) {
			img->gray_mat[i] = malloc(img->width * sizeof(unsigned char));
			DIE(!img->gray_mat[i], MALLOC_ERROR);
		}
	} else if (*magic_number == 3 || *magic_number == 6) {
		img->px_mat = malloc(img->height * sizeof(pixel *));
		DIE(!img->px_mat, MALLOC_ERROR);
		for (int i = 0; i < img->height; i++) {
			img->px_mat[i] = malloc(img->width * sizeof(pixel));
			DIE(!img->px_mat[i], MALLOC_ERROR);
		}
	}
}

// reads an image matrix in text format
void read_text(image *img, FILE *in, int *magic_number)
{
	if (*magic_number == 2) {
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				fscanf(in, "%hhu", &img->gray_mat[i][j]);
	} else if (*magic_number == 3) {
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++) {
				fscanf(in, "%hhu", &img->px_mat[i][j].r);
				fscanf(in, "%hhu", &img->px_mat[i][j].g);
				fscanf(in, "%hhu", &img->px_mat[i][j].b);
			}
	}
}

// reads an image matrix in binary format
void read_binary(image *img, FILE *in_text, int *magic_number, char *file)
{
	// retain the current position
	int file_location = ftell(in_text);
	fclose(in_text);

	// next, we'll read only the matrix
	FILE *in = fopen(file, "rb");
	// we know the file exists since in text format we can open it
	DIE(!in, FILE_ERROR);
	fseek(in, file_location, SEEK_SET);

	if (*magic_number == 5) {
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++)
				fread(&img->gray_mat[i][j], sizeof(unsigned char), 1, in);
	} else if (*magic_number == 6) {
		for (int i = 0; i < img->height; i++)
			for (int j = 0; j < img->width; j++) {
				fread(&img->px_mat[i][j].r, sizeof(unsigned char), 1, in);
				fread(&img->px_mat[i][j].g, sizeof(unsigned char), 1, in);
				fread(&img->px_mat[i][j].b, sizeof(unsigned char), 1, in);
			}
	}
	fclose(in);
}

int check_parameters(char P, int magic_number, int width, int height)
{
	if (P != 'P' || width <= 0 || height <= 0)
		return 0;
	if (magic_number != 2 && magic_number != 3 &&
		magic_number != 5 && magic_number != 6)
		return 0;
	return 1;
}

void ignore_commentaries(FILE *in_text, int *magic_number, image *img,
						 char *trash)
{
	char *buff = malloc(STRING_MAX);
	DIE(!buff, MALLOC_ERROR);
	int max_val, ignore;
	while (fgets(buff, STRING_MAX, in_text))
		if (buff[0] != '#')
			break;

	ignore = sscanf(buff, "%c%d", &(*trash), &(*magic_number));
	DIE(ignore != 2, "Invalid commentaries in image");
	while (fgets(buff, STRING_MAX, in_text))
		if (buff[0] != '#')
			break;

	ignore = sscanf(buff, "%d%d", &img->width, &img->height);
	DIE(ignore != 2, "Invalid commentaries in image");
	while (fgets(buff, STRING_MAX, in_text))
		if (buff[0] != '#')
			break;

	ignore = sscanf(buff, "%d", &max_val);
	DIE(ignore != 1, "Invalid commentaries in image");
	free(buff);
}

// loads an image in the memory
void load_image(image *img, int *magic_number, int *x1, int *x2, int *y1,
				int *y2)
{
	char *cmd_line = malloc(STRING_MAX);
	DIE(!cmd_line, MALLOC_ERROR);

	/* Treating the case where file name does not exist */
	fgets(cmd_line, STRING_MAX, stdin);
	if (strcmp(cmd_line, "\n") == 0) {
		fprintf(stdout, "Invalid command\n");
		free(cmd_line);
		return;
	}

	clear_image(&(*magic_number), img);

	cmd_line[strlen(cmd_line) - 1] = '\0';
	char *file = malloc(STRING_MAX);
	DIE(!file, MALLOC_ERROR);
	// we have 1 space before file_name
	strcpy(file, cmd_line + 1);
	free(cmd_line);

	FILE *in_text = fopen(file, "rt");
	if (!in_text) {
		fprintf(stdout, "Failed to load %s\n", file);
		free(file);
		return;
	}

	char trash;
	ignore_commentaries(in_text, &(*magic_number), img, &trash);

	if (!check_parameters(trash, *magic_number, img->width, img->height)) {
		fprintf(stdout, "Failed to load %s\n", file);
		*magic_number = -1;
		free(file);
		return;
	}

	// by default, the entire image is selected
	*x1 = 0, *x2 = img->width, *y1 = 0, *y2 = img->height;
	if (*magic_number == 2 || *magic_number == 5) {
		alloc_image_matrix(img, magic_number);
		if (*magic_number == 2) {
			read_text(img, in_text, magic_number);
			fclose(in_text);
		} else {
			// the binary and text file are closed in the function
			read_binary(img, in_text, magic_number, file);
		}
	} else if (*magic_number == 3 || *magic_number == 6) {
		alloc_image_matrix(img, magic_number);
		if (*magic_number == 3) {
			read_text(img, in_text, magic_number);
			fclose(in_text);
		} else {
			read_binary(img, in_text, magic_number, file);
		}
	}

	fprintf(stdout, "Loaded %s\n", file);
	free(file);
}
