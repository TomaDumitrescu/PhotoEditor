// Copyright 2023 <Dumitrescu Toma-Ioan>

#include "skel.h"

// removes the last image if exists, using magic number as ref
void deallocate(image *img, int magic_number)
{
	if ((magic_number == 2 || magic_number == 5) && img->gray_mat) {
		for (int i = 0; i < img->height; i++)
			free(img->gray_mat[i]);
		free(img->gray_mat);
	} else if (magic_number != -1) {
		for (int i = 0; i < img->height; i++)
			free(img->px_mat[i]);
		free(img->px_mat);
	}
	free(img);
}
