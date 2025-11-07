#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define WIDTH 5
#define HEIGHT 5
#define PORT 3124


typedef struct 
{
    int start_col;
    int start_row;
    int cols;
    int rows;
} Block;

void smooth_image(int input[HEIGHT][WIDTH], int output[HEIGHT][WIDTH]);
void error(const char *msg);