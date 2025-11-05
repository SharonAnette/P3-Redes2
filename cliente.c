#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;
    int n;
    int len = sizeof(servaddr);

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Error al inicializar Winsock");
        return EXIT_FAILURE;
    }

    // Crear el socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Inicializar la dirección del servidor
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // Asignar la dirección IP del servidor
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (servaddr.sin_addr.s_addr == INADDR_NONE) {
        perror("Error en inet_addr");
        closesocket(sockfd);
        WSACleanup();
        return EXIT_FAILURE;
    }

    while (1) {
        printf("Ingrese el mensaje: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Eliminar el salto de línea

        // Enviar el mensaje al servidor (bloqueante)
        if (sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&servaddr, len) < 0) {
            perror("Error al enviar mensaje");
            closesocket(sockfd);
            WSACleanup();
            return EXIT_FAILURE;
        }

        // Recibir la respuesta del servidor (bloqueante)
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);
        if (n < 0) {
            perror("Error al recibir respuesta");
            closesocket(sockfd);
            WSACleanup();
            return EXIT_FAILURE;
        }
        buffer[n] = '\0';
        printf("Servidor : %s\n", buffer);
    }

    closesocket(sockfd);
    WSACleanup(); // Limpiar Winsock
    return 0;
}
