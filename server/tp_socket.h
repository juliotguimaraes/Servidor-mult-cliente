/* tp_socket.h - Interface a ser usada no acesso a sockets UDP, para realizar a comunica��o servidor-cliente. 
*/

#ifndef _TP_SOCKET_H_ 
#define _TP_SOCKET_H_ 

/* tp_init: Deve ser chamada antes de qualquer outra fun��o desta interface. Ela � respons�vel pela
 * configura��o da interface. Retorna zero se tudo estiver certo e 
 * um valor menor que zero em caso de erro.
 */
int tp_init(void);


/* so_addr deve ser considerado como um tipo oculto que n�o precisa ser
 * manipulado diretamente, para simplificar o c�digo. 
 */
typedef struct sockaddr_in so_addr;

/* tp_socket cria um socket;
 * Associado ao par�metro ou o SO escolhido caso port == 0
 */
int tp_socket(unsigned short port);

/* tp_build_addr cria a struct sockaddr_in correspondente ao par
 * (host,porto) indicados pelos par�metros. 
 */
int tp_build_addr(so_addr* addr, char* hostname, int port);

/* tp_sendto � apenas um encapsulamento � chamada sendto da biblioteca de
 * sockets e deve ser usada sempre em seu lugar.
 */
int tp_sendto(int so, char* buff, int buff_len, so_addr* to_addr);

/* tp_recvfrom � a sim�trica da anterior. Nesse caso, from_addr � um
 * buffer que ser� preenchido no retorno da fun��o com o endere�o de
 * origem da mensagem recebida.
 */
int tp_recvfrom(int so, char* buff, int buff_len, so_addr* from_addr);

/* tp_mtu retorna o tamanho m�ximo da mensagem que pode ser enviada
 * atrav�s desta interface. Esse valor varia entre 512 e 2048 bytes
 * e deve ser testado a cada execu��o do protocolo.
 */
int tp_mtu(void);

#endif /* _TP_SOCKET_H_ */
