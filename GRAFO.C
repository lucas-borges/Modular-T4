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


/***********************************************************************
*
*  $TC Tipo de dados: GRF Vértice
*
*
***********************************************************************/

   typedef struct tagVertice {

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

   /***** Protótipos das funções encapuladas no módulo *****/

   int BuscarVertice ( int chave , LIS_tppLista pLista );

   void DestruirVertice ( void * pVertice );

   int EncontraCaminho ( tpVertice * atual , tpVertice * destino , int * buffer );

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



 /********** Fim do módulo de implementação: GRF Grafo não orientado **********/
