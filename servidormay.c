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

    if(bind(sockserv,(struct sockaddr*) &sockstruct_serv, sizeof(sockstruct_serv)) < 0){ //Asignación de dirección al socket de servidor, guardada en la estructura del servidor.
        perror("No se pudo asignar dirección");
        exit(EXIT_FAILURE);
    }

    if(listen(sockserv,5) < 0){ //Marca el socket como pasivo.
        perror("Error de listen");
        exit(EXIT_FAILURE);
    }

    while(1){ //loop infinito para aceptar conexiones secuenciales
        if((sockcon = accept(sockserv, (struct sockaddr*)&sockstruct_con, &con_len)) < 0 ){ //Acepta la conexión del cliente y devuelve un socket de conexión.
            perror("No se pudo aceptar la conexion");
            continue;
        }

        char ip_cliente[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sockstruct_con.sin_addr,ip_cliente,sizeof(ip_cliente)); //Guarda en ip_cliente la dirección IP.
        int puerto_cliente = ntohs(sockstruct_con.sin_port); //Guarda el puerto, convertido de red a host.

        printf("IP:%s\tPuerto:%d\n",ip_cliente,puerto_cliente);

        send(sockcon,saludo,strlen(saludo),0);  //Envía un mensaje a los clientes que se conecten.
        send(sockcon,mensaje2,strlen(mensaje2),0);
    }

    close(sockserv);
    return (EXIT_SUCCESS);
}