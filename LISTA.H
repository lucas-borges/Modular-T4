#if ! defined( LISTA_ )
#define LISTA_
/***************************************************************************
*
*  $MCD M�dulo de defini��o: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.h
*  Letras identificadoras:      LIS
*
*  Projeto: INF 1301 Automatiza��o dos testes de m�dulos C
*  Gestor:  DI/PUC-Rio
*  Autores: avs - Arndt von Staa
*			cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*     6		 csgslb	30/set/2014 documenta��o de assertivas de entrada e sa�da
*	  5		 csgslb 03/set/2014 padroniza��o de retornos para teste automatizado
*								e dos nomes das fun��es
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Implementa listas gen�ricas duplamente encadeadas.
*     Podem existir n listas em opera��o simultaneamente.
*     As listas possuem uma cabe�a encapsulando o seu estado.
*
*     Cada lista � homog�nea quanto ao tipo dos dados que armazena.
*     Cada elemento da lista referencia o valor que cont�m.
*
*     Os ponteiros para os dados s�o copiados para elementos da lista.
*        N�o � copiado o valor apontado por estes ponteiros.
*
*     O controle da destrui��o do valor de um elemento a ser exclu�do
*        � realizado por uma fun��o fornecida pelo usu�rio.
*
*     Cada lista referencia uma fun��o que determina como devem ser
*        desalocados os dados nela contidos.
*
*     A fun��o de libera��o dos valores contidos nos elementos deve
*        assegurar a libera��o de todos os espa�os refer�nciados pelo
*        valor contido em um elemento.
*        Esta fun��o � chamada antes de se desalocar um elemento
*        de uma lista.
*        Caso n�o seja necess�rio desalocar o valor referenciado pelo
*        elemento, o ponteiro para a fun��o de libera��o poder� ser NULL .
*        Caso o elemento da lista seja a �nica �ncora do valor referenciado,
*        esta fun��o deve promover a destrui��o (free) desse valor e
*        de todos os dados nele ancorados.
*
***************************************************************************/
 
#if defined( LISTA_OWN )
   #define LISTA_EXT
#else
   #define LISTA_EXT extern
#endif

/***** Declara��es exportadas pelo m�dulo *****/

/* Tipo refer�ncia para uma lista */

typedef struct LIS_tagLista * LIS_tppLista ;


/***********************************************************************
*
*  $TC Tipo de dados: LIS Condi��es de retorno
*
*
*  $ED Descri��o do tipo
*     Condi��es de retorno das fun��es da lista
*
***********************************************************************/

   typedef enum {

         LIS_CondRetOK ,
               /* Concluiu corretamente */

         LIS_CondRetListaVazia ,
               /* A lista n�o cont�m elementos */

         LIS_CondRetFimLista ,
               /* Foi atingido o fim de lista */

         LIS_CondRetListaNaoExiste ,
               /* A lista n�o existe */

         LIS_CondRetFaltouMemoria ,
               /* Faltou mem�ria ao tentar criar um elemento de lista */
#ifdef _DEBUG

		 LIS_CondRetErroEstrutura,
			   /* Erro na estrutura da Lista */
			   
		 LIS_CondRetErroPonteiroSucessor,
			   /* Erro no ponteiro sucessor do vertice corrente */

		 LIS_CondRetErroPonteiroPredecessor,
			   /* Erro no ponteiro predecessor do vertice corrente */
#endif
		 
   } LIS_tpCondRet ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Criar lista
*
*  $ED Descri��o da fun��o
*     Cria uma lista gen�rica duplamente encadeada.
*     Os poss�veis tipos s�o desconhecidos a priori.
*     A tipagem � implicita.
*     N�o existe identificador de tipo associado � lista.
*
*  $EP Par�metros
*     ppLista - refer�ncia para ponteiro da lista a ser criada
*
*     ExcluirValor  - ponteiro para a fun��o que processa a
*                     exclus�o do valor referenciado pelo elemento
*                     a ser exclu�do.
*                     Ver descri��o do m�dulo.
*
*  $FV Valor retornado
*     LIS_CondRetOK            - criou lista sem problemas
*     LIS_CondRetFaltouMemoria - faltou mem�ria ao alocar espa�o
*
*  $AE Assertivas de entrada
*     - pLista � o endere�o de um ponteiro v�lido
*
*  $AS Assertivas de sa�da
*     - A lista existe e � vazia
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_CriarLista( LIS_tppLista* pLista ,
             void   ( * ExcluirValor ) ( void * pDado ) ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Destruir lista
*
*  $ED Descri��o da fun��o
*     Destr�i a lista fornecida.
*     O par�metro ponteiro para a lista n�o � modificado.
*     Se ocorrer algum erro durante a destrui��o, a lista resultar�
*     estruturalmente incorreta.
*     OBS. n�o existe previs�o para poss�veis falhas de execu��o.
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - A lista n�o existe
*	  - O conte�do dos elementos da lista s�o desalocados segundo
*		a fun��o ExcluirValor fornecida
*
***********************************************************************/

   void LIS_DestruirLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Esvaziar lista
*
*  $ED Descri��o da fun��o
*     Elimina todos os elementos, sem contudo eliminar a lista
*
*  $EP Par�metros
*     pLista - ponteiro para a lista a ser esvaziada
*
*  $FV Valor retornado
*     LIS_CondRetOK             - esvaziou sem problemas
*	  LIS_CondRetListaNaoExiste - ponteiro da lista � NULL
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - Se a lista existia, agora a lista � vazia
*	  - Se a lista existia o conte�do dos elementos da lista s�o desalocados segundo
*		a fun��o ExcluirValor fornecida
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_EsvaziarLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Inserir elemento antes
*
*  $ED Descri��o da fun��o
*     Insere novo elemento antes do elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Par�metros
*     pLista - ponteiro para a lista onde deve ser inserido o elemento
*     pValor - ponteiro para o valor do novo elemento
*              Pode ser NULL
*
*  $FV Valor retornado
*     LIS_CondRetOK              - inseriu antes sem problemas
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista � NULL
*     LIS_CondRetFaltouMemoria   - faltou mem�ria ao alocar espa�o
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - O elemento corrente � o seguinte ao qual se quer inserir
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*     - pValor � um ponteiro v�lido para o valor que se quer inserir
*
*  $AS Assertivas de sa�da
*     - Se a lista existia, tem um elemento a mais
*     - Se a lista existia, elemento inserido contem pValor
*	  - Se a lista exisita, elemento corrente � o elemento criado
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                           void * pValor        ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Inserir elemento ap�s
*
*  $ED Descri��o da fun��o
*     Insere novo elemento ap�s o elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Par�metros
*     Par�metros
*        pLista - ponteiro para a lista onde deve ser inserido o elemento
*        pValor - ponteiro para o valor do novo elemento
*                 Pode ser NULL
*           
*
*  $FV Valor retornado     
*     LIS_CondRetOK              - inseriu ap�s sem problemas
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista � NULL
*     LIS_CondRetFaltouMemoria   - faltou mem�ria ao alocar espa�o
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - O elemento corrente � o anterior ao qual se quer inserir
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*     - pValor � um ponteiro v�lido para o valor que se quer inserir
*
*  $AS Assertivas de sa�da
*     - Se a lista existia, tem um elemento a mais
*	  - Se a lista existia, elemento corrente � o elemento criado
*     - Se a lista existia, elemento inserido contem pValor
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Excluir elemento
*
*  $ED Descri��o da fun��o
*     Exclui o elemento corrente da lista dada.
*     Se existir o elemento a esquerda do corrente ser� o novo corrente.
*     Se n�o existir e existir o elemento � direita, este se tornar� corrente.
*     Se este tamb�m n�o existir a lista tornou-se vazia.
*
*  $EP Par�metros
*     pLista    - ponteiro para a lista na qual deve excluir.
*
*  $FV Valor retornado
*     LIS_CondRetOK              - excluiu elemento sem problemas
*     LIS_CondRetListaVazia      - lista � vazia
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista � NULL
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - O elemento corrente � o que deve ser exclu�do
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - Se a lista existia, possui um elemento a menos
*	  - Se a lista existia, elemento foi excluido e o corrente � o descrito
*		na descri��o da fun��o
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Obter refer�ncia para o valor contido no elemento
*
*  $ED Descri��o da fun��o
*     Obtem a refer�ncia para o valor contido no elemento corrente da lista
*
*  $EP Par�metros
*     pLista  - ponteiro para a lista de onde se quer o valor
*     ppValor - refer�ncia para ponteiro que receber� o ponteiro do elemento
*
*  $FV Valor retornado
*     LIS_CondRetOK              - obteve refer�ncia para elemento sem problemas
*     LIS_CondRetListaVazia      - lista � vazia
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista � NULL
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - O elemento corrente � o qual se quer obter a refer�ncia do valor
*     - ppValor � o endere�o de um ponteiro de mesmo tipo do valor contido
*       no elemento
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - A lista permanece inalterada
*     - Se a lista existia, ppValor agora aponta para o valor contido no elemento corrente
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_ObterValor( LIS_tppLista pLista , void ** ppValor) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Ir para o elemento inicial
*
*  $ED Descri��o da fun��o
*     Torna corrente o primeiro elemento da lista.
*
*  $EP Par�metros
*     pLista - ponteiro para a lista a manipular
*
*  $FV Valor retornado
*		LIS_CondRetOK              - foi para elemento inicial sem problemas
*       LIS_CondRetListaVazia      - lista � vazia
*		LIS_CondRetListaNaoExiste  - ponteiro da lista � NULL
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - Os elementos da lista n�o foram alterados
*	  - Se a lista existia, o elemento corrente � o primeiro elemento da lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

    LIS_tpCondRet LIS_IrInicioLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Ir para o elemento final
*
*  $ED Descri��o da fun��o
*     Torna corrente o elemento final da lista.
*
*  $EP Par�metros
*     pLista - ponteiro para a lista a manipular
*
*  $FV Valor retornado
*		LIS_CondRetOK              - foi para elemento final sem problemas
*       LIS_CondRetListaVazia      - lista � vazia
*		LIS_CondRetListaNaoExiste  - ponteiro da lista � NULL
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - Os elementos da lista n�o foram alterados
*	  - Se a lista existia, o elemento corrente � o �ltimo elemento da lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_IrFinalLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Fun��o: LIS  &Avan�ar elemento
*
*  $ED Descri��o da fun��o
*     Avan�a o elemento corrente numElem elementos na lista
*     Se numElem for positivo avan�a em dire��o ao final
*     Se numElem for negativo avan�a em dire��o ao in�cio
*     numElem pode ser maior do que o n�mro de elementos existentes na
*               dire��o desejada
*     Se numElem for zero somente verifica se a lista est� vazia
*
*  $EP Par�metros
*     pLista  - ponteiro para a lista a ser manipulada
*     numElem - o n�mero de elementos a andar
*
*  $FV Valor retornado
*     CondRetOK             - se numElem elementos tiverem sido andados
*     CondRetFimLista       - se encontrou o fim da lista antes de andar numElem
*                             elementos
*     CondRetListaVazia     - se a lista est� vazia
*	  CondRetListaNaoExiste - se a lista n�o existir
*
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - Os elementos da lista n�o foram alterados
*	  - Se a lista existia, o elemento corrente � o elemento situado "numElem" � frente na lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada 
*
***********************************************************************/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem ) ;

/***********************************************************************
*
*  $FC Fun��o: LIS  &Alterar Valor do Elemento Corrente
*
*  $ED Descri��o da fun��o
*	   Altera o ponteiro contido  no elemento corrente.
*	   O espa�o referenciado pelo ponteiro original n�o �
*	   desalocado.
*     
*  $EP Par�metros
*     pLista     - ponteiro para a lista a ser manipulada
*     pValorNovo - ponteiro a ser inserido no elemento corrente
*
*  $FV Valor retornado  
*     CondRetOK             - o valor do elemento corrente foi alterado sem problemas
*     CondRetListaVazia     - lista � vazia
*	  CondRetListaNaoExiste - ponteiro da lista � NULL
*     
*  $AE Assertivas de entrada
*     - pLista � um ponteiro v�lido para uma lista
*	  - O elemento corrente � o qual se quer alterar o valor
*     - pValorNovo � um ponteiro para um tipo de valor que se deseja inserir
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de sa�da
*     - Se a lista existia, exceto ao elemento corrente, os demais n�o
*		foram modificados
*	  - Se a lista existia, o elemento corrente � o qual se queria alterar o valor
*	  - Se a lista existia, o valor do elemento corrente � "pValorNovo"
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_AlterarValor( LIS_tppLista pLista ,
                                          void * pValorNovo        ) ;

   /***********************************************************************
*
*  $FC Fun��o: LIS  &Verificar Lista
*
*  $ED Descri��o da fun��o
*     
*
*  $EP Par�metros
*     $P pLista  - lista a ser verificada
*
***********************************************************************/
  
#ifdef _DEBUG

   LIS_tpCondRet VerificaLista (LIS_tppLista pLista);

#endif

/***********************************************************************
*
*  $FC Fun��o: LIS  &Deturpar Lista
*
*  $ED Descri��o da fun��o
*     
*
*  $EP Par�metros
*     $P pListaParm  - lista a ser deturpada
*     $P ModoDeturpar - identifica como deve ser feita a deturpa��o,
*                       LIS_tpModosDeturpacao identifica os modos de
*                       deturpa��o conhecidos
*
***********************************************************************/
  
#ifdef _DEBUG

   void LIS_Deturpar( void * pListaParm ,
                      int ModoDeturpar ) ;

#endif

#undef LISTA_EXT

/********** Fim do m�dulo de defini��o: LIS  Lista duplamente encadeada **********/

#else
#endif
