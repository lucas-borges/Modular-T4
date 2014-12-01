/***************************************************************************
*  $MCI Módulo de implementação: CHR Estrutura de caracter
*
*  Arquivo gerado:              CARACTER.C
*  Letras identificadoras:      CHR
*
*  Projeto: INF 1301 Automatização dos testes de módulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs -  Clara Szwarcman
*			gs -  Guilherme Simas
*			lb -  Lucas Borges
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*       1    csgslb 29/ago/2014 início desenvolvimento
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


   /*****  Código das funções exportadas pelo módulo  *****/

/***************************************************************************
*
*  Função: CHR Criar Caracter
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

   } /* Fim função: CHR &Criar Caracter */

/***************************************************************************
*
*  Função: CHR Destruir Caracter
*  ****/

   void CHR_DestruirCaracter ( void * pCaracter )
   {

	   free(pCaracter);

   } /* Fim função: CHR &Destruir Caracter */

/***************************************************************************
*
*  Função: CHR Obter caracter
*  ****/

   char CHR_ObterValor( CHR_tppCaracter pCaracter) 
   {
	   
      return pCaracter->Valor ; 

   } /* Fim função: &CHR Obter caracter */

   /***************************************************************************
*
*  Função: CHR Compara caracter
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

   } /* Fim função: CHR &Compara Caracter */

   /********** Fim do módulo de implementação: CHR Estrutura de caracter **********/