﻿/***************************************************************************
*  $MCI Módulo de implementação: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
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
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>
#include   <assert.h>

#define LISTA_OWN
#include "LISTA.h"
#undef LISTA_OWN

#ifdef _DEBUG
#include "Generico.H"
#include "CONTA.H"
#include "CESPDIN.H"
#include "IdTiposEspaco.def"
	/*contadores*/

#define CONTADOR_FALHAS_ESTRUTURA "FalhasEstrutura"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

   typedef struct tagElemLista {

         void * pValor ;
               /* Ponteiro para o valor contido no elemento */

         struct tagElemLista * pAnt ;
               /* Ponteiro para o elemento predecessor */

         struct tagElemLista * pProx ;
               /* Ponteiro para o elemento sucessor */

   } tpElemLista ;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeça de lista
*
*
***********************************************************************/

   typedef struct LIS_tagLista {

         tpElemLista * pOrigemLista ;
               /* Ponteiro para a origem da lista */

         tpElemLista * pFimLista ;
               /* Ponteiro para o final da lista */

         tpElemLista * pElemCorr ;
               /* Ponteiro para o elemento corrente da lista */

         int numElem ;
               /* Número de elementos da lista */

         void ( * ExcluirValor ) ( void * pValor ) ;
               /* Ponteiro para a função de destruição do valor contido em um elemento */

   } LIS_tpLista ;

/***** Protótipos das funções encapuladas no módulo *****/

   static void LiberarElemento( LIS_tppLista   pLista ,
                                tpElemLista  * pElem   ) ;

   static tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                       void *       pValor  ) ;

   static void LimparCabeca( LIS_tppLista pLista ) ;

/*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: LIS  &Criar lista
*  ****/

   LIS_tpCondRet LIS_CriarLista( LIS_tppLista *ppLista ,
             void   ( * ExcluirValor ) ( void * pDado ) )
   {
	  
      *ppLista = ( LIS_tpLista * ) malloc( sizeof( LIS_tpLista )) ;
      if ( *ppLista == NULL )
      {
         return LIS_CondRetFaltouMemoria ;
      } /* if */

      LimparCabeca( *ppLista ) ;

      (*ppLista)->ExcluirValor = ExcluirValor ;

	#ifdef _DEBUG
		CED_DefinirTipoEspaco(*ppLista,LIS_TipoEspacoElemento);
	#endif

      return LIS_CondRetOK ; 

   } /* Fim função: LIS  &Criar lista */

/***************************************************************************
*
*  Função: LIS  &Destruir lista
*  ****/

   void LIS_DestruirLista( LIS_tppLista pLista )
   {

      LIS_EsvaziarLista( pLista ) ;

      free( pLista ) ;

   } /* Fim função: LIS  &Destruir lista */

/***************************************************************************
*
*  Função: LIS  &Esvaziar lista
*  ****/

   LIS_tpCondRet LIS_EsvaziarLista( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;
      tpElemLista * pProx ;
	        
	  if ( pLista == NULL ) /* Lista não existe */
	  {
		  return LIS_CondRetListaNaoExiste;
	  }/* if */

      pElem = pLista->pOrigemLista ;
      while ( pElem != NULL )
      {
         pProx = pElem->pProx ;
         LiberarElemento( pLista , pElem ) ;
         pElem = pProx ;
      } /* while */

      LimparCabeca( pLista ) ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Esvaziar lista */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento antes
*  ****/

   LIS_tpCondRet LIS_InserirElementoAntes( LIS_tppLista pLista ,
                                           void * pValor        )
   {

      tpElemLista * pElem ;

		 if ( pLista == NULL ) /* Lista não existe */
		 {
			 return LIS_CondRetListaNaoExiste;
		 } /* if */

      /* Criar elemento a inerir antes */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

		 #ifdef _DEBUG
         CED_DefinirTipoEspaco( pElem, LIS_TipoEspacoElemento ) ;
      #endif

      /* Encadear o elemento antes do elemento corrente */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pAnt != NULL )
            {
               pElem->pAnt  = pLista->pElemCorr->pAnt ;
               pLista->pElemCorr->pAnt->pProx = pElem ;
            } else
            {
               pLista->pOrigemLista = pElem ;
            } /* if */

            pElem->pProx = pLista->pElemCorr ;
            pLista->pElemCorr->pAnt = pElem ;
         } /* if */

         pLista->pElemCorr = pElem ;

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento antes */

/***************************************************************************
*
*  Função: LIS  &Inserir elemento após
*  ****/

   LIS_tpCondRet LIS_InserirElementoApos( LIS_tppLista pLista ,
                                          void * pValor        )
   {

      tpElemLista * pElem ;

		 if ( pLista == NULL ) /* Lista não existe */
		 {
			 return LIS_CondRetListaNaoExiste;
		 } /* if */

      /* Criar elemento a inserir após */

         pElem = CriarElemento( pLista , pValor ) ;
         if ( pElem == NULL )
         {
            return LIS_CondRetFaltouMemoria ;
         } /* if */

		  #ifdef _DEBUG
         CED_DefinirTipoEspaco( pElem, LIS_TipoEspacoElemento ) ;
      #endif

      /* Encadear o elemento após o elemento */

         if ( pLista->pElemCorr == NULL )
         {
            pLista->pOrigemLista = pElem ;
            pLista->pFimLista = pElem ;
         } else
         {
            if ( pLista->pElemCorr->pProx != NULL )
            {
               pElem->pProx  = pLista->pElemCorr->pProx ;
               pLista->pElemCorr->pProx->pAnt = pElem ;
            } else
            {
               pLista->pFimLista = pElem ;
            } /* if */

            pElem->pAnt = pLista->pElemCorr ;
            pLista->pElemCorr->pProx = pElem ;

         } /* if */
                  
         pLista->pElemCorr = pElem ;
                  
         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Inserir elemento após */

/***************************************************************************
*
*  Função: LIS  &Excluir elemento
*  ****/

   LIS_tpCondRet LIS_ExcluirElemento( LIS_tppLista pLista )
   {

      tpElemLista * pElem ;

	  if ( pLista == NULL ) /* Lista não existe */
	  {
			 return LIS_CondRetListaNaoExiste;
	  } /* if */

      if ( pLista->pElemCorr == NULL ) /* Lista vazia */
      {
         return LIS_CondRetListaVazia ;
      } /* if */

      pElem = pLista->pElemCorr ;

      /* Desencadeia à esquerda */

         if ( pElem->pAnt != NULL )
         {
            pElem->pAnt->pProx   = pElem->pProx ;
            pLista->pElemCorr    = pElem->pAnt ;
         } else 
		 {
            pLista->pElemCorr    = pElem->pProx ;
            pLista->pOrigemLista = pLista->pElemCorr ;
         } /* if */

      /* Desencadeia à direita */

         if ( pElem->pProx != NULL )
         {
            pElem->pProx->pAnt = pElem->pAnt ;
         } else
         {
            pLista->pFimLista = pElem->pAnt ;
         } /* if */

      LiberarElemento( pLista , pElem ) ;

      return LIS_CondRetOK ;

   } /* Fim função: LIS  &Excluir elemento */

/***************************************************************************
*
*  Função: LIS  &Obter referência para o valor contido no elemento
*  ****/

   LIS_tpCondRet LIS_ObterValor( LIS_tppLista pLista , void ** ppValor)
   {

	  if ( pLista == NULL ) /* Lista não existe */
	  {
		  return LIS_CondRetListaNaoExiste;
	  } /* if */

      if ( pLista->pElemCorr == NULL ) /* Lista vazia */
      {
		  return LIS_CondRetListaVazia ;
      } /* if */

      (*ppValor)= pLista->pElemCorr->pValor ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Obter referência para o valor contido no elemento */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento inicial
*  ****/

   LIS_tpCondRet LIS_IrInicioLista( LIS_tppLista pLista )
   {

	  if ( pLista == NULL ) /* Lista não existe */
	  {
		  return LIS_CondRetListaNaoExiste;
	  } /* if */

	  if ( pLista->pElemCorr == NULL ) /* Lista vazia */
      {
		  return LIS_CondRetListaVazia ;
      } /* if */

      pLista->pElemCorr = pLista->pOrigemLista ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Ir para o elemento inicial */

/***************************************************************************
*
*  Função: LIS  &Ir para o elemento final
*  ****/

   LIS_tpCondRet LIS_IrFinalLista( LIS_tppLista pLista )
   {

	  if ( pLista == NULL ) /* Lista não existe */
	  {
		  return LIS_CondRetListaNaoExiste;
	  } /* if */

	  if ( pLista->pElemCorr == NULL ) /* Lista vazia */
      {
		  return LIS_CondRetListaVazia ;
      } /* if */

      pLista->pElemCorr = pLista->pFimLista ;

	  return LIS_CondRetOK;

   } /* Fim função: LIS  &Ir para o elemento final */

/***************************************************************************
*
*  Função: LIS  &Avançar elemento
*  ****/

   LIS_tpCondRet LIS_AvancarElementoCorrente( LIS_tppLista pLista ,
                                              int numElem )
   {

      int i ;

      tpElemLista * pElem ;

	  if ( pLista == NULL ) /* Lista não existe */
	  {
			 return LIS_CondRetListaNaoExiste;
	  } /* if */

      /* Tratar lista vazia */

         if ( pLista->pElemCorr == NULL )
         {
            return LIS_CondRetListaVazia ;
         } /* fim ativa: Tratar lista vazia */

      /* Tratar avançar para frente */

         if ( numElem > 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i > 0 ; i-- )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pProx ;
            } /* for */

			

            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pFimLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avançar para frente */

      /* Tratar avançar para trás */

         else if ( numElem < 0 )
         {

            pElem = pLista->pElemCorr ;
            for( i = numElem ; i < 0 ; i++ )
            {
               if ( pElem == NULL )
               {
                  break ;
               } /* if */
               pElem    = pElem->pAnt ;
            } /* for */
			
			
            if ( pElem != NULL )
            {
               pLista->pElemCorr = pElem ;
               return LIS_CondRetOK ;
            } /* if */

            pLista->pElemCorr = pLista->pOrigemLista ;
            return LIS_CondRetFimLista ;

         } /* fim ativa: Tratar avançar para trás */

      /* Tratar não avançar */

         return LIS_CondRetOK ;

   } /* Fim função: LIS  &Avançar elemento */

/***************************************************************************
*
*  Função: LIS  &Alterar Valor do Elemento Corrente
*  ****/

   LIS_tpCondRet LIS_AlterarValor( LIS_tppLista pLista ,
                                          void * pValorNovo ) 
   {
	   if(pLista == NULL) /* Lista não existe */
	   {
		   return LIS_CondRetListaNaoExiste;
	   }/* if */	   
	   if(pLista->pElemCorr == NULL) /* Lista vazia */
	   {
		   return LIS_CondRetListaVazia;
	   }/* if */
	  
	   pLista->pElemCorr->pValor=pValorNovo;

	   return LIS_CondRetOK;

   } /* Fim função: LIS  &Alterar valor do elemento corrente */

   
 #ifdef _DEBUG

/***************************************************************************
*
*  Função: LIS  &Deturpar Lista
*  ****/

   void LIS_Deturpar( void * pListaParm ,
                      int ModoDeturpar )
   {

	  static char EspacoLixo[ 256 ] =
             "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
            /* Espaço de dados lixo usado ao testar */
      LIS_tpLista * pLista = NULL ;

      if ( pListaParm == NULL )
      {
         return ;
      } /* if */

      pLista = ( LIS_tpLista * )( pListaParm ) ;

      switch ( ModoDeturpar ) {

      /* Anula ponteiro sucessor */

         case 0 :
         {

			  printf("%p\n",pLista->pElemCorr->pProx);
			 pLista->pElemCorr->pProx=NULL;
			 printf("%p\n",pLista->pElemCorr->pProx);

            break ;

         } /* fim ativa: Anula ponteiro sucessor */

      /* Anula ponteiro predecessor */

         case 1 :
         {

 			  printf("%p\n",pLista->pElemCorr->pAnt);
			 pLista->pElemCorr->pAnt=NULL;
			 printf("%p\n",pLista->pElemCorr->pAnt);

            break ;

         } /* fim ativa: Anula ponteiro predecessor */

	  /* Faz sucessor apontar para lixo  */

         case 2 :
         {

 			 printf("%p\n",pLista->pElemCorr->pProx);
			 pLista->pElemCorr->pProx=(tpElemLista *)(EspacoLixo);
			 printf("%p\n",pLista->pElemCorr->pProx);

            break ;

         } /* fim ativa: Faz sucessor apontar para lixo  */

	  /* Faz predecessor apontar para lixo */

         case 3 :
         {

 			
			 pLista->pElemCorr->pAnt=(tpElemLista *)(EspacoLixo);

            break ;

         } /* fim ativa: Faz predecessor apontar para lixo */

	   /* Destaca vértice */

		 case 4 :
         {

 			pLista->pElemCorr->pValor=NULL;
			LIS_ExcluirElemento(pLista);

            break ;

         } /* fim ativa: Destaca vértice */


      /* ModoDetrupar desconhecido */

         default :
		 {
			
			printf("ModoDeturpar desconhecido");
         
            break ;

         } /* fim ativa: ModoDetrupar desconhecido */

      } /* fim seleciona: Raiz de GRF  &Deturpar grafo */

   } /* Fim função: GRF  &Deturpar grafo */

#endif

/*****  Código das funções encapsuladas no módulo  *****/


/***********************************************************************
*
*  $FC Função: LIS  -Liberar elemento da lista
*
*  $ED Descrição da função
*     Elimina os espaços apontados pelo valor do elemento e o
*     próprio elemento.
*
***********************************************************************/

   void LiberarElemento( LIS_tppLista   pLista ,
                         tpElemLista  * pElem   )
   {

      if ( ( pLista->ExcluirValor != NULL )
        && ( pElem->pValor != NULL        ))
      {
         pLista->ExcluirValor( pElem->pValor ) ;
      } /* if */

      free( pElem ) ;

      pLista->numElem-- ;

   } /* Fim função: LIS  -Liberar elemento da lista */


/***********************************************************************
*
*  $FC Função: LIS  -Criar o elemento
*
*  $ED Descrição da função
*     Cria um elemento de lista com um ponteiro para o valor fornecido,
*	  aumenta o número de elementos contado pela cabeça da lista mas
*	  não insere o elemento na lista.
*
***********************************************************************/

   tpElemLista * CriarElemento( LIS_tppLista pLista ,
                                void *       pValor  )
   {

      tpElemLista * pElem ;

      pElem = ( tpElemLista * ) malloc( sizeof( tpElemLista )) ;
      if ( pElem == NULL )
      {
         return NULL ;
      } /* if */

      pElem->pValor = pValor ;
      pElem->pAnt   = NULL  ;
      pElem->pProx  = NULL  ;

      pLista->numElem ++ ;

      return pElem ;

   } /* Fim função: LIS  -Criar o elemento */


/***********************************************************************
*
*  $FC Função: LIS  -Limpar a cabeça da lista
*
*  $ED Descrição da função
*    Limpa a cabeça da lista atribuindo NULL a todos os ponteiros
*    e 0 ao número de elementos.
*
*
***********************************************************************/

   void LimparCabeca( LIS_tppLista pLista )
   {

      pLista->pOrigemLista = NULL ;
      pLista->pFimLista = NULL ;
      pLista->pElemCorr = NULL ;
      pLista->numElem   = 0 ;

   } /* Fim função: LIS  -Limpar a cabeça da lista */

   #ifdef _DEBUG

   LIS_tpCondRet VerificaLista (LIS_tppLista pLista)
   {
	   LIS_tppLista ant = NULL;
	   tpElemLista * aux;
	   int elementosPercorridos = 1 ;
	   printf("verificar lista 1\n");
	   if( pLista == NULL )
	   {
		   return LIS_CondRetOK;
	   }/*if*/

	   aux = pLista->pOrigemLista;
	   printf("verificar lista 2\n");
	   if( aux->pAnt != NULL )
	   {
		   CNT_CONTAR ("LIS_ErroPrimeiro") ;

		    CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ; //algo antes do primeiro elemento
	   }/* if */
	    printf("verificar lista 3\n");
	   do
	   {
		   printf("HAHAHA: %d, %d\n",LIS_TipoEspacoElemento,CED_ObterTipoEspaco(aux));
			if(TST_CompararInt(LIS_TipoEspacoElemento,CED_ObterTipoEspaco(aux),"Tipo de espaço do elemento não é elemento da lista")!=TST_CondRetOK)
			{
				CNT_CONTAR(CONTADOR_FALHAS_ESTRUTURA);
				printf("vai retornar\n");
				return LIS_CondRetErroEstrutura;
			}
			printf("--\n");
		   if(TST_CompararPonteiro(aux->pProx->pAnt,aux,"Erro no ponteiro para proximo")!=TST_CondRetOK)
		   {
			   CNT_CONTAR ("LIS_ErroProximo") ;

			   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
		   }/*if*/

		   aux=aux->pProx ;

		   elementosPercorridos++;

	   } while(aux->pProx!=NULL);
	    printf("verificar lista 4\n");
	   if(elementosPercorridos!=pLista->numElem)
	   {
		   CNT_CONTAR ("LIS_ErroNumElem") ;

		   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ; //numero de elementos percorridos é diferente do numero da cabeça da lista
	   }/* if */

	    printf("verificar lista 5\n");

	   if(TST_CompararPonteiro(pLista->pFimLista,aux,"Ultimo elemento nao e o esperado.")!=TST_CondRetOK)
	   {
		   CNT_CONTAR ("LIS_ErroUltimo") ;

		   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
	   }/* if */
	   return LIS_CondRetOK;
   }

#endif

/********** Fim do módulo de implementação: LIS  Lista duplamente encadeada **********/

