#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(){
//1.MANEJO DE IPs     FUNCIONES inet_pton E inet_ntop
    char ip[INET_ADDRSTRLEN] = "182.175.85.250";
    struct in_addr miip;
    if (inet_pton(AF_INET,ip,&miip) != 1){
        printf("Formato de dirección incorrecto");
        exit(EXIT_FAILURE);
    }
    printf("Formato hexadecimal: 0x%X\n",miip.s_addr);

    char ipnet[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET,&miip,ipnet,INET_ADDRSTRLEN) != NULL){
        printf("Formato decimal: %s\n",ipnet);
    }

    char ip6[INET6_ADDRSTRLEN] = "1080:0:0:0:8:800:200C:417A";
    struct in6_addr miip6;
    if (inet_pton(AF_INET6,ip6,&miip6) != 1){
        printf("Formato de dirección incorrecto");
        exit(EXIT_FAILURE);
    }
    
    for(int i=0;i<16;i++){
        printf("Formato hexadecimal byte %d: 0x%x\n",i,miip6.s6_addr[i]);
    }
    
    char ipnet6[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6,&miip6,ipnet6,INET6_ADDRSTRLEN) != NULL){
        printf("Formato decimal: %s\n\n",ipnet6);
    }

//2.CAMBIO DE ORDEN DE LOS BYTES
    uint16_t inicial = 4040;
    uint16_t conv = htons(inicial);
    uint16_t inv = ntohs(conv);

    printf("Inicial:%x\tConversión:%x\tInversa:%x\n",inicial,conv,inv);

    uint16_t inicial2 = miip.s_addr;
    uint16_t conv2 = htons(inicial2);
    uint16_t inv2 = ntohs(conv2);
    printf("Inicial:%x\tConversión:%x\tInversa:%x\n",inicial2,conv2,inv2);

    uint16_t inicial3 = ipnet;
    uint16_t conv3 = htons(inicial3);
    uint16_t inv3 = ntohs(conv3);
    printf("Inicial:%x\tConversión:%x\tInversa:%x\n\n",inicial3,conv3,inv3);
    
}

