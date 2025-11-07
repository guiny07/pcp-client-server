#include "utils.h"


void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void smooth_image(int input[HEIGHT][WIDTH], int output[HEIGHT][WIDTH], Block *block)
{
    for(int i = block->start_row; i < block->rows; i++)
    {
        for(int j = block->start_col; j < block->cols; j++)
        {
            int sum = input[i][j];
            int count = 1;

            // Upper neighbor
            if(i > 0) {sum += input[i - 1][j];count++; }
            // Lower neighbor
            if(i < HEIGHT - 1) {sum += input[i + 1][j]; count++; }
            // Left neighbor
            if(j > 0) {sum += input[i][j - 1]; count++; }
            // Right neighbor
            if(j < WIDTH - 1) {sum += input[i][j + 1]; count++; }

            output[i][j] = sum / count;
        }
    }
}