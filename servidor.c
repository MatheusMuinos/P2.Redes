#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "funciones.c"

int main(int argc, char *argv[]) {

    int sockserv, sockcon;
    struct sockaddr_in dirserv, dircon;
    socklen_t tamdir;
    char* message;
    int puerto;
    
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    puerto = atoi(argv[1]);
    if (puerto <= 0 || puerto > 65535) {
        fprintf(stderr, "Puerto inválido\n");
        exit(EXIT_FAILURE);
    }

    sockserv = socket(AF_INET, SOCK_STREAM, 0);
    if (sockserv < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket creado con éxito\n");

    dirserv.sin_family = AF_INET;
    dirserv.sin_addr.s_addr = htonl(INADDR_ANY);
    dirserv.sin_port = htons(puerto);

    int address = bind(sockserv, (struct sockaddr *)&dirserv, sizeof(dirserv));
    if (address < 0){
        perror("No se pudo asignar direccion");
        exit(EXIT_FAILURE);
    }

    if (listen(sockserv, 5) > 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }
    printf("Socket marcado como pasivo, escuchando conexiones...\n");

    // Bucle para atender clientes secuencialmente
    for (;;) {
        tamdir = sizeof(struct sockaddr_in);
        sockcon = accept(sockserv, (struct sockaddr *)&dircon, &tamdir);
        if (sockcon < 0) {
            perror("Error en accept");
            continue; // sigue esperando otros clientes
        }
        printf("\nConexion aceptada\n");

        char ip_cliente[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &dircon.sin_addr, ip_cliente, sizeof(ip_cliente)) != NULL) {
            printf("Conexion desde IP: %s, Puerto: %d\n", ip_cliente, ntohs(dircon.sin_port));
        } else {
            perror("Error al convertir la direccion IP");
        }

        const char *mensaje = "Hola, cliente!\n";
        ssize_t bytes_enviados = send(sockcon, mensaje, strlen(mensaje), 0);
        if (bytes_enviados < 0) {
            perror("Error al enviar el mensaje");
        } else {
            printf("Mensaje enviado (%zd bytes)\n", bytes_enviados);
        }

        close(sockcon);
        printf("Conexion con cliente cerrada. Esperando otro cliente...\n");

    }

    close(sockserv);

}