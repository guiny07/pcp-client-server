#include <stdio.h>
#include <stdlib.h> 

#define WIDTH 2000
#define HEIGHT 2000


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