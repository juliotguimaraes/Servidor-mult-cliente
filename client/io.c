
/** ------------------------------------------------------------
  * Dependencias
  */
	#include "io.h"


/** ------------------------------------------------------------
  * Print de erro
  */
	void error (const char *msg)
	{
		perror(msg);
		exit(0);
	}


/** ------------------------------------------------------------
  * Obtem o tamanho de um vetor de caractere
  * @param	cadeia	- vetor a ser conferido
  * @return tam		- tamanho do vetor
  */
	int get_size (char* cadeia)
	{
		int i;
		int length = 0;
		for (i = 0;cadeia[i] != 0;i++)
			length++;
		return length;
	}


/** ------------------------------------------------------------
  * Insere uma cadeia de caracteres em uma str
  * @param	cadeia 	- cadea a ser inserida
  * @param	posicao	- posicao onde a cadeia sera inserida
  * @param  atual   - cadeia na qual a outra sera inserida
  * @param  atual   - cadeia modificada
  */
	char* insere_cadeia (char* atual,char* cadeia,int posicao)
	{
		int	i = 0;
		while (i < get_size(atual))
			atual[posicao++] = cadeia[i++];
		atual[posicao] = 0;
		return atual;
	}


/** ------------------------------------------------------------
  * Insere um caractere em uma str
  * @param	str	    - cadeia atual
  * @param	c 	    - caractere a ser inserido
  * @param  posicao - posicao onde o caractere sera inserido
  * @return str	    - Str atualizada
  */
	char* insere_char (char* str,char c,int posicao)
	{
		str[posicao] = c;
		return str;
	}


/** ------------------------------------------------------------
  * Insere um inteiro em uma str
  * @param	str	    - Str atual
  * @param	num     - inteiro a ser inserido
  * @param  posicao - posicao onde o inteiro sera inserido
  * @return str	    - Str atualizada
  */
	char* insere_int (char* str,int num,int posicao)
	{
		char*   num_convertido = (char*)malloc(256*sizeof(char));
		num_convertido = limpar(num_convertido);
		snprintf(num_convertido,255,"%d",num);
		str = insere_cadeia(str,num_convertido,posicao);
		free(num_convertido);
		return str;
	}


/** ------------------------------------------------------------
  * Compara uma str com a outra. (sensitive)
  * @param	str1	- Str a ser comparada
  * @param 	str2	- Str a ser comparada
  * @return Se as str's sao iguais ou nao
  */
	int compara (char* str1,char* str2)
	{
		int	i = 0;
		if (get_size(str1) != get_size(str2))
			return 0;
		while (i < get_size(str1))
            if (str1[i] != str2[i])
                return 0;
		return 1;
	}


/** ------------------------------------------------------------
  * Compara uma str com a outra. (insensitive)
  * @param	str1	- Str a ser comparada
  * @param 	str2	- Str a ser comparada
  * @return Se as str's sao iguais ou nao
  */
	int compara_insensitive (char* str1,char* str2)
	{
		int		i = 0;
		char	r1;
		char	r2;
		if (get_size(str1) != get_size(str2))
			return 0;
		while (i < get_size(str1))
		{
			r1 = tolower (str1[i]);
			r2 = tolower (str2[i]);
			if (r1 != r2)
                return 0;
		}
		return 1;
	}


/** ------------------------------------------------------------
  * Limpa uma str
  * @param	str - Str a ser limpa
  * @return str limpa
  */
    char* limpar (char* str)
    {
        int i = 0;
        while (i < 256)
            str[i++] = 0;
        return str;
    }


/** ------------------------------------------------------------
  * Constroi uma mensagem com os dados do cliente
  * @param  estado  [0 - n encontrou, 1 - encontrou, 2 - 1a vez]
  * @param  id      - id do cliente
  * @param  limites - limites
  * @param	buf     - buffer
  * @return buffer com dados a serem enviados para o servidor
  */
    char* construir_mensagem_de_dados (int estado,int id,
                                       int* limites,char* buf)
    {
		int i;
        limpar(buf);
        sprintf(buf,"%d;%d",estado,id);
        if ((estado == 0) | (estado == 1))
        {
			for (i = 0; i < 4; i++)
			{
				buf = insere_char(buf,';',get_size(buf));
				buf = insere_int(buf,limites[i],get_size(buf));
			}
        }
        return buf;
    }


/** ------------------------------------------------------------
  * Constroi uma mensagem de retorno do cliente para ser
  * ser exibido na pagina html
  * @param  buf - mensagem de retorno do cliente
  * @return msg - mensagem a ser exibida na pagian html
  */
    char* construir_mensagem_de_dados_html (char* buf)
    {
        int     i;
        int     id;
        int     cont;

        int*    limites;
        char*   char_temp;
        char*   msg;

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
        while (buf[i] != ';')
        {
            char_temp = insere_char(char_temp,buf[i],cont++);
            i++;
        }

        id = atoi (char_temp);

        /* Obter limites */
        char_temp = limpar(char_temp);
        i++;
        cont = 0;
        while (i < get_size(buf))
        {
            char_temp = insere_char(char_temp,buf[i],cont++);
            i++;
        }
        limites = obter_limites_de_mensagem(char_temp,limites);

        free (char_temp);

        /* Montar mensagem */
        if (buf[0] == 1)
        {
            sprintf(msg,
            "O cliente %d OBTEVE sucesso no intervalo Bmin = %d, Bmax = %d, Emin = %d, Emax = %d",
            id,limites[0],limites[1],limites[2],limites[3]);
        }
        else
        {
            sprintf(msg,
            "O cliente %d NAO obteve sucesso no intervalo Bmin = %d, Bmax = %d, Emin = %d, Emax = %d",
            id,limites[0],limites[1],limites[2],limites[3]);
        }

        free (limites);

        return msg;
    }


/** ------------------------------------------------------------
  * Obtem o id do cliente dado a mensagem do cliente
  * @param	buf     - buffer (estado;id;limites)
  * @return id do cliente
  */
    int obter_id_do_cliente (char* buf)
    {
        int     i;
        int     id;
        int     cont;
        char*   char_temp = (char*)malloc(256*sizeof(char));
        if (char_temp == NULL)
            error("Memoria insuficiente.");

        /* Obter ID do cliente */
        i = 2;
        cont = 0;
        char_temp = limpar(char_temp);
        while (buf[i] != ';')
        {
            char_temp = insere_char(char_temp,buf[i],cont++);
            i++;
        }

        id = atoi (char_temp);
        free (char_temp);

        return id;
    }


/** ------------------------------------------------------------
  * Extrai os limites de uma mensagem
  * @param	buf     - buffer
  * @param  limites - array para os limites
  * @return limites - array com limites
  */
    int* obter_limites_de_mensagem (char* buf,int* limites)
    {
        int     i = 0;
        int     j = 0;
        int     cont = 0;
        char*    num = (char*)malloc(256*sizeof(char));
        num = limpar(num);

        while (i < get_size(buf))
		{
			if (buf[i] != ';')
            	num = insere_char(num,buf[i],cont++);
			else
			{
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


/** ------------------------------------------------------------
  * Verifica se a mensagem equivale a 'bye'
  * @param	buf     - buffer
  * @return se equivale ou nao
  */
	int is_bye (char* buf)
	{
		int resp = 1;
		resp &= buf[0] == 'b';
		resp &= buf[1] == 'y';
		resp &= buf[2] == 'e';
		return resp;
	}
