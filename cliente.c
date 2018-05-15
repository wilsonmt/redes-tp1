#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

//bibliotecas socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define LEN 5

struct sockaddr_in6 remoto;



int main(int argc, char const *argv[])
{
	char nome_ip[200];
	int porta;

	if (argc>=3)
	{
		strcpy(nome_ip, argv[1]);
		porta = atoi(argv[2]);
	}
	else{
		printf("Parametros passados incorretamente...\n");
		return 0;
	}

	int sockfd;
	int len = sizeof(remoto);
	int slen;
	char buffer[LEN];
	struct hostent *server;
	
	sockfd = socket(AF_INET6, SOCK_STREAM, 0);

	printf("Criando socket...\n");
	if(sockfd == -1){
		printf("Erro ao criar socket...\n");
		return 0;
    } 

    printf("Configurando socket...\n");
	server = gethostbyname2(nome_ip, AF_INET6);
	if (server == NULL){
		printf("Convertendo endereco IP...\n");
		char nome_ip_aux[200] = "::ffff:";
		strcat(nome_ip_aux, nome_ip);
		strcpy(nome_ip, nome_ip_aux);
		server = gethostbyname2(nome_ip, AF_INET6);
	}

	if (server == NULL){
		printf("IP incorreto...\n");
		return(1);
	}

	memset((char *) &remoto, 0x0, sizeof(remoto));
	remoto.sin6_flowinfo = 0;
	remoto.sin6_family = AF_INET6;
	memmove((char *) &remoto.sin6_addr.s6_addr, (char *) server->h_addr, server->h_length);
	remoto.sin6_port = htons(porta);

	printf("Conectando ao servidor...\n");

	if(connect(sockfd,(struct sockaddr*)&remoto, len) == - 1){
		printf("Erro ao conectar...\n");
		return 0;
	}

	printf("Concluido!\n\n");

	int meu_campo[11][11];
	int numero_uns_matriz_cliente = monta_matriz_by_arquivo(meu_campo);
	if (numero_uns_matriz_cliente!=30){
		printf("Matriz informada nao corresponde aos padroes definidos...\n");
		return 1;
	}
	imprime_matriz(meu_campo, 0);

	int campo_inimigo[11][11];
	zera_matriz(campo_inimigo);

	int meus_acertos = 0;
	int is_jogo_on = 1, is_primeira_exec = 1;
	int recebido_x, recebido_y, adversario_acertou;
	int meu_ultimo_tiro_x, meu_ultimo_tiro_y;

	while(is_jogo_on){
        if((slen = recv(sockfd,buffer,LEN,0)) > 0) {

			printf("Campo atacado pelo adversario: %c%c\n", buffer[0], buffer[1]);
			if (buffer[0]== 'L'){
				printf("\nVoce perdeu...\n\n");
				break;
			}

            if (is_primeira_exec){
            	is_primeira_exec = 0;
            }
            else if (buffer[2] == 'A'){
				campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y] = 1;
				printf("Voce acertou o ultimo tiro!\n");
				meus_acertos++;
			}
			else if (buffer[2] == 'E'){
				campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y] = -1;
				printf("Voce errou o utimo tiro.\n");
			}

            adversario_acertou = computa_msg_recebida(meu_campo, buffer);			
        }

        if (meus_acertos == 30){
        	is_jogo_on = 0;
        	printf("\nVocê venceu!\n\n");
        }

        while(1){
			memset(buffer, 0x0, LEN);        	
        	if (!is_jogo_on){
   		        buffer[0] = 'L';
        		break;
        	}

        	printf("\n------------------------------------------------\n");
        	do{
				memset(buffer, 0x0, LEN);
				printf("Digite um comando ou o campo que deseja atacar: ");
				fgets(buffer, LEN, stdin); // le mensagem do teclado
        	} while (
        		!((buffer[0]>=65 && buffer[0]<=74
				&& buffer[1]>=48 && buffer[1]<=57)
        		|| buffer[0]=='P' || buffer[0]=='Q'));

	        if (buffer[0]=='P'){
		        imprime_matriz(meu_campo, 0);
		    	imprime_matriz(campo_inimigo, 1);
	        }
	        else if(buffer[0]=='Q'){
	        	printf("\nVoce desistiu do jogo...\n\n");
  	        	is_jogo_on = 0;
  	        	break;
	        }
			else{
				meu_ultimo_tiro_x = buffer[0]-65;
				meu_ultimo_tiro_y = buffer[1]-48;
				if(campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y] == 0)
					break; 
	        }
        }

		if(adversario_acertou)
	    	buffer[2] = 'A';
		else
	    	buffer[2] = 'E';

        send(sockfd, buffer, strlen(buffer), 0);
    }

	return 0;
}