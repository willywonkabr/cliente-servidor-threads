/*
 * Cliente TCP
 */
#include <winsock2.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	int porta_servidor = 2000;
	char ip_servidor[50];
	char *mensagem;
	int sock, retorno;
	struct sockaddr_in endereco_cliente, endereco_servidor;

	sprintf(ip_servidor, "127.0.0.1");
	mensagem = (char *) malloc(7);
	// Byte 0 = 6
	// Byte 1 = P
	// Byte 2 = A
	// Byte 3 = B
	// Byte 4 = L
	// Byte 5 = O
	// Byte 6 = \0
	*mensagem = 6;
	sprintf(mensagem+1, "Pablo");

	endereco_servidor.sin_family =  AF_INET;
	endereco_servidor.sin_addr.s_addr = inet_addr(ip_servidor);
	endereco_servidor.sin_port = htons(porta_servidor);

	if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
		WSACleanup();
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("%s: não foi possível abrir socket \n", ip_servidor);
		exit(1);
	}

	endereco_cliente.sin_family = AF_INET;
	endereco_cliente.sin_addr.s_addr = htonl(INADDR_ANY);
	endereco_cliente.sin_port = htons(0);

	retorno = connect(sock, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor));
	if (retorno != 0) {
		printf("\nConexao falhou!");
		exit(0);
	}

	retorno = send(sock, mensagem, 7,0);
	if (retorno < 0) {
		printf("Não foi possível enviar mensagem \n");
		close(sock);
		exit(1);
	}
	free(mensagem);
	mensagem = (char *) malloc(20);
	retorno = recv(sock, mensagem, 20,0);
	if (retorno < 0) {
		printf("Não foi possível receber resposta \n");
		close(sock);
		exit(1);
	}
	printf(mensagem);

	return 1;

}