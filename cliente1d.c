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
    if (argc != 3) { //El programa debe recibir 2 argumentos de entrada, siendo el primero la IP y el segundo el puerto.
        printf("Falta el argumento de IP o Puerto\n");
        exit(EXIT_FAILURE);
    }
    
    int puerto = atoi(argv[2]);
    char* ip = argv[1];
    int sockclient;
    int n;
    int total = 0;
    int tamano_mensaje = 5;
    struct sockaddr_in sockstruct_client;
    char mensaje[1000];

    
    if((sockclient = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //Creación de un socket IPv4 orientado a TCP.
        perror("No se pudo crear el socket\n");
        exit(EXIT_FAILURE);
    }

    sockstruct_client.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sockstruct_client.sin_addr);
    sockstruct_client.sin_port = htons(puerto);

    if(connect(sockclient,(struct sockaddr*) &sockstruct_client, sizeof(sockstruct_client)) < 0){ 
        perror("No se pudo conectar\n");
        exit(EXIT_FAILURE);
    }

    while ((n = recv(sockclient, mensaje, tamano_mensaje, 0)) > 0) {
        mensaje[n] = '\0'; // Asegura que el string esté terminado
        printf("Recibido: '%s' (%d bytes)\n", mensaje, n);
        total += n;
    }
    printf("Total de bytes recibidos: %d\n", total);

    close(sockclient);
    return(EXIT_SUCCESS);
}