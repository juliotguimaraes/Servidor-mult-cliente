#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <math.h>
#include "io.h"
#include "tp_socket.h"



/** ------------------------------------------------------------
  * Realiza potencias com long long e retorna o resultado
  * @param  b       - base
  * @param  e       - expoente
  * @return result  - resultado
  */
unsigned long long pot ( int b, int e ){
  unsigned long long base = (unsigned long long) b;
  unsigned long long exp = (unsigned long long) e;
  unsigned long long result = 1ULL;

  while(exp){
    if ( exp & 1 ){
      result *= base;
    }
    exp >>= 1;
    base *= base;
  }
  return result;
}


/** ------------------------------------------------------------
  * Verifica se um dado numero e' primo ou nao
  * @param  nb  - numero a ser verificado
  * @return se o numero e' primo ou nao
  */
int ehPrimo (int nb){
  int limite;
  int i;

  limite = obter_raiz(nb);

  if (nb == 2){
    return 1;
  }
  else if ((nb > 2) && (nb % 2 != 0)){
    for (i = 3 ; i <= limite; i+=2)
      if (nb % i == 0)
        return 0;

    return 1;
  }
  return 0;
}


/** ------------------------------------------------------------
  * Obtem a raiz (inteira) de um dado numero
  * @param  nb  - numero do qual a raiz sera tirada
  * @return raiz do numero
  */
int obter_raiz (int nb){
  int i = 1;
  while ((i*i) < nb)
    i++;
  i--;
  return i;
}


/** ------------------------------------------------------------
  * Verifica se a equacao da conjectura e' valida
  * @param  ib  - base A
  * @param  jb  - base B
  * @param  kb  - base C
  * @param  ie  - expoente X
  * @param  je  - expoente J
  * @param  ke  - expoente Z
  * @return se A^X + B^Y = C^Z
  */
int verifica_equacao(int ib,int jb,int kb, int ie,int je,int ke){
  unsigned long long int p1 = pot(ib,ie);
  unsigned long long int p2 = pot(jb,je);
  unsigned long long int p3 = pot(kb,ke);
  if (p1 <= 0ULL || p2 <= 0ULL || p3 <= 0ULL)
    return 0;
  unsigned long long int resp1 = p1 + p2;
  int resp2 = resp1 == p3;
  return resp2;
}


/** ------------------------------------------------------------
  * Verifica se as bases da conjectura possuem primo em comum
  * @param  ib  - base A
  * @param  jb  - base B
  * @param  kb  - base C
  * @return se A, B e C possuem primo em comum
  */
int possui_primo_em_comum (int bi, int bj, int bk){
  int x;
  int contar = 0;
  int menor = obter_menor (bi,bj,bk);
  int primos_do_menor[menor];


  for (x = 2 ; x <= menor ; x++)
    if (ehPrimo(x))
      primos_do_menor[contar++] = x;

  for (x = 0 ; x < contar ; x++){
    if ((bi%primos_do_menor[x] == 0) & (bj%primos_do_menor[x] == 0) & (bk%primos_do_menor[x] == 0))
                  return 1;
  }
  return 0;
}


/** ------------------------------------------------------------
  * Dadas as 3 bases, obtem a menor delas
  * @param  i   - base A
  * @param  j   - base B
  * @param  k   - base C
  * @return menor base
  */
int obter_menor (int i, int j, int k){
  if (i <= j && i <= k)
    return i;
  if (j <= i && j <= k)
    return j;
  return k;
}

void p (int ib,int ie,int jb, int je,int kb,int ke){
  printf("%d^%d + %d^%d = %d^%d\n",ib,ie,jb,je,kb,ke);
}


/** ------------------------------------------------------------
  * Tenta obter contra exemplo
  * @param  limites - vetor de inteiros com limites
  * @return se encontrou um contra exemplo ou nao
  */
int tentar_contra_exemplo (int* limites){
  int encontrou = 0;

  FILE* contra_exemplos;
  char filename[255];
  int ib;
  int jb;
  int kb;
  int ie;
  int je;
  int ke;
  int iniciob = limites[0];
  int fimb = limites[1];
  int inicioe = limites[2];
  int fime = limites[3];
  for (ib = iniciob ; ib <= fimb ; ib++)
    for (jb = ib ; jb <= fimb ; jb++)
      for (kb = jb ; kb <= fimb ; kb++)
        for (ie = inicioe ; ie <= fime ; ie++)
          for (je = inicioe ; je <= fime ; je++)
            for (ke = inicioe ; ke <= fime ; ke++)
              if (verifica_equacao (ib,jb,kb,ie,je,ke))
                if (!possui_primo_em_comum(ib,jb,kb))
                  return 1;

  return encontrou;
}


/** ------------------------------------------------------------
  * Principal
  * argv = <host_do_servidor><porto_do_servidor>
  */
int main (int argc, char* argv[]){
  if (tp_init() != 0)
    error ("Erro ao iniciar o cliente.");

  int sockfd;			//socket
  int n;	        //variavel auxiliar
  int estado = 0;     // 0 - nao encontrou
                      // 1 - encontrou
                      // 2 - primeira vez

  int* limites;               // vetor com os limites

  char* buf;                  // buffer

  so_addr		to_addr;		// endereco de destino
  so_addr*	from_addr;		// endereco de origem

  pid_t id = getpid ();	// id do cliente


  limites = (int*)malloc(4*sizeof(int));
  if (limites == NULL)
    error("Memoria insuficiente.");

  buf = (char*) malloc (256*sizeof(char));
    if (buf == NULL)
      error ("Memoria insuficiente.");

  if (argc != 3)
    error ("A quantidade de argumentos esta' incorreta.");

  while (1){
    sockfd = socket (PF_INET,SOCK_STREAM,0);
    if (sockfd < 0)
      error ("Erro ao criar socket.");

    n = tp_build_addr (&to_addr,argv[1],atoi(argv[2]));
    if (n < 0)
      error ("Erro ao criar addr.");

    n = connect (sockfd,(struct sockaddr*) &to_addr, sizeof(struct sockaddr_in));
    if (n < 0)
      error ("Erro na conexao.");

    fprintf (stderr,"Conectado...\n");

    bzero (buf,256);
    buf = construir_mensagem_de_dados(estado,id,limites,buf);
    n = write (sockfd,buf,get_size(buf));
    if (n < 0)
      error ("Erro ao escrever no socket.");
    fprintf(stderr,"Enviou: %s\n",buf);

    bzero(buf,256);
    n = read (sockfd,buf,255);
    if (n < 0)
      error ("Erro ao ler do socket.");
    fprintf(stderr,"Valores recebidos: %s\n",buf);
    
    close (sockfd);

    if (is_bye(buf)){
      fprintf(stderr,"Fim\n");
      return 0;
    }

    fprintf(stderr,"Calculando...%d\n",id);

    limites = obter_limites_de_mensagem(buf,limites);

    estado = tentar_contra_exemplo(limites);

    fprintf(stderr,"Consegui: %d\n",estado);

  }
  free(limites);
  free(buf);
  return 0;
}