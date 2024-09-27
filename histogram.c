// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// visualization of pixel values frequencies
void histogram(image *img, int magic_number)
{
	char *cmd_line = malloc(STRING_MAX);
	DIE(!cmd_line, MALLOC_ERROR);
	fgets(cmd_line, STRING_MAX, stdin);

	if (magic_number == -1) {
		fprintf(stdout, "No image loaded\n");
		free(cmd_line);
		return;
	}
	if (strcmp(cmd_line, "\n") == 0) {
		fprintf(stdout, "Invalid command\n");
		free(cmd_line);
		return;
	}

	cmd_line[strlen(cmd_line) - 1] = '\0';
	if (magic_number == 3 || magic_number == 6) {
		fprintf(stdout, "Black and white image needed\n");
		free(cmd_line);
		return;
	}

	int x, y;
	char *params = strtok(cmd_line, " ");
	if (!params) {
		fprintf(stdout, "Invalid command\n");
		free(cmd_line);
		return;
	}
	x = atoi(params);
	params = strtok(NULL, " ");
	if (!params) {
		fprintf(stdout, "Invalid command\n");
		free(cmd_line);
		return;
	}
	y = atoi(params);
	params = strtok(NULL, " ");
	if (params) {
		fprintf(stdout, "Invalid command\n");
		free(cmd_line);
		return;
	}
	free(cmd_line);

	int *frequency = calloc(MAX_VAL, sizeof(int));
	DIE(!frequency, CALLOC_ERROR);
	int *bins = calloc(y, sizeof(int));
	DIE(!bins, CALLOC_ERROR);
	for (int i = 0; i < img->height; i++)
		for (int j = 0; j < img->width; j++)
			frequency[img->gray_mat[i][j]]++;

	/* We split [0, 255] in intervals of length l which can be obtained
	   using the number of bins, then we add up all frequencies of pixels
	   from all intervals and create the bin_frequencies. We change the bin
	   to the next one when i + 1 is divisible by l, because of 0-indexation */
	int l = (MAX_VAL + 1) / y, idx = 0, stars, f_max = -1;
	for (int i = 0; i <= MAX_VAL; i++) {
		bins[idx] += frequency[i];
		if ((i + 1) % l == 0)
			idx++;
	}

	// maximum determining algorithm
	for (int i = 0; i < y; i++)
		if (bins[i] > f_max)
			f_max = bins[i];

	free(frequency);
	for (int i = 0; i < y; i++) {
		stars = round((bins[i] * x) / f_max);
		fprintf(stdout, "%d\t|\t", stars);
		for (int i = 0; i < stars; i++)
			fprintf(stdout, "*");
		fprintf(stdout, "\n");
	}
	free(bins);
}
