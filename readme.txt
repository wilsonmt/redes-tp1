
 Para compilar e executar o jogo, seguir os seguintes comandos:

  make
  ./servidor <porta>
  ./cliente <ip> <porta>


 Após isso, o jogo estará em funcionamento. Algumas observações:

  -> O  servidor joga contra o cliente e possui sua matriz e suas
	jogadas definidas internamente de forma aleatória
	(com funcionalidades extras descritas na documentação)

  -> A matriz do cliente é definida pelo arquivo
	entrada_cliente.txt contido na pasta entrada/

  -> Ao acessar como cliente, deve-se inserir o campo do adversário
	a ser atacado. O campo é descrito por uma sequência de
	uma letra e um número
	
	-> letra de A a J (sempre maiúscula)
	-> número de 0 a 9
	   -> EX: B4

  -> O servidor enviará respostas informando o erro ou acerto do
	último tiro, um novo campo a ser atacado deve ser informado.

  -> O jogo segue sucessivamente até o momento em que um dos lados
	é completamente destruído. Nesse momento uma mensagem
	é exibida na tela, informando o resultado da partida.

  -> É possível imprimir as matrizes do lado do cliente
	com o comando P+[ENTER]

  -> É possível desistir da partida a qualquer momento
	com o comando Q+[ENTER]


 Boa partida!

