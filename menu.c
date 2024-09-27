// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

int main(void)
{
	char *cmd = malloc(STRING_MAX);
	DIE(!cmd, MALLOC_ERROR);
	image *img = malloc(sizeof(image));
	// magic_number retains the image type
	int magic_number = -1, x1, x2, y1, y2;
	bool loop = true;

	// do...while menu calling functions from "skel.h"
	while (loop) {
		fscanf(stdin, "%s", cmd);

		if (strcmp(cmd, "LOAD") == 0) {
			load_image(img, &magic_number, &x1, &x2, &y1, &y2);
		} else if (strcmp(cmd, "SELECT") == 0) {
			select_types(img, magic_number, &x1, &x2, &y1, &y2);
		} else if (strcmp(cmd, "EQUALIZE") == 0) {
			equalize(img, magic_number);
		} else if (strcmp(cmd, "CROP") == 0) {
			crop(img, magic_number, &x1, &x2, &y1, &y2);
		} else if (strcmp(cmd, "APPLY") == 0) {
			apply(img, magic_number, x1, x2, y1, y2);
		} else if (strcmp(cmd, "SAVE") == 0) {
			save(img, magic_number);
		} else if (strcmp(cmd, "HISTOGRAM") == 0) {
			histogram(img, magic_number);
		} else if (strcmp(cmd, "EXIT") == 0) {
			deallocate(img, magic_number);
			loop = false;
			if (magic_number == -1)
				printf("No image loaded\n");
		} else {
			fgets(cmd, STRING_MAX, stdin);
			fprintf(stdout, "Invalid command\n");
		}
	}

	free(cmd);
	return 0;
}
