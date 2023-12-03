/*
 * Servidor TCP
 */
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

unsigned long fatorial(unsigned long numero) {
	unsigned long resultado = 1;
	while (numero > 0) {
		resultado = resultado * numero;
		numero = numero - 1;
	}
	return resultado;
}
void *rotina(void *arg) {
	int nova_conexao, retorno;
	unsigned long numero;

	memcpy(&nova_conexao, arg, sizeof(int));
	pthread_mutex_unlock(&mutex);
	sleep(2);
	if (nova_conexao < 0) {
		printf("Erro ao aceitar conexao\n");
		exit(0);
	}
	retorno = recv(nova_conexao, &numero, sizeof(unsigned long), 0);
	if (retorno < 0) {
		printf("\nProblema com o recebimento de dados: %d", retorno);
		return NULL;
	}
	numero = fatorial(numero);
	retorno = send(nova_conexao, &numero, sizeof(unsigned long), 0);
	if (retorno < 0) {
		printf("\nProblema com o envio de resposta: %d", retorno);
		return NULL;
	}

	close(nova_conexao);
	pthread_exit(arg);
}
int main(int argc, char *argv[]) {
	WSADATA wsaData;
	int porta_servidor = 2000;
	char ip_servidor[50];
	int sock, retorno, nova_conexao;
	struct sockaddr_in endereco_cliente, endereco_servidor;

	if (pthread_mutex_init(&mutex, NULL) != 0) {
		perror("mutex_lock");
		exit(1);
	}

	sprintf(ip_servidor, "127.0.0.1");


	// Iniciar a biblioteca winsock...
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("%s: nao foi possivel abrir socket\n", ip_servidor);
		exit(1);
	}

	endereco_servidor.sin_family = AF_INET;
	endereco_servidor.sin_addr.s_addr = htonl(INADDR_ANY);
	endereco_servidor.sin_port = htons(porta_servidor);

	retorno = bind(sock, (SOCKADDR *) &endereco_servidor, sizeof(endereco_servidor));
	if (retorno == SOCKET_ERROR) {
		close(sock);
		WSACleanup();
		return -1;
	}
	retorno = listen(sock, 5);

	if (retorno != 0) {
		printf("Erro na escuta por conexoes\n");
		exit(0);
	}

	printf("aguardando mensagens da porta TCP");

	pthread_t thread_id;
	void *thread_res;
	int status;
	while (1) {
		int len = sizeof(endereco_cliente);
		pthread_mutex_lock(&mutex);
		nova_conexao = accept(sock, (struct sockaddr *) &endereco_cliente, &len);
		status = pthread_create(&thread_id, NULL, rotina, (void *) &nova_conexao);
	}
}