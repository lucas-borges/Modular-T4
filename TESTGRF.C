/***************************************************************************
*  $MCI Módulo de implementação: TGRF Teste grafo
*
*  Arquivo gerado:              TESTGRF.c
*  Letras identificadoras:      TGRF
*
*  Projeto: INF 1301 Automatização dos testes de módulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Histórico de evolução:
*     Versão  Autor    Data     Observações
*	  1      csgslb 23/set/2014 início desenvolvimento
*
*  $ED Descrição do módulo
*     Este modulo contém as funções específicas para o teste do
*     módulo grafo genérico. Ele trata de apenas um grafo de cada vez.
*
*  $EIU Interface com o usuário pessoa
*     Comandos de teste específicos para testar o módulo lista:
*
*     
*	  =criargrafo <CondRetEsp>                        - chama a função GRF_CriarGrafo () que cria um grafo 
*                                                       de listas duplamente encadeadas vazio
*														Obs. notação:
*														<CondRetEsperada> é a condição de retorno
*														que se espera do comando de teste. Estão
*														descritas no GRAFO.H.
*
*	  =destroigrafo <CondRetEsp>                      - chama a função GRF_DestroiGrafo () que desaloca
*					                                    todos os vertices do grafo e a cabeça do grafo.
*
*	  =inserevertice <Chave> <CondRetEsp>             - chama função GRF_CriaVertice () para criar
*						                                um vertice com a chave passada como parametro
*						                                e em seguida insere o vertice na lista de 
*						                                vertices do grafo.
*						                                Obs. notação:
*														<Chave> é a chave que identifica o vértice
*														tem de ser um inteiro não negativo
*														<Char> é o caracter a ser inserido no vértice
*
*     =criaaresta <Chave> <Chave> <CondRetEsp>        - chama a função GRF_CriaAresta () para criar
*								                        uma aresta entre os vertices correspondentes
*								                        às chaves passadas como parâmetro
*
*     =removearesta <Chave> <Chave> <CondRetEsp>      - chama a função GRF_RemoveAresta () para remover
*								                        a aresta entre os dois vertices correspondentes 
*								                        às chaves passadas como parâmetro
*
*     =existecaminho <Chave> <Chave> <CondRetEsp>     - chama a função GRF_ObtemCaminho () para verificar
*								                        se os vertices correspondentes às chaves passadas
*								                        como parametro estão interligados por meio de arestas
*
*     =existevertice <Chave> <CondRetEsp>             - chama a função GRF_ExisteVertice () para verificar
*								                        se o vertice correspondente à chave passada como
*								                        parametro está contido no grafo
*
*     =esvaziagrafo <CondRetEsp>                      - chama a função GRF_EsvaziaGrafo () que desaloca todos
*								                        os vertices do grafo
*
*     =alteracorrente <Chave> <CondRetEsp>            - chama a função GRF_AlteraCorrente () que altera o
*														vértice corrente para o especificado
*
*     =existearesta <Chave> <Chave> <CondRetEsp>      - chama a função GRF_ExisteAresta () para verificar
*														se existe uma aresta entre os vértices identificados
*														pelas chaves fornecidas
*
*     =irvizinho <Chave> <CondRetEsp>                 - chama a função GRF_IrVizinho () que move o corrente
*														para o vértice de chave dada se eles são vizinhos
*
*     =obtercorrente <ChaveEsp> <CharEsp> <CondRetEsp> - chama a função GRF_ObterCorr () que obtém a chave
*														 e o valor do vértice corrente e confere se são os
*														 esperados
*
*     =alteravalorcorr <Char> <CondRetEsp>	     	  - chama a função GRF_AlterarValorCorr () para alterar
*														o valor do vértice corrente para o char dado	
*
*     =caminhar <Chave> <CondRetEsp>                  - chama a função GRF_Caminhar () que move o corrente
*														para o vértice da chave dada se eles estiverem 
*														conectados
*
*
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>
#include <stdlib.h>
#include    "TST_Espc.h"

#include    "Generico.h"
#include    "LerParm.h"

#include	"GRAFO.H"

#include	"CARACTER.H"

#ifdef _DEBUG
#include	"CONTA.H"
#endif

static const char CRIAR_GRAFO_CMD	    [ ] = "=criargrafo"  ;
static const char DESTROI_GRAFO_CMD     [ ] = "=destroigrafo" ;
static const char INSERE_VERTICE_CMD    [ ] = "=inserevertice" ;
static const char CRIA_ARESTA_CMD	    [ ] = "=criaaresta" ;
static const char EXISTE_CAMINHO_CMD    [ ] = "=existecaminho" ;
static const char REMOVE_ARESTA_CMD     [ ] = "=removearesta";
static const char EXISTE_VERTICE_CMD    [ ] = "=existevertice";
static const char ESVAZIA_GRAFO_CMD	    [ ] = "=esvaziagrafo" ;
static const char ALTERA_CORR_CMD       [ ] = "=alteracorrente";
static const char EXISTE_ARESTA_CMD     [ ] = "=existearesta";
static const char IR_VIZINHO_CMD        [ ] = "=irvizinho";
static const char OBTER_CORR_CMD        [ ] = "=obtercorrente";
static const char ALTERA_VALOR_CORR_CMD [ ] = "=alteravalorcorr";
static const char CAMINHAR_CMD			[ ] = "=caminhar";

GRF_tppGrafo pGrafo;

/* os comandos a seguir somente operam em modo _DEBUG */
#ifdef _DEBUG
const char VERIFICAR_CMD[ ]   = "=verificar" ;
const char DETURPAR_CMD [ ]   = "=deturpar" ;



#endif
/*****  Código das funções exportadas pelo módulo  *****/


/***********************************************************************
*
*  $FC Função: TGRF &Efetuar operações de teste específicas para grafo
*
*  $ED Descrição da função
*     Efetua os diversos comandos de teste específicos para o módulo
*     grafo sendo testado.
*
*  $EP Parâmetros
*     $P ComandoTeste - String contendo o comando
*
*  $FV Valor retornado
*     Ver TST_tpCondRet definido em TST_ESPC.H
*
***********************************************************************/

TST_tpCondRet TST_EfetuarComando( char * ComandoTeste )
{

	int numLidos   = -1 , 
		CondRetEsp = -1 ,
		CondRetObt = -1 ;
	/* Inicializa para qualquer coisa */

	
	/* Testar Criar Grafo */
	if( strcmp( ComandoTeste , CRIAR_GRAFO_CMD ) == 0 )
	{
		numLidos = LER_LerParametros ( "i" ,
			 &CondRetEsp ) ;
		
		if ( numLidos != 1 )
		{
			return TST_CondRetParm ;
		}/* if */

		CondRetObt = GRF_CriarGrafo ( &pGrafo , CHR_DestruirCaracter ) ;

		return TST_CompararInt ( CondRetEsp , CondRetObt , 
				"Retorno errado ao criar grafo.") ;

	} /* fim ativa: Testar Criar Grafo */

	/* Testar Destroi Grafo */
	else if ( strcmp( ComandoTeste , DESTROI_GRAFO_CMD ) == 0 )
	{

		numLidos = LER_LerParametros( "i" ,
			&CondRetEsp ) ;

		if ( numLidos != 1 )
		{
			return TST_CondRetParm ;
		} /* if */

		GRF_DestroiGrafo ( pGrafo ) ;
		CondRetObt=GRF_CondRetOK;
		pGrafo = NULL ;

		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao destruir o Grafo." );

	} /* fim ativa: Testar Destroi Grafo */

	/* Testar Insere Vertice */
	else if ( strcmp( ComandoTeste , INSERE_VERTICE_CMD ) == 0 )
	{

		CHR_tppCaracter pCaracter ;
		char ValorVertice ;
		int ChaveVertice;
	
		numLidos = LER_LerParametros( "ici" ,
			&ChaveVertice, &ValorVertice , &CondRetEsp ) ;

		if ( numLidos != 3 || ChaveVertice<0 )
		{
			return TST_CondRetParm ;
		} /* if */

		pCaracter = CHR_CriarCaracter ( ValorVertice ) ;

		if ( pCaracter == NULL )
		{
			return TST_CondRetMemoria ;
		}

		CondRetObt = GRF_CriaVertice ( pGrafo , pCaracter, ChaveVertice ) ;

		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao criar o vertice." );

	} /* fim ativa: Testar Cria Vertice */

	/* Testar Cria Aresta */
	else if ( strcmp( ComandoTeste , CRIA_ARESTA_CMD ) == 0 )
	{
		int verticeA , verticeB ;
		
		numLidos = LER_LerParametros( "iii" ,
			&verticeA , &verticeB , &CondRetEsp ) ;

		if ( numLidos != 3 )
		{
			return TST_CondRetParm ;
		} /* if */


		CondRetObt = GRF_CriaAresta ( pGrafo, verticeA , verticeB ) ;

		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao criar a aresta." );

	} /* fim ativa: Testar Cria Aresta */

	/*Testar Remove Aresta */
	else if ( strcmp( ComandoTeste, REMOVE_ARESTA_CMD) ==0)
	{
		int verticeA , verticeB ;
		
		numLidos = LER_LerParametros( "iii" ,
			&verticeA , &verticeB , &CondRetEsp ) ;

		if ( numLidos != 3 )
		{
			return TST_CondRetParm ;
		} /* if */


		CondRetObt = GRF_RemoveAresta ( pGrafo,verticeA , verticeB) ;

		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao remover a aresta." );
	}

	/* fim ativa: Testar Criar Aresta */

	/* Testar Obtem Caminho */
	else if ( strcmp( ComandoTeste , EXISTE_CAMINHO_CMD ) == 0 )
	{
		int verticeOrigem , verticeDestino ;
		

		numLidos = LER_LerParametros( "iii" ,
			&verticeOrigem , &verticeDestino , &CondRetEsp ) ;

		if ( numLidos != 3 )
		{
			return TST_CondRetParm ;
		} /* if */


		CondRetObt=GRF_ObtemCaminho ( pGrafo , verticeOrigem, verticeDestino,NULL) ;
		
		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao verificar se existe caminho entre os vertices." );

	} /* fim ativa: Testar Obtem Caminho */

	/* Testar Existe Vertice */
	else if ( strcmp( ComandoTeste , EXISTE_VERTICE_CMD ) == 0 )
	{
		int chaveVertice ;


		numLidos = LER_LerParametros( "ii" , &chaveVertice,
			&CondRetEsp ) ;

		if ( numLidos != 2 )
		{
			return TST_CondRetParm ;
		} /* if */

		CondRetObt = GRF_ExisteVertice ( pGrafo , chaveVertice  ) ;


		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao verificar se existe vertice." );

	} /* fim ativa: Testar Existe Vertice */

	/* Testar Esvazia Grafo */
	else if ( strcmp( ComandoTeste , ESVAZIA_GRAFO_CMD ) == 0 )
	{

		numLidos = LER_LerParametros( "i" ,
			&CondRetEsp ) ;

		if ( numLidos != 1 )
		{
			return TST_CondRetParm ;
		} /* if */

		CondRetObt=GRF_EsvaziaGrafo ( pGrafo ) ;

		return TST_CompararInt( CondRetEsp , CondRetObt ,
			"Retorno errado ao esvaziar o grafo." );

	} /* fim ativa: Testar Esvazia Grafo */

	/* Testar Altera Corrente */
	else if ( strcmp( ComandoTeste, ALTERA_CORR_CMD ) == 0 )
	{
		int vertice;

		numLidos = LER_LerParametros( "ii", &vertice, &CondRetEsp);

		if ( numLidos != 2)
		{
			return TST_CondRetParm;
		} /*if*/

		CondRetObt = GRF_AlteraCorrente ( pGrafo, vertice );

		return TST_CompararInt( CondRetEsp, CondRetObt, 
			"Retorno errado ao alterar a corrente" );

	}/* fim ativa: Testar Altera Corrente */

	/* Testar Existe Aresta */
	else if( strcmp( ComandoTeste, EXISTE_ARESTA_CMD ) == 0 )
	{
		int verticeA, verticeB;

		numLidos = LER_LerParametros ( "iii", &verticeA, &verticeB, &CondRetEsp);

		if( numLidos != 3 )
		{
			return TST_CondRetParm;
		}/*if*/

		CondRetObt = GRF_ExisteAresta ( pGrafo, verticeA, verticeB);

		return TST_CompararInt( CondRetEsp, CondRetObt,
			"Retorno errado na existe aresta");

	}/* fim ativa: Testar Altera Corrente */

	/* Testar Ir Vizinho*/
	else if( strcmp( ComandoTeste, IR_VIZINHO_CMD ) == 0 )
	{
		int vertice;

		numLidos = LER_LerParametros( "ii", &vertice, &CondRetEsp);

		if( numLidos != 2)
		{
			return TST_CondRetParm;
		}/*if*/

		CondRetObt = GRF_IrVizinho ( pGrafo, vertice );

		return TST_CompararInt( CondRetEsp , CondRetObt,
			"Retorno errado ao ir vizinho");

	}/* fim ativa: Testar Ir Vizinho */

	/* Testar Obter Corrente*/
	else if( strcmp(ComandoTeste, OBTER_CORR_CMD ) == 0)
	{
		int verticeEsperado, verticeObtido;
		char valorEsperado;
		void* temp;
		char valorObtido;
		TST_tpCondRet Ret;

		numLidos = LER_LerParametros( "ici", &verticeEsperado, &valorEsperado, &CondRetEsp);

		if( numLidos != 3)
		{
			return TST_CondRetParm;
		}/*if*/

		CondRetObt = GRF_ObterCorr ( pGrafo, &verticeObtido, &temp);

		Ret =  TST_CompararInt( CondRetEsp , CondRetObt,
			"Retorno errado ao obter corrente");

		if ( Ret != TST_CondRetOK)
		{
			return Ret;
		}/*if*/

		if ( CondRetEsp != GRF_CondRetOK )
		{
			return TST_CondRetOK;
		}/*if*/

		Ret = TST_CompararInt( verticeEsperado, verticeObtido,
			"Retorno errado ao obter corrente");

		if ( Ret != TST_CondRetOK)
		{
			return Ret;
		}/*if*/

		valorObtido = CHR_ObterValor((CHR_tppCaracter)temp);

		return TST_CompararChar( valorEsperado, valorObtido,
			"Retorno errado ao obter corrente");

	}/* fim ativa: Testar Obter Corrente */

	/* Testar Alterar Valor Corr*/
	else if ( strcmp( ComandoTeste, ALTERA_VALOR_CORR_CMD ) == 0 )
	{
		char valor;
		CHR_tppCaracter pCaracter;

		numLidos = LER_LerParametros( "ci", &valor, &CondRetEsp);

		if( numLidos != 2)
		{
			return TST_CondRetParm;
		}/*if*/

		pCaracter = CHR_CriarCaracter (valor);

		CondRetObt = GRF_AlterarValorCorr ( pGrafo, (void*)pCaracter);

		return TST_CompararInt ( CondRetEsp, CondRetObt, 
			"Retorno errado ao alterar valor corrente");

	}/* fim ativa: Testar Alterar Valor Corr */

	/* Testar Caminhar*/
	else if( strcmp( ComandoTeste, CAMINHAR_CMD ) == 0 )
	{
		int vertice;

		numLidos = LER_LerParametros ( "ii", &vertice, &CondRetEsp);

		if( numLidos != 2 )
		{
			return TST_CondRetParm;
		}/*if*/

		CondRetObt = GRF_Caminhar( pGrafo , vertice );

		return TST_CompararInt( CondRetEsp, CondRetObt,
			"Retorno errado ao caminhar");

	}/* fim ativa: Testar Caminhar */

	/* Deturpar o grafo */
      #ifdef _DEBUG

         else if ( strcmp( ComandoTeste , DETURPAR_CMD ) == 0 )
         {
			GRF_tpModosDeturpacao modo;

            numLidos = LER_LerParametros( "i" ,
                                &modo ) ;

            if ( ( numLidos != 1 ))
            {
               return TST_CondRetParm ;
            } /* if */

            GRF_Deturpar( pGrafo , modo ) ;
			
            return TST_CondRetOK ;

         } /* fim ativa: Deturpar o grafo */

	/* Verificar o grafo */

		 else if ( strcmp( ComandoTeste , VERIFICAR_CMD ) == 0 )
         {
			int falhasEsperado,falhasObtido;

            numLidos = LER_LerParametros( "i" ,
                                &falhasEsperado ) ;

            if ( ( numLidos != 1 ))
            {
               return TST_CondRetParm ;
            } /* if */

            if( GRF_VerificarGrafo( pGrafo )!=GRF_CondRetOK)
			{
				printf ( "Erros da estrutura impedem verificacao completa. Numero de falhas detectadas ate o momento: %d" , CNT_ObterContagem ( CONTADOR_FALHAS_ESTRUTURA ) ) ;
				return TST_CondRetOK ;
			} /* if */
			
			falhasObtido = CNT_ObterContagem ( CONTADOR_FALHAS_ESTRUTURA ) ;

            return TST_CompararInt ( falhasEsperado , falhasObtido , "Diferenca entre numero de falhas esperado e numero de falhas obtido." ) ;

         } /* fim ativa: Verificar o grafo */

      #endif


	return TST_CondRetNaoConhec;

} /* Fim função: TGRF &Testar Grafo */

/********** Fim do módulo de implementação: TGRF Teste grafo genérico **********/
