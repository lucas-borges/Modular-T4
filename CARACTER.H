/***************************************************************************
*
*  $MCD Módulo de definição: CHR Estrutura de caracter
*
*  Arquivo gerado:              CARACTER.h
*  Letras identificadoras:      CHR
*
*  Projeto: INF 1301 / 1628 Automatização dos testes de módulos C
*  Gestor:  
*  Autores: cs -  Clara Szwarcman
*			gs -  Guilherme Simas
*			lb -  Lucas Borges
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*       1    csgslb 29/ago/2014 início desenvolvimento 
*
*  $ED Descrição do módulo
*    Implementa uma estrutura simples contendo um caracter.
*	 Podem existir n caracteres em operação simultaneamente.
*
***************************************************************************/

/***** Declarações exportadas pelo módulo *****/

/* Tipo referência para um caracter */

typedef struct CHR_tagCaracter * CHR_tppCaracter ;

/***********************************************************************
*
*  $FC Função: CHR Criar Caracter
*
*  $ED Descrição da função
*     Cria um caracter.
*
*  $EP Parâmetros
*     ValorParm  - Valor do caracter
*
*  $FV Valor retornado
*     Se executou corretamente retorna o ponteiro para o caracter.
*     Este ponteiro será utilizado pelas funções que manipulem este caracter.
*
*     Se ocorreu algum erro, por exemplo falta de memória ou dados errados,
*     a função retornará NULL.
*     Não será dada mais informação quanto ao problema ocorrido.
*
***********************************************************************/

	CHR_tppCaracter CHR_CriarCaracter( char ValorParm );

/***********************************************************************
*
*  $FC Função: CHR Destruir Caracter
*
*  $ED Descrição da função
*     Destrói o caracter fornecido.
*
*  $EP Parâmetros
*     pCaracter  - ponteiro para o caracter a ser destruido
*
***********************************************************************/

	void CHR_DestruirCaracter (  void * pCaracter );

/***********************************************************************
*
*  $FC Função: CHR Obtém valor do caracter
*
*  $ED Descrição da função
*     Obtem o valor do caracter fornecido.
*
*  $EP Parâmetros
*     pCaracter - ponteiro para o caracter de qual se quer o valor
*
*  $FV Valor retornado
*     Caracter contido na estrutura.
*
***********************************************************************/

	char CHR_ObterValor(  CHR_tppCaracter pCaracter);

/***********************************************************************
*
*  $FC Função: CHR Compara Caracter
*
*  $ED Descrição da função
*     Compara dois ponteiros para um caracter.
*
*  $EP Parâmetros
*     pValorA    - Ponteiro para valor de um caracter
*     pValorB    - Ponteiro para valor de um caracter
*
*  $FV Valor retornado
*     Se valor de ambos caracteres for igual, a funçao retorna 0.
*     
*     Caso sejam diferentes, a função retorna 1.
*
***********************************************************************/

	int CHR_ComparaCaracter( void * pCaracterA , void * pCaracterB ) ;

/********** Fim do módulo de definição: CHR Estrutura de caracter **********/