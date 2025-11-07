#include <stdio.h>
#include <stdlib.h> 

#define WIDTH 2000
#define HEIGHT 2000


void smooth_image(int input[HEIGHT][WIDTH], int output[HEIGHT][WIDTH])
{

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



    return 0;
}