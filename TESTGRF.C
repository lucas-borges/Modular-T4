/***************************************************************************
*  $MCI M�dulo de implementa��o: TGRF Teste grafo
*
*  Arquivo gerado:              TESTGRF.c
*  Letras identificadoras:      TGRF
*
*  Projeto: INF 1301 Automatiza��o dos testes de m�dulos C
*  Gestor:  DI/PUC-Rio
*  Autores: cs  -  Clara Szwarcman
*			gs  -  Guilherme Simas
*			lb  -  Lucas Borges
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*	  1      csgslb 23/set/2014 in�cio desenvolvimento
*
*  $ED Descri��o do m�dulo
*     Este modulo cont�m as fun��es espec�ficas para o teste do
*     m�dulo grafo gen�rico. Ele trata de apenas um grafo de cada vez.
*
*  $EIU Interface com o usu�rio pessoa
*     Comandos de teste espec�ficos para testar o m�dulo lista:
*
*     
*	  =criargrafo <CondRetEsp>                        - chama a fun��o GRF_CriarGrafo () que cria um grafo 
*                                                       de listas duplamente encadeadas vazio
*														Obs. nota��o:
*														<CondRetEsperada> � a condi��o de retorno
*														que se espera do comando de teste. Est�o
*														descritas no GRAFO.H.
*
*	  =destroigrafo <CondRetEsp>                      - chama a fun��o GRF_DestroiGrafo () que desaloca
*					                                    todos os vertices do grafo e a cabe�a do grafo.
*
*	  =inserevertice <Chave> <CondRetEsp>             - chama fun��o GRF_CriaVertice () para criar
*						                                um vertice com a chave passada como parametro
*						                                e em seguida insere o vertice na lista de 
*						                                vertices do grafo.
*						                                Obs. nota��o:
*														<Chave> � a chave que identifica o v�rtice
*														tem de ser um inteiro n�o negativo
*														<Char> � o caracter a ser inserido no v�rtice
*
*     =criaaresta <Chave> <Chave> <CondRetEsp>        - chama a fun��o GRF_CriaAresta () para criar
*								                        uma aresta entre os vertices correspondentes
*								                        �s chaves passadas como par�metro
*
*     =removearesta <Chave> <Chave> <CondRetEsp>      - chama a fun��o GRF_RemoveAresta () para remover
*								                        a aresta entre os dois vertices correspondentes 
*								                        �s chaves passadas como par�metro
*
*     =existecaminho <Chave> <Chave> <CondRetEsp>     - chama a fun��o GRF_ObtemCaminho () para verificar
*								                        se os vertices correspondentes �s chaves passadas
*								                        como parametro est�o interligados por meio de arestas
*
*     =existevertice <Chave> <CondRetEsp>             - chama a fun��o GRF_ExisteVertice () para verificar
*								                        se o vertice correspondente � chave passada como
*								                        parametro est� contido no grafo
*
*     =esvaziagrafo <CondRetEsp>                      - chama a fun��o GRF_EsvaziaGrafo () que desaloca todos
*								                        os vertices do grafo
*
*     =alteracorrente <Chave> <CondRetEsp>            - chama a fun��o GRF_AlteraCorrente () que altera o
*														v�rtice corrente para o especificado
*
*     =existearesta <Chave> <Chave> <CondRetEsp>      - chama a fun��o GRF_ExisteAresta () para verificar
*														se existe uma aresta entre os v�rtices identificados
*														pelas chaves fornecidas
*
*     =irvizinho <Chave> <CondRetEsp>                 - chama a fun��o GRF_IrVizinho () que move o corrente
*														para o v�rtice de chave dada se eles s�o vizinhos
*
*     =obtercorrente <ChaveEsp> <CharEsp> <CondRetEsp> - chama a fun��o GRF_ObterCorr () que obt�m a chave
*														 e o valor do v�rtice corrente e confere se s�o os
*														 esperados
*
*     =alteravalorcorr <Char> <CondRetEsp>	     	  - chama a fun��o GRF_AlterarValorCorr () para alterar
*														o valor do v�rtice corrente para o char dado	
*
*     =caminhar <Chave> <CondRetEsp>                  - chama a fun��o GRF_Caminhar () que move o corrente
*														para o v�rtice da chave dada se eles estiverem 
*														conectados
*
*
*
***************************************************************************/

#include    <string.h>
#include    <stdio.h>
#include    <malloc.h>
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
/*****  C�digo das fun��es exportadas pelo m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: TGRF &Efetuar opera��es de teste espec�ficas para grafo
*
*  $ED Descri��o da fun��o
*     Efetua os diversos comandos de teste espec�ficos para o m�dulo
*     grafo sendo testado.
*
*  $EP Par�metros
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
				TST_NotificarFalha ( "Erros da estrutura impedem verificacao completa." ) ;
			
				return TST_CondRetOK ;
			} /* if */
			
			falhasObtido = CNT_ObterContagem ( CONTADOR_FALHAS_ESTRUTURA ) ;
			
            return TST_CompararInt ( falhasEsperado , falhasObtido , "Diferenca entre numero de falhas esperado e numero de falhas obtido." ) ;

         } /* fim ativa: Verificar o grafo */

      #endif


	return TST_CondRetNaoConhec;

} /* Fim fun��o: TGRF &Testar Grafo */

/********** Fim do m�dulo de implementa��o: TGRF Teste grafo gen�rico **********/
