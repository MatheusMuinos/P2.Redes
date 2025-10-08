#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// Função para converter nome do arquivo para maiúsculas
void nome_maiusculo(const char *orig, char *dest) {
	int i = 0;
	while (orig[i]) {
		dest[i] = toupper((unsigned char)orig[i]);
		i++;
	}
	dest[i] = '\0';
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Uso: %s <arquivo_entrada> <ip_servidor> <puerto>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	char *nome_entrada = argv[1];
	char *ip = argv[2];
	int puerto = atoi(argv[3]);
	if (puerto <= 0 || puerto > 65535) {
		fprintf(stderr, "Puerto inválido\n");
		exit(EXIT_FAILURE);
	}

	FILE *fin = fopen(nome_entrada, "r");
	if (!fin) {
		perror("Erro ao abrir arquivo de entrada");
		exit(EXIT_FAILURE);
	}
	char nome_saida[256];
	nome_maiusculo(nome_entrada, nome_saida);
	FILE *fout = fopen(nome_saida, "w");
	if (!fout) {
		perror("Erro ao abrir arquivo de saída");
		fclose(fin);
		exit(EXIT_FAILURE);
	}

	int sockclient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockclient < 0) {
		perror("Erro ao criar socket");
		fclose(fin); fclose(fout);
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in dirserv;
	dirserv.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &dirserv.sin_addr);
	dirserv.sin_port = htons(puerto);

	if (connect(sockclient, (struct sockaddr *)&dirserv, sizeof(dirserv)) < 0) {
		perror("Erro ao conectar ao servidor");
		fclose(fin); fclose(fout); close(sockclient);
		exit(EXIT_FAILURE);
	}

	char linha[1001], resposta[1001];
	while (fgets(linha, 1000, fin)) {
		size_t tam = strlen(linha);
		if (send(sockclient, linha, tam, 0) < 0) {
			perror("Erro ao enviar linha");
			break;
		}
		ssize_t n = recv(sockclient, resposta, 1000, 0);
		if (n <= 0) {
			perror("Erro ao receber linha");
			break;
		}
		resposta[n] = '\0';
		fputs(resposta, fout);
		sleep(10); // Espera 10 segundos para ler cada linha
	}

	fclose(fin);
	fclose(fout);
	close(sockclient);
	printf("Arquivo convertido salvo como: %s\n", nome_saida);
	return 0;
}
