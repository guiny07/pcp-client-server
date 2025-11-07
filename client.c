#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORTA 3124
#define WIDTH 2000
#define HEIGHT 2000

// Função de suavização com stencil de 5 pontos
void smoothImage(int linhas, int colunas, int imagem[linhas][colunas], int resultado[linhas][colunas]) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int soma = imagem[i][j];
            int cont = 1;

            if (i > 0) { soma += imagem[i - 1][j]; cont++; }
            if (i < linhas - 1) { soma += imagem[i + 1][j]; cont++; }
            if (j > 0) { soma += imagem[i][j - 1]; cont++; }
            if (j < colunas - 1) { soma += imagem[i][j + 1]; cont++; }

            resultado[i][j] = soma / cont;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servidor_addr;

    // Criação do socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        exit(1);
    }

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(PORTA);
    servidor_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP do servidor

    // Conexão ao servidor
    if (connect(sockfd, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0) {
        perror("Erro ao conectar ao servidor");
        close(sockfd);
        exit(1);
    }

    printf("[CLIENTE] Conectado ao servidor.\n");

    // Recebe número de blocos que este cliente vai processar
    int num_blocos;
    recv(sockfd, &num_blocos, sizeof(int), 0);
    printf("[CLIENTE] Receberá %d bloco(s).\n", num_blocos);

    for (int b = 0; b < num_blocos; b++) {
        int linhas, colunas;
        recv(sockfd, &linhas, sizeof(int), 0);
        recv(sockfd, &colunas, sizeof(int), 0);

        printf("[CLIENTE] Recebendo bloco %d (%dx%d)...\n", b + 1, linhas, colunas);

        // Aloca memória dinamicamente para o bloco
        int (*image)[colunas] = malloc(linhas * sizeof(*image));
        int (*result)[colunas] = malloc(linhas * sizeof(*result));

        if (!image || !result) {
            perror("Erro de alocação de memória");
            close(sockfd);
            exit(1);
        }

        // Recebe pixels do bloco
        int bytes_esperados = linhas * colunas * sizeof(int);
        int recebidos = 0;
        char *ptr = (char *)image;

        while (recebidos < bytes_esperados) {
            int n = recv(sockfd, ptr + recebidos, bytes_esperados - recebidos, 0);
            if (n <= 0) {
                perror("Erro ao receber bloco de imagem");
                free(image);
                free(result);
                close(sockfd);
                exit(1);
            }
            recebidos += n;
        }

        printf("[CLIENTE] Bloco %d recebido. Processando...\n", b + 1);
        smoothImage(linhas, colunas, image, result);

        // Envia resultado de volta ao servidor
        int enviados = 0;
        char *ptr_envio = (char *)result;
        int total_bytes = linhas * colunas * sizeof(int);

        while (enviados < total_bytes) {
            int n = send(sockfd, ptr_envio + enviados, total_bytes - enviados, 0);
            if (n <= 0) {
                perror("Erro ao enviar resultado");
                free(image);
                free(result);
                close(sockfd);
                exit(1);
            }
            enviados += n;
        }

        printf("[CLIENTE] Bloco %d enviado com sucesso!\n", b + 1);

        free(image);
        free(result);
    }

    printf("[CLIENTE] Todos os blocos processados e enviados.\n");

    close(sockfd);
    return 0;
}
