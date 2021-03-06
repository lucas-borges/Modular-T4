/***************************************************************************
*  $MCI M�dulo de implementa��o: GRF Grafo n�o orientado
*
*  Arquivo gerado:              GRAFO.c
*  Letras identificadoras:      grf
*
*  Projeto: INF 1301 Automatiza��o dos testes de m�dulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*	  1		 csgslb 23/set/2014 in�cio desenvolvimento
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
*  $TC Tipo de dados: GRF V�rtice
*
*
***********************************************************************/

   typedef struct tagVertice {

		#ifdef _DEBUG

         GRF_tppGrafo pCabeca ;
               /* Ponteiro para cabeca
               *
               *$ED Descri��o
               *   Todos os v�rtices do grafo devem apontar para a respectiva cabe�a.
               *   Esse ponteiro corresponde a um identificador do grafo para fins
               *   de verifica��o da integridade. */


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
			   /* Ponteiro para a fun��o de destrui��o do valor contido em um vertice */

   } tpVertice ;

/***********************************************************************
*
*  $TC Tipo de dados: GRF Descritor da cabe�a de grafo
*
*
***********************************************************************/

   typedef struct GRF_tagGrafo {

         LIS_tppLista vertices;

		 tpVertice * pVerticeCorr;

		 void ( * ExcluirValor ) ( void * pvalor ) ;
			   /* Ponteiro para a fun��o de destrui��o do valor contido em um vertice */

		#ifdef _DEBUG
			 int numVertices;
		#endif

   } GRF_tpGrafo ;


   /*****  Dados encapsulados no m�dulo  *****/

      #ifdef _DEBUG

      static char EspacoLixo[ 256 ] =
             "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ;
            /* Espa�o de dados lixo usado ao testar */

      #endif


   /***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   int BuscarVertice ( int chave , LIS_tppLista pLista );

   void DestruirVertice ( void * pVertice );

   int EncontraCaminho ( tpVertice * atual , tpVertice * destino , int * buffer );

   #ifdef _DEBUG

   static GRF_tpCondRet VerificarVertices( GRF_tppGrafo pGrafo );
   static GRF_tpCondRet VerificarArestas ( tpVertice * vertice ) ;
   static GRF_tpCondRet GRF_VerificarCabeca( void * pCabecaParm );

	#endif

   /*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: GRF  &Criar grafo
*****/

   GRF_tpCondRet GRF_CriarGrafo ( GRF_tppGrafo * ppGrafo , void ( * ExcluirValor ) ( void * pValor ) )
   {

	   *ppGrafo= ( GRF_tpGrafo * ) malloc ( sizeof ( GRF_tpGrafo )) ;
	   if ( *ppGrafo == NULL )
	   {
		   return GRF_CondRetFaltouMemoria;
	   } /* if */

	   #ifdef _DEBUG
		 (*ppGrafo)->numVertices=0;
         CED_DefinirTipoEspaco( *ppGrafo, GRF_TipoEspacoCabeca ) ;
      #endif

	   LIS_CriarLista ( &( ( * ppGrafo ) -> vertices ) , DestruirVertice ) ; //ao destruir a lista de v�rtices tem que destruir o v�rtice
	   (*ppGrafo)->ExcluirValor=ExcluirValor;
	   (*ppGrafo)->pVerticeCorr=NULL;

	   return GRF_CondRetOK;

   }   /* Fim fun��o: GRF  &Criar Grafo */


/***************************************************************************
*
*  Fun��o: GRF  &Criar Vertice
*****/
   
 GRF_tpCondRet GRF_CriaVertice ( GRF_tppGrafo pGrafo , void * pValor , int chave ) 
 {

	   int Ret;
	   tpVertice* vertice;

	   if(pGrafo==NULL)
	   {
		   return GRF_CondRetGrafoNaoExiste;
	   } /* if */

	   if(GRF_ExisteVertice ( pGrafo , chave ) == GRF_CondRetVerticeJaExiste ) //Checa se o vertice j� existe
	   {
			return GRF_CondRetVerticeJaExiste ;
	   } /* if */


	   vertice = (tpVertice * ) malloc ( sizeof (tpVertice) );//cria vertice
	   if ( vertice == NULL )
	   {
		   return GRF_CondRetFaltouMemoria;
	   } /* if */

	   #ifdef _DEBUG
	     pGrafo->numVertices++;
		 vertice->pCabeca=pGrafo;
         CED_DefinirTipoEspaco( vertice, GRF_TipoEspacoVertice ) ;
      #endif

	   vertice->pValor = pValor;
	   vertice->ExcluirValor=pGrafo->ExcluirValor;
	   LIS_CriarLista ( &vertice->arestas , NULL ); //NULL pois a lista vai apontar para outros v�rtices e n�o queremos que 
													//ao remover uma aresta os v�rtices que elas apontam sejam removidos tamb�m
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

   }  /* Fim fun��o: GRF  &Criar Vertice */


/***************************************************************************
*
*  Fun��o: GRF &Cria Aresta
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

	   /*Verifica se aresta j� existe*/

	   if(GRF_ExisteAresta(pGrafo,chaveA,chaveB)==GRF_CondRetArestaJaExiste)  //aresta j� existe
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

   } /* Fim fun��o: GRF  &Cria Aresta */


/***************************************************************************
*
*  Fun��o: GRF  &Remove Aresta
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

	   /*Verifica se aresta n�o existe*/

	   if(GRF_ExisteAresta(pGrafo,chaveA,chaveB)==GRF_CondRetArestaNaoExiste)  //aresta n�o existe
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

   }  /* Fim fun��o: GRF  &Remove Aresta */

   
/***************************************************************************
*
*  Fun��o: GRF  &Destroi grafo
*****/
   
   void GRF_DestroiGrafo (GRF_tppGrafo pGrafo)
   {
	  
	   if(pGrafo==NULL)
	   {
		   return ;
	   } /* if */
	   
	   LIS_DestruirLista(pGrafo->vertices);
	   free(pGrafo);

   } /* Fim fun��o: GRF  &Destroi grafo */
   

/***************************************************************************
*
*  Fun��o: GRF  &Esvaziar Grafo
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

   } /* Fim fun��o: GRF  &Esvaziar Grafo */

  
/***************************************************************************
*
*  Fun��o: GRF  &Obtem Caminho
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

   }   /* Fim fun��o: GRF  &Obtem Caminho */

/***************************************************************************
*
*  Fun��o: GRF  &Existe V�rtice
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

   } /* Fim fun��o: GRF  &Existe V�rtice */

/***************************************************************************
*
*  Fun��o: GRF  &Alterar Corrente
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

   }  /* Fim fun��o: GRF  &Alterar Corrente */

 /***************************************************************************
*
*  Fun��o: GRF  &Existe Aresta
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

	   /*Verifica se aresta j� existe*/

	   if(BuscarVertice(chaveB, verticeA->arestas )==1)  //aresta j� existe
	   {
		   return GRF_CondRetArestaJaExiste; 
	   } /* if */

	   return GRF_CondRetArestaNaoExiste;

 } /* Fim fun��o: GRF  &Existe Aresta */

 /***************************************************************************
*
*  Fun��o: GRF  &Ir Vizinho
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

 } /* Fim fun��o: GRF  &Ir Vizinho */

 /***************************************************************************
*
*  Fun��o: GRF  &Obter Corrente
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

 }/* Fim fun��o: GRF  &Obter Corrente */

 /***************************************************************************
*
*  Fun��o: GRF  &Alterar Valor Corrente
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

 }/* Fim fun��o: GRF  &Alterar Valor Corrente */

 /***************************************************************************
*
*  Fun��o: GRF  &Caminhar
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
	 
 }/* Fim fun��o: GRF  &Caminhar */

    #ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: GRF  &Verificar um grafo
*  ****/

   GRF_tpCondRet GRF_VerificarGrafo( void * pGrafoParm )
   {

      GRF_tppGrafo pGrafo = NULL ;
	  int CondRet;
	  printf("verificar grafo 1\n");
	 
	  if( pGrafoParm == NULL )
	  {
		 
         CNT_CONTAR( "GRF_GrafoNull" ) ;
      

		  return GRF_CondRetOK ;
	  } /* if */
	  CondRet=GRF_VerificarCabeca( pGrafoParm );
		if(CondRet ==  GRF_CondRetGrafoVazio)
		{
			return GRF_CondRetOK;
		}
      if ( CondRet != GRF_CondRetOK )
      {
		  
	   
         CNT_CONTAR( "GRF_ErroCabeca" ) ;
      

         return GRF_CondRetErroEstrutura ;
      } /* if */
	  printf("verificar grafo 2\n");
      CED_MarcarEspacoAtivo( pGrafoParm ) ;

      pGrafo = ( GRF_tppGrafo ) ( pGrafoParm ) ;
	  printf("verificar grafo 3\n");
	  return VerificarVertices( pGrafo ) ;

   } /* Fim fun��o: GRF  &Verificar um grafo */

#endif

 #ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: GRF  &Deturpar Grafo
*  ****/

   void GRF_Deturpar( void * pGrafoParm ,
                      GRF_tpModosDeturpacao ModoDeturpar )
   {

      GRF_tpGrafo * pGrafo = NULL ;
	  void * pValor;

      if ( pGrafoParm == NULL )
      {
         return ;
      } /* if */

      pGrafo = ( GRF_tpGrafo * )( pGrafoParm ) ;
	  printf("deturpou\n");
      switch ( ModoDeturpar ) {

      /* Elimina elemento corrente */

         case DeturpaEliminaCorrente :
         {
			
			 free(pGrafo->pVerticeCorr);

            break ;

         } /* fim ativa: Elimina elemento corrente */

      /* Anula ponteiro sucessor */

         case DeturpaSucessorNulo :
         {
			 
 			 LIS_IrInicioLista(pGrafo->vertices);
			 LIS_ObterValor(pGrafo->vertices,&pValor);

			 while (pValor != pGrafo->pVerticeCorr)
			 {
				 LIS_AvancarElementoCorrente(pGrafo->vertices,1);
				 LIS_ObterValor(pGrafo->vertices,&pValor);
			 } /* while */

			 LIS_Deturpar(pGrafo->vertices,0);
			 

            break ;

         } /* fim ativa: Anula ponteiro sucessor */

      /* Anula ponteiro predecessor */

         case DeturpaPredecessorNulo :
         {

			 LIS_IrInicioLista(pGrafo->vertices);
			 LIS_ObterValor(pGrafo->vertices,&pValor);

			 while (pValor != pGrafo->pVerticeCorr)
			 {
				 LIS_AvancarElementoCorrente(pGrafo->vertices,1);
				 LIS_ObterValor(pGrafo->vertices,&pValor);
			 } /* while */

			 LIS_Deturpar(pGrafo->vertices,1);

            break ;

         } /* fim ativa: Anula ponteiro predecessor */

      /* Faz sucessor apontar para lixo */

         case DeturpaSucessorLixo :
         {

			 LIS_IrInicioLista(pGrafo->vertices);
			 LIS_ObterValor(pGrafo->vertices,&pValor);

			 while (pValor != pGrafo->pVerticeCorr)
			 {
				 LIS_AvancarElementoCorrente(pGrafo->vertices,1);
				 LIS_ObterValor(pGrafo->vertices,&pValor);
			 } /* while */

			  LIS_Deturpar(pGrafo->vertices,2);
			  
	
            break ;

         } /* fim ativa: Faz sucessor apontar para lixo */

      /* Faz predecessor apontar para lixo */

         case DeturpaPredecessorLixo :
         {

			 LIS_IrInicioLista(pGrafo->vertices);
			 LIS_ObterValor(pGrafo->vertices,&pValor);

			 while (pValor != pGrafo->pVerticeCorr)
			 {
				 LIS_AvancarElementoCorrente(pGrafo->vertices,1);
				 LIS_ObterValor(pGrafo->vertices,&pValor);
			 } /* while */

			  LIS_Deturpar(pGrafo->vertices,3);

            break ;

         } /* fim ativa: Faz predecessor apontar para lixo */

      /* Anula valor do v�rtice corrente */

         case DeturpaValorNulo :
         {

			LIS_AlterarValor(pGrafo->vertices,NULL);

            break ;

         } /* fim ativa: Anula valor do v�rtice corrente */

	  /* Altera tipo de estrutra apontado pelo v�rtice */

         case DeturpaTipoEstrutura :
         {

			CED_DefinirTipoEspaco( pGrafo->pVerticeCorr, CED_ID_TIPO_VALOR_NULO ) ;

            break ;

         } /* fim ativa: Altera tipo de estrutra apontado pelo v�rtice */

      /* Destaca v�rtice do grafo sem liber�-lo */

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

			 CED_MarcarEspacoNaoAtivo(aux);

            break ;

         } /* fim ativa: Destaca c�rtice do grafo sem liber�-lo */

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

   } /* Fim fun��o: GRF  &Deturpar grafo */

#endif


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/

/***********************************************************************
*
*  $FC Fun��o: GRF  -Destruir v�rtice
*
*  $ED Descri��o da fun��o
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

   } /* Fim fun��o: GRF  -Destruir v�rtice */

/***********************************************************************
*
*  $FC Fun��o: GRF  -Buscar V�rtice
*
*  $ED Descri��o da fun��o
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

   } /* Fim fun��o: GRF  -Buscar V�rtice */

  /***********************************************************************
*
*  $FC Fun��o: GRF  -Encontra Caminho
*
*  $ED Descri��o da fun��o
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
	  

   } /* Fim fun��o: GRF  -Encontra caminho */


#ifdef _DEBUG

/***************************************************************************
*
*  Fun��o: GRF  -Verificar cabe�a de grafo
*  ****/

   GRF_tpCondRet GRF_VerificarCabeca( void * pCabecaParm )
   {

      GRF_tppGrafo pGrafo = NULL ;
	  void * pValor;
	  printf("verifica_cabeca 1\n");
      /* Verifica o tipo do espa�o */

         if ( ! CED_VerificarEspaco( pCabecaParm , NULL ))
         {
			 
			CNT_CONTAR( "GRF_ErroEspacoCabeca" ) ;
   
            TST_NotificarFalha( "Controle do espa�o acusou erro." ) ;
			CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
            return GRF_CondRetErroEstrutura ;
         } /* if */

         if ( TST_CompararInt( GRF_TipoEspacoCabeca ,
              CED_ObterTipoEspaco( pCabecaParm ) ,
              "Tipo do espa�o de dados n�o � cabe�a de grafo." ) != TST_CondRetOK ) 
         {
			 CNT_CONTAR( "GRF_ErroTipoEspacoCabeca" ) ;

             CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
			 return GRF_CondRetErroEstrutura ;
         } /* if */

         pGrafo = ( GRF_tppGrafo )( pCabecaParm ) ;

  printf("verifica_cabeca 2\n");
      /* Verifica corrente */
 
		if((pGrafo->pVerticeCorr == NULL))
		{
			if (LIS_ObterValor(pGrafo->vertices,&pValor)!=LIS_CondRetListaVazia)
			{
				CNT_CONTAR("GRF_ErroPonteiroCorrenteListaVazia");
				 CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA);
				 TST_NotificarFalha("Corrente NULL mas grafo n�o � vazio");
				 return GRF_CondRetErroEstrutura;
			}
			else
			{
				return GRF_CondRetGrafoVazio;
			} /* if */
		}/* if */
		 if ( pGrafo->pVerticeCorr != NULL )
         {
			 if ( TST_CompararPonteiro( pCabecaParm , pGrafo->pVerticeCorr->pCabeca ,
                 "V�rtice corrente n�o aponta para cabe�a." ) != TST_CondRetOK )
            {
				CNT_CONTAR( "GRF_ErroPonteiroCorrenteProCabeca" ) ;

			   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
               return GRF_CondRetErroEstrutura ;
            } /* if */
			 
          
         } /* if */
		   printf("verifica_cabeca 3\n");

		   CNT_CONTAR( "GRF_CabecaOK" ) ;

      return GRF_CondRetOK ;

   } /* Fim fun��o: GRF  -Verificar cabe�a de grafo */

#endif

   #ifdef _DEBUG


/***********************************************************************
*
*  $FC Fun��o: GRF  -Explorar verificando os v�rtices
*
***********************************************************************/

   GRF_tpCondRet VerificarVertices( GRF_tppGrafo pGrafo )
   {

      GRF_tpCondRet CondErro = GRF_CondRetOK ;

	  void * aux;

	  tpVertice * vertice;
	  printf("verificar vertice 1\n");
      if ( pGrafo->vertices == NULL )
      {
		  CNT_CONTAR( "GRF_VerticesVazios" ) ;
         return GRF_CondRetOK ;
      } /* if */

      CED_MarcarEspacoAtivo( pGrafo->vertices ) ;
	  printf("verificar vertice 2\n");

	  if(VerificaLista(pGrafo->vertices) != LIS_CondRetOK)
	  {
		  CNT_CONTAR( "GRF_ErroListaVertices" ) ;
		  printf("retornando de ver vertice\n");
		  return GRF_CondRetErroEstrutura ;
	  } /* if */

     LIS_IrInicioLista(pGrafo->vertices);
	 printf("verificar vertice 3\n");
	 do
	 {
		 LIS_ObterValor(pGrafo->vertices,&aux);
		 
		 vertice = (tpVertice*)aux;

		 if ( TST_CompararInt( GRF_TipoEspacoVertice ,
              CED_ObterTipoEspaco( vertice ) ,
              "Tipo do espa�o de dados n�o � vertice de grafo." ) != TST_CondRetOK ) 
         {
			 CNT_CONTAR( "GRF_ErroTipoEspacoVertice" ) ;

            CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
         } /* if */
		 else if ( TST_CompararPonteiro( pGrafo , vertice->pCabeca,
                 "V�rtice n�o aponta para cabe�a." ) != TST_CondRetOK )
            {
				CNT_CONTAR( "GRF_ErroPonteiroProCabeca" ) ;

               CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA) ;
            } /* if */
		 else if( VerificarArestas ( vertice ) != GRF_CondRetOK )
		 {
			 CNT_CONTAR("GRF_ErroArestas");

			 return GRF_CondRetErroEstrutura ;
		 }/* if */

	 } while ( LIS_AvancarElementoCorrente ( pGrafo->vertices , 1 ) == LIS_CondRetOK ) ;
	 printf("verificar vertice 4\n");

	 CNT_CONTAR( "GRF_VerticesOK" ) ;

	 return GRF_CondRetOK;

   } /* Fim fun��o: GRF  -Explorar verificando os v�rtices */

/***********************************************************************
*
*  $FC Fun��o: GRF  -Verificar arestas
*
***********************************************************************/

   GRF_tpCondRet VerificarArestas ( tpVertice * vertice ) 
   {
	   int flag;
	   void * aux;
	   tpVertice * verticeDestino;

	   if( VerificaLista(vertice->arestas) != LIS_CondRetOK )
	   {
		   CNT_CONTAR( "GRF_ErroListaArestas" ) ;

		   return GRF_CondRetErroEstrutura;
	   }/* if */

	   if( LIS_IrInicioLista (vertice->arestas) == LIS_CondRetListaNaoExiste )
	   {
		   CNT_CONTAR( "GRF_ArestasVazias" ) ;

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
			   CNT_CONTAR( "GRF_ArestaDirecionada" ) ;

			   CNT_CONTAR (CONTADOR_FALHAS_ESTRUTURA); //aresta nao direcionada encontrada
			   return GRF_CondRetOK;
		   }/* if */

	   } while(LIS_AvancarElementoCorrente(vertice->arestas,1)==LIS_CondRetOK);

	   CNT_CONTAR( "GRF_ArestasOK" ) ;

	   return GRF_CondRetOK;
   }

#endif




 /********** Fim do m�dulo de implementa��o: GRF Grafo n�o orientado **********/
