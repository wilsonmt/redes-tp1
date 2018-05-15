#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

//bibliotecas socket
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define LEN 5

struct sockaddr_in6 local;
struct sockaddr_in6 remoto;



int main(int argc, char const *argv[])
{
	int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	int client;
	int len = sizeof(remoto);
	int slen;
	char buffer[LEN];

	printf("Criando socket...\n");
	if(sockfd == -1){ 
		printf("Erro ao criar socket...\n");
		return 0;
	}

	printf("Configurando socket...\n");
	int porta;
	if (argc>=2)
		porta = atoi(argv[1]);
	else{
		printf("Parametros passados incorretamente...\n");
		return 0;
	}
	bzero((char *) &local, sizeof(local));
	local.sin6_flowinfo = 0;
	local.sin6_family = AF_INET6;
	local.sin6_addr = in6addr_any;
	local.sin6_port = htons(porta);

	printf("Atribuindo endereco e porta...\n");
	if(bind(sockfd,(struct sockaddr*)&local, sizeof(local)) == -1){
		printf("Erro ao atribuir endereco e porta...\n");
		return 0;
	}

	printf("Aguardando cliente...\n\n");
	listen(sockfd, 1);

	if((client = accept(sockfd,(struct sockaddr*)&remoto, &len)) == - 1){
		printf("Erro ao aguardar requisicao...\n");
		return 0;
	}

	//monta matrizes 
	int meu_campo[11][11];
	monta_matriz_aleatoria(meu_campo);
	imprime_matriz(meu_campo, 0);
	int campo_inimigo[11][11];
	zera_matriz(campo_inimigo);

	int is_jogo_on = 1;
	int aleatorio_x, aleatorio_y;
	int tiro_recebido_x, tiro_recebido_y;
	int adversario_acertou = 0;
	int meus_acertos = 0;
	int meu_ultimo_tiro_x, meu_ultimo_tiro_y;
	int acertou_ultimo = 0;
	int meu_ultimo_acerto_x, meu_ultimo_acerto_y;
	int iteracoes = 0;

	//loop de comunicação e realização do jogo
	while(is_jogo_on){

		if (meus_acertos == 30)
		{
			buffer[0] = 'L';
			buffer[1] = '\0';
			is_jogo_on = 0;
			printf("\nVoce venceu!\n\n");
		}
		else {
			if (!acertou_ultimo){
				do{
					aleatorio_x = rand()%10;
					aleatorio_y = rand()%10;
				}
				while (campo_inimigo[aleatorio_x][aleatorio_y] != 0);
			}
			else {
				if (meu_ultimo_tiro_y<9 
					&& campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y+1]==0){
					aleatorio_x = meu_ultimo_tiro_x;
					aleatorio_y = meu_ultimo_tiro_y+1;
				}
				else if (meu_ultimo_tiro_y>0
					&& campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y-1]==0){
					aleatorio_x = meu_ultimo_tiro_x;
					aleatorio_y = meu_ultimo_tiro_y-1;
				}
				else if (meu_ultimo_tiro_x<9
					&& campo_inimigo[meu_ultimo_tiro_x+1][meu_ultimo_tiro_y]==0){
					aleatorio_x = meu_ultimo_tiro_x+1;
					aleatorio_y = meu_ultimo_tiro_y;
				}
				else if (meu_ultimo_tiro_x>0
					&& campo_inimigo[meu_ultimo_tiro_x-1][meu_ultimo_tiro_y]==0){
					aleatorio_x = meu_ultimo_tiro_x-1;
					aleatorio_y = meu_ultimo_tiro_y;
				}
				else{
					do{
						aleatorio_x = rand()%10;
						aleatorio_y = rand()%10;
					}
					while (campo_inimigo[aleatorio_x][aleatorio_y] != 0);
				}
			}

			meu_ultimo_tiro_x=aleatorio_x;
			meu_ultimo_tiro_y=aleatorio_y;

			//convertendo para char pela tabela ascii
			buffer[0] = aleatorio_x + '0';
			buffer[0] += 17;
			buffer[1] = aleatorio_y + '0';
			if(adversario_acertou)
				buffer[2] = 'A';
			else
				buffer[2] = 'E';
			buffer[3] = '\0';

        	printf("\n------------------------------------------------\n");
			printf("Campo atacado: %c%c\n", buffer[0], buffer[1]);
			iteracoes++;
		}

		if(send(client, buffer, strlen(buffer), 0)){
			if (!is_jogo_on)
				break;
			
			//laco de espera de mensagem do cliente
            while(1){
                memset(buffer, 0x0, LEN);

                //se recebeu a mensagem
				if((slen = recv(client, buffer, LEN, 0)) > 0){
					buffer[slen] = '\0';

					//trata resposta do meu tiro anterior										
					if (buffer[0] == 'Q'){
						is_jogo_on = 0;
						printf("\nO adversario desistiu, voce venceu!\n\n");
					}
					else {
						printf("Campo atacado pelo adversario: %c%c\n", buffer[0], buffer[1]);				
					
						if (buffer[0] == 'L'){
							is_jogo_on = 0;
							printf("\nVoce perdeu...\n\n");
							break;
						}
						else if (buffer[2] == 'A'){
							campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y] = 1;
							meus_acertos++;
							acertou_ultimo = 1;
							meu_ultimo_acerto_x = meu_ultimo_tiro_x;
							meu_ultimo_acerto_y = meu_ultimo_tiro_y;
							printf("Voce acertou o ultimo tiro!\n");
						}
						else if (buffer[2] == 'E'){
							campo_inimigo[meu_ultimo_tiro_x][meu_ultimo_tiro_y] = -1;
							acertou_ultimo = 0;
							printf("Voce errou o utimo tiro.\n");
						}

						if (iteracoes==5){
							imprime_matriz(meu_campo, 0);
							imprime_matriz(campo_inimigo, 1);
							iteracoes = 0;
						}
					}

					//trata tiro do adversario
					adversario_acertou = computa_msg_recebida(meu_campo, buffer);
					memset(buffer, 0x0, LEN);
					break;
                }
            }
        }
	}

	// encerra conexao com cliente
	close(client);

	return 0;
}