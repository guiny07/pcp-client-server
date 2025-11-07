#include <stdio.h>
#include <stdlib.h> 
#include "utils.h"

#define WIDTH 2000
#define HEIGHT 2000

#define N_CLIENT 1
#define N_BLOCKS 4
#define CLIENT_HEIGHT 1000
#define CLIENT_WIDTH 1000

void smooth_image(int input[HEIGHT][WIDTH], int output[HEIGHT][WIDTH])
{
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
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

/*
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        perror("You must provide a file name.");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if(!input)
    {
        printf("Can't open %s", argv[1]);
        return 1;
    }

    static int image[HEIGHT][WIDTH];
    static int result[HEIGHT][WIDTH];

    // Read the image file
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            if(fscaf(input, "%d", &image[i][j]) != 1)
            {
                fprintf(stderr, "Error reading line from file.");
                fclose(input);
                return 1;
            }
        }
    }
    fclose(input);

    // Processing
    smooth_image(image, result);

    // Write the output file
    FILE *output = fopen("saida.txt", 'w');
    if(!output)
    {
        perror("Can't create output file...");
        return 1;
    }

    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fprintf(output, "%d ", result[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    printf("Smoothing finished!");
    return 0;
}
*/

void generate_blocks(Block *blocks) {
    int idx = 0;
    int num_blocks_y = HEIGHT / CLIENT_HEIGHT;
    int num_blocks_x = WIDTH / CLIENT_WIDTH;

    for (int by = 0; by < num_blocks_y; by++) {
        for (int bx = 0; bx < num_blocks_x; bx++) {
            blocks[idx++] = (Block){
                .start_row = by * CLIENT_HEIGHT,
                .start_col = bx * CLIENT_WIDTH,
                .rows = CLIENT_HEIGHT,
                .cols = CLIENT_WIDTH
            };
        }
    }
}

int main()
{
    Block blocks[N_BLOCKS];
    generate_blocks(blocks);

    for(int i = 0; i < N_BLOCKS; i++)
    {
        printf("Bloco %d: \n", i);
        printf("Start row: %d | Start col: %d | rows: %d | cols: %d \n", blocks[i].start_row, blocks[i].start_col, blocks[i].rows, blocks[i].cols);
    }
    return 0;
}