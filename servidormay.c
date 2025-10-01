#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	int puerto = atoi(argv[1]);
	if (puerto <= 0 || puerto > 65535) {
		fprintf(stderr, "Puerto inválido\n");
		exit(EXIT_FAILURE);
	}

	int sockserv = socket(AF_INET, SOCK_STREAM, 0);
	if (sockserv < 0) {
		perror("Error al crear el socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in dirserv, dircon;
	dirserv.sin_family = AF_INET;
	dirserv.sin_addr.s_addr = htonl(INADDR_ANY);
	dirserv.sin_port = htons(puerto);

	if (bind(sockserv, (struct sockaddr *)&dirserv, sizeof(dirserv)) < 0) {
		perror("Error en bind");
		close(sockserv);
		exit(EXIT_FAILURE);
	}
	if (listen(sockserv, 5) < 0) {
		perror("Error en listen");
		close(sockserv);
		exit(EXIT_FAILURE);
	}
	printf("Servidor de mayúsculas escuchando en el puerto %d\n", puerto);

	while (1) {
		socklen_t tamdir = sizeof(dircon);
		int sockcon = accept(sockserv, (struct sockaddr *)&dircon, &tamdir);
		if (sockcon < 0) {
			perror("Error en accept");
			continue;
		}
		char ip_cliente[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &dircon.sin_addr, ip_cliente, sizeof(ip_cliente)) != NULL) {
			printf("Conexión de %s:%d\n", ip_cliente, ntohs(dircon.sin_port));
		}

		char buffer[1001];
		ssize_t n;
		while ((n = recv(sockcon, buffer, 1000, 0)) > 0) {
			buffer[n] = '\0';
			// Convertir a mayúsculas
			for (int i = 0; i < n; i++) {
				buffer[i] = toupper((unsigned char)buffer[i]);
			}
			send(sockcon, buffer, n, 0);
		}
		close(sockcon);
	}
	close(sockserv);
	return 0;
}
