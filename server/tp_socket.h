/* tp_socket.h - Interface a ser usada no acesso a sockets UDP, para realizar a comunicação servidor-cliente. 
*/

#ifndef _TP_SOCKET_H_ 
#define _TP_SOCKET_H_ 

/* tp_init: Deve ser chamada antes de qualquer outra função desta interface. Ela é responsável pela
 * configuração da interface. Retorna zero se tudo estiver certo e 
 * um valor menor que zero em caso de erro.
 */
int tp_init(void);


/* so_addr deve ser considerado como um tipo oculto que não precisa ser
 * manipulado diretamente, para simplificar o código. 
 */
typedef struct sockaddr_in so_addr;

/* tp_socket cria um socket;
 * Associado ao parâmetro ou o SO escolhido caso port == 0
 */
int tp_socket(unsigned short port);

/* tp_build_addr cria a struct sockaddr_in correspondente ao par
 * (host,porto) indicados pelos parâmetros. 
 */
int tp_build_addr(so_addr* addr, char* hostname, int port);

/* tp_sendto é apenas um encapsulamento à chamada sendto da biblioteca de
 * sockets e deve ser usada sempre em seu lugar.
 */
int tp_sendto(int so, char* buff, int buff_len, so_addr* to_addr);

/* tp_recvfrom é a simétrica da anterior. Nesse caso, from_addr é um
 * buffer que será preenchido no retorno da função com o endereço de
 * origem da mensagem recebida.
 */
int tp_recvfrom(int so, char* buff, int buff_len, so_addr* from_addr);

/* tp_mtu retorna o tamanho máximo da mensagem que pode ser enviada
 * através desta interface. Esse valor varia entre 512 e 2048 bytes
 * e deve ser testado a cada execução do protocolo.
 */
int tp_mtu(void);

#endif /* _TP_SOCKET_H_ */
