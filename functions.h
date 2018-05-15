#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 5

/*
  MONTAGEM DE MATRIZ ALEATORIA
  	USADA NO SERVIDOR */

		/*
		  insercao de item aleatorio na matriz
		  	com tamanho e quantidade definidos */
		void insere_item_na_matriz_aleatoria(int matriz[11][11], 
				int tam_item, int quant_item);

		/*
		  montagem da matriz */
		void monta_matriz_aleatoria(int matriz[11][11]);


/*
  MONTAGEM DE MATRIZ ATRAVES DE ARQUIVO
  	USADA NO CLIENTE */

		/*
		  montagem da matriz a partir da leitura de um arquivo */
		int monta_matriz_by_arquivo(int matriz[11][11]);


/*
  METODOS GERAIS DE MATRIZ 
  	USADA EM AMBOS */

		/*
		  zera matriz */
		void zera_matriz(int matriz[11][11]);

		/*
		  imprime matriz */
		void imprime_matriz(int matriz[11][11], int tipo);


/*
  METODO DE COMPUTAR SE TIRO ACERTOU CORRETAMENTE
  	USADA EM AMBOS */
		int computa_msg_recebida(int meu_campo[11][11], 
				char buffer[LEN]);