#if ! defined( LISTA_ )
#define LISTA_
/***************************************************************************
*
*  $MCD Módulo de definição: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.h
*  Letras identificadoras:      LIS
*
*  Projeto: INF 1301 Automatização dos testes de módulos C
*  Gestor:  DI/PUC-Rio
*  Autores: avs - Arndt von Staa
*			cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*     6		 csgslb	30/set/2014 documentação de assertivas de entrada e saída
*	  5		 csgslb 03/set/2014 padronização de retornos para teste automatizado
*								e dos nomes das funções
*     4       avs   01/fev/2006 criar linguagem script simbólica
*     3       avs   08/dez/2004 uniformização dos exemplos
*     2       avs   07/jul/2003 unificação de todos os módulos em um só projeto
*     1       avs   16/abr/2003 início desenvolvimento
*
*  $ED Descrição do módulo
*     Implementa listas genéricas duplamente encadeadas.
*     Podem existir n listas em operação simultaneamente.
*     As listas possuem uma cabeça encapsulando o seu estado.
*
*     Cada lista é homogênea quanto ao tipo dos dados que armazena.
*     Cada elemento da lista referencia o valor que contém.
*
*     Os ponteiros para os dados são copiados para elementos da lista.
*        Não é copiado o valor apontado por estes ponteiros.
*
*     O controle da destruição do valor de um elemento a ser excluído
*        é realizado por uma função fornecida pelo usuário.
*
*     Cada lista referencia uma função que determina como devem ser
*        desalocados os dados nela contidos.
*
*     A função de liberação dos valores contidos nos elementos deve
*        assegurar a liberação de todos os espaços referênciados pelo
*        valor contido em um elemento.
*        Esta função é chamada antes de se desalocar um elemento
*        de uma lista.
*        Caso não seja necessário desalocar o valor referenciado pelo
*        elemento, o ponteiro para a função de liberação poderá ser NULL .
*        Caso o elemento da lista seja a única âncora do valor referenciado,
*        esta função deve promover a destruição (free) desse valor e
*        de todos os dados nele ancorados.
*
***************************************************************************/
 
#if defined( LISTA_OWN )
   #define LISTA_EXT
#else
   #define LISTA_EXT extern
#endif

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para uma lista */

typedef struct LIS_tagLista * LIS_tppLista ;


/***********************************************************************
*
*  $TC Tipo de dados: LIS Condições de retorno
*
*
*  $ED Descrição do tipo
*     Condições de retorno das funções da lista
*
***********************************************************************/

   typedef enum {

         LIS_CondRetOK ,
               /* Concluiu corretamente */

         LIS_CondRetListaVazia ,
               /* A lista não contém elementos */

         LIS_CondRetFimLista ,
               /* Foi atingido o fim de lista */

         LIS_CondRetListaNaoExiste ,
               /* A lista não existe */

         LIS_CondRetFaltouMemoria ,
               /* Faltou memória ao tentar criar um elemento de lista */
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
*  $FC Função: LIS  &Criar lista
*
*  $ED Descrição da função
*     Cria uma lista genérica duplamente encadeada.
*     Os possíveis tipos são desconhecidos a priori.
*     A tipagem é implicita.
*     Não existe identificador de tipo associado à lista.
*
*  $EP Parâmetros
*     ppLista - referência para ponteiro da lista a ser criada
*
*     ExcluirValor  - ponteiro para a função que processa a
*                     exclusão do valor referenciado pelo elemento
*                     a ser excluído.
*                     Ver descrição do módulo.
*
*  $FV Valor retornado
*     LIS_CondRetOK            - criou lista sem problemas
*     LIS_CondRetFaltouMemoria - faltou memória ao alocar espaço
*
*  $AE Assertivas de entrada
*     - pLista é o endereço de um ponteiro válido
*
*  $AS Assertivas de saída
*     - A lista existe e é vazia
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_CriarLista( LIS_tppLista* pLista ,
             void   ( * ExcluirValor ) ( void * pDado ) ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Destruir lista
*
*  $ED Descrição da função
*     Destrói a lista fornecida.
*     O parâmetro ponteiro para a lista não é modificado.
*     Se ocorrer algum erro durante a destruição, a lista resultará
*     estruturalmente incorreta.
*     OBS. não existe previsão para possíveis falhas de execução.
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - A lista não existe
*	  - O conteúdo dos elementos da lista são desalocados segundo
*		a função ExcluirValor fornecida
*
***********************************************************************/

   void LIS_DestruirLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Esvaziar lista
*
*  $ED Descrição da função
*     Elimina todos os elementos, sem contudo eliminar a lista
*
*  $EP Parâmetros
*     pLista - ponteiro para a lista a ser esvaziada
*
*  $FV Valor retornado
*     LIS_CondRetOK             - esvaziou sem problemas
*	  LIS_CondRetListaNaoExiste - ponteiro da lista é NULL
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - Se a lista existia, agora a lista é vazia
*	  - Se a lista existia o conteúdo dos elementos da lista são desalocados segundo
*		a função ExcluirValor fornecida
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_EsvaziarLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Inserir elemento antes
*
*  $ED Descrição da função
*     Insere novo elemento antes do elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Parâmetros
*     pLista - ponteiro para a lista onde deve ser inserido o elemento
*     pValor - ponteiro para o valor do novo elemento
*              Pode ser NULL
*
*  $FV Valor retornado
*     LIS_CondRetOK              - inseriu antes sem problemas
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista é NULL
*     LIS_CondRetFaltouMemoria   - faltou memória ao alocar espaço
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - O elemento corrente é o seguinte ao qual se quer inserir
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*     - pValor é um ponteiro válido para o valor que se quer inserir
*
*  $AS Assertivas de saída
*     - Se a lista existia, tem um elemento a mais
*     - Se a lista existia, elemento inserido contem pValor
*	  - Se a lista exisita, elemento corrente é o elemento criado
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                           void * pValor        ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Inserir elemento após
*
*  $ED Descrição da função
*     Insere novo elemento após o elemento corrente.
*     Caso a lista esteja vazia, insere o primeiro elemento da lista.
*
*  $EP Parâmetros
*     Parâmetros
*        pLista - ponteiro para a lista onde deve ser inserido o elemento
*        pValor - ponteiro para o valor do novo elemento
*                 Pode ser NULL
*           
*
*  $FV Valor retornado     
*     LIS_CondRetOK              - inseriu após sem problemas
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista é NULL
*     LIS_CondRetFaltouMemoria   - faltou memória ao alocar espaço
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - O elemento corrente é o anterior ao qual se quer inserir
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*     - pValor é um ponteiro válido para o valor que se quer inserir
*
*  $AS Assertivas de saída
*     - Se a lista existia, tem um elemento a mais
*	  - Se a lista existia, elemento corrente é o elemento criado
*     - Se a lista existia, elemento inserido contem pValor
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Excluir elemento
*
*  $ED Descrição da função
*     Exclui o elemento corrente da lista dada.
*     Se existir o elemento a esquerda do corrente será o novo corrente.
*     Se não existir e existir o elemento à direita, este se tornará corrente.
*     Se este também não existir a lista tornou-se vazia.
*
*  $EP Parâmetros
*     pLista    - ponteiro para a lista na qual deve excluir.
*
*  $FV Valor retornado
*     LIS_CondRetOK              - excluiu elemento sem problemas
*     LIS_CondRetListaVazia      - lista é vazia
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista é NULL
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - O elemento corrente é o que deve ser excluído
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - Se a lista existia, possui um elemento a menos
*	  - Se a lista existia, elemento foi excluido e o corrente é o descrito
*		na descrição da função
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Obter referência para o valor contido no elemento
*
*  $ED Descrição da função
*     Obtem a referência para o valor contido no elemento corrente da lista
*
*  $EP Parâmetros
*     pLista  - ponteiro para a lista de onde se quer o valor
*     ppValor - referência para ponteiro que receberá o ponteiro do elemento
*
*  $FV Valor retornado
*     LIS_CondRetOK              - obteve referência para elemento sem problemas
*     LIS_CondRetListaVazia      - lista é vazia
*	  LIS_CondRetListaNaoExiste  - ponteiro da lista é NULL
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - O elemento corrente é o qual se quer obter a referência do valor
*     - ppValor é o endereço de um ponteiro de mesmo tipo do valor contido
*       no elemento
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - A lista permanece inalterada
*     - Se a lista existia, ppValor agora aponta para o valor contido no elemento corrente
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_ObterValor( LIS_tppLista pLista , void ** ppValor) ;


/***********************************************************************
*
*  $FC Função: LIS  &Ir para o elemento inicial
*
*  $ED Descrição da função
*     Torna corrente o primeiro elemento da lista.
*
*  $EP Parâmetros
*     pLista - ponteiro para a lista a manipular
*
*  $FV Valor retornado
*		LIS_CondRetOK              - foi para elemento inicial sem problemas
*       LIS_CondRetListaVazia      - lista é vazia
*		LIS_CondRetListaNaoExiste  - ponteiro da lista é NULL
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - Os elementos da lista não foram alterados
*	  - Se a lista existia, o elemento corrente é o primeiro elemento da lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

    LIS_tpCondRet LIS_IrInicioLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Ir para o elemento final
*
*  $ED Descrição da função
*     Torna corrente o elemento final da lista.
*
*  $EP Parâmetros
*     pLista - ponteiro para a lista a manipular
*
*  $FV Valor retornado
*		LIS_CondRetOK              - foi para elemento final sem problemas
*       LIS_CondRetListaVazia      - lista é vazia
*		LIS_CondRetListaNaoExiste  - ponteiro da lista é NULL
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - Os elementos da lista não foram alterados
*	  - Se a lista existia, o elemento corrente é o último elemento da lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_IrFinalLista( LIS_tppLista pLista ) ;


/***********************************************************************
*
*  $FC Função: LIS  &Avançar elemento
*
*  $ED Descrição da função
*     Avança o elemento corrente numElem elementos na lista
*     Se numElem for positivo avança em direção ao final
*     Se numElem for negativo avança em direção ao início
*     numElem pode ser maior do que o númro de elementos existentes na
*               direção desejada
*     Se numElem for zero somente verifica se a lista está vazia
*
*  $EP Parâmetros
*     pLista  - ponteiro para a lista a ser manipulada
*     numElem - o número de elementos a andar
*
*  $FV Valor retornado
*     CondRetOK             - se numElem elementos tiverem sido andados
*     CondRetFimLista       - se encontrou o fim da lista antes de andar numElem
*                             elementos
*     CondRetListaVazia     - se a lista está vazia
*	  CondRetListaNaoExiste - se a lista não existir
*
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - Os elementos da lista não foram alterados
*	  - Se a lista existia, o elemento corrente é o elemento situado "numElem" à frente na lista
*	  - Valem as assertivas estruturais da lista duplamente encadeada 
*
***********************************************************************/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem ) ;

/***********************************************************************
*
*  $FC Função: LIS  &Alterar Valor do Elemento Corrente
*
*  $ED Descrição da função
*	   Altera o ponteiro contido  no elemento corrente.
*	   O espaço referenciado pelo ponteiro original não é
*	   desalocado.
*     
*  $EP Parâmetros
*     pLista     - ponteiro para a lista a ser manipulada
*     pValorNovo - ponteiro a ser inserido no elemento corrente
*
*  $FV Valor retornado  
*     CondRetOK             - o valor do elemento corrente foi alterado sem problemas
*     CondRetListaVazia     - lista é vazia
*	  CondRetListaNaoExiste - ponteiro da lista é NULL
*     
*  $AE Assertivas de entrada
*     - pLista é um ponteiro válido para uma lista
*	  - O elemento corrente é o qual se quer alterar o valor
*     - pValorNovo é um ponteiro para um tipo de valor que se deseja inserir
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
*  $AS Assertivas de saída
*     - Se a lista existia, exceto ao elemento corrente, os demais não
*		foram modificados
*	  - Se a lista existia, o elemento corrente é o qual se queria alterar o valor
*	  - Se a lista existia, o valor do elemento corrente é "pValorNovo"
*	  - Valem as assertivas estruturais da lista duplamente encadeada
*
***********************************************************************/

   LIS_tpCondRet LIS_AlterarValor( LIS_tppLista pLista ,
                                          void * pValorNovo        ) ;

#undef LISTA_EXT

/********** Fim do módulo de definição: LIS  Lista duplamente encadeada **********/

#else
#endif
