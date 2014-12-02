/***************************************************************************
*
*  $MCD Módulo de definição: GRF Grafo não orientado
*
*  Arquivo gerado:              GRAFO.h
*  Letras identificadoras:      GRF
*
*  Projeto: INF 1301 Automatização dos testes de módulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     1      csgslb 23/set/2014 início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa grafos não orientados utilizando listas 
*	  genéricas duplamente encadeadas.
*     Podem existir n grafos em operação simultaneamente.
*     O grafo possui uma cabeça encapsulando o seu estado.
*	  Cada vértice possui uma chave que o identifica. A chave
*	  tem de ser um número inteiro não negativo. Dois vértices
*	  não podem ter a mesma chave.
*
*
*
*************************************************************************/


/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um grafo */

typedef struct GRF_tagGrafo * GRF_tppGrafo ;

/***********************************************************************
*
*  $TC Tipo de dados: GRF Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções da lista
*
***********************************************************************/

   typedef enum {

         GRF_CondRetOK ,
               /* Concluiu corretamente */

		 GRF_CondRetVerticeJaExiste ,
		       /* O vertice ja existe no grafo */

         GRF_CondRetVerticeNaoExiste,
               /* o vertice nao existe no grafo*/

         GRF_CondRetArestaJaExiste ,
               /* a aresta ja existe no grafo */

         GRF_CondRetArestaNaoExiste,
               /* a aresta nao existe */

		 GRF_CondRetGrafoNaoExiste ,
			   /* O grafo nao existe */

         GRF_CondRetFaltouMemoria ,
               /* Faltou memória ao tentar criar um elemento do grafo */

		 GRF_CondRetErroAoObterValor,
		       /*erro ao obter algum valor do grafo */

		 GRF_CondRetVerticesDesconexos,
		       /* nao existe caminho entre os vertices */

		 GRF_CondRetErroInserirVertice,
		      /* erro ao inserir o vértice */

		 GRF_CondRetErroInserirAresta,
			  /* erro ao inserir a aresta */

         GRF_CondRetErroEstrutura
			/* erro na estrutura do grafo*/

		 
   } GRF_tpCondRet ;


/***********************************************************************
*
*  $TC Tipo de dados: GRF Modos de deturpar
*
*
***********************************************************************/

#ifdef _DEBUG

   typedef enum {

         DeturpaEliminaCorrente       =  1 ,
               /* Elimina o elemento corrente da estrutura escolhida */

         DeturpaSucessorNulo          =  2 ,
               /* Atribui NULL ao ponteiro para um vértice sucessor */

         DeturpaPredecessorNulo       =  3 ,
               /* Atribui NULL ao ponteiro para um vértice predecessor */

         DeturpaSucessorLixo          =  4 ,
               /* Atribui lixo ao ponteiro para a referência a um vértice sucessor */

         DeturpaPredecessorLixo       =  5 ,
               /* Atribui lixo ao ponteiro para a referência a um vértice predecessor */

         DeturpaValorNulo             =  6 ,
               /* Atribui NULL ao ponteiro para o conteúdo do vértice */

         DeturpaTipoEstrutura         =  7 ,
               /* Altera o tipo de estrutura apontado no vértice */

         DeturpaDestacaVertice        =  8 ,
               /* Destaca vértice do grafo sem liberá-lo com free */

         DeturpaCorrenteNulo          =  9 ,
               /* Atribui NULL ao ponteiro corrente */


   } GRF_tpModosDeturpacao ;

   /* contadores */


#define CONTADOR_FALHAS_ESTRUTURA "FalhasEstrutura"

#endif


/***********************************************************************
*
*  $FC Função: GRF  &Criar Grafo
*
*  $ED Descrição da função
*		Cria um Grafo genérico e não direcionado, com uma lista duplamente encadeada
*		para seus vértices e uma para suas origens.
*		Não existe identificador de tipo associado ao grafo.
*    
*  $EP Parâmetros
*		ppGrafo - referência para ponteiro de grafo a ser criado
*
*		ExcluirValor  - ponteiro para a função que processa a
*                     exclusão do valor referenciado pelo elemento
*                     a ser excluído.
*                     Ver descrição do módulo.
*
*  $FV Valor retornado
*		GRF_CondRetOK            - criou grafo sem problemas
*		GRF_CondRetFaltouMemoria - faltou memória ao alocar espaço  
*
*  $AE Assertivas de entrada
*     - O grafo não existe
*
*  $AS Assertivas de saída
*     - O grafo existe e é vazio
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_CriarGrafo ( GRF_tppGrafo * ppGrafo , void ( * ExcluirValor ) ( void * pValor ) );


/***********************************************************************
*
*  $FC Função: GRF  &Criar Vertice
*
*  $ED Descrição da função
*		Cria um vértice com uma chave genérica e o insere na lista
*		de vértices.
*    
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se deseja criar o vértice
*
*		pValor - ponteiro para o valor contido no vértice a ser criado
*
*		chave  - valor da chave do vertice, deve ser obrigatoriamente um inteiro não negativo
*		
*  $FV Valor retornado
*		GRF_CondRetOK                   - criou vértice sem problemas
*		GRF_CondRetFaltouMemoria        - faltou memória ao alocar espaço  
*		GRF_CondRetGrafoNaoExiste       - grafo passado como ponteiro não existe 
*		GRF_CondRetVerticeJaExiste      - já existe um vértice com a chave passada 
*							              como ponteiro
*		GRF_CondRetErroAoInserirVertice - falhou ao inserir vértice na lista de vértices
*
*  $AE Assertivas de entrada
*	  - pGrafo é um ponteiro válido para um Grafo
*	  - Valem as assertivas estruturais do grafo
*	  - chave é um valor válido para uma chave (inteiro não negativo)
*
*  $AS Assertivas de saída
*	  - Se o grafo existe ele possui um vertice a mais
*     - O vertice criado será o vertice corrente do grafo
*       SOMENTE se o grafo era vazio
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_CriaVertice ( GRF_tppGrafo pGrafo , void * pValor , int chave ) ;

   /***********************************************************************
*
*  $FC Função: GRF  &Cria Aresta
*
*  $ED Descrição da função
*		Cria uma aresta entre dois vértices do grafo.
*    
*
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se deseja criar o vértice
*
*		chaveA - chave de um dos vértice a ser criada a aresta
*
*		chaveB - chave de um dos vértice a ser criada a aresta		
*
*  $FV Valor retornado
*		GRF_CondRetOK                - criou aresta sem problemas
*		GRF_CondRetFaltouMemoria     - faltou memória ao alocar espaço  
*		GRF_CondRetGrafoNaoExiste    - grafo passado como ponteiro não existe 
*		GRF_ArestaJaExiste           - já existe uma aresta entre os vértices passados 
*							           como ponteiro
*		GRF_CondRetVerticeNaoExiste  - ao menos um dos vértices não existe
*		GRF_CondRetErroInserirAresta - falhou ao inserir aresta
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para um Grafo
*	  - As chaves A e B são válidas
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - Se o grafo existe, existe agora uma aresta entre os vértices
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/


   GRF_tpCondRet GRF_CriaAresta( GRF_tppGrafo pGrafo , int chaveA, int chaveB);


   /***********************************************************************
*
*  $FC Função: GRF  &Remove Aresta
*
*  $ED Descrição da função
*		Remove uma aresta entre dois vértices do grafo.
*    
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se deseja criar a aresta
*
*		chaveA - chave de um dos vértice a ser removida a aresta
*
*		chaveB - chave de um dos vértice a ser removida a aresta		
*
*  $FV Valor retornado
*		GRF_CondRetOK               - removeu aresta sem problemas
*		GRF_CondRetGrafoNaoExiste   - grafo passado como ponteiro não existe 
*		GRF_CondRetArestaNaoExiste  - Não existe uma aresta com a chave passada 
*							          como ponteiro
*		GRF_CondRetVerticeNaoExiste - ao menos um dos vértices não existe
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para um Grafo
*	  - chaveA e chaveB são valores válidos de chaves
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*	  - Se o grafo existia, não existe mais aresta entre os vértices
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_RemoveAresta(GRF_tppGrafo pGrafo, int chaveA , int chaveB);


/***********************************************************************
*
*  $FC Função: GRF  &Destroi Grafo
*
*  $ED Descrição da função
*		Destrói o grafo fornecido
*
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo a ser destruido
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro valido para um Grafo
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - O grafo não existe
*	  - O conteúdo dos vértices foi desalocado
*
***********************************************************************/

   void GRF_DestroiGrafo (GRF_tppGrafo pGrafo);

/***********************************************************************
*
*  $FC Função: GRF  &Esvazia Grafo
*
*  $ED Descrição da função
*		Retorna o grafo ao seu estado original, sem nenhum vértice
*
*  $EP Parâmetros
*		pGrafo - ponteiro para o grafo que se deseja esvaziar	
*
*  $FV Valor retornado
*		GRF_CondRetOK             - grafo foi esvaziado com sucesso
*		GRF_CondRetGrafoNaoExiste - grafo passado como ponteiro não existe
*
*  $AE Assertivas de entrada
*		- pGrafo é um ponteiro válido para um Grafo
*		- Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*		- Se o grafo existia, o grafo nao possui mais nenhum vertice
*		- Valem as assertivas estruturais do grafo
*
***********************************************************************/

	GRF_tpCondRet GRF_EsvaziaGrafo ( GRF_tppGrafo pGrafo ) ;


/***********************************************************************
*
*  $FC Função: GRF  &Obtem Caminho
*
*  $ED Descrição da função
*		Procura um caminho entre dois vértices do grafo.
*    
*  $EP Parâmetros
*		pGrafo       -  ponteiro para o grafo onde se deseja procurar o caminho
*
*		chaveOrigem  - chave de um dos vértices a procurar o caminho
*
*		chaveDestino - chave de um dos vértices a procurar o caminho		
*
*		buffer       - buffer de saída onde será armazenada a sequência de chaves 
*					   na ordem do caminho da origem até a saída, NULL se não é necessário
*
*  $FV Valor retornado
*		GRF_CondRetOK                 - encontrou caminho entre os vértices
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*		GRF_CondRetVerticeNaoExiste   - Não existe vértice com a chave passada 
*									    como ponteiro
*		GRF_CondRetErroAoObterValor   - houve erro ao obter um elemento de uma 
*									    das listas
*		GRF_CondRetVerticesDesconexos - não há caminho entre os vértices
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para um Grafo
*	  - chaveOrigem e chaveDestino são tipos válidos de chave
*	  - Valem as assertivas estruturais do grafo
*     - buffer é um vetor de inteiros de tamanho suficiente para conter
*	  - todo o caminho ou NULL
*
*  $AS Assertivas de saída
*     - O grafo permanece inalterado
*	  - buffer contém as chaves na ordem do caminho da origem ate a saida
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

    GRF_tpCondRet GRF_ObtemCaminho ( GRF_tppGrafo pGrafo , int chaveOrigem , int chaveDestino , int * buffer );


/***********************************************************************
*
*  $FC Função: GRF  &Existe Vertice
*
*  $ED Descrição da função
*		Verifica se o vertice passado como parametro esta contido no grafo
*		passado como parametro
*
*  $EP Parâmetros
*		pGrafo - grafo aonde sera buscado o vertice
*		pValor - chave do vertice que se deseja buscar
*
*  $FV Valor retornado
*		GRF_CondRetGrafoNaoExiste   - grafo nao existe
*		GRF_CondRetVerticeJaExiste  - vertice esta contido no grafo
*		GRF_COndRetVerticeNaoExiste - vertice nao esta contido no grafo
*
*  $AE Assertivas de entrada
*		- pGrafo é um ponteiro válido para Grafo
*		- Valem as assertivas estruturais do grafo
*		- A chave passada e de um tipo valido de chave
*
*  $AS Assertivas de saída
*		- O grafo permanece inalterado
*		- Valem as assertivas estruturais do grafo
*
***********************************************************************/

	GRF_tpCondRet GRF_ExisteVertice ( GRF_tppGrafo pGrafo , int chave );


/***********************************************************************
*
*  $FC Função: GRF  &Altera Corrente
*
*  $ED Descrição da função
*		altera o elemento corrente do grafo para o elemento de chave passada
*		como parametro, independentemente se estão interconectados.
*    
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se deseja alterar o elemento corrente
*
*		chave  - chave do elemento que deseja que se torne corrente
*
*  $FV Valor retornado
*		GRF_CondRetOK                 - alterou corrente sem problemas  
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*		GRF_VerticeNaoExiste          - nao existe um vértice com a chave passada
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro valido para Grafo
*	  - A chave passada é de um tipo valido de chave
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - Se o grafo existe, seu elemento corrente agora é o da chave passada como parametro
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_AlteraCorrente ( GRF_tppGrafo pGrafo , int chave ) ;

   /***********************************************************************
*
*  $FC Função: GRF  &Existe Aresta
*
*  $ED Descrição da função
*		verifica se a aresta entre os vertices de chaves passadas como parametro
*		existe.       
*    
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se procurar a aresta      
*
*		chaveA - chave de um dos vertices pertencentes à aresta
*
*       chaveB - chave de um dos vertices pertencentes à aresta
*
*  $FV Valor retornado
*		GRF_CondRetVerticesDesconexos - Nao existe aresta entre os vertices
*		GRF_CondRetArestaJaExiste     - Existe aresta entre os vertices
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*		GRF_CondRetVerticeNaoExiste   - o vertice de uma das chaves passadas como
*							            parametro nao existe     
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para Grafo
*     - as chaves passadas como parametro são de um tipo válido
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - O grafo permanece inalterado
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_ExisteAresta ( GRF_tppGrafo pGrafo , int chaveA , int chaveB ) ;

   /***********************************************************************
*
*  $FC Função: GRF  &Ir Vizinho
*
*  $ED Descrição da função
*		altera o elemento corrente do grafo passado como parametro para
*		o vertice vizinho correspondente à chave passada como parametro
*    
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se deseja inserir a origem
*
*		chave  -  chave do vértice a ser o novo elemento corrente do grafo
*
*  $FV Valor retornado
*		GRF_CondRetOK                 - alterou o elemento corrente sem problemas
*		GRF_CondRetVerticesDesconexos - o vertice da chave passada como parametro não
*										é vizinho do vertice corrente
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*		GRF_CondRetVerticeNaoExiste   - o vertice correspondente à chave passada 
*							            como parametro nao existe
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para Grafo
*	  - <chave> é um tipo válido de chave
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - Se o grafo existia seu elemento corrente agora é o vertice de chave <chave>
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_IrVizinho (GRF_tppGrafo pGrafo , int chave) ;

 /***********************************************************************
*
*  $FC Função: GRF  &Obter Corrente
*
*  $ED Descrição da função
*		obtem o valor contido no vertice elemento corrente do grafo assim
*		como sua chave
*    
*  $EP Parâmetros
*		pGrafo  - ponteiro para o grafo onde se deseja obter o valor contido na corrente
*
*		ppValor - endereço de um ponteiro que irá receber o valor contido no vertice corrente
*
*		pChave  - endereço da variavel que receberá o valor da chave do elemento corrente
*
*  $FV Valor retornado
*		GRF_CondRetOK                 - obteve valores com sucesso 
*		GRF_CondRetVerticeNaoExiste   - vertice da chave passada como parametro nao existe
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para Grafo
*	  - ppValor é um ponteiro para uma variavel de tipo válido para um valor contido em um vertice
*     - pChave é um ponteiro válido para uma variavel de tipo int
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - O grafo permanece inalterado
*	  - Se o grafo existia e não era vazio, pValor agora aponta para o valor do vertice corrente
*	    e pChave agora aponta para o valor da chave do vertice corrente
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/
 
   GRF_tpCondRet GRF_ObterCorr ( GRF_tppGrafo pGrafo , int * pChave , void ** ppValor );

   /***********************************************************************
*
*  $FC Função: GRF  &Alterar Valor Corrente
*
*  $ED Descrição da função
*		altera valor contido no vertice corrente desalocando o valor atual
*    
*  $EP Parâmetros
*		pGrafo -  ponteiro para o grafo onde se deseja alterar o valor do vertice corrente
*
*		pValor - ponteiro para o valor a ser inserido no vertice corrente
*
*  $FV Valor retornado
*		GRF_CondRetOK                 - alterou valor do vertice sem problemas
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*		GRF_CondVerticeNaoExiste      - o vertice corrente nao existe
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para Grafo
*	  - pValor é um ponteiro válido para o tipo de valor a ser inserido no Grafo
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - Se o grafo e o vertice corrente existiam, o valor do vertice corrente agora é pValor
*       e o valor contido anteriormente foi desalocado
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_AlterarValorCorr ( GRF_tppGrafo pGrafo , void * pValor );

   /***********************************************************************
*
*  $FC Função: GRF  &Caminhar
*
*  $ED Descrição da função
*		altera o vertice corrente do grafo para um elemento vertice de chave
*		conectado ao vertice corrente atual
*		
*    
*  $EP Parâmetros
*		pGrafo       - ponteiro para o grafo onde se deseja alterar o vertice corrente
*
*		chaveDestino - chave do vértice a ser vertice corrente
*
*  $FV Valor retornado
*		GRF_CondRetOK                 - alterou vertice corrente sem problemas
*		GRF_CondRetVerticeNaoExiste   - vertice corrente nao existe ou vertice da 
*                                       chave de destino nao existe  
*		GRF_CondRetGrafoNaoExiste     - grafo passado como ponteiro não existe 
*		GRF_CondRetVerticesDesconexos - vertice corrente e o vertice da chave de destino 
*							            são desconexos
*
*  $AE Assertivas de entrada
*     - pGrafo é um ponteiro válido para Grafo
*	  - chaveDestino é de um tipo válido de chave
*	  - Valem as assertivas estruturais do grafo
*
*  $AS Assertivas de saída
*     - Se o grafo existe, o vertice corrente existe, e os vertices estavam 
*       conectados, o vertice corrente do grafo agora é o da chaveDestino
*	  - Valem as assertivas estruturais do grafo
*
***********************************************************************/

   GRF_tpCondRet GRF_Caminhar ( GRF_tppGrafo pGrafo , int chaveDestino ) ;

/***********************************************************************
*
*  $FC Função: GRF  &Verificar um grafo
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Verifica completamente uma determinada árvore.
*     Também marca todos os espaços por ela ocupados.
*
***********************************************************************/

#ifdef _DEBUG

   GRF_tpCondRet GRF_VerificarGrafo( void * pGrafoParm ) ;

#endif


/***********************************************************************
*
*  $FC Função: GRF  &Verificar um nó cabeça
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Verifica a integridade de um nó cabeça da árvore.
*
*  $EP Parâmetros
*     $P pCabecaParm - ponteiro para um espaço que deveria ser uma cabeça
*                      de árvore.
*
*  $FV Valor retornado
*     Condição de retorno de teste
*
***********************************************************************/

#ifdef _DEBUG

   GRF_tpCondRet GRF_VerificarCabeca( void * pCabecaParm ) ;

#endif


/***********************************************************************
*
*  $FC Função: GRF  &Verificar um nó de estrutura
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Verifica a integridade de um nó estrutural da árvore.
*
*  $EP Parâmetros
*     $P pNoParm - ponteiro para espaço que deveria ser um nó estrutural.
*
*  $FV Valor retornado
*     Número de erros encontrados.
*
***********************************************************************/

#ifdef _DEBUG

   GRF_tpCondRet GRF_VerificarVertice( void * pVerticeParm ) ;

#endif


/***********************************************************************
*
*  $FC Função: GRF  &Deturpar Grafo
*
*  $ED Descrição da função
*     Função da interface de teste.
*     Corrompe elementos específicos da estrutura do grafo.
*     Essa função destina-se a preparar os cenários de teste dos
*     casos de teste utilizados ao testar os verificadores estruturais
*     do grafo.
*     Esta função não tem proteção contra erros de uso, consequentemente
*     poderá levar o programa a ser cancelado pelo sistema operacional.
*
*  $EP Parâmetros
*     $P pGrafoParm  - grafo a ser deturpada
*     $P ModoDeturpar - identifica como deve ser feita a deturpação,
*                       GRF_tpModosDeturpacao identifica os modos de
*                       deturpação conhecidos
*
***********************************************************************/
  
#ifdef _DEBUG

   void GRF_Deturpar( void * pGrafoParm ,
                      GRF_tpModosDeturpacao ModoDeturpar ) ;

#endif
/********** Fim do módulo de definição: GRF Grafo não orientado **********/


