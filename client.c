#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    static int image[HEIGHT][WIDTH];
    static int result[HEIGHT][WIDTH];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("Erro ao criar socket");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
        error("Endereço inválido");

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Falha na conexão");

    printf("Conectado ao servidor.\n");

    // Recebe a imagem
    ssize_t received = recv(sock, image, sizeof(image), MSG_WAITALL);
    if (received != sizeof(image))
        fprintf(stderr, "Aviso: recebi %ld bytes, esperava %ld\n", received, sizeof(image));

    printf("Imagem recebida (%ld bytes)\n", received);

    // Processa a imagem
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            printf("%d ", image[i][j]);
        }
        printf("\n");
    }

    smooth_image(image, result);

    // Envia o resultado
    send(sock, result, sizeof(result), 0);
    printf("Resultado enviado.\n");

    close(sock);
    return 0;
}
