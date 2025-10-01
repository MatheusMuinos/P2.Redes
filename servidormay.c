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
    if (argc != 2) { //El programa debe recibir como argumento el puerto.
        printf("Falta el argumento del puerto\n");
        exit(EXIT_FAILURE);
    }
    
    int puerto = atoi(argv[1]);
    int sockserv, sockcon;
    struct sockaddr_in sockstruct_serv, sockstruct_con;
    char* saludo = "Hola, cliente!\n";
    char* mensaje2 = "¿Cómo encontraste este puerto?";
    socklen_t con_len = sizeof(sockstruct_con);

    //Se inicializa la estructura del servidor
    sockstruct_serv.sin_family = AF_INET;
    sockstruct_serv.sin_addr.s_addr = INADDR_ANY;
    sockstruct_serv.sin_port = htons(puerto);

    if ((sockserv = socket(AF_INET,SOCK_STREAM,0)) < 0){ //Creación del socket TCP IPv4.
        perror("No se pudo crear el socket");
        exit(EXIT_FAILURE);
    }
}