#include "io.h"

void error (const char *msg){
  
  perror(msg);
  
  exit(0);
}


int get_size (char* cadeia){

  int i;
  int length = 0;

  for (i = 0;cadeia[i] != 0;i++)
    length++;

  return length;
}

char* insere_cadeia (char* atual,char* cadeia,int posicao){

  int	i = 0;

  while (i < get_size(atual))
    atual[posicao++] = cadeia[i++];

  atual[posicao] = 0;

  return atual;
}

char* insere_char (char* str,char c,int posicao){

  str[posicao] = c;

  return str;
}

char* insere_int (char* str,int num,int posicao){

  char*   num_convertido = (char*)malloc(256*sizeof(char));

  num_convertido = limpar(num_convertido);

  snprintf(num_convertido,255,"%d",num);

  str = insere_cadeia(str,num_convertido,posicao);

  free(num_convertido);

  return str;
}

int compara (char* str1,char* str2){

  int	i = 0;

  if (get_size(str1) != get_size(str2))
    return 0;

  while (i < get_size(str1))
    if (str1[i] != str2[i])
      return 0;

  return 1;
}

int compara_insensitive (char* str1,char* str2){
		
  int		i = 0;
  char	r1;
  char	r2;

  if (get_size(str1) != get_size(str2))
    return 0;

  while (i < get_size(str1)){

    r1 = tolower (str1[i]);
    r2 = tolower (str2[i]);

    if (r1 != r2)
      return 0;

  }

  return 1;
}

char* limpar (char* str){
  int i = 0;
  while (i < 256)
    str[i++] = 0;
  return str;
}

char* construir_mensagem_de_dados (int estado,int id, int* limites,char* buf){
  
  int i;
  limpar(buf);
  sprintf(buf,"%d;%d",estado,id);

  if ((estado == 0) | (estado == 1)){

    for (i = 0; i < 4; i++){

      buf = insere_char(buf,';',get_size(buf));
      buf = insere_int(buf,limites[i],get_size(buf));
    }
  }

  return buf;
}

char* construir_mensagem_de_dados_html (char* buf){
  
  int i;
  int id;
  int cont;

  int* limites;
  char* char_temp;
  char* msg;

  if (buf[0] == '2')
    return NULL;

  limites = (int*)malloc(4*sizeof(int));
  char_temp = (char*)malloc(256*sizeof(char));
  msg = (char*)malloc(256*sizeof(char));

  if ((msg == NULL) | (char_temp == NULL) | (limites == NULL))
    error("Memoria insuficiente.");

  msg = limpar(msg);
  char_temp = limpar(char_temp);

  /* Obter ID do cliente */
  i = 2;
  cont = 0;
  
  while (buf[i] != ';'){
  
    char_temp = insere_char(char_temp,buf[i],cont++);
    i++;
  }

  id = atoi (char_temp);

  char_temp = limpar(char_temp);
  i++;
  cont = 0;
  while (i < get_size(buf)){
  
    char_temp = insere_char(char_temp,buf[i],cont++);
    i++;
  }
  limites = obter_limites_de_mensagem(char_temp,limites);

  free (char_temp);

  /* Montar mensagem */
  if (buf[0] == 1){
    sprintf(msg,
    "Cliente %d: SUCESSO no intervalo Bmin = %d, Bmax = %d, Emin = %d, Emax = %d",
    id,limites[0],limites[1],limites[2],limites[3]);
  }
  else{
    sprintf(msg,
    "Cliente %d: NAO conseguiu no intervalo Bmin = %d, Bmax = %d, Emin = %d, Emax = %d",
    id,limites[0],limites[1],limites[2],limites[3]);
  }

  free (limites);

  return msg;
}

int obter_id_do_cliente (char* buf){
  int i;
  int id;
  int cont;
  char* char_temp = (char*)malloc(256*sizeof(char));
  if (char_temp == NULL)
    error("Memoria insuficiente.");

  i = 2;
  cont = 0;
  char_temp = limpar(char_temp);
  while (buf[i] != ';'){
    char_temp = insere_char(char_temp,buf[i],cont++);
    i++;
  }

  id = atoi (char_temp);
  free (char_temp);

  return id;
}

int* obter_limites_de_mensagem (char* buf,int* limites){
  int i = 0;
  int j = 0;
  int cont = 0;
  char* num = (char*)malloc(256*sizeof(char));
  num = limpar(num);

  while (i < get_size(buf)){
    if (buf[i] != ';')
      num = insere_char(num,buf[i],cont++);
    else{
      limites[j++] = atoi (num);
      cont = 0;
      num = limpar(num);
    }
    i++;
  }
  limites[j++] = atoi (num);

  free (num);
  return limites;
}

int is_bye (char* buf){
		
  int resp = 1;
  resp &= buf[0] == 'b';
  resp &= buf[1] == 'y';
  resp &= buf[2] == 'e';
  return resp;
}