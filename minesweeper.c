/**
 * comp2129 - assignment 1
 * Jacky Liang
 * jlia6991
 */

/*	0-8 state for each cell indicates how many mines are around
	9 is for a mine
	10 is for a flag
	11-19 is the uncovered state of a non-mine cell

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

void display_grid(int *matrixp, int *widthp, int *heightp);

int count_adj_cells(int *matrixp, int *widthp, int *heightp, int i);

int main(void) {

	int width;
	int *widthp = &width;
	int height;
	int *heightp = &height;
	char input[100];
	char type;
	int x = -1;
	int y = -1;
	int flagged = 0;
	int uncovered = 0;
	int check = -1;

	if (fgets(input, 100, stdin) == NULL) {
		printf("error\n");
		return 1;
	}

	if (sscanf(input,"%c %d %d %d", &type, &width, &height, &check) != 3) {
		printf("error\n");
		return 1;
	}

	// first input must be grid initialisation
	if (type != 'g' || width < 1 || width > 100 || height < 1 || height > 100 || width*height < 10) {
		printf("error\n");
		return 1;
	}

	// no integers after the first character
	for (int i = 1; i < strlen(input); ++i) {
		if (!isdigit(input[i]))	{
			if (!isspace(input[i])) {
				printf("error\n");
				return 1;
			}
		}
	}

	printf("%s", input);

	int matrix[width*height];
	int *matrixp = &matrix[0];

	//initialise array with everything to -1
	for (int i = 0; i < width*height; i++) {
		matrix[i] = -1;
	}

	//entering 10 mines
	for (int i = 0; i < 10; i++) {

		if (fgets(input, 100, stdin) == NULL) {
			printf("error\n");
			return 1;
		}

		if (sscanf(input,"%c %d %d %d", &type, &x, &y, &check) != 3) {
			printf("error\n");
			return 1;
		}

		// no integers after the first character
		for (int i = 1; i < strlen(input); ++i) {
			if (!isdigit(input[i]))	{
				if (!isspace(input[i])) {
					printf("error\n");
					return 1;
				}
			}
		}

		//check type
		if (type != 'b') {
			printf("error\n");
			return 1;
		}

		//check valid location
		if (x >= width || x < 0 || y < 0 || y >= height) {
			printf("error\n");
			return 1;
		}

		// check if location is already a bomb or not
		if (matrix[y*width+x] == 9) {
			printf("error\n");
			return 1;
		}

		printf("%s", input);

		// give bomb an int of 9
		matrix[y*width+x] = 9;
	}

	// initialise the hidden cells to display surrounding mines
	for (int i = 0; i < width*height; i++) {

		// if it's not flagged
		if (matrix[i] != 9) {
			matrix[i] = count_adj_cells(matrixp, widthp, heightp, i);
		}

	}

	// pass pointer locations and display
	display_grid(matrixp, widthp, heightp);

	while(1) {

		if (fgets(input, 100, stdin) == NULL) {
			printf("error\n");
			return 1;
		}

		if (sscanf(input,"%c %d %d %d", &type, &x, &y, &check) != 3) {
			printf("error\n");
			return 1;
		}

		//check type, if not flag
		if (type != 'f' && type != 'u') {
			printf("error\n");
			return 1;
		}

		// no integers after the first character
		for (int i = 1; i < strlen(input); ++i) {
			if (!isdigit(input[i]))	{
				if (!isspace(input[i])) {
					printf("error\n");
					return 1;
				}
			}
		}

		//check valid location
		if (x >= width || x < 0 || y < 0 || y >= height) {
			printf("error\n");
			return 1;
		}

		// check if location is already a flag or not, or uncovered

		if (matrix[y*width+x] == 10 || matrix[y*width+x] > 10) {
			printf("error\n");
			return 1;
		}

		if (type == 'f') {

			flagged = flagged +1;

			if (flagged == 11) {
				printf("error\n");
				return 1;
			}

			printf("%s", input);

			matrix[y*width+x] = 10;

			display_grid(matrixp, widthp, heightp);

			if ((flagged+uncovered) == width*height && flagged == 10) {
				printf("won\n");
				return 0;
			}

		}

		if (type == 'u') {

			// if you uncover a mine
			if (matrix[y*width+x] == 9) {
				printf("lost\n");
				return 0;
			}

			printf("%s", input);

			matrix[y*width+x] = matrix[y*width+x]+11;

			display_grid(matrixp, widthp, heightp);

			uncovered = uncovered +1;

			if ((flagged+uncovered) == width*height && flagged == 10) {
				printf("won\n");
				return 0;
			}

		}

	}




}

void display_grid(int *matrixp, int *widthp, int *heightp) {

	//top border line
	printf("+");
	for (int i = 0; i < *widthp; i++) {
		printf("-");
	}
	printf("+\n");

	// for every cell
	for (int i = 0; i < *heightp; i++) {
		printf("|");
		for (int j = 0; j < *widthp; j++) {

			// if the cell is not a flag, print *
			if (*(matrixp+(i * *widthp + j)) < 10) {
				printf("*");

			// if the cell is a flag, print
			} else if (*(matrixp+(i * *widthp + j)) == 10) {
				printf("f");

			// print out how many surrounding mines
			} else {
				printf("%d", *(matrixp+(i * *widthp + j))-11);
			}

		}
		printf("|\n");
	}

	// bottom border
	printf("+");
	for (int i = 0; i < *widthp; i++) {
		printf("-");
	}
	printf("+\n");

}


int count_adj_cells(int *matrixp, int *widthp, int *heightp, int i) {

	int count = 0;

	if (*heightp != 1 && *widthp != 1) {

		// if left corner
		if (i == 0) {

			if (*(matrixp+1) == 9) {
				count++;
			}

			if (*(matrixp+*widthp) == 9) {
				count++;
			}

			if (*(matrixp+*widthp+1) == 9) {
				count++;
			}

			return count;

		// top row excluding corners
		} else if (i < *widthp-1 && i > 0) {

			if (*(matrixp+i-1) == 9) {
				count++;
			}

			if (*(matrixp+i+1) == 9) {
				count++;
			}

			for (int col = -1; col < 2; col++) {

				if (*(matrixp+i+*widthp+col) == 9) {
					count++;
				}

			}

			return count;

		// top right corner
		} else if (i == *widthp-1) {

			if (*(matrixp+i-1) == 9) {
				count++;
			}

			if (*(matrixp+i+*widthp) == 9) {
				count++;
			}

			if (*(matrixp+i+*widthp-1) == 9) {
				count++;
			}

			return count;

		// left hand side excluding corners
		} else if (i >= *widthp && i <= *widthp*(*heightp-1) && i % *widthp == 0) {

			if (*(matrixp+i-*widthp) == 9) {
				count++;
			}

			if (*(matrixp+i-*widthp + 1) == 9) {
				count++;
			}

			if (*(matrixp+i+1) == 9) {
				count++;
			}

			if (*(matrixp+i+*widthp) == 9) {
				count++;
			}

			if (*(matrixp+i+*widthp+1) == 9) {
				count++;
			}

			return count;

		// right hand side excluding corners
		} else if (i > *widthp && i < *widthp*(*heightp-1) && (i+1) % *widthp == 0) {

			if (*(matrixp+i-*widthp-1) == 9) {
				count++;
			}

			if (*(matrixp+i-*widthp) == 9) {
				count++;
			}

			if (*(matrixp+i-1) == 9) {
				count++;
			}

			if (*(matrixp+i+*widthp-1) == 9) {
				count++;
			}

			if (*(matrixp+i+*widthp) == 9) {
				count++;
			}

			return count;

		// bottom left corner
		} else if (i == *widthp * (*heightp-1)) {

			if (*(matrixp+i-*widthp) == 9) {
				count++;
			}

			if (*(matrixp+i-*widthp+1) == 9) {
				count++;
			}

			if (*(matrixp+i+1) == 9) {
				count++;
			}

			return count;

		// bottom right corner
		} else if (i == (*widthp**heightp)-1) {

			if (*(matrixp+i-*widthp-1) == 9) {
				count++;
			}

			if (*(matrixp+i-*widthp) == 9) {
				count++;
			}

			if (*(matrixp+i-1) == 9) {
				count++;
			}

			return count++;

		// bottom row excluding corners
		} else if (i > *widthp*(*heightp-1) && i < (*widthp**heightp)-1) {

			for (int row = -1; row < 2; row ++) {
				if (*(matrixp+i-*widthp+row) == 9) {
					count++;
				}
			}

			if (*(matrixp+i-1) == 9) {
				count++;
			}

			if (*(matrixp+i+1) == 9) {
				count++;
			}

			return count;

		// every other cell that's not on the border
		} else {

			for (int row = -1; row < 2; row++) {
				if (*(matrixp+i-*widthp+row) == 9) {
					count++;
				}
			}

			if (*(matrixp+i-1) == 9) {
				count++;
			}

			if (*(matrixp+i+1) == 9) {
				count++;
			}

			for (int row = -1; row < 2; row++) {
				if (*(matrixp+i+*widthp+row) == 9) {
					count++;
				}
			}

			return count;

		}

	// single column case
	} else {

		// top or left
		if (i == 0) {

			if (*(matrixp+1) == 9 ) {

				count++;

			}

			return count;

		// bottom or right
		} else if (i == (*widthp**heightp)-1) {

			if (*(matrixp+i-1) == 9 ) {

				count++;

			}

			return count;

		// everything inbetween
		} else {

			if (*(matrixp+i-1) == 9 ) {

				count++;

			}

			if (*(matrixp+i+1) == 9 ) {

				count++;

			}

			return count;
		}

	}

}
