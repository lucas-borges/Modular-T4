/***************************************************************************
*  $MCI M�dulo de implementa��o: CHR Estrutura de caracter
*
*  Arquivo gerado:              CARACTER.C
*  Letras identificadoras:      CHR
*
*  Projeto: INF 1301 Automatiza��o dos testes de m�dulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs -  Clara Szwarcman
*			gs -  Guilherme Simas
*			lb -  Lucas Borges
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*       1    csgslb 29/ago/2014 in�cio desenvolvimento
*
***************************************************************************/

#include <stdio.h>
#include   <malloc.h>
#include "caracter.h"

/***********************************************************************
*
*  $TC Tipo de dados: CHR Caracter
*
*
***********************************************************************/

   typedef struct CHR_tagCaracter {

         char Valor ;
               /* Valor do caracter */

   } CHR_tpCaracter ;


   /*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: CHR Criar Caracter
*  ****/

   CHR_tppCaracter CHR_CriarCaracter( char ValorParm )
   {

	   CHR_tpCaracter * pCaracter = NULL;

	   pCaracter = (CHR_tpCaracter *) malloc (sizeof (CHR_tpCaracter));

	   if ( pCaracter == NULL )
	   {
		   return NULL ;
	   } /* if */

	   pCaracter->Valor = ValorParm;

	   return pCaracter ;

   } /* Fim fun��o: CHR &Criar Caracter */

/***************************************************************************
*
*  Fun��o: CHR Destruir Caracter
*  ****/

   void CHR_DestruirCaracter ( void * pCaracter )
   {

	   free(pCaracter);

   } /* Fim fun��o: CHR &Destruir Caracter */

/***************************************************************************
*
*  Fun��o: CHR Obter caracter
*  ****/

   char CHR_ObterValor( CHR_tppCaracter pCaracter) 
   {
	   
      return pCaracter->Valor ; 

   } /* Fim fun��o: &CHR Obter caracter */

   /***************************************************************************
*
*  Fun��o: CHR Compara caracter
*  ****/

   int CHR_ComparaCaracter( void * pCaracterA , void * pCaracterB ) 
   {
	   CHR_tppCaracter pChrA , pChrB ;

	   pChrA = ( CHR_tppCaracter ) pCaracterA ;
	   pChrB = ( CHR_tppCaracter ) pCaracterB ;
       
	   if ( CHR_ObterValor ( pChrA ) == CHR_ObterValor ( pChrB ) )
	   {
		   return 0 ;
	   }
	   
	   return 1 ; 

   } /* Fim fun��o: CHR &Compara Caracter */

   /********** Fim do m�dulo de implementa��o: CHR Estrutura de caracter **********/