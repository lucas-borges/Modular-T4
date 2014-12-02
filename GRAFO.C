/***************************************************************************
*  $MCI Módulo de implementação: GRF Grafo não orientado
*
*  Arquivo gerado:              GRAFO.c
*  Letras identificadoras:      grf
*
*  Projeto: INF 1301 Automatização dos testes de módulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*	  1		 csgslb 23/set/2014 início desenvolvimento
*
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <malloc.h>

#include "LISTA.H"
#include "GRAFO.H"

#ifdef _DEBUG
   #include   "Generico.h"
   #include   "Conta.h"
   #include   "cespdin.h"
   #include   "IdTiposEspaco.def"
#endif

/***********************************************************************
*
*  $TC Tipo de dados: GRF Vértice
*
*
***********************************************************************/

   typedef struct tagVertice {

		#ifdef _DEBUG

         GRF_tppGrafo * pCabeca ;
               /* Ponteiro para cabeca
               *
               *$ED Descrição
               *   Todos os vértices do grafo devem apontar para a respectiva cabeça.
               *   Esse ponteiro corresponde a um identificador do grafo para fins
               *   de verificação da integridade. */

         #endif

         void * pValor ;
               /* Ponteiro para o valor contido no elemento */

         int visitado;
               /* Ponteiro para o elemento predecessor */

         LIS_tppLista arestas ;
               /* Ponteiro para o elemento sucessor */	
		 
		 int chave ;
			   /* Valor que representa chave do vertice */

		 void ( * ExcluirValor ) ( void * pvalor ) ;
			   /* Ponteiro para a função de destruição do valor contido em um vertice */

   } tpVertice ;

/***********************************************************************
*
*  $TC Tipo de dados: GRF Descritor da cabeça de grafo
*
*
***********************************************************************/

   typedef struct GRF_tagGrafo {

         LIS_tppLista vertices;

		 tpVertice * pVerticeCorr;

		 void ( * ExcluirValor ) ( void * pvalor ) ;
			   /* Ponteiro para a função de destruição do valor contido em um vertice */

   } GRF_tpGrafo ;


   /*****  Dados encapsulados no módulo  *****/

      #ifdef _DEBUG

      static char EspacoLixo[ 256 ] =
             "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
            /* Espaço de dados lixo usado ao testar */

      #endif


   /***** Protótipos das funções encapuladas no módulo *****/

   int BuscarVertice ( int chave , LIS_tppLista pLista );

   void DestruirVertice ( void * pVertice );

   int EncontraCaminho ( tpVertice * atual , tpVertice * destino , int * buffer );

   #ifdef _DEBUG

   static GRF_tpCondRet VerificarVertices( GRF_tppGrafo pGrafo );
   static GRF_tpCondRet VerificarArestas ( tpVertice * vertice ) ;
   static GRF_tpCondRet GRF_VerificarCabeca( void * pCabecaParm );

	#endif

   /*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: GRF  &Criar grafo
*****/

   GRF_tpCondRet GRF_CriarGrafo ( GRF_tppGrafo * ppGrafo , void ( * ExcluirValor ) ( void * pValor ) )
   {

	   *ppGrafo= ( GRF_tpGrafo * ) malloc ( sizeof ( GRF_tpGrafo )) ;
	   if ( *ppGrafo == NULL )
	   {
		   return GRF_CondRetFaltouMemoria;
	   } /* if */

	   #ifdef _DEBUG
         CED_DefinirTipoEspaco( ppGrafo, GRF_TipoEspacoCabeca ) ;
      #endif

	   LIS_CriarLista ( &( ( * ppGrafo ) -> vertices ) , DestruirVertice ) ; //ao destruir a lista de vértices tem que destruir o vértice
	   (*ppGrafo)->ExcluirValor=ExcluirValor;
	   (*ppGrafo)->pVerticeCorr=NULL;

	   return GRF_CondRetOK;

   }   /* Fim função: GRF  &Criar Grafo */


/***************************************************************************
*
*  Função: GRF  &Criar Vertice
*****/
   
 GRF_tpCondRet GRF_CriaVertice ( GRF_tppGrafo pGrafo , void * pValor , int chave ) 
 {

	   int Ret;
	   tpVertice* vertice;
	   

	   if(pGrafo==NULL)
	   {
		   return GRF_CondRetGrafoNaoExiste;
	   } /* if */

	   if(GRF_ExisteVertice ( pGrafo , chave ) == GRF_CondRetVerticeJaExiste ) //Checa se o vertice já existe
	   {
			return GRF_CondRetVerticeJaExiste ;
	   } /* if */


	   vertice = (tpVertice * ) malloc ( sizeof (tpVertice) );//cria vertice
	   if ( vertice == NULL )
	   {
		   return GRF_CondRetFaltouMemoria;
	   } /* if */

	   #ifdef _DEBUG
         CED_DefinirTipoEspaco( vertice, GRF_TipoEspacoVertice ) ;
      #endif

	   vertice->pValor = pValor;
	   vertice->ExcluirValor=pGrafo->ExcluirValor;
	   LIS_CriarLista ( &vertice->arestas , NULL ); //NULL pois a lista vai apontar para outros vértices e não queremos que 
													//ao remover uma aresta os vértices que elas apontam sejam removidos também
	   vertice->chave=chave;

	   if( vertice->arestas == NULL )
	   {
		   return GRF_CondRetFaltouMemoria;
	   } /* if */

	   Ret = LIS_InserirElementoApos ( pGrafo->vertices , vertice );
	   if( Ret != LIS_CondRetOK )
	   {
		   return GRF_CondRetErroInserirVertice;
	   } /* if */

	   if(pGrafo->pVerticeCorr==NULL) //Caso seja o primeiro seta corrente
	   {
		   pGrafo->pVerticeCorr=vertice;
	   } /* if */

	   return GRF_CondRetOK;

   }  /* Fim função: GRF  &Criar Vertice */


/***************************************************************************
*
*  Função: GRF &Cria Aresta
*****/

   GRF_tpCondRet GRF_CriaAresta(GRF_tppGrafo pGrafo , int chaveA, int chaveB)
   {

	   LIS_tpCondRet lis_ret;
	   tpVertice * verticeA, * verticeB;
	   void * pTemp;

	   if(pGrafo==NULL)
	   {
		   return GRF_CondRetGrafoNaoExiste;
	   } /* if */

	   /*Verifica se os vertices existem*/

	   if(GRF_ExisteVertice(pGrafo , chaveA)==GRF_CondRetVerticeNaoExiste)  //vertice nao existe
	   {
		   return GRF_CondRetVerticeNaoExiste; 
	   }/* if */

	   
	   if(GRF_ExisteVertice(pGrafo , chaveB)==GRF_CondRetVerticeNaoExiste)  //vertice nao existe
	   {
		   return GRF_CondRetVerticeNaoExiste; 
	   } /* if */

	   /*Verifica se aresta já existe*/

	   if(GRF_ExisteAresta(pGrafo,chaveA,chaveB)==GRF_CondRetArestaJaExiste)  //aresta já existe
	   {
		   return GRF_CondRetArestaJaExiste; 
	   } /* if */	   
	   
	   /*Inserir Aresta de a para b e de b para a*/

	   BuscarVertice ( chaveA , pGrafo->vertices ) ;
	   LIS_ObterValor( pGrafo->vertices , &pTemp );
	   verticeA = ( tpVertice * ) pTemp ;

	   BuscarVertice ( chaveB , pGrafo->vertices ) ;
	   LIS_ObterValor( pGrafo->vertices , &pTemp );
	   verticeB = ( tpVertice * ) pTemp ;

	   lis_ret= LIS_InserirElementoApos(verticeA->arestas,verticeB);

	   if(lis_ret!=LIS_CondRetOK)
	   {
		   return GRF_CondRetErroInserirAresta;
	   } /* if */

	   lis_ret= LIS_InserirElementoApos(verticeB->arestas,verticeA);

	   if(lis_ret!=LIS_CondRetOK)
	   {
		   return GRF_CondRetErroInserirAresta; 
	   } /* if */

	   return GRF_CondRetOK;

   } /* Fim função: GRF  &Cria Aresta */


/***************************************************************************
*
*  Função: GRF  &Remove Aresta
*****/
   
   GRF_tpCondRet GRF_RemoveAresta(GRF_tppGrafo pGrafo, int chaveA , int chaveB)
   {

	   tpVertice * verticeA, *verticeB;
	   void * pTemp;
	   
	   if(pGrafo==NULL)
	   {
		   return GRF_CondRetGrafoNaoExiste;
	   } /* if */

	    /*Verifica se os vertices existem*/

	   if(GRF_ExisteVertice(pGrafo , chaveA)==GRF_CondRetVerticeNaoExiste)  //vertice nao existe
	   {
		   return GRF_CondRetVerticeNaoExiste; 
	   }/* if */

	   
	   if(GRF_ExisteVertice(pGrafo , chaveB)==GRF_CondRetVerticeNaoExiste)  //vertice nao existe
	   {
		   return GRF_CondRetVerticeNaoExiste; 
	   } /* if */

	   /*Verifica se aresta não existe*/

	   if(GRF_ExisteAresta(pGrafo,chaveA,chaveB)==GRF_CondRetArestaNaoExiste)  //aresta não existe
	   {
		   return GRF_CondRetArestaNaoExiste; 
	   } /* if */	   
	   
	   /*Excluir Aresta de a para b e de b para a*/

	   BuscarVertice ( chaveA , pGrafo->vertices ) ;
	   LIS_ObterValor( pGrafo->vertices , &pTemp );
	   verticeA = ( tpVertice * ) pTemp ;

	   BuscarVertice ( chaveB , pGrafo->vertices ) ;
	   LIS_ObterValor( pGrafo->vertices , &pTemp );
	   verticeB = ( tpVertice * ) pTemp ;
	   
	   BuscarVertice(chaveB, verticeA->arestas );
	   
	   LIS_ExcluirElemento(verticeA->arestas);
	   
	   BuscarVertice(chaveA, verticeB->arestas );
	   
	   LIS_ExcluirElemento(verticeB->arestas);
	   
	   return GRF_CondRetOK;

   }  /* Fim função: GRF  &Remove Aresta */

   
/***************************************************************************
*
*  Função: GRF  &Destroi grafo
*****/
   
   void GRF_DestroiGrafo (GRF_tppGrafo pGrafo)
   {

	   if(pGrafo==NULL)
	   {
		   return ;
	   } /* if */

	   LIS_DestruirLista(pGrafo->vertices);
	   free(pGrafo);

   } /* Fim função: GRF  &Destroi grafo */
   

/***************************************************************************
*
*  Função: GRF  &Esvaziar Grafo
*****/

   GRF_tpCondRet GRF_EsvaziaGrafo ( GRF_tppGrafo pGrafo )
   {
	   
	   if ( pGrafo == NULL )
	   {
		   return GRF_CondRetGrafoNaoExiste ;
	   } /* if */

	   LIS_EsvaziarLista ( pGrafo->vertices ) ;
	   pGrafo->pVerticeCorr=NULL;
	   
	   return GRF_CondRetOK ;

   } /* Fim função: GRF  &Esvaziar Grafo */

  
/***************************************************************************
*
*  Função: GRF  &Obtem Caminho
*****/

   GRF_tpCondRet GRF_ObtemCaminho ( GRF_tppGrafo pGrafo , int chaveOrigem , int chaveDestino , int * buffer )
   {

	   tpVertice * origem , * destino , * aux ;
	   void * temp;
	   
	   if ( pGrafo == NULL )
	   {
		   return GRF_CondRetGrafoNaoExiste ;
	   } /* if */

	   if ( BuscarVertice ( chaveOrigem , pGrafo->vertices ) == -1 )
	   {
		   return GRF_CondRetVerticeNaoExiste ;
	   } /* if */

	   if ( LIS_ObterValor ( pGrafo->vertices , &temp ) != LIS_CondRetOK )
	   {
		   return GRF_CondRetErroAoObterValor ;
	   } /* if */

	   origem = ( tpVertice * ) temp ;


	   if ( BuscarVertice ( chaveDestino , pGrafo->vertices ) == -1 )
	   {
		   return GRF_CondRetVerticeNaoExiste ;
	   } /* if */

	   if ( LIS_ObterValor ( pGrafo->vertices , &temp ) != LIS_CondRetOK )
	   {
		   return GRF_CondRetErroAoObterValor ;
	   } /* if */

	   destino = ( tpVertice * ) temp ;

	   if ( LIS_IrInicioLista ( pGrafo->vertices ) != LIS_CondRetOK )
	   {
		   return GRF_CondRetErroAoObterValor ;
	   } /* if */
 
	   do 
	   {			
		
		    LIS_ObterValor ( pGrafo->vertices , &temp ) ;
			aux = ( tpVertice * ) temp ;
			aux->visitado = 0 ;

	   } while ( LIS_AvancarElementoCorrente ( pGrafo->vertices , 1 ) == LIS_CondRetOK ) ;

	   LIS_IrInicioLista ( origem->arestas ) ;
	   
	   if ( EncontraCaminho ( origem , destino , buffer) == -1 )
	   {
		   return GRF_CondRetVerticesDesconexos ;
	   } /* if */

	   return GRF_CondRetOK;

   }   /* Fim função: GRF  &Obtem Caminho */

/***************************************************************************
*
*  Função: GRF  &Existe Vértice
*****/

   GRF_tpCondRet GRF_ExisteVertice ( GRF_tppGrafo pGrafo , int chave )
   {

	   if ( pGrafo == NULL )
	   {
		   return GRF_CondRetGrafoNaoExiste;
	   }/* if */
	   
	   if (BuscarVertice(chave, pGrafo->vertices)==1)
	   {
		   return GRF_CondRetVerticeJaExiste;
	   } /* if */

	   return GRF_CondRetVerticeNaoExiste;

   } /* Fim função: GRF  &Existe Vértice */

/***************************************************************************
*
*  Função: GRF  &Alterar Corrente
*****/
   
 GRF_tpCondRet GRF_AlteraCorrente ( GRF_tppGrafo pGrafo , int chave ) 
 {
	 void * ptemp;

	 if (pGrafo==NULL) 
	 {
		 return GRF_CondRetGrafoNaoExiste;
	 } /* if */

	 if (BuscarVertice (chave, pGrafo->vertices)==-1)
	 {
		 return GRF_CondRetVerticeNaoExiste;
	 }/* if */
	 
	 LIS_ObterValor(pGrafo->vertices,&ptemp);
	 pGrafo->pVerticeCorr=(tpVertice*)ptemp;

	 return GRF_CondRetOK;

   }  /* Fim função: GRF  &Alterar Corrente */

 /***************************************************************************
*
*  Função: GRF  &Existe Aresta
*****/
   
 GRF_tpCondRet GRF_ExisteAresta ( GRF_tppGrafo pGrafo , int chaveA , int chaveB ) 
 { 
	   tpVertice * verticeA, * verticeB;
	   void * temp;   
	 
	   if(pGrafo==NULL)
	   {
		   return GRF_CondRetGrafoNaoExiste;
	   } /* if */

	   /*Verifica se os vertices existem*/

	   if(BuscarVertice(chaveA , pGrafo->vertices )==-1)  //vertice nao existe
	   {
		   return GRF_CondRetVerticeNaoExiste; 
	   }/* if */

	   LIS_ObterValor ( pGrafo->vertices, &temp ) ;
	   verticeA = ( tpVertice * ) temp ;

	   if(BuscarVertice(chaveB ,pGrafo->vertices )==-1)  //vertice nao existe
	   {
		   return GRF_CondRetVerticeNaoExiste; 
	   } /* if */

	   LIS_ObterValor ( pGrafo->vertices, &temp ) ;
	   verticeB = ( tpVertice * ) temp ;

	   /*Verifica se aresta já existe*/

	   if(BuscarVertice(chaveB, verticeA->arestas )==1)  //aresta já existe
	   {
		   return GRF_CondRetArestaJaExiste; 
	   } /* if */

	   return GRF_CondRetArestaNaoExiste;

 } /* Fim função: GRF  &Existe Aresta */

 /***************************************************************************
*
*  Função: GRF  &Ir Vizinho
*****/
 GRF_tpCondRet GRF_IrVizinho (GRF_tppGrafo pGrafo , int chave)
 {
	 void * pTemp;
	 
	 if(pGrafo==NULL)
	 {
		 return GRF_CondRetGrafoNaoExiste ;
	 }/* if */

	 if(GRF_ExisteVertice(pGrafo,chave)==GRF_CondRetVerticeNaoExiste)
	 {
		 return GRF_CondRetVerticeNaoExiste;
	 }/* if */

	 if( BuscarVertice( chave , pGrafo->pVerticeCorr->arestas ) == -1 )
	 {
		 return GRF_CondRetVerticesDesconexos;
	 }/* if */

	 LIS_ObterValor(pGrafo->pVerticeCorr->arestas,&pTemp);

	 pGrafo->pVerticeCorr = (tpVertice * ) pTemp ;
	 
	 return GRF_CondRetOK;

 } /* Fim função: GRF  &Ir Vizinho */

 /***************************************************************************
*
*  Função: GRF  &Obter Corrente
*****/

 GRF_tpCondRet GRF_ObterCorr ( GRF_tppGrafo pGrafo , int * pChave , void ** ppValor )
 {
	 if(pGrafo==NULL)
	 {
		 return GRF_CondRetGrafoNaoExiste;
	 }/* if */

	 if(pGrafo->pVerticeCorr==NULL)
	 {
		 return GRF_CondRetVerticeNaoExiste;
	 }/* if */

	 *pChave = pGrafo->pVerticeCorr->chave ;

	 if (ppValor !=NULL)
	 {
		* ppValor = pGrafo->pVerticeCorr->pValor ;
	 } /* if */

	 return GRF_CondRetOK; 

 }/* Fim função: GRF  &Obter Corrente */

 /***************************************************************************
*
*  Função: GRF  &Alterar Valor Corrente
*****/

 GRF_tpCondRet GRF_AlterarValorCorr ( GRF_tppGrafo pGrafo , void * pValor )
 {
	 if( pGrafo == NULL )
	 {
		 return GRF_CondRetGrafoNaoExiste;
	 }/* if */
	 
	 if(pGrafo->pVerticeCorr==NULL)
	 {
		 return GRF_CondRetVerticeNaoExiste;
	 }/* if */

	 pGrafo->ExcluirValor(pGrafo->pVerticeCorr->pValor);

	 pGrafo->pVerticeCorr->pValor=pValor ;

	 return GRF_CondRetOK;	

 }/* Fim função: GRF  &Alterar Valor Corrente */

 /***************************************************************************
*
*  Função: GRF  &Caminhar
*****/

 GRF_tpCondRet GRF_Caminhar ( GRF_tppGrafo pGrafo , int chaveDestino )
 {
	 tpVertice * verticeDestino ;
	 void * pTemp;

	 if( pGrafo == NULL )
	 {
		 return GRF_CondRetGrafoNaoExiste ;
	 }/* if */

	 if(pGrafo->pVerticeCorr==NULL)
	 {
		 return GRF_CondRetVerticeNaoExiste;
	 }/* if */

	 if(GRF_ExisteVertice (pGrafo,chaveDestino) == GRF_CondRetVerticeNaoExiste )
	 {
		 return GRF_CondRetVerticeNaoExiste;
	 }/* if */	 

	 LIS_ObterValor(pGrafo->vertices , &pTemp);
	 verticeDestino = (tpVertice * ) pTemp ;
	 
	 if ( GRF_ObtemCaminho ( pGrafo , pGrafo->pVerticeCorr->chave , chaveDestino , NULL ) != GRF_CondRetOK )
	 {
		 return GRF_CondRetVerticesDesconexos;
	 }/* if */

	 pGrafo->pVerticeCorr=verticeDestino;

	 return GRF_CondRetOK;
	 
 }/* Fim função: GRF  &Caminhar */

    #ifdef _DEBUG

/***************************************************************************
*
*  Função: GRF  &Verificar um grafo
*  ****/

   GRF_tpCondRet GRF_VerificarGrafo( void * pGrafoParm )
   {

      GRF_tppGrafo pGrafo = NULL ;

	  if( pGrafoParm == NULL )
	  {
		  return GRF_CondRetOK ;
	  } /* if */

      if ( GRF_VerificarCabeca( pGrafoParm ) != GRF_CondRetOK )
      {
         return GRF_CondRetErroEstrutura ;
      } /* if */

      CED_MarcarEspacoAtivo( pGrafoParm ) ;

      pGrafo = ( GRF_tppGrafo ) ( pGrafoParm ) ;

	  return VerificarVertices( pGrafo ) ;

   } /* Fim função: ARV  &Verificar uma árvore */

#endif

 #ifdef _DEBUG

/***************************************************************************
*
*  Função: GRF  &Deturpar Grafo
*  ****/

   void GRF_Deturpar( void * pGrafoParm ,
                      GRF_tpModosDeturpacao ModoDeturpar )
   {

      GRF_tpGrafo * pGrafo = NULL ;

      if ( pGrafoParm == NULL )
      {
         return ;
      } /* if */

      pGrafo = ( GRF_tpGrafo * )( pGrafoParm ) ;

      switch ( ModoDeturpar ) {

      /* Elimina elemento corrente */

         case DeturpaEliminaCorrente :
         {

			
			 free(pGrafo->pVerticeCorr);
			 printf("oi\n");
			 printf("chave: %c",*((char *)pGrafo->pVerticeCorr->pValor));

            break ;

         } /* fim ativa: Elimina elemento corrente */

      /* Anula ponteiro sucessor */

         case DeturpaSucessorNulo :
         {

 			 
			 LIS_Deturpar(pGrafo->vertices,0);

			 if(LIS_AvancarElementoCorrente(pGrafo->vertices,1)==LIS_CondRetFimLista){
				 printf("fim \n");}
			 

            break ;

         } /* fim ativa: Anula ponteiro sucessor */

      /* Anula ponteiro predecessor */

         case DeturpaPredecessorNulo :
         {

			 
			 LIS_Deturpar(pGrafo->vertices,1);

            break ;

         } /* fim ativa: Anula ponteiro predecessor */

      /* Faz sucessor apontar para lixo */

         case DeturpaSucessorLixo :
         {

			 
			  LIS_Deturpar(pGrafo->vertices,2);
			  LIS_AvancarElementoCorrente(pGrafo->vertices,1);
			  LIS_AvancarElementoCorrente(pGrafo->vertices,-1);
			  printf("oi");
	
            break ;

         } /* fim ativa: Faz sucessor apontar para lixo */

      /* Faz predecessor apontar para lixo */

         case DeturpaPredecessorLixo :
         {

			 
			  LIS_Deturpar(pGrafo->vertices,3);

            break ;

         } /* fim ativa: Faz predecessor apontar para lixo */

      /* Anula valor do vértice corrente */

         case DeturpaValorNulo :
         {

			pGrafo->pVerticeCorr->pValor=NULL;

            break ;

         } /* fim ativa: Anula valor do vértice corrente */

	  /* Altera tipo de estrutra apontado pelo vértice */

         case DeturpaTipoEstrutura :
         {

			CED_DefinirTipoEspaco( pGrafo->pVerticeCorr->pValor , CED_ID_TIPO_VALOR_NULO ) ;

            break ;

         } /* fim ativa: Altera tipo de estrutra apontado pelo vértice */

      /* Destaca vértice do grafo sem liberá-lo */

         case DeturpaDestacaVertice :
         {

			 tpVertice * aux;
			 tpVertice * aux2;
			 void *pvalor;

			 LIS_ObterValor(pGrafo->vertices,&pvalor);
			 aux=(tpVertice *) pvalor;
			 GRF_AlteraCorrente(pGrafo,aux->chave);

			 LIS_Deturpar(pGrafo->vertices, 4);
			 LIS_IrInicioLista(aux->arestas);

			 while (LIS_AvancarElementoCorrente(aux->arestas,1)==LIS_CondRetOK)
			 {
				 LIS_ObterValor(aux->arestas,&pvalor);
				 aux2=(tpVertice*)pvalor;
				 GRF_RemoveAresta(pGrafo,aux->chave,aux2->chave);
			 } /* while */

            break ;

         } /* fim ativa: Destaca cértice do grafo sem liberá-lo */

      /* Anula ponteiro pra elemento corrente */

         case DeturpaCorrenteNulo :
         {

			pGrafo->pVerticeCorr=NULL;

            break ;

         } /* fim ativa: Anula ponteiro pra elemento corrente */

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
*  $FC Função: GRF  -Destruir vértice
*
*  $ED Descrição da função
*     
*
***********************************************************************/
  
   void DestruirVertice ( void * pVertice )
   {
		tpVertice* ptemp = (tpVertice*) pVertice;

	    LIS_DestruirLista(ptemp->arestas);
		if (ptemp->ExcluirValor!=NULL)
		{
			ptemp->ExcluirValor(ptemp->pValor);
		} /* if */
		free(pVertice);

   } /* Fim função: GRF  -Destruir vértice */

/***********************************************************************
*
*  $FC Função: GRF  -Buscar Vértice
*
*  $ED Descrição da função
*     
*
***********************************************************************/

   int BuscarVertice ( int chave , LIS_tppLista pLista )
   {

	    void * temp;
		tpVertice * vertice;
		LIS_tpCondRet lis_ret;
		LIS_IrInicioLista(pLista);

		do
		{
			lis_ret=LIS_ObterValor ( pLista, &temp ) ;

			if(lis_ret!=LIS_CondRetOK)
			{
				return -1;
			}
			vertice = ( tpVertice * ) temp ;

			if ( vertice->chave == chave ) 
			{
				return 1;
			} /* if */


		} while ( LIS_AvancarElementoCorrente ( pLista , 1 ) == LIS_CondRetOK ) ;

		return -1 ;

   } /* Fim função: GRF  -Buscar Vértice */

  /***********************************************************************
*
*  $FC Função: GRF  -Encontra Caminho
*
*  $ED Descrição da função
*     
*
***********************************************************************/
  
   int EncontraCaminho ( tpVertice * atual , tpVertice * destino , int * buffer)
   {
	   void * temp ;
	   tpVertice * proximo ;

	     
	   if ( atual->visitado == 1 )
	   {
		   return -1 ;
	   } /* if */
	   
	   atual->visitado = 1 ;


	   if ( atual->chave == destino->chave )
	   {
		   if(buffer != NULL)
		   {
			   buffer[0]=atual->chave;
			   buffer[1]=-1;
		   }/* if */
		   
		   return 1;
	   } /* if */

	   if(LIS_IrInicioLista(atual->arestas)!=LIS_CondRetOK)
	   {
		   return -1;
	   } /* if */
	   
	   do
	   {
		   LIS_ObterValor (atual->arestas , &temp );   
		   proximo = ( tpVertice * ) temp ;

		
		   if ( buffer!=NULL )
		   {
			   if(EncontraCaminho ( proximo , destino , &buffer[1] ) == 1)
			   {
					buffer[0]=atual->chave;
					return 1 ;
		       }/* if */
		   }/* if */
		   else if (EncontraCaminho ( proximo , destino , NULL ) == 1 )
		   {
			   return 1 ;
		   }/* if */

	   } while(LIS_AvancarElementoCorrente ( atual->arestas , 1 )==LIS_CondRetOK);

	   return -1 ;
	  

   } /* Fim função: GRF  -Encontra caminho */


#ifdef _DEBUG

/***************************************************************************
*
*  Função: GRF  &Verificar um nó cabeça
*  ****/

   GRF_tpCondRet GRF_VerificarCabeca( void * pCabecaParm )
   {

      GRF_tppGrafo pGrafo = NULL ;

      /* Verifica o tipo do espaço */

         if ( ! CED_VerificarEspaco( pCabecaParm , NULL ))
         {
            TST_NotificarFalha( "Controle do espaço acusou erro." ) ;
			CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
            return GRF_CondRetErroEstrutura ;
         } /* if */

         if ( TST_CompararInt( GRF_TipoEspacoCabeca ,
              CED_ObterTipoEspaco( pCabecaParm ) ,
              "Tipo do espaço de dados não é cabeça de grafo." ) != TST_CondRetOK ) 
         {
             CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
			 return GRF_CondRetErroEstrutura ;
         } /* if */

         pGrafo = ( GRF_tppGrafo )( pCabecaParm ) ;


      /* Verifica corrente */

		 if ( pGrafo->pVerticeCorr != NULL )
         {
			 if ( TST_CompararPonteiro( pCabecaParm , pGrafo->pVerticeCorr->pCabeca ,
                 "Nó corrente não aponta para cabeça." ) != TST_CondRetOK )
            {
			   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
               return GRF_CondRetErroEstrutura ;
            } /* if */
          
         } /* if */

      return GRF_CondRetOK ;

   } /* Fim função: GRF  &Verificar um nó cabeça */

#endif

   #ifdef _DEBUG


/***********************************************************************
*
*  $FC Função: ARV  -Explorar verificando os nós de uma árvore
*
*  $ED Descrição da função
*     Percorre recursivamente a árvore verificando os nós à medida que forem
*     visitados. Caso seja encontrado alguma falha, a verificação será
*     suspensa. Portanto, no caso de falha, é possível que nem todos
*     os nós da árvore sejam visitados.
*
***********************************************************************/

   GRF_tpCondRet VerificarVertices( GRF_tppGrafo pGrafo )
   {

      GRF_tpCondRet CondErro = GRF_CondRetOK ;

	  void * aux;

	  tpVertice * vertice;

      if ( pGrafo->vertices == NULL )
      {
         return GRF_CondRetOK ;
      } /* if */

      CED_MarcarEspacoAtivo( pGrafo->vertices ) ;

	  if(VerificaLista(pGrafo->vertices) != LIS_CondRetOK)
	  {
		  return GRF_CondRetErroEstrutura ;
	  } /* if */

     LIS_IrInicioLista(pGrafo->vertices);

	 do
	 {
		 LIS_ObterValor(pGrafo->vertices,&aux);
		 
		 vertice = (tpVertice*)aux;

		 if ( TST_CompararInt( GRF_TipoEspacoVertice ,
              CED_ObterTipoEspaco( vertice ) ,
              "Tipo do espaço de dados não é vertice de grafo." ) != TST_CondRetOK ) 
         {
            CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
         } /* if */
		 else if ( TST_CompararPonteiro( pGrafo , vertice->pCabeca,
                 "Nó corrente não aponta para cabeça." ) != TST_CondRetOK )
            {
               CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
            } /* if */
		 else if( VerificarArestas ( vertice ) != GRF_CondRetOK )
		 {
			 return GRF_CondRetErroEstrutura ;
		 }/* if */

	 } while ( LIS_AvancarElementoCorrente ( pGrafo->vertices , 1 ) == LIS_CondRetOK ) ;

	 return GRF_CondRetOK;

   } /* Fim função: ARV  -Explorar verificando os nós de uma árvore */

   GRF_tpCondRet VerificarArestas ( tpVertice * vertice ) 
   {
	   int flag;
	   void * aux;
	   tpVertice * verticeDestino;

	   if( VerificaLista(vertice->arestas) != LIS_CondRetOK )
	   {
		   return GRF_CondRetErroEstrutura;
	   }/* if */

	   if( LIS_IrInicioLista (vertice->arestas) == LIS_CondRetListaNaoExiste )
	   {
		   return GRF_CondRetOK;
	   }/* if */

	   do
	   {
		   LIS_ObterValor(vertice->arestas,&aux);

		   verticeDestino = ( tpVertice * ) aux ;

		   flag = 0;

		   LIS_IrInicioLista ( verticeDestino->arestas ) ;
		   do
		   {
			   LIS_ObterValor(verticeDestino->arestas,&aux);

			   if(vertice == aux)
			   {
				   flag = 1;
				   break;
			   }/* if */

		   } while(LIS_AvancarElementoCorrente(verticeDestino->arestas,1)==LIS_CondRetOK);

		   if(!flag)
		   {
			   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA); //aresta nao direcionada encontrada
			   return GRF_CondRetOK;
		   }/* if */

	   } while(LIS_AvancarElementoCorrente(vertice->arestas,1)==LIS_CondRetOK);

	   return GRF_CondRetOK;
   }

#endif




 /********** Fim do módulo de implementação: GRF Grafo não orientado **********/
