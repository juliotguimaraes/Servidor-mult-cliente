#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include "io.h"
#include "tp_socket.h"


   int	l = 0 ;
   int fim = 0;

void altera_html (char* str){
	FILE* html = fopen ("log.html","r+");
	if (html == NULL)
		error ("Erro ao abrir o arquivo de log.");

	fseek(html,-19,SEEK_END);
	fprintf(html,"%s</br></br>",str);
	fprintf(html,"</br></body></html>");

	fclose(html);
}


/** ------------------------------------------------------------
  * Funcao que pega proxima linha do arquivo
  * @param  arq     - arquivo
  * @param  linha   - ponteiro para a linha
  * @return linha lida
  */
char* acha_linha (FILE* arq,char* linha){
	linha = limpar (linha);
	int cont = 0;
	while (fscanf(arq,"%s",linha) != -1){
        if (cont++ != l)
            linha = limpar(linha);
        else{
            l++;
            return linha;
        }
	}
	fim = 1;
	free(linha);
	return NULL;
}


/** ------------------------------------------------------------
  * Funcao que utiliza thread para estabelecer a conexao entre servidor e clienteS.
  */
void * comunicar (void * newsocketfd){
	int	socket = *(int *)newsocketfd;
	int	id_cliente;
	int n;
   	char* buf = (char*) malloc (256*sizeof(char));
   	if (buf == NULL)
		error ("Memoria insuficiente.");
	FILE*   arq = fopen("arquivo","r");
	if (arq == NULL)
		error("Nao foi possivel abrir o arquivo com os limites.");

	bzero(buf,256);

	n = read (socket,buf,255);
	if (n < 0)
		error ("Erro ao ler do socket.");

	id_cliente = obter_id_do_cliente(buf);

	char str[256];
	bzero(str,256);
	sprintf(str,"Recebendo do cliente %d: %s\n",id_cliente, buf);
	altera_html (str);

	if ((buf[0] == '0') | (buf[0] == '1')){
		char* str2 = construir_mensagem_de_dados_html(buf);
		altera_html(str2);
		free (str2);
	}

	bzero(buf,256);
	buf = acha_linha (arq,buf);
	fclose(arq);

	if (buf == NULL){
		buf = "bye";
		n = write (socket,"bye",3);
		if (n < 0)
			error ("Erro ao escrever no socket.");
		}
		else{
			n = write (socket,buf,get_size(buf));
			if (n < 0)
				error ("Erro ao escrever no socket.");
		}

	bzero (str,256);
	sprintf(str,"Enviando para o cliente %d: %s\n",id_cliente,buf);
	altera_html (str);
	free(buf);
	close (socket);
	return NULL;
}

int main (int argc, char* argv[]){
	if (tp_init () != 0)
		error ("Erro ao iniciar o servidor.");

	int sockfd;
	int newsockfd;
	int n;
	int id_cliente;

	so_addr* from_addr;
	socklen_t from_addr_len;

	pid_t id = getpid ();
	char buf[256];
	pthread_t thread_id;

	FILE* html = fopen("log.html","w");
	if (html == NULL)
		error ("Erro ao criar html.");

	fprintf(html,
        "<html><head><meta http-equiv=\"refresh\" content=\"1\"><title>LOG Comunicacao</title></head><body></br></body></html>");

	fclose(html);

	system("xdg-open log.html");

	if (argc != 2)
		error ("Argumentos incorreto");

	sockfd = tp_socket ((unsigned short)atoi(argv[1]));
	if (sockfd < 0)
		error ("Erro ao criar socket.");

	from_addr_len = sizeof(struct sockaddr_in);

	listen (sockfd,5);

	while (1){
		newsockfd = accept (sockfd,
	  	  (struct sockaddr *) &from_addr,&from_addr_len);
		if (newsockfd < 0)
		error ("Não aceito.");

		n = pthread_create( &thread_id , NULL , comunicar , (void*) &newsockfd);
		if( n < 0)
			error("Nao foi possivel criar a thread");
			pthread_join( thread_id , NULL);
	}

	close (sockfd);
	return 0;
}
