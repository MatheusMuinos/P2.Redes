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

void _strip_line(char *linea) {
    if (linea) {
        linea[strcspn(linea, "\r\n")] = '\0';
    }
}


int main(int argc, char *argv[]){
    if (argc != 4) { //El programa debe recibir 3 argumentos de entrada, siendo el primero el archivo, el segundo la IP y el tercero el puerto.
        printf("Falta el argumento de archivo, IP o Puerto\n");
        exit(EXIT_FAILURE);
    }
    
    char* archivo = argv[1];
    char* ip = argv[2];
    int puerto = atoi(argv[3]);
    
    int sockclient,bit;
    struct sockaddr_in sockstruct_client;
    char mensaje[1000],linea[1000];

    FILE* file = fopen(archivo,"r");
    if (file == NULL) {
        printf("Error!\n");
        return 1;
    }



    
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

    //sleep(1); //Espera para que el servidor envíe los 2 mensajes

    do{
        linea = fgets(linea,sizeof(linea),stdin);

        

        if(bit = recv(sockclient, mensaje, sizeof(mensaje), 0)){ 
        if( bit < 0){
            perror("No se pudo recibir el mensaje\n");
            exit(EXIT_FAILURE);
        }
    }

    } while(linea != NULL);
    
    
    printf("%s\n",mensaje);
    printf("%d bytes\n",bit);
    
    fclose(file);
    close(sockclient);
    return(EXIT_SUCCESS);
}