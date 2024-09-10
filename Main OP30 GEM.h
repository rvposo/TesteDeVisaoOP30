/* GM OP30 Patch 0.4.5 - Detecção do Guia de Montagem */ 
#define VERSAO						"0.4.5"

#ifndef OFF
#define OFF	    					0
#endif /* OFF */

#ifndef ON
#define ON	    					1
#endif /* ON */

#ifndef off
#define off	    					0
#endif /* off */

#ifndef on
#define on	    					1
#endif /* on */

#define	NUM_PINOS_DISPL				6

#define	ABORTAR						-2 
#define	TABELAS_REDE_VERSAO			"Ver_1.1"
#define	PASTA_TAB_REDE				"V:\\GEM\\OP030\\Tabelas\\"
#define	PASTA_REDE_CTR				"V:\\GEM\\"
#define	PASTA_TAB_LOCAIS			"Tabelas\\" 
#define TABELA_MODELOS_REDE			PASTA_TAB_REDE"Modelos.txt"
#define TABELA_MODELOS_LOCAL		PASTA_TAB_LOCAIS"Modelos.txt"
#define ARQ_FTT						"..\\Dados\\ftt.txt"

#define TEMPO_MAX_SCANNER_LIGADO	25.0 // segundos 
#define	NAO_SAIR					0
#define SAIR_LOOP_SEM_ERRO			1
#define	SEM_ERRO					0
#define	SUCESSO						0
#define	FALHA						-1
#define	FALHA_TIMEOUT				-2
#define	EMERG_ACIONADA				-3
#define FALHA_MODELO_NAO_ENCONTRADO -12
#define FALHA_PRODUTO_AUSENTE		-14
#define FALHA_REJ_OPERADOR			-15
#define FALHA_MODULO_ABORTADO		-17
#define FALHA_GRAVE					-500
#define RES_FALHA					-999
#define	EXIT_PROGRAM				999


#define ABERTO			  			0
#define CONT			  			1
#define REJ			  				2
#define CONT_REJ	  				3
#define BOT_TIMEOUT	  				-1
#define BOT_ON			  			1 
#define	BOT_OFF		  				0
#define	PRODUTO_OFF					0
#define	PRODUTO_TESTANDO			1
#define	PRODUTO_APROVADO			10
#define	PRODUTO_REJEITADO			-10

#define MONITOR_A					1
#define MONITOR_B					2

#define	CPK 						2
#define	HISTOR_CPK					3

#define ARQ_TIPO_ATUAL				"Tabelas\\ModeloAtual.txt"
#define TAB_MODEL_SELECT			"Tabelas\\Model_Selecionado.txt"

#define	CAMINHO_DADOS	 			"..\\Dados\\"
#define	CAMINHO_APROVADOS 			CAMINHO_DADOS"Aprovados\\"
#define	CAMINHO_CPK_APROV   		CAMINHO_APROVADOS"CPK\\" 
#define	CAMINHO_REJEITOS 			CAMINHO_DADOS"Rejeitados\\"
#define	CAMINHO_TEMPLATES 			"..\\Templates\\"
#define	CAMINHO_SONS 	 			"..\\Sons\\"
#define DIR_CONFIG					"..\\Config\\"
#define ARQ_CONFIG_MAQ				DIR_CONFIG"Config_Maq.txt"
#define NUM_MAX_LINHAS_ARQ			150  //numero máximo de linhas permitido em um arquivo

#define MAX_QUANT_MODELOS			200
////////////////////////////////////////////////////////////////////////////////////
#define	TAMANHO_BARCODE_PWB		11
#define	TAMANHO_PREFIXO_PWB		5  
#define	BARCODE_PADRAO_PWB		"SJN02A00001"
#define	PADRAO_PREFIX_PWB		"SJN02"


#define TEMPO_MAX_BIMANUAL			1.00 // segundos
#define TEMPO_MAX_AUSENCIA			2.50 // segundos
#define TIME_OUT_BCR_ENDITEM		6.0  //tempo em segundos

//////////    ENDEREÇOS CARTÕES DE I/O    //////////

#define END_CARTAO_1_ENTRADA		0x40
#define END_CARTAO_2_ENTRADA		0x4E
#define END_CARTAO_3_SAIDA			0x42
#define END_CARTAO_4_SAIDA			0x44

#define TEMPO_MOSTRAR_WARNING		4.0  //segundos
#define TIME_OUT_FOTO				3.6
#define DEBOUNCING_EMERGEN			5 //Quant leituras p/ debouncing
#define DEBOUNCING_CORTINA			3
#define DEBOUNCING_DESCARTE			10
#define MODO_SLEEP					VAL_SLEEP_SOME  //VAL_SLEEP_MORE
#define TAM_MAX_RESP_PIC			30

enum
{
	MENS_OFF,
	MENS_BOT_TRAVADO,
	MENS_MODEL_FILE_ERROR,
	MENS_MODEL_CADAST_ERROR,
	MENS_TEST_TAB_ERROR,
	MENS_CIM_ON,
	MENS_CIM_OFF,
	MENS_CIM_ERROR,
	MENS_PRODUTO_AUSENTE,
	MENS_CORTINA,

}
MENS_WARNING;

enum
{
	RETESTA,
	NAO_RETESTA
}
TRAT_REJ;

enum
{
	MENS_VERMELHO,
	MENS_AMARELO,
	MENS_AZUL,
	MENS_PRETO,
	MENS_CINZA,
	MENS_VERDE
}
COR_MENSAGENS;

typedef enum
{
	SEM_SOM,
	TOCAR_SOM,
	TOCOU_SOM
}ALARME_SOM;
/**** Enumeracao dos tipos de montagem ******/
typedef enum
{
	DIFUSOR,
	SHIELD,
	APLIQUE,
	LCD,
	DISPLAY,
	LINER_INF,
	LIGHT_GUIDE_TACO,
	LIGHT_GUIDE_DISP,
	LIGHT_GUIDE_VELO,
	LINER_SUP ,
	LOCALIZADOR,
	DIFUS_PONTO_1,
	DIFUS_PONTO_2,
	DISP_LINER_PONTO_1,
	DISP_LINER_PONTO_2,
	DISP_LINER_PONTO_3,
	DISP_LINER_PONTO_4
} TIPO_MONTAGEM;

/******* Enumeracao dos modos de funcionamento da thread de sistema *******/
typedef enum 
{ 
	LEITURA_APENAS,
	ESCRITA_APENAS,
	ESCRITA_E_LEITURA,
	ESCRITA_MOTOR,
	PARAR_ESC_LEIT
} MODO_THREAD;
MODO_THREAD modo_thread;

/************************* Estrutura da Tabela ***********************************************/
struct TabelaTestes
{
int	    
	Numero;
char	
	Nome[80],
	Unidade[10],
	Condicao[10];
double	
	LimAlto,
	LimBaixo,
	Resultado,
	Reteste,
	Tempo_teste;
}Teste[50], TestNSample[50];

/****************************** ESTRUTURA PORTAS I/O ******************************/
typedef struct //esta estrutura define o formato dos bits em uma porta
{
	unsigned int 
	bit_a0:1,
	bit_a1:1,
	bit_a2:1,
	bit_a3:1,
	bit_a4:1,
	bit_a5:1,
	bit_a6:1,
	bit_a7:1,
	
	bit_b0:1,
	bit_b1:1,
	bit_b2:1,
	bit_b3:1,
	bit_b4:1,
	bit_b5:1,
	bit_b6:1,
	bit_b7:1;
	
}porta;

//****************************** Declaração das variáveis de ENTRADA ******************************/
struct 
{
    /////////// cartao_1 ///////////
    unsigned int
        Trava_Avancado_1 :1,
        Trava_Avancado_2 :1,
        Combitac_Encaixado :1,
        Presenca_PCB :1,
        Descarte_Liner :1,
        Spare_in_105 :1,
        Spare_in_106 :1,
        Spare_in_107 :1,
        Spare_in_108 :1,
        Spare_in_109 :1,
        Spare_in_110 :1,
        Spare_in_111 :1,
        BiManual :1,
        Rejeita :1,
        Continua :1,
        Emergencia :1;


} STATUS;

/****************************** Declaração das variaveis de SAIDA ******************************/

struct 
{
    /////////// cartao_3 ///////////
    unsigned int
        iluminacao_superior_max :1,
        iluminacao_superior_med :1,
        iluminacao_superior_min :1,
        Liga_Ionizador :1,
        Sol_Combitac :1,
        Spare_in_304 :1,
        Sol_Trava_Produto :1,
		Spare_in_305 :1,
		Spare_in_306 :1,
        Sinaleiro_Verde :1,
        Sinaleiro_Vermelho:1,
        Sinaleiro_Buzzer :1,
        Spare_in_310 :1,
        Spare_in_311 :1,
        Spare_in_312 :1,
        Sol_Descarte_Liner :1,
        ResetaEmergencia :1,
        HabitaEmergencia :1;

		
} SAIDA; 

//////////    Declaração de variaveis    //////////

ALARME_SOM alarme_som;

char
	mens[100],
	//prefixo_EndItem[20],
	MensagemFalha1[150],
	mens_montagem_1[200],
	mens_montagem_2[100],
	mens_montagem_3[100],
	mens_montagem_falha[100],
	//tipo_amostra[50],
	strGotScanner[35],
	texto[300],
	tab_config[50],
	cluster_sw_version[30],
	//descricao_modelo[20],
	descricao_modelo_atual[20],
	N_Serie_pwb[20] ,
	N_Serie_EndItem_anterior[20],
	nomeCADASTRO[80],
	sprontuario[40],
	Data[20],
	
	Dtimer[50],
	DmesI[50],
	strGP0[10],
	strGP1[10];
char
	*Rodape;
int 
	modelo_atual_index,
	horas, 
	minutos, 
	segundos,
	Mes,
	Dia,
	Ano,
	teste_retorno,
	portGP0[16], 
	portGP1[16],
	cim_conectado,
	coletando_dados,
	em_debug,
	scanner_habilitado_DM,
	tempo_scanner_ligado_DM,
	sair_programa,
	rejeitado, 
	rejeita_montagem,
	max_teste,
  	teste,
	pos_teste, 
	testar_produto,
	retestar,
	aborta_testes,
	modulo_em_teste,
	status_produto,
  	modelo,
	esperando_ler_botoeira,
	finalizar_thread_sistema,
	status_debounc_presenca_produto,
	retornar_movimentos,
    motions_clear_to_move,
    habilitar_leitor_enditem,
    desabilitar_leitor_enditem,
	leu_serial_scanner_manual,
	bcr_resetado,
	tamanho_enditem_lido,
	barcode_Enditem_lido,
	parar_mensagens_auto,
	painel_auto,
	painel_manual,
	painel_logo,
	painel_mensagens,
	tipo_monitor,
	cont,
	
	offset_daq_c0,
	
	iSwap,
	status_emergencia,
	all_motions_home,
	cont_cortina,
	descartou_liner,
	cortina_luz_presente,
	succao_presente,
	prensa_shield_presente,
    ler_string_scanner,
	leitor_ativado,
	tempo_scanner_ligado,
	scanner_habilitado,
	comm_scanner,
	habilita_leitor_barcode,
	disable_leitor_barcode,
	comm_sistema,
	////////////////
	PoolHandle,
	ciclo_threadID,
	sistema_threadID,
	////////////////
	loop_manual,
	modo_manual;
int
	acesso_remoto,
	bimanual_remoto,
	cont_remoto,
	rej_remoto;
	
double
	tempo_inicial_teste,
	tempo_inic_total_teste,
	tempo_total,
    ////////////////
 	TotalAprovados , 
 	TotalRejeitados, 
	TotalTestados,
	TotalFTT,
	TotalTestados_ant,
	////////////////
	ftt;

double 
	pixel_por_mm,
	tempo_Vortex,
	tempo_bcr_enditem ;
char	
	//model slector
	id_model_byte[MAX_QUANT_MODELOS][10],
	id_prefix_enditem[MAX_QUANT_MODELOS][10],
	id_prefix_board [MAX_QUANT_MODELOS][110],
 	id_pn_custumer[MAX_QUANT_MODELOS][20], 
	id_tab_config[MAX_QUANT_MODELOS][50],
	id_tab_testes[MAX_QUANT_MODELOS][60],
	id_display[MAX_QUANT_MODELOS][50], 
	id_arq_vision[MAX_QUANT_MODELOS][256], 
	id_tab_cfg[MAX_QUANT_MODELOS][20], 
	id_pn_visteon[MAX_QUANT_MODELOS][30],
	id_aplique[MAX_QUANT_MODELOS][20], 
	id_tipo_amostra[MAX_QUANT_MODELOS][20], 
	id_tipo[40][MAX_QUANT_MODELOS][20]; 
/*********************** Variaveis para uso no cim ************************/	 

char stationID[15],
	mensagem[500],  
	*errordesc,
	host[20], 
	service[20], 
	*resp_itostr,
	*resp_str_mid;
HRESULT rescim;
short 
	sockID, 
	resultcim;
long 
	timestamp;
CAObjHandle cimsockets;


/*********************************************************************************************
 *						        		PROTOTIPOS DE FUNÇÕES  						         *
 *********************************************************************************************/

int    CVICALLBACK ThreadSistema (void *parametro);
void   Atualiza_Manual(void);
void   Status_Seguranca(void);
int    LeWORDfromMCP23017 ( int Add, int *PortA, int *PortB );
int    EscreveMCP23017byPIC ( char *send_data, int *GP0, int *GP1 );
//int    esperaRetornoPIC( char *bRetorno );
int   EsperaRetornoPIC( char *bRetorno );
int    EnviaCommand_C_ToPIC ( int Add );
int    EnviaCommand_A_ToPIC ( int Add );
int    EsperaBimanual(void);
void   Delay_thread(double segundos);
int    LeBotoeira(void);
int    ResetarEmergencia(int testar_consistencia);
int    IniciaPortasDeSaida(char *mens_erro);
int	   Le_Entradas(int);
int	   AtualizaCartaoSaida(int cartao);
int    VerificaNoButton(void);
int    EsperaSairMontagem(int botao_saida, char* image_file, double tempo_imagens, int descartar_liner); 
int    MoveTravaClampProduto (int acao);
void   Ativa_Leitor(void);
void   AtualizaFTT(void);
int    CarregaTabela(void);
int    CVICALLBACK CicloAutomatico (void *parametro);
void   ColetaDados(int Max_Test);
int    DefModel(char *prefixo);
int    FuncRejeitado(char *Rodape, char *mensagem_teste);
void   Finaliza_Loop_Principal(void);
int    InicializaCIM(void);
int    InicializaPortasSeriais(void);
int    Inicializar_CircuitosSeguranca(int testar_consistencia );
int    LeBarcode(void);
int    LoopPrincipal(void);
int    main (int argc, char *argv[]);
void   MensagensAuto(void);
void   MoveOut_Build_Cim(char *prefixo, double Res, int Rej, int Test);
int    PresetaTeste(void);
void   ProntoParaTeste(void);
int    Reconect_CIM(void);
void   ResetBarcode(int bcr_enditem_pronto);
int    RetornaMovimentos(int testar_consistencia, char *mens_falha);
int    SetEmDebug(void);
int    SolicitaAtualizarTabela(void);
int    Verifica_Barcodes(void) ;
int    VerificaSerialScanner(void);
void   Zera(void);
int    InicializaTestes(void);
double ExecutaTestes(int pos_teste, char*tab_spare, char*pn_cliente, int modelo);
int    FinalizaTestes(double res);
int    StatusTeste(double res);
void Habilita_scanner_DM(void);
void Desliga_scanner_DM(void);
//---------------- Testes -----------------------------
double Montar_Difusor(char * mens_rejeito);
double Montar_LCD_Retirar_Liner(char * mens_rejeito);
double Montar_LCD(char * mens_rejeito);
double Montar_Shield(char * mens_rejeito);
double VisionVerificaMontagem(int montagem, char * mens_rejeito, int referenciar);
double VisionVerificaTravaAplique(char * mens_rejeito);
double VisionVerificaBarcodeLCD(char * mens_rejeito, int referenciar);
double VisionVerificaBarcodeAplique(char * mens_rejeito, int referenciar);
double Medidas_Travas_Conector_J2901(char * mens_rejeito);
double Localizador_painel(char * mens_rejeito);

//----------------- funcoes em geral.c -----------------
char   *itostr( int number, unsigned int casas );
int    CVICALLBACK CINT ( double );
void   Data_e_Hora ( char *Data, char *Time, char *Mlabel );
int    LeConfiguracoes(void);
void   Mensagem(char *Mens1, char *Mens2, char *Mens3,char *Mens4, int cor);
short  CVI_PlaySound (char *fileName, short asynchronous);
void   CVI_StopCurrentSound (void);
int    MensFinal(char *Rodape , int Mdl_Rj , double Tempo_Total , double res , double Hi_Lim , 
			double Low_Lim , char *Nome_Teste , int teste);
int RegistSeqNegSamp_Semanal(void) ;
int InicializaMaquina(void);
int HomePosition (char *mens_falha);
int ReadIndexFile (void);
int ReadChoicedIndexFile (void);
int VerificaDescarteLiner(void);

