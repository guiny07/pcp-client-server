#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "utils.h"


int main()
{
    int sock;
    struct sockaddr_in server_addr;

    static int image[HEIGHT][WIDTH];
    static int result[HEIGHT][WIDTH];
    Block block;

    //Creates socket
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("Can't create socket... \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connects to the server
    if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        error("Connection failed \n");

    printf("[Client] Connected to server. \n");

    int block_result[CLIENT_HEIGHT * CLIENT_WIDTH];

    while(recv(sock, &block, sizeof(Block), MSG_WAITALL) > 0)
    {
        recv(sock, image, sizeof(image), MSG_WAITALL);

         // Smoothing localy the image
        smooth_image(image, result, &block);


        for (int i = 0; i < block.rows; i++) 
        {
            memcpy(&block_result[i * block.cols], &result[block.start_row + i][block.start_col], block.cols * sizeof(int));
        }

        printf("[Client] Block processed. Sending back to the server... \n");

        send(sock, block_result, sizeof(int) * block.rows * block.cols, 0);

    }

    printf("[Client] Closing connection... \n");
    close(sock);

    return 0;
}