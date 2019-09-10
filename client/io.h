
/** ------------------------------------------------------------
  * Dependencias
  */
	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>
	#include <stdlib.h>


/** ------------------------------------------------------------
  * Definicoes
  */
	#define MAX_LENGTH 256


/** ------------------------------------------------------------
  * Print de erro
  */
	void error (const char *msg);


/** ------------------------------------------------------------
  * Obtem o tamanho de um vetor de caractere
  * @param	cadeia	- vetor a ser conferido
  * @return tam		- tamanho do vetor
  */
	int get_size (char* cadeia);


/** ------------------------------------------------------------
  * Insere uma cadeia de caracteres em uma str
  * @param	cadeia 	- cadea a ser inserida
  * @param	posicao	- posicao onde a cadeia sera inserida
  * @param  atual   - cadeia na qual a outra sera inserida
  * @param  atual   - cadeia modificada
  */
	char* insere_cadeia (char* atual,char* cadeia,int posicao);


/** ------------------------------------------------------------
  * Insere um caractere em uma str
  * @param	str	    - cadeia atual
  * @param	c 	    - caractere a ser inserido
  * @param  posicao - posicao onde o caractere sera inserido
  * @return str	    - Str atualizada
  */
	char* insere_char (char* str,char c,int posicao);


/** ------------------------------------------------------------
  * Insere um inteiro em uma str
  * @param	str	    - Str atual
  * @param	num     - inteiro a ser inserido
  * @param  posicao - posicao onde o inteiro sera inserido
  * @return str	    - Str atualizada
  */
	char* insere_int (char* str,int num,int posicao);


/** ------------------------------------------------------------
  * Compara uma str com a outra. (sensitive)
  * @param	str1	- Str a ser comparada
  * @param 	str2	- Str a ser comparada
  * @return Se as str's sao iguais ou nao
  */
	int compara (char* str1,char* str2);


/** ------------------------------------------------------------
  * Compara uma str com a outra. (insensitive)
  * @param	str1	- Str a ser comparada
  * @param 	str2	- Str a ser comparada
  * @return Se as str's sao iguais ou nao
  */
	int compara_insensitive (char* str1,char* str2);


/** ------------------------------------------------------------
  * Limpa uma str
  * @param	str - Str a ser limpa
  * @return str limpa
  */
    char* limpar (char* str);


/** ------------------------------------------------------------
  * Constroi uma mensagem ack+limites
  * @param	buf     - buffer
  * @param  limites - limites
  * @return buffer com ack + limites
  */
    char* construir_mensagem_com_limites (char* buf,char* limites);


/** ------------------------------------------------------------
  * Constroi uma mensagem com os dados do cliente
  * @param  estado  [0 - n encontrou, 1 - encontrou, 2 - 1a vez]
  * @param  id      - id do cliente
  * @param  limites - limites
  * @param	buf     - buffer
  * @return buffer com dados a serem enviados para o servidor
  */
    char* construir_mensagem_de_dados (int estado,int id,
                                       int* limites,char* buf);


/** ------------------------------------------------------------
  * Constroi uma mensagem de retorno do cliente para ser
  * ser exibido na pagina html
  * @param  buf - mensagem de retorno do cliente
  * @return msg - mensagem a ser exibida na pagian html
  */
    char* construir_mensagem_de_dados_html (char* buf);



/** ------------------------------------------------------------
  * Obtem o id do cliente dado a mensagem do cliente
  * @param	buf     - buffer (estado;id;limites)
  * @return id do cliente
  */
    int obter_id_do_cliente (char* buf);


/** ------------------------------------------------------------
  * Extrai os limites de uma mensagem
  * @param	buf     - buffer
  * @param  limites - array para os limites
  * @return limites - array com limites
  */
    int* obter_limites_de_mensagem (char* buf,int* limites);


/** ------------------------------------------------------------
  * Verifica se a mensagem equivale a 'bye'
  * @param	buf     - buffer
  * @return se equivale ou nao
  */
	int is_bye (char* buf);
