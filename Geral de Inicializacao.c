
#include "CIMSockets.h"
#include <formatio.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include <rs232.h>
#include "Main OP30 GEM.h"
#include "Auto OP30 GEM.h"
#include "Manual OP30 GEM.h"
#include "Vision Scripts OP30 Cluster GEM.h"
#include "Master_e_Negative_Samples.h"
#include "Global Error Tracking.h"


//static double tempo_bcr_enditem, tempo_bcr_bezel;
static void Delay_thread(double segundos);
void Ativa_Leitor(void);

//void PrintMensWarning(ALARME_SOM alarme);
void SetMensAutoAllOFF(char *mens1,char *mens2, char *mens3);



//
// Função: Atualiza_Manual()
//
void Atualiza_Manual(void)
{
	Ativa_Leitor();


	// Atualiza status sensores de Trava do Clamp
	SetCtrlAttribute (painel_manual, MANUAL_TRAVA_AVANCADO_1, ATTR_CTRL_VAL, STATUS.Trava_Avancado_1);
	SetCtrlAttribute (painel_manual, MANUAL_TRAVA_AVANCADO_2, ATTR_CTRL_VAL, STATUS.Trava_Avancado_2);
	
	// Atualiza status sensores de segurança, botoeiras e demais recursos
	SetCtrlAttribute (painel_manual, MANUAL_EMERGENCIA, ATTR_CTRL_VAL, STATUS.Emergencia);
	SetCtrlAttribute (painel_manual, MANUAL_CONTINUA, ATTR_CTRL_VAL, STATUS.Continua);		
	SetCtrlAttribute (painel_manual, MANUAL_REJEITA, ATTR_CTRL_VAL, STATUS.Rejeita);		
	SetCtrlAttribute (painel_manual, MANUAL_DESCARTE_LINER, ATTR_CTRL_VAL, STATUS.Descarte_Liner);
	
}

//
// Função: InicializaMaquina()
//
int InicializaMaquina(void)
{
int 
   atualizacao_tabs, 
   status;

char 
	arq_tabs_rede[256], 
	cmd_str[300],
	tabela[200];
	
	
	modo_thread                      = PARAR_ESC_LEIT;
	em_debug                         = OFF;
	modo_manual	                     = OFF;
	status_emergencia                = ON;
	TRAT_REJ                         = RETESTA;// NAO_RETESTA ou RETESTA
	leitor_ativado                   = OFF;
	MENS_WARNING                     = MENS_OFF;
	ler_string_scanner               = OFF;
	bcr_resetado                     = OFF;
	esperando_ler_botoeira           = OFF;
	finalizar_thread_sistema         = OFF;
	leu_serial_scanner_manual        = OFF;
	coletando_dados                  = HISTOR_CPK;
	loop_manual                      = OFF;
	parar_mensagens_auto             = OFF;
	tempo_scanner_ligado   	         = 0; 
	rejeita_montagem                 = OFF;
	imagem_camera 			         = imaqCreateImage (IMAQ_IMAGE_RGB, 7);
	imagem_trabalho  		         = imaqCreateImage (IMAQ_IMAGE_RGB, 7);
	strcpy(N_Serie_EndItem_anterior, "");

	status = ConfigGlobal_ErrorTrack(ERR_TRACK_GRAVA);
	if(status != SEM_ERRO)
	{
		MessagePopup ("Erro ao tentar inicializar Err Tracking", "Falha na inicialização de Error Tracking");
		return -1;
	}
	if ( LeConfiguracoes() < 0 )  //Configurações
		return -1;
	if ((painel_auto = LoadPanel (0, "Auto OP30 GEM.uir", AUTO)) < 0)
		return -1;
	if((painel_manual = LoadPanel (0, "Manual OP30 GEM.uir", MANUAL)) < 0)
		return -1;
	if ((painel_logo = LoadPanel (0, "Auto OP30 GEM.uir",   LOGO_BAR)) < 0)
		return -1;
	if(tipo_monitor == MONITOR_B)
	{
		SetPanelAttribute (painel_auto, ATTR_HEIGHT, 940 );
		SetPanelAttribute (painel_auto, ATTR_TOP, VAL_AUTO_CENTER);
		SetPanelAttribute (painel_manual, ATTR_HEIGHT, 940 );
		SetPanelAttribute (painel_manual, ATTR_TOP, VAL_AUTO_CENTER);
		SetPanelAttribute (painel_auto, ATTR_WIDTH, 1280 );
		SetPanelAttribute (painel_auto, ATTR_LEFT, VAL_AUTO_CENTER);
		SetPanelAttribute (painel_manual, ATTR_WIDTH, 1280 );
		SetPanelAttribute (painel_manual, ATTR_LEFT, VAL_AUTO_CENTER);
	}

	DisplayPanel(painel_auto);//mostra painel auto

	cim_conectado = InicializaCIM();//CIM
	if(!cim_conectado && !em_debug)
	{
		if(ConfirmPopup ("Falha no CIM", "Deseja continuar e trabalhar com o CIM desligado?")==0)
			return -1;
		else
			SetPanelAttribute (painel_auto, ATTR_BACKCOLOR,0x00FF0000 ); //Seta a cor do background para vermelho
	}
	//##########################################################################################
	if(acesso_remoto == OFF && !em_debug)
	{
			//------------ Atualiza tabelas local a partir do driver de rede ----------------------
			strcpy(arq_tabs_rede,  PASTA_TAB_REDE);
			SetBreakOnLibraryErrors (0);
			Fmt(cmd_str,"%s<%s%s",arq_tabs_rede, "\\*.*"); 
			atualizacao_tabs = CopyFile (cmd_str, PASTA_TAB_LOCAIS);
			if(atualizacao_tabs == 0)
			{
			        strcat(arq_tabs_rede,  TABELAS_REDE_VERSAO);
					Fmt(cmd_str,"%s<%s%s",arq_tabs_rede, "\\TST\\*.*"); 
					Fmt(tabela, "%s<%s%s",PASTA_TAB_LOCAIS, "\\TST");
					atualizacao_tabs = CopyFile (cmd_str, tabela);
					if(atualizacao_tabs == 0)
						atualizacao_tabs = 1;
				
			}
			SetBreakOnLibraryErrors (1);
			if(atualizacao_tabs != 1)
			{
				if(ConfirmPopup ("Falha na cópia de tabelas a partir da rede", 
					"Houve um falha na atualização das tabelas de testes e configurações a partir do servidor.\nNÃO é recomendável trabalhar com esta falha. Você deseja prosseguir?")==0)
				{
					return ABORTAR ;		
				}
				atualizacao_tabs = 0;
			}
			
	
	}
	//##########################################################################################
	
	SetCtrlVal (painel_auto, AUTO_TXT_VERSAO , VERSAO);
	SetCtrlVal (painel_auto, AUTO_VERSAO_TABS, TABELAS_REDE_VERSAO );

	///////////////////////////////////////////////////////////////////////////////////
	if(ReadIndexFile () < 0)
	{
		MessagePopup ( " ERRO AO CARREGAR index de modelos !!!" , "") ;
		return -1 ;//erro nas cargas de tabela   
	}
	
	if(InicializaPortasSeriais() < 0)// Portas seriais
	{
		return -1;
	}
	
	if( InicializaCameras() < 0 )
		return -1;  
	
	SetCtrlVal (painel_auto, 	AUTO_TXT_VERSAO, 	VERSAO);
	SetCtrlVal (painel_logo, 	LOGO_BAR_VERSAO, 	VERSAO);
	
  	if(acesso_remoto == ON || modo_samples == OPCIONAL)
	{
		master_sample_ok = ON;
		negativ_sample_ok= ON;
	}
	else
	{
		if(freq_solicit_samples == TURNO_RESET)
		{
			Deleta_Status_das_Samples();
		}
		else
		{
			if(Verifica_Status_das_Samples() < 0)
			{
				MessagePopup ( "ERRO NA LEITURA DO ARQUIVO DE SAMPLES" , 
					"Falha ao tentar ler arquivos de Master e Negative Samples ") ;
				
			}
			master_sample_ok = ON;
			verificado_freq_sample  = ON;
		}
	}

	return 0;
}  

////////////////// Verifica_Barcodes ///////////////////////////////////// 
int Verifica_Barcodes(void)
{
	Ativa_Leitor();
	if (!modo_manual && !testar_produto  && (leitor_ativado || leu_serial_scanner_manual)) //verifica scanner
	{
		if(modo_manual || (leitor_ativado == OFF && leu_serial_scanner_manual == OFF))
		{
			tamanho_enditem_lido = 0;
			return 0;
		}
		if((!testar_produto && status_debounc_presenca_produto) || leu_serial_scanner_manual) //verifica scanner
		{
			if( VerificaSerialScanner() > 0 || leu_serial_scanner_manual)
			{
				LeBarcode() ;
			}
			tamanho_enditem_lido = strlen(N_Serie_pwb);
			if(tamanho_enditem_lido>0)
			{
				bcr_resetado=OFF;
			}
		}
		else
		{
			leu_serial_scanner_manual = OFF;
			return 0;
		}
	}
////////////////////// Verifica timeout ou BCR pronto ////////////////////
	if( tamanho_enditem_lido == TAMANHO_BARCODE_PWB)
	{
		if(!status_debounc_presenca_produto || bcr_resetado == OFF)
		{
			if( Timer()-tempo_bcr_enditem > TIME_OUT_BCR_ENDITEM )
				ResetBarcode(OFF);
			else
				ResetBarcode(ON);
		}
	}
	else
	{
		if(!testar_produto && !bcr_resetado)
			ResetBarcode(OFF);
	}

	return 0;
}


////////////////////// Função: InicializaPortasSeriais ////////////////////

int InicializaPortasSeriais(void)
{
	char
		resposta[100];
	int
		status;

	Mensagem( "Aguarde...","Configurando comunicação Scanner.", "", "", MENS_CINZA);
	DisableBreakOnLibraryErrors ();

//////////////////////////////// Scanner //////////////////////////////

	if(comm_scanner > 0)
	{
		status = OpenComConfig (comm_scanner, "", 115200, 0, 8, 1, 512, -1);
		if ( status < 0 )
		{
			CloseCom (comm_scanner);
			Fmt( resposta, "Porta de comunicação com 'SCANNER' não disponível.\rO Programa será finalizado.");
			MessagePopup ( " Alerta ", resposta );
			EnableBreakOnLibraryErrors();
			return -1;
		}
		else
		{
			status = SetComTime(comm_scanner, 1 );
			Habilita_scanner_DM();
			leitor_ativado = ON;
		}
	}
	ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "Aguarde...");
	InsertTextBoxLine (painel_auto, AUTO_TXT_MENSAGEM ,-1, "Configurando cartões de I/O - PIC.");
	ProcessDrawEvents ();

	//////////////////////////// Sistema //////////////////////////////

	status = OpenComConfig (comm_sistema, "", 115200, 0, 8, 1, 512, -1);

	if ( status < 0 )
	{
		CloseCom (comm_sistema);
		Fmt( resposta, "Porta de comunicação com PIC MASTER não disponível.\rO Programa será finalizado.");
		MessagePopup ( " Alerta ", resposta );
		EnableBreakOnLibraryErrors ();
		return -1;
	}
	else
	{
		status = SetComTime(comm_sistema, 1 );
	}
	EnableBreakOnLibraryErrors ();

	return 0;
}


//////////////////////////////// Função: SetMensAuto_ON //////////////////////////////

void SetMensAuto_ON(char *mens1)
{
	// mens1
	if(status_produto == PRODUTO_TESTANDO && !rejeitado)
		strcpy(mens1,"Montando o cluster ...");
	else if(status_produto == PRODUTO_TESTANDO && rejeitado)
	{
		strcpy(mens1,"Produto está REJEITADO  - Aperte 'CONT' p/ Retestar ou 'REJ' p/ Abortar");
	}
//	else if(status_produto == PRODUTO_REJEITADO)
//	{
//		strcpy(mens1,"Produto está REJEITADO - Retire o Placa e coloque etiqueta de rejeitado");
//	}
	else if(status_produto == PRODUTO_APROVADO)
	{
		strcpy(mens1,"Produto está APROVADO - Retire o Placa e envie para próxima estação");
	}
}


//////////////////////////////// Função: SetMensAutoAllOFF //////////////////////////////

void SetMensAutoAllOFF(char *mens1, char *mens2, char *mens3)
 
{
	
	static int index = 0;
	int
		tamanho_end_item;

	if( criar_negat_sample)
	{
		strcpy(mens1,"MÁQUINA SETADA PARA CRIAÇÃO DE AMOSTRA NEGATIVA");
	}
	else if(amostra_tipo == MASTER_SAMPLE )
	{
		strcpy(mens1,"Máquina setada para teste de MASTER Sample");
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE && !criar_negat_sample)
	{
		strcpy(mens1,"Amostra Negativa");
	}
	else if(!master_sample_ok && !criar_negat_sample && modo_samples == SEMPRE)
	{
		strcpy(mens1,"Máquina necessita passar MASTER Sample");
	}
	else if(master_sample_ok && !criar_negat_sample && !negativ_sample_ok && modo_samples == SEMPRE)
	{
		strcpy(mens1,"Máquina necessita Amostra Negativa");
	}
	else
	{
		strcpy(mens1,"PRONTO PARA MONTAGEM ");
		strcpy(mens2," ");
	}
	strcpy(mens3," ");
	strcpy(mens_montagem_falha,"");
	strcpy(MensagemFalha1,"");
	COR_MENSAGENS= MENS_AZUL;
	tamanho_end_item=strlen(N_Serie_pwb);
	if(!testar_produto )
	{
		if(tamanho_end_item == TAMANHO_BARCODE_PWB)  //bar code lido
		{
			if(!status_debounc_presenca_produto)
			{
				if(!master_sample_ok && !criar_negat_sample && modo_samples == SEMPRE)
				{
					strcpy(mens2,"Leia o código de barras do Cluster");
					strcpy(mens3,"\0");
				}
				else if(master_sample_ok && !criar_negat_sample && !negativ_sample_ok && modo_samples == SEMPRE)
				{
					Fmt(ident_da_sample, "%s<%s%s%s",negat_samples_registr[index_neg_samp_testando].nome, 
							" -Número Série =", negat_samples_registr[index_neg_samp_testando].serial); 
					strcpy(mens2, ident_da_sample);
				}
				else
				{
					strcpy(mens1,"-- Coloque o produto no berço --");
					strcpy(mens2," ");
					strcpy(mens3," ");
				}
			}
		}
		else
		{
			if(status_debounc_presenca_produto)
			{
				strcpy(mens1,"1- Retire o Cluster do berço de Testes");
				strcpy(mens2,"2- Leia o Código de Barras da Placa");
				strcpy(mens3,"");
			} 
			else 
			{
				if(leitor_ativado == OFF && !status_debounc_presenca_produto && tempo_scanner_ligado != 0 && tamanho_enditem_lido == 0)
				{
					COR_MENSAGENS= MENS_VERMELHO;
					strcpy(mens1,"");
					strcpy(mens2," Leitor de Código de Barras está desligado");
					strcpy(mens3," Aperte 'CONT' ou 'REJ'");
					index++;
					if(index > 2 && index <= 4)
					{
						COR_MENSAGENS= MENS_PRETO;
					}
					else if(index > 4)
						index = 0;
				}
				else if(criar_negat_sample)
				{
					strcpy(mens1,"MÁQUINA SETADA PARA CRIAÇÃO DE AMOSTRA NEGATIVA");
					strcpy(mens2,"1- Leia o Código de Barras");
					strcpy(mens3,"2- Posicione a Amostra no Berço");

				}
				else if(!master_sample_ok && !criar_negat_sample)
				{
					strcpy(mens1,"Máquina necessita passar MASTER SAMPLE");
					strcpy(mens2,"1- Leia o Código de Barras");
					strcpy(mens3,"2- Posicione a Master Sample no Berço");
				}
				else if(!negativ_sample_ok && master_sample_ok && !criar_negat_sample)
				{
					Fmt(ident_da_sample, "%s<%s%s%s",negat_samples_registr[index_neg_samp_testando].nome, 
						" -Número Série =", negat_samples_registr[index_neg_samp_testando].serial); 
					strcpy(mens1,"Máquina necessita passar Amostra NEGATIVA");
					strcpy(mens2, ident_da_sample);
				}
				else
				{
					strcpy(mens1," Leia o Código de Barras da Placa");
					strcpy(mens2," Coloque  o CLUSTER no berço de Montagem");
					strcpy(mens3,"");
				}
			}
		}
	}
}


////////////////// Função: RetornaMovimentos///////////////////////////////

int RetornaMovimentos(int testar_consistencia, char *mens_falha)
{
	int
		status;

	modo_thread = ESCRITA_E_LEITURA;
	retornar_movimentos = ON;
	all_motions_home	= OFF;
	Delay_thread(0.100);
	Mensagem( "Aguarde...", "Retornando para home position", "", "", MENS_CINZA);
	
	while( (all_motions_home==OFF || !status_emergencia) && !sair_programa )
	{
		if((status=MoveTravaClampProduto(OFF)) < SEM_ERRO)
		{
			strcpy(mens_falha, "Falha no posicionmento da 'Trava do Clamp' para HomePosition");
			return status;
		}
		
	}
	ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "");	
	retornar_movimentos = OFF;
	return SEM_ERRO;
}


///////////////// Função: CircuitosSeguranca////////////////////////

int CircuitosSeguranca(int testar_consistencia)
{
	
	modo_thread = ESCRITA_E_LEITURA;
	retornar_movimentos = ON;
	Delay_thread(0.200);
	
	// Reset Emergencia
	Mensagem( "Aguarde...", "Resetando Emergencia", "", "", MENS_CINZA);
	if(  Inicializar_CircuitosSeguranca(testar_consistencia) < 0 )
	{
		return FALHA;
	}		
	return SUCESSO;
}


//////////////////// Função: HomePosition///////////////////////

int HomePosition (char *mens_falha)
{
	int
		status;

	modo_thread = PARAR_ESC_LEIT;
	Delay_thread(0.600);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// IMPORTATE
	// ---------	
	//
	// TO DO: Incluir o tratamento para combitac
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Mensagem( "Aguarde...","Configurando os cartões de I/O",  "" ,"", MENS_CINZA);
	status = IniciaPortasDeSaida(mens_falha);
	if( status < 0 )
		return FALHA;

	if( CircuitosSeguranca( ON )<0 )
	{
		strcpy(mens_falha,"Falha ao tentar inicializar Circuito de Emergência");
		return FALHA;
	}

	if( RetornaMovimentos( ON, mens_falha ) < 0)
		return FALHA;

	if(sair_programa)
		return FALHA;

	return 0;
}

//
// Função: PrintMensWarning()
//
void PrintMensWarning(ALARME_SOM alarme)
{
	static double 
		tempo_mostra_warning = 0;
	char
		path[200];
	
	if(MENS_WARNING == MENS_BOT_TRAVADO)
	
	{
		Mensagem( "Botão REJ ou CONT travado(s)","Libere as botoeiras p/ prosseguir.","", "", MENS_VERMELHO);
	}
	else if(MENS_WARNING == MENS_MODEL_FILE_ERROR) 
	{
		Mensagem( "Falha cadastro Modelo","Erro ao abrir o arquivo de modelos","", "", MENS_VERMELHO);
	}
	else if(MENS_WARNING == MENS_MODEL_CADAST_ERROR) 
	{
		Mensagem( "Falha cadastro Modelo","Modelo com erro no cadastro na tabela de Modelos","", "", MENS_VERMELHO);
	}
	else if(MENS_WARNING == MENS_TEST_TAB_ERROR) 
	{
		Mensagem( "","ERRO NA CARGA DA TABELA DE TESTES", "", "", MENS_VERMELHO); 
	}
	else if(MENS_WARNING == MENS_CIM_OFF) 
	{
		Mensagem( "","CIM DESLIGADO", "", "", MENS_VERMELHO);
	}
	else if(MENS_WARNING == MENS_CIM_ON) 
	{
		Mensagem( "","CIM ACIONADO", "", "", MENS_AZUL);
	}
	else if(MENS_WARNING == MENS_PRODUTO_AUSENTE) 
	{
		Mensagem( "","Produto rejeitado !!", "Produto Removido do Berço", "", MENS_VERMELHO);
	}
	else if(MENS_WARNING == MENS_CIM_ERROR) 
	{
		Mensagem( "Erro na comunicação com o CIM",mens_montagem_1,mens_montagem_2, "", MENS_VERMELHO);
	}
	
	if(tempo_mostra_warning == 0)
		tempo_mostra_warning = Timer();
	if((Timer() - tempo_mostra_warning) > TEMPO_MOSTRAR_WARNING)
	{
		MENS_WARNING 			= MENS_OFF;
		tempo_mostra_warning 	= 0;
	}
	SetActiveCtrl (painel_auto, AUTO_TXT_SERIE);
	if(alarme == TOCAR_SOM)
	{
		strcpy(path,CAMINHO_SONS);
		strcat(path,"Laser_curto.wav");	 
		CVI_PlaySound (path, ON );
		Delay_thread(1.5);
		CVI_StopCurrentSound();
	}
}


void Ativa_Leitor(void)
{
 
	
	if(modo_manual /*|| motions_clear_to_move*/) 
	{
   		if(leitor_ativado)
   		{
			leitor_ativado		   = OFF;
			disable_leitor_barcode = ON ;	
   			Delay_thread(0.01);
		}
		return;
	}
	
 	if(leitor_ativado == OFF && !testar_produto && tempo_scanner_ligado == 0)
	{
		habilita_leitor_barcode = ON;
		ler_string_scanner 		= ON;
		Delay_thread(0.001);
		leitor_ativado			= ON;
		tempo_scanner_ligado	= Timer();
	}

	if(leitor_ativado == OFF && !testar_produto && tempo_scanner_ligado != 0 && (STATUS.Rejeita || STATUS.Continua || status_debounc_presenca_produto) )
	{
		habilita_leitor_barcode = ON;
		ler_string_scanner 		= ON;
		Delay_thread(0.001);
		leitor_ativado			= ON;
		tempo_scanner_ligado	= Timer();
	}
	
	else if(leitor_ativado == ON && (testar_produto ))
	{
		disable_leitor_barcode = ON;
		ler_string_scanner 	   = OFF;
		Delay_thread(0.001);													
		leitor_ativado		   = OFF;
		tempo_scanner_ligado   = 0;
	}
	else if(leitor_ativado == ON && (Timer()-tempo_scanner_ligado > TEMPO_MAX_SCANNER_LIGADO))
	{
		disable_leitor_barcode = ON;
		ler_string_scanner 	   = OFF;
		Delay_thread(0.001);													
		leitor_ativado		   = OFF;
	}
}


//
// Função: VerificaSerialScanner()
//
int VerificaSerialScanner(void)
{
	char
		buffer[70]="";
	int
		tamanho;

	if( strlen(strGotScanner) == 0 )
		return 0;
	Delay_thread(0.050);
	strcpy( buffer, strGotScanner );
	ResetTextBox( painel_auto,AUTO_TXT_SERIE, "\0");
	tamanho=strlen(buffer);
	if( tamanho != TAMANHO_BARCODE_PWB)
	{
		strcpy( buffer,"err leit");
		tamanho = 0;
	}
	SetCtrlVal  ( painel_auto,AUTO_TXT_SERIE , buffer);
	strcpy( strGotScanner,"");

	return tamanho;
}

//
// Função: ProntoParaTeste()
//
void ProntoParaTeste(void)
{
	static double
		t_tela=0;
	char
		path[200];
	
	if( barcode_Enditem_lido && status_debounc_presenca_produto && !testar_produto )
	{
		testar_produto  = ON;
	}
	else if( barcode_Enditem_lido == OFF && modulo_em_teste==OFF && !testar_produto)
	{
		strcpy(path,CAMINHO_TEMPLATES);
		if( (Timer()-t_tela)>TIME_OUT_FOTO )
		{
			if(iSwap == 3)
				iSwap = 0;
			switch(iSwap)
			{
				case 0:
					strcat(path,"Pre Montagem.jpg");
					break;
				case 1:
					strcat(path,"Ler Barcode.jpg");
					break;
				case 2:
					strcat(path,"MPlate Berco.jpg");
					break;
				default:
					break;
			}
			DisplayImageFile (painel_auto, AUTO_PIC_INSTRUCAO, path);
			t_tela = Timer();
			iSwap ++;
		}
	}
	else if( barcode_Enditem_lido == ON && modulo_em_teste==OFF && !testar_produto)
	{
		strcpy(path,CAMINHO_TEMPLATES);
		strcat(path,"");
	}
}

//
// Função: Inicializar_CircuitosSeguranca()
//
int Inicializar_CircuitosSeguranca(int testar_consistencia )
{

	if(!status_emergencia || testar_consistencia)
	{
		if(ResetarEmergencia(testar_consistencia) != SEM_ERRO)
		{
			MessagePopup ("Erro no Hardware de Emergência", "Falha na consistência do circuito de Emergência");
			return FALHA;
		}
		ResetarEmergencia(OFF);
	}
	while(!status_emergencia)
	{
		if( status_emergencia == OFF )
		{
			SetCtrlAttribute  (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR ,VAL_RED);
			ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "Emergencia acionada...");
			InsertTextBoxLine (painel_auto, AUTO_TXT_MENSAGEM ,-1, "Libere o botão de emergência.");
			InsertTextBoxLine (painel_auto, AUTO_TXT_MENSAGEM ,-1, "Acione BIMANUAL para retornar movimentos à posição inicial.");
			while( !status_emergencia && !sair_programa )
			{
				Delay_thread(0.1);
				if(STATUS.BiManual)
					ResetarEmergencia(OFF);
			}
		}
	}
	return SUCESSO;
}

//
// Função: FuncRejeitado()
//
int FuncRejeitado(char *Rodape, char *mensagem_teste)
{
	double
		t;
	int
		status;

	Delay_thread(0.1);
	status = ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "");
	status = SetCtrlAttribute  (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR ,0x000080L);
	DisplayPanel (painel_auto);
	status = BOT_TIMEOUT;//zera status
	t= Timer();
	if(!status_emergencia)
	{
		while (status != CONT  && status != REJ)
		{
			//forca leitura de rej. ou cont
			esperando_ler_botoeira = ON;
			status = LeBotoeira();
		}
	}
	else
	{
		while (status != CONT  && status != REJ && status != CONT_REJ && status != FALHA_PRODUTO_AUSENTE && !sair_programa)//forca leitura de rej., cont ou cont_rej
		{
			esperando_ler_botoeira = ON;
			status = LeBotoeira();
		}
		if(status == CONT_REJ)
		{
			DisplayPanel (painel_auto);
			esperando_ler_botoeira = OFF;
			return CONT_REJ;
		}
	}
	esperando_ler_botoeira = OFF;
	if(status == CONT)
	{
		strcpy(mensagem_teste,"");
		DisplayPanel (painel_auto);
		return CONT;
	}
	if(status == FALHA_PRODUTO_AUSENTE)
	{
		strcpy(mensagem_teste, "");
		return FALHA_PRODUTO_AUSENTE;
	}
	return (REJ);
}

//
// Função: Finaliza_Loop_Principal()
//
void Finaliza_Loop_Principal(void)
{
	SuspendTimerCallbacks ();
	desabilitar_leitor_enditem = ON;
	modo_thread = ESCRITA_E_LEITURA;

	Delay_thread(1);

	finalizar_thread_sistema = ON;
	DiscardPanel (painel_auto);
	ComWrt(comm_scanner, "F", 1 ); // Desabilita leitura de codigo de barras
	
}

//
// Função: DefModel()
//
int DefModel(char *prefixo)
{
	int 
		arquivo, 
		i=0;
	char 
		linha[300], 
		virgula[3],
		dado1[200],
		dado2[50]="",
		dado3[50]="",
		dado4[50]="",
		dado5[50]="",
		dado6[50]="",
		dado7[50]="",
		dado8[50]="",
		dado9[50]="",
		dado10[50]="",
		arq_tabs_rede[60]="",
		arq_modelos[60]={0},
		error_msg[80]={0};
		
	strcpy(id_arq_vision[modelo_atual_index], DIR_CONFIG_VISION);
	//------------- Seleciona tabelas local ou driver de rede -------------
	strcpy(arq_tabs_rede,  PASTA_TAB_REDE);
	strcat(arq_tabs_rede,  TABELAS_REDE_VERSAO);
	if(cim_conectado)
	{
		Fmt(id_tab_testes[modelo_atual_index],"%s<%s%s",arq_tabs_rede, "\\TST\\"); 
		strcpy(arq_modelos, TABELA_MODELOS_REDE);
	}
	else
	{
		Fmt(id_tab_testes[modelo_atual_index], "%s<%s%s",PASTA_TAB_LOCAIS, "TST\\");
		strcpy(arq_modelos, TABELA_MODELOS_LOCAL);
	}
	arquivo = OpenFile ( arq_modelos, VAL_READ_ONLY, VAL_APPEND, VAL_ASCII );
	if (arquivo < 0)
	{
		Fmt(error_msg,"%s%s%s", "Arquivo ", arq_modelos, "não encontrado");
		MessagePopup ("Erro ao abrir arquivo", error_msg);
	    return -2;
	}
	while (ReadLine(arquivo,linha, 250) > 0){//le demais linhas ate o fim do arquivo 
		StringUpperCase (linha);    	
  		Scan(linha, "%s>%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]"
  		,dado1,virgula, dado2,virgula,dado3,virgula,dado4,virgula,dado5,virgula,dado6,virgula,dado7,virgula,dado8,virgula,dado9,virgula,dado10);
  		
		Scan(dado1, "%s>%s", dado1);//elimina possivel tab ou espaco apos a string
  		Scan(dado2, "%s>%s", dado2);//elimina possivel tab ou espaco apos a string
  		Scan(dado3, "%s>%s", dado3);//elimina possivel tab ou espaco apos a string
  		Scan(dado4, "%s>%s", dado4);//elimina possivel tab ou espaco apos a string
  		Scan(dado5, "%s>%s", dado5);//elimina possivel tab ou espaco apos a string
  		Scan(dado6, "%s>%s", dado6);//elimina possivel tab ou espaco apos a string
  		Scan(dado7, "%s>%s", dado7);//elimina possivel tab ou espaco apos a string
  		Scan(dado8, "%s>%s", dado8);//elimina possivel tab ou espaco apos a string
  		Scan(dado9, "%s>%s", dado9);//elimina possivel tab ou espaco apos a string
  		Scan(dado10, "%s>%s", dado10);//elimina possivel tab ou espaco apos a string
 		if (strcmp (dado1, prefixo)==0)
 		{
			strcpy(id_aplique[modelo_atual_index],  dado2); // nome do modelo	  //pn_customer
			strcpy(id_display[modelo_atual_index],  dado3); // nome do arquivo de configuração  //tab_config
			strcat(id_tab_testes[modelo_atual_index],       dado4); // nome da tabela de testes 
			strcat(id_arq_vision[modelo_atual_index],       dado5); // nome do arquivo de vision  
			//strcpy(pn_visteon,       dado7); // nome do modelo para a Visteon  
			strcpy(id_tipo[modelo_atual_index], dado8); // descricao do modelo de produto
			strcpy(id_tipo_amostra[modelo_atual_index], 	 dado9); // descricao do tipo de amostra
			//strcpy(prefixo_EndItem, 	 dado10); // descricao do tipo de amostra
			barcode_Enditem_lido = ON;  
			i=1;
			break;
		}	
		if(strcmp(dado1,"FIM")==0) 
			break;
	}
	CloseFile (arquivo);
	if (i==0)
	{
		barcode_Enditem_lido = OFF;
		MENS_WARNING = MENS_MODEL_CADAST_ERROR;
		return FALHA_MODELO_NAO_ENCONTRADO;
	}
	if(strstr(id_tipo_amostra[modelo_atual_index],"MSAMPLE") != NULL) 
	{
		amostra_tipo 		= MASTER_SAMPLE;
		strcpy(id_tipo_amostra[modelo_atual_index], "Master Sample");
	}
	else if(strstr(id_tipo_amostra[modelo_atual_index],"NEGATIVA") != NULL) 
	{
		amostra_tipo 		= NEGATIVE_SAMPLE;
		strcpy(id_tipo_amostra[modelo_atual_index], "Amostra Negativa");
	}
	else 
	{
		strcpy(id_tipo_amostra[modelo_atual_index], "Teste");
		amostra_tipo 		= TESTE;
	}
	ProcessDrawEvents();
	
	return SEM_ERRO;				
}


//
// Função: MensagensAuto()
//
void MensagensAuto(void)
{
	if(modo_manual)
		return;

	SetCtrlAttribute (painel_auto, AUTO_PRESENCA, ATTR_CTRL_VAL, status_debounc_presenca_produto);
	SetCtrlAttribute (painel_auto, AUTO_LED_MSAMPLE, ATTR_CTRL_VAL, master_sample_ok); 
	SetCtrlAttribute (painel_auto, AUTO_LED_NEGSAMPLE, ATTR_CTRL_VAL, negativ_sample_ok); 
	if(MENS_WARNING != MENS_OFF)
	{
		if(alarme_som != TOCOU_SOM)
			alarme_som = TOCAR_SOM;
		PrintMensWarning(alarme_som);
		alarme_som = TOCOU_SOM;
		return;
	}
	alarme_som = SEM_SOM;
	if(parar_mensagens_auto)
		return;

	//////////////////// Mens geral /////////////////////////
	if( !modulo_em_teste ) 	   // nenhum berço em teste
		SetMensAutoAllOFF(mens_montagem_1, mens_montagem_2, mens_montagem_3);
	else if( modulo_em_teste ) // berço em teste
		SetMensAuto_ON(mens_montagem_1);

	/////////////////// Mensagens de falha //////////////////
	if(strlen(MensagemFalha1) > 0 || rejeitado)
	{
		if(strlen(MensagemFalha1)>0)
			strcpy(mens_montagem_falha, MensagemFalha1);
		Mensagem( mens_montagem_1, mens_montagem_2, mens_montagem_3, mens_montagem_falha, MENS_VERMELHO);
	} else {
		Mensagem( mens_montagem_1, mens_montagem_2, mens_montagem_3, mens_montagem_falha, COR_MENSAGENS);
	}
}



/********************** AtualizaDadosModelo() *********************************************************/
int AtualizaDadosModelo(void)
{
		
		SetCtrlAttribute (painel_auto,AUTO_SERIAL_APLIQUE, ATTR_CTRL_VAL , id_aplique[modelo_atual_index]);
		SetCtrlAttribute (painel_auto,AUTO_PN_VISTEON, ATTR_CTRL_VAL , id_pn_visteon[modelo_atual_index]);
		
		return	0;
}

/********************** ReadIndexFile() *********************************************************/
int ReadIndexFile (void)
{
int 
	status,
	i=0, 
	k=0, 
	n=0, 
	file, 
	file1, 
	nMaxItens,
	res;
char	
	choice[25], 
	label[150], 
	item[25], 
	aux[5], 
	arq_modelos[180], 
	nome1[80],
	linha[300], 
	virgula[3],
	dado1[300],
	dado2[50],
	dado3[30],
	dado4[50],
	dado5[50],
	dado6[50]="",
	dado7[50]="",
	dado8[50]="",
	dado9[50]="",
	dado10[70]="",
	dado11[50]="",
	dado12[50]="",
	dado13[300]="",
	dado14[50]="",
	dado15[50]="",
	dado16[50]="",
	dado17[50]="",
	dado18[50]="",
	arq_tabs_rede[256],
	dir_tabs[180];

	////////////////////////////////////////////////////////////////////
	//------------- Seleciona tabelas local ou driver de rede -------------
	strcpy(arq_tabs_rede,  PASTA_TAB_REDE);
	strcat(arq_tabs_rede,  TABELAS_REDE_VERSAO);
	if(cim_conectado)
	{
		Fmt(dir_tabs,"%s<%s%s",arq_tabs_rede, "\\TST\\"); 
		strcpy(arq_modelos, TABELA_MODELOS_REDE);
	}
	else
	{
		Fmt(dir_tabs, "%s<%s%s",PASTA_TAB_LOCAIS, "TST\\");
		strcpy(arq_modelos, TABELA_MODELOS_LOCAL);
	}
  	
	file = OpenFile ( arq_modelos, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	if ( file < 0 ){
		MessagePopup( " ERRO ", "Erro na abertura do arquivo de modelos" ) ;
		return -1 ;
	}
  	
	i = 1 ; 
	status = ReadLine ( file, linha, 180 ) ;
	while( ON )
	{
		status = ReadLine ( file, linha, 180 ) ;
		if( strcmp( linha, "FIM") == 0 || strcmp( linha, "END") == 0 || status <0) 
			break ;
	 	StringUpperCase (linha);    	
   		Scan(linha, "%s>%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]" //
   		,dado1,virgula, dado2,virgula,dado3,virgula,dado4,virgula,dado5,virgula,dado6,virgula,dado7,virgula,dado8,virgula,dado9,virgula,dado10,virgula,dado11,virgula,dado12);
		Scan(dado1, "%s>%s", dado1);  
   		Scan(dado2, "%s>%s", dado2);  
   		Scan(dado3, "%s>%s", dado3);  
   		Scan(dado4, "%s>%s", dado4); 
   		Scan(dado5, "%s>%s", dado5);  
   		Scan(dado6, "%s>%s", dado6);  
   		Scan(dado7, "%s>%s", dado7);  
		Scan(dado8, "%s>%s", dado8);
		Scan(dado9, "%s>%s", dado9);
		Scan(dado10, "%s>%s", dado10);  			
   		Scan(dado11, "%s>%s", dado11);  
		strcpy(id_prefix_board[i]    ,dado1);
		strcpy(id_aplique[i]     ,dado2);//ck	
		strcat(id_display[i]      ,dado3);	
		
		strcpy(id_tab_testes[i]      ,dir_tabs);					  
		strcat(id_tab_testes[i]      ,dado4);					  
		strcpy(id_arq_vision[i], DIR_CONFIG_VISION);
	 	strcat(id_arq_vision[i]      ,dado5);
		strcpy(id_pn_visteon[i]      ,dado7);
		strcpy(id_tipo[i]            ,dado8);				
		strcpy(id_tipo_amostra[i]      ,dado9);
		strcpy(id_prefix_enditem[i]  ,dado10);
		i++ ;	
	}
	
	CloseFile( file ) ;
	
	nMaxItens = i ;
	ReplaceListItem ( painel_auto, AUTO_MODELOS, 0, id_model_byte[i], 0);

	for ( i=1; i <= nMaxItens;i++ )
		InsertListItem ( painel_auto, AUTO_MODELOS, -1, id_prefix_enditem[i], i );
	
	//////////////////////////////////////////////////////////////////
	file = OpenFile ( TAB_MODEL_SELECT, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII);

	if ( file < 0 ){
		MessagePopup( " ERRO ", "Erro na abertura do arquivo TAB_MODEL_SELECT" ) ;
		return -1 ;
	}
	
	status = ReadLine ( file, choice, 20 ) ;
	status = ReadLine ( file, item,   20 ) ;
	
	CloseFile( file  ) ;
	
	modelo_atual_index = atoi(item) ;
	if(modelo_atual_index < 0 || modelo_atual_index > MAX_QUANT_MODELOS)
		modelo_atual_index =0;
	SetCtrlIndex ( painel_auto, AUTO_MODELOS, modelo_atual_index ) ;
	res = AtualizaDadosModelo();
	
	return res ;
}


/********************** ReadChoicedIndexFile() *********************************************************/
int ReadChoicedIndexFile (void)
{

int 
	status,
	i=0, 
	file, 
	file1, 
	Num_max_itens, 
	nMaxItens,
	res;
char	
	itemLabel[20],  
	aux[5],
	nome[120]  ;
	
	//////////////////////////////////////////////////////////////
	GetCtrlIndex ( painel_auto, AUTO_MODELOS, &modelo_atual_index ) ;
	
	GetLabelFromIndex ( painel_auto, AUTO_MODELOS, modelo_atual_index, itemLabel );
	
	file = OpenFile ( TAB_MODEL_SELECT, VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII);
	if ( file < 0 ){
		MessagePopup( " ERRO ", "Erro na abertura do arquivo TAB_MODEL_SELECT" ) ;
		return -1 ;
	}
	
	status = WriteLine( file, itemLabel, strlen(itemLabel) ) ;
	Fmt( aux, "%d", modelo_atual_index ) ;
	status = WriteLine( file, aux, strlen(aux) ) ;
	
	CloseFile( file  ) ;
	res = AtualizaDadosModelo();
	
	return res;

}




//
// Função: InicializaCIM()
//
int InicializaCIM(void)
{
	int
		file,
		i,
		status,
		tamanho;
	char
		cim_buffer[50],
		Num_Maquina[15],		   
		cim_label[50],
		tmp[90];
	short
		timeout = 5;

	Mensagem( "Aguarde...","Iniciando CIM e contagem produção...", "", "", MENS_CINZA);

	/************** Inicia a contagem da produção **************/
	file = OpenFile( ARQ_FTT, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII );
	status = ReadLine( file, tmp,  5 );
	TotalTestados   = atoi(tmp);
	status = ReadLine( file, tmp,  5 );
	TotalAprovados  = atoi(tmp);
	status = ReadLine( file, tmp,  5 );
	TotalRejeitados = atoi(tmp);
	status = ReadLine( file, tmp,  5 );
	TotalFTT        = atof(tmp);
	CloseFile (file);

	//**********  Inicializa variaveis do CIM  **********
	strcpy(host,"");
	strcpy(service,"");
	strcpy(stationID,"");
	file = OpenFile ("..\\CIM\\Config_Cim.txt", VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	if (file < 0)
	{
		MessagePopup ( " Erro abrindo arquivo CIM", "Falha ao abrir arquivo de configuração do CIM..." );
		return OFF;
	}
	for(i=0; i < 10; i++) //le até 10 linhas
	{
		if(ReadLine (file, cim_buffer, 29)<0)//fim do arquivo
			break;
		strcpy (cim_label, cim_buffer);
		tamanho = strlen(cim_label);
		if (!tamanho) continue; //continua no loop se string for vazia
		StringUpperCase (cim_label);
		if((strstr(cim_label,"FIM") != NULL || strstr(cim_label,"END") != NULL) && strlen(cim_label)<=4)
			break;

		if(strstr(cim_label, "MAQUINA=") != NULL)
		{
			CopyString( Num_Maquina, 0, cim_label, 8, 12 ) ;
			strcpy( stationID, Num_Maquina ) ;
		}
		else if (strstr (cim_label, "HOST=") != NULL)
		{
			CopyBytes(host,0,cim_buffer,5,tamanho-5);
		}
		else if (strstr (cim_label, "SERVICE=")!= NULL)
		{
			CopyBytes(service,0,cim_buffer,8,tamanho-8);
		}
	}
	CloseFile (file);
	rescim = CIMSockets_New_SMF (NULL, 0, LOCALE_NEUTRAL, 0, &cimsockets);//"CIM Sockets - SMF", "Criando Object Handle";
	if(rescim < 0 || cimsockets < 0)
	{
		Mensagem( "","CIM Sockets - SMF", "Erro parametros do CIM - criação do socket", "", MENS_VERMELHO);
		Delay(3);
		goto erro_cim;
	}
	rescim = CIMSockets__SMFopen_sock (cimsockets, NULL, host, service, timeout, &sockID);//CIM Sockets - SMF", Opening Socket
	if (rescim < 0 || sockID < 0)
	{
		Mensagem ( "","CIM Sockets - SMF", "Erro parametros do CIM - Erro abrindo socket", "", MENS_VERMELHO);
		Delay(3);
		goto erro_cim;
	}
	rescim = CIMSockets__SMFsync_sock (cimsockets, NULL, sockID, stationID,
									   timeout, &timestamp);
	if (rescim < 0)
	{
		Mensagem( "","CIM Sockets - SMF", "Erro sincronizando conexão rede CIM", "", MENS_VERMELHO);
		Delay(3);
		goto erro_cim;
	}
	if (timestamp < 0)
	{
		Mensagem( "","CIM Sockets - SMF", "Erro de conexão com a rede CIM - time stamp", "", MENS_VERMELHO);
		Delay(3);
		goto erro_cim;
	}
	status = SetCtrlVal (painel_auto, AUTO_LED_CIM, ON);
	status = SetCtrlAttribute (painel_auto, AUTO_LED_CIM, ATTR_LABEL_TEXT,
							   "CIM Conectado");
	return ON;

erro_cim:

	status = SetCtrlVal (painel_auto, AUTO_LED_CIM, OFF);
	status = SetCtrlAttribute (painel_auto, AUTO_LED_CIM, ATTR_LABEL_TEXT,
							   "CIM Desconectado");
	return OFF;
}
