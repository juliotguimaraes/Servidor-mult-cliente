#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

void error (const char *msg);
int get_size (char* cadeia);
char* insere_cadeia (char* atual,char* cadeia,int posicao);
char* insere_char (char* str,char c,int posicao);
char* insere_int (char* str,int num,int posicao);
int compara (char* str1,char* str2);
int compara_insensitive (char* str1,char* str2);
char* limpar (char* str);
char* construir_mensagem_com_limites (char* buf,char* limites);
char* construir_mensagem_de_dados (int estado,int id, int* limites,char* buf);
char* construir_mensagem_de_dados_html (char* buf);
int obter_id_do_cliente (char* buf);
int* obter_limites_de_mensagem (char* buf,int* limites);
int is_bye (char* buf);
