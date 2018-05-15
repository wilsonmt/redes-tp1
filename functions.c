#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functions.h"

#define MAX_TAM 100
#define LEN 5


/*
  MONTAGEM DE MATRIZ ALEATORIA
  	USADA NO SERVIDOR */

		/*
		  insercao de item aleatorio na matriz
		  	com tamanho e quantidade definidos */
		void insere_item_na_matriz_aleatoria(int matriz[11][11], 
				int tam_item, int quant_item){
			int is_horizontal, is_verificado = 0, is_ok = 0;
			float rand_aux;
			int rand_x, rand_y;

			for (int i=0; i<quant_item; i++){
				is_verificado = 0;
				while(is_verificado == 0){
					rand_aux = rand()%2;
					is_horizontal = rand_aux;
					if (is_horizontal){
						rand_aux = rand()%(tam_item+1);
						rand_x = rand_aux;
						rand_aux = rand()%10;
						rand_y = rand_aux;
					}
					else {
						rand_aux = rand()%10;
						rand_x = rand_aux;
						rand_aux = rand()%(tam_item+1);
						rand_y = rand_aux;
					}

					is_ok = 1;
					if (is_horizontal){
						for(int i=rand_x; i<rand_x+tam_item; i++){
							if (matriz[rand_y][i] == 1)
								is_ok = 0;
						}
						if(is_ok == 1){
							is_verificado = 1;
							for(int i=rand_x; i<rand_x+tam_item; i++)
								matriz[rand_y][i] = 1;
						}
					}
					else {
						for(int i=rand_y; i<rand_y+tam_item; i++){
							if (matriz[i][rand_x] == 1)
								is_ok = 0;
						}
						if(is_ok == 1){
							is_verificado = 1;
							for(int i=rand_y; i<rand_y+tam_item; i++)
								matriz[i][rand_x] = 1;
						}
					}
				}		
			} 
		}

		/*
		  montagem da matriz */
		void monta_matriz_aleatoria(int matriz[11][11]){
			
			//zera matriz
			for (int i=0; i<10; i++) {
				for(int j=0; j<10; j++)
					matriz[i][j] = 0;
			}

			srand(time(NULL));

			insere_item_na_matriz_aleatoria(matriz, 5, 1);
			insere_item_na_matriz_aleatoria(matriz, 4, 2);
			insere_item_na_matriz_aleatoria(matriz, 3, 3);
			insere_item_na_matriz_aleatoria(matriz, 2, 4);
		}


/*
  MONTAGEM DE MATRIZ ATRAVES DE ARQUIVO
  	USADA NO CLIENTE */

		/*
		  montagem da matriz a partir da leitura de um arquivo */
		int monta_matriz_by_arquivo(int matriz[11][11]){
			char * linha;
			char buf[MAX_TAM];
			FILE *arq;
			int numero_uns = 0;

			arq = fopen("entrada/entrada_cliente.txt", "r");
			if (arq==NULL){
				printf("Nao foi possivel abrir o arquivo");
			}
			else{
				for (int i=0; i<10; i++) {
					fgets(buf, MAX_TAM, arq);
					linha = strtok(buf, "\n\0,");

					for(int j=0; j<10; j++){
						matriz[i][j] = linha[j]-48;
						if (matriz[i][j] == 1)
							numero_uns++;
					}		
				}
				fclose(arq);
			}
			return numero_uns;
		}


/*
  METODOS GERAIS DE MATRIZ 
  	USADA EM AMBOS */

		/*
		  zera matriz */
		void zera_matriz(int matriz[11][11]){
			for(int i=0; i<10; i++){
				for(int j=0; j<10; j++)
					matriz[i][j] = 0;
			}
		}

		/*
		  imprime matriz */
		void imprime_matriz(int matriz[11][11], int tipo){
			if (tipo==0)
				printf("\nSua base naval: \n\n");
			else if (tipo==1)
				printf("\nBase naval adversaria: \n\n");

			printf("   [0][1][2][3][4][5][6][7][8][9]\n");

			char linha;

			for (int i=0; i<10; i++) {
				linha = i + 65;
				printf("[%c]", linha);
				
				for(int j=0; j<10; j++){
					if (matriz[i][j]==-1)
						printf("%d ", matriz[i][j]);
					else
						printf(" %d ", matriz[i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}


/*
  METODO DE COMPUTAR SE TIRO ACERTOU CORRETAMENTE
  	USADA EM AMBOS */
		int computa_msg_recebida(int meu_campo[11][11], char buffer[LEN])
		{
			int recebido_x, recebido_y;
			recebido_x = buffer[0]-65;
            recebido_y = buffer[1]-48;

            if (meu_campo[recebido_x][recebido_y])
            	return 1;
            else
            	return 0;
		}