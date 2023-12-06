/*
 * Cliente TCP
 */
#include <winsock2.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct sockaddr_in endereco_cliente, endereco_servidor;
int porta_servidor = 2000;
char ip_servidor[50];
unsigned long fatorial1, fatorial2, numero_inicio, numero_fim;
pthread_mutex_t mutex;

void *rotina(void *arg) {
	int retorno;
	unsigned char *buffer = (unsigned long*)malloc(2 * sizeof(unsigned long));

	memcpy(buffer, &numero_inicio, sizeof(unsigned long));
	memcpy(buffer + sizeof(unsigned long), &numero_fim, sizeof(unsigned long));

	pthread_mutex_unlock(&mutex);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
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

	retorno = send(sock, buffer, 2 * sizeof(unsigned long),0);
	if (retorno < 0) {
		printf("Não foi possível enviar mensagem \n");
		close(sock);
		exit(1);
	}

	retorno = recv(sock, arg, sizeof(unsigned long),0);
	if (retorno < 0) {
		printf("Não foi possível receber resposta \n");
		close(sock);
		exit(1);
	}

	close(sock);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	pthread_t thread_id;

	sprintf(ip_servidor, "127.0.0.1");

	endereco_servidor.sin_family =  AF_INET;
	endereco_servidor.sin_addr.s_addr = inet_addr(ip_servidor);
	endereco_servidor.sin_port = htons(porta_servidor);

	if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
		WSACleanup();
		return -1;
	}

	numero_inicio = 1;
	numero_fim = 3;

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_lock(&mutex);
	pthread_create(&thread_id, NULL, rotina, (void *) &fatorial1);

	pthread_mutex_lock(&mutex);

	numero_inicio = 4;
	numero_fim = 6;

	pthread_create(&thread_id, NULL, rotina, (void *) &fatorial2);

	pthread_join(thread_id, NULL);

	printf("Resultado: %lu", fatorial1 * fatorial2);

	return 1;

}