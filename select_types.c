#include "skel.h"

// classical swap using auxiliary variable
void conditioned_swap_numbers(int *x, int *y)
{
	if (*x <= *y)
		return;
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

// verify if the selected coordinates are in bounds
int check_coordinates(int x1, int x2, int y1, int y2, int lim_w, int lim_h)
{
	if (x1 < 0 || y1 < 0)
		return 0;
	if (x2 > lim_w || y2 > lim_h)
		return 0;
	if (x1 == x2 || y1 == y2)
		return 0;
	return 1;
}

// verifies if param is an integer
bool valid_param(char *params)
{
	if (!params)
		return false;

	size_t len = strlen(params);

	// '0' is valid, so len > 1
	if (len > 1 && params[0] == '0')
		return false;

	for (size_t i = 0; i < len; i++) {
		if (params[i] == '-' && i == 0)
			continue;
		if (params[i] < '0' || params[i] > '9')
			return false;
	}

	return true;
}

// after the function, only [x1, x2) and [y1, y2) pixels will be transformed
void select_types(image *img, int magic_number, int *x1, int *x2, int *y1,
				  int *y2)
{
	// verifying if we have no parameters or less than needed
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

	// fgets adds \n at the final of the string
	size_t len = strlen(cmd_line);
	cmd_line[len - 1] = '\0';
	char *verify_type = malloc(STRING_MAX);
	DIE(!verify_type, MALLOC_ERROR);
	strcpy(verify_type, cmd_line + 1);
	free(cmd_line);

	char *params = strtok(verify_type, " ");
	if (strcmp(params, "ALL") == 0) {
		// open interval for right
		*x1 = 0, *x2 = img->width, *y1 = 0, *y2 = img->height;
		fprintf(stdout, "Selected ALL\n");
		free(verify_type);
		return;
	}

	int cx1 = *x1, cx2 = *x2, cy1 = *y1, cy2 = *y2;
	// transforms the string in an integer, neglecting invalid command
	if (!valid_param(params)) {
		fprintf(stdout, "Invalid command\n");
		free(verify_type);
		return;
	}
	*x1 = atoi(verify_type);
	params = strtok(NULL, " ");
	if (!valid_param(params)) {
		*x1 = cx1;
		fprintf(stdout, "Invalid command\n");
		free(verify_type);
		return;
	}
	*y1 = atoi(params);
	params = strtok(NULL, " ");
	if (!valid_param(params)) {
		*x1 = cx1, *y1 = cy1;
		fprintf(stdout, "Invalid command\n");
		free(verify_type);
		return;
	}
	*x2 = atoi(params);
	params = strtok(NULL, " ");
	if (!valid_param(params)) {
		*x1 = cx1, *y1 = cy1, *x2 = cx2;
		fprintf(stdout, "Invalid command\n");
		free(verify_type);
		return;
	}
	*y2 = atoi(params);

	conditioned_swap_numbers(&(*x1), &(*x2));
	conditioned_swap_numbers(&(*y1), &(*y2));
	if (!check_coordinates(*x1, *x2, *y1, *y2, img->width, img->height)) {
		fprintf(stdout, "Invalid set of coordinates\n");
		*x1 = cx1, *x2 = cx2, *y1 = cy1, *y2 = cy2;
		free(verify_type);
		return;
	}

	fprintf(stdout, "Selected %d %d %d %d\n", *x1, *y1, *x2, *y2);
	free(verify_type);
}
