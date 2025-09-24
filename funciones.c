#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// Crear un socket TCP
int crear_socket() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Error al crear el socket");
    }
    return s;
}

// Asignar dirección a un socket
int asignar_direccion(int sock, struct sockaddr_in *dir) {
    int r = bind(sock, (struct sockaddr *)dir, sizeof(struct sockaddr_in));
    if (r < 0) {
        perror("Error en bind");
    }
    return r;
}

// Marcar socket como pasivo
int marcar_pasivo(int sock, int backlog) {
    int r = listen(sock, backlog);
    if (r < 0) {
        perror("Error en listen");
    }
    return r;
}

// Solicitar conexión (cliente)
int solicitar_conexion(int sock, struct sockaddr_in *dir) {
    int r = connect(sock, (struct sockaddr *)dir, sizeof(struct sockaddr_in));
    if (r < 0) {
        perror("Error en connect");
    }
    return r;
}

// Aceptar conexión (servidor)
int aceptar_conexion(int sock, struct sockaddr_in *dir, socklen_t *tamdir) {
    int s = accept(sock, (struct sockaddr *)dir, tamdir);
    if (s < 0) {
        perror("Error en accept");
    }
    return s;
}

// Enviar datos
ssize_t enviar_datos(int sock, const void *buf, size_t len, int flags) {
    ssize_t r = send(sock, buf, len, flags);
    if (r < 0) {
        perror("Error en send");
    }
    return r;
}

// Recibir datos
ssize_t recibir_datos(int sock, void *buf, size_t len, int flags) {
    ssize_t r = recv(sock, buf, len, flags);
    if (r < 0) {
        perror("Error en recv");
    }
    return r;
}

// Enviar datos sin conexión
ssize_t enviar_a(int sock, const void *buf, size_t len, int flags, struct sockaddr_in *dir) {
    ssize_t r = sendto(sock, buf, len, flags, (struct sockaddr *)dir, sizeof(struct sockaddr_in));
    if (r < 0) {
        perror("Error en sendto");
    }
    return r;
}

// Recibir datos sin conexión
ssize_t recibir_de(int sock, void *buf, size_t len, int flags, struct sockaddr_in *dir, socklen_t *tamdir) {
    ssize_t r = recvfrom(sock, buf, len, flags, (struct sockaddr *)dir, tamdir);
    if (r < 0) {
        perror("Error en recvfrom");
    }
    return r;
}

// Cerrar socket
int cerrar_socket(int sock) {
    int r = close(sock);
    if (r < 0) {
        perror("Error al cerrar el socket");
    }
    return r;
}