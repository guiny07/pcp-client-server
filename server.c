#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "utils.h"

void generate_blocks(Block *blocks);
void *handle_client(void *arg);

int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    static int image[HEIGHT][WIDTH];
    static int result[HEIGHT][WIDTH];

    pthread_t threads[N_CLIENT];

    // Read image
    FILE *input = fopen("fake_image.txt", "r");
    if(!input) { error("Error opening fake_image.txt..."); }

    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fscanf(input, "%d", &image[i][j]);
        }
    }
    fclose(input);
    
    Block blocks[N_BLOCKS];
    generate_blocks(blocks);

    // Creates server
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("Error creating socket...");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        error("Bind error");
    if(listen(server_fd, N_CLIENT) < 0)
        error("Listen error");

    printf("[Server] Waiting for %d clients...\n", N_CLIENT);

    //Accepting connections and create threads for each client.
    if(N_CLIENT == 1)
    {
        int client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if(client_fd < 0)
            error("Accept error \n");

        printf("[Server] Client connected \n");

        for(int i = 0; i < N_BLOCKS; i++)
        {
            ClientData *data = malloc(sizeof(ClientData));
            data->client_fd = client_fd;
            data->block = blocks[i];
            data->image = image;
            data->result = result;
            send(client_fd, &blocks[i], sizeof(Block), 0);
            send(client_fd, image, sizeof(image), 0);

            recv(client_fd, result, sizeof(result), MSG_WAITALL);
        }
    }
    else
    {
        for(int i = 0; i < N_CLIENT; i++)
        {
            int client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
            if(client_fd < 0)
            error("Accept error \n");

            printf("[Server] Client %d connected. \n", i);

            ClientData *data = malloc(sizeof(ClientData));
            data->client_fd = client_fd;
            data->block = blocks[i];
            data->image = image;
            data->result = result;

            pthread_create(&threads[i], NULL, handle_client, data);
        }

        for(int i = 0; i < N_CLIENT; i++)
            pthread_join(threads[i], NULL);
    }

    FILE *output = fopen("out.txt", "w");
    if(!output)
        error("Can't open out.txt");
    
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fprintf(output, "%d ", result[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    printf("[Server] Smoothed image saved in out.txt");
    close(server_fd);
    return 0;
}



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

void *handle_client(void *arg)
{
    ClientData *data = (ClientData *)arg;
    int client_fd = data->client_fd;
    Block block = data->block;
    int block_result[CLIENT_HEIGHT][CLIENT_WIDTH];
    size_t block_size = block.rows * block.cols * sizeof(int);

    printf("[Server] Sending block of %dx%d starting in  [%d][%d] \n", data->block.rows, data->block.cols, data->block.start_row, data->block.start_col);


    send(client_fd, &data->block, sizeof(block), 0);
    send(client_fd, data->image, sizeof(int) * HEIGHT * WIDTH, 0);


    recv(client_fd, block_result, block_size, MSG_WAITALL);

    for (int i = 0; i < block.rows; i++) 
    {
        memcpy(&data->result[block.start_row + i][block.start_col], &block_result[i], block.cols * sizeof(int));
    }

    close(client_fd);
    free(data);
    pthread_exit(NULL);
}