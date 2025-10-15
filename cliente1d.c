/*
MODIFICACIONES:
- Eliminar la espera (sleep).
- Leer en bucle con recv hasta que el servidor cierre: while ((n = recv(...)) > 0).
- Permitir elegir el tamaño de lectura (trozo) por argumento [TAMANO_MENSAJE] para probar con 5, 10, etc.
- Acumular el total recibido y mostrar cada trozo.

RESULTADO OBTENIDO (ejemplos típicos):
- TAM=5: 3 lecturas de 5 bytes cada una (total 15): 'Hola,' (5), ' clie' (5), 'nte!\n' (5).
- TAM=10: 2 lecturas (10 + 5 = 15): 'Hola, clie' (10), 'nte!\n' (5).
- El bucle termina cuando recv devuelve 0 (servidor cerró) o <0 (error).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    if (argc < 3) {
        printf("Uso: %s <IP> <PUERTO> [TAMANO_MENSAJE]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int puerto = atoi(argv[2]);
    const char* ip = argv[1];
    int sockclient;
    int n;
    int total = 0;
    int tamano_mensaje = (argc >= 4) ? atoi(argv[3]) : 5;
    if (tamano_mensaje <= 0) {
        fprintf(stderr, "Tamano de mensaje invalido\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sockstruct_client;
    char mensaje[1000];

    if((sockclient = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }

    sockstruct_client.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sockstruct_client.sin_addr);
    sockstruct_client.sin_port = htons(puerto);

    if(connect(sockclient,(struct sockaddr*) &sockstruct_client, sizeof(sockstruct_client)) < 0){ 
        perror("No se pudo conectar");
        exit(EXIT_FAILURE);
    }

    // SIN sleep: leer en trozos fijos 
    while ((n = recv(sockclient, mensaje, tamano_mensaje, 0)) > 0) {
        // Asegurar terminación para imprimir como string
        if (n >= (int)sizeof(mensaje)) n = (int)sizeof(mensaje) - 1;
        mensaje[n] = '\0';
        printf("Recibido: '%s' (%d bytes)\n", mensaje, n);
        total += n;
    }

    if (n == 0) {
        printf("Conexion cerrada por el servidor\n");
    } else if (n < 0) {
        perror("Error en recv");
    }
    printf("Total de bytes recibidos: %d\n", total);

    close(sockclient);
    return EXIT_SUCCESS;
}