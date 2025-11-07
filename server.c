#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"

int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    static int image[HEIGHT][WIDTH];
    static int result[HEIGHT][WIDTH];

    FILE *input = fopen("entrada.txt", "r");
    if(!input) { error("Error opening entrada.txt..."); }

    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fscanf(input, "%d", &image[i][j]);
        }
    }
    fclose(input);
    
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("Error creating socket...");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        error("Bind error");
    if(listen(server_fd, 1) < 0)
        error("Listen error");

    printf("Server waiting for connection... \n");
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);

    if(client_fd < 0)
        error("Error trying to accept");
    printf("Client connected! \n");

    //Send image
    send(client_fd, image, sizeof(image), 0);

    size_t received = recv(client_fd, result, sizeof(result), MSG_WAITALL);

    printf("Image processed and received! \n");

    // Save result
    FILE *output = fopen("saida.txt", "w");

    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fprintf(output, "%d ", result[i][j]);
        }
        fprintf(output, "\n");
    }
    fclose(output);

    close(client_fd);
    close(server_fd);
    return 0;
}