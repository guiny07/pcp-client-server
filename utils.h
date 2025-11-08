#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#define N_CLIENT 2
#define N_BLOCKS 2
#define CLIENT_HEIGHT 1000
#define CLIENT_WIDTH 2000
#define WIDTH 2000
#define HEIGHT 2000
#define PORT 3124


typedef struct 
{
    int start_col;
    int start_row;
    int cols;
    int rows;
} Block;

typedef struct
{
    int client_fd;
    Block block;
    int (*image)[WIDTH];
    int (*result)[WIDTH];
}ClientData;

void smooth_image(int input[HEIGHT][WIDTH], int output[HEIGHT][WIDTH], Block *block);
void error(const char *msg);