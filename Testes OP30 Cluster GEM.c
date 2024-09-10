
#include <cviauto.h>
#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include "CIMSockets.h"
#include "Main OP30 GEM.h"
#include "Auto OP30 GEM.h"
#include "Manual OP30 GEM.h"
#include "Vision Scripts OP30 Cluster GEM.h"
#include "Master_e_Negative_Samples.h"

//
// Função: ExecutaTestes()
//
double ExecutaTestes(int pos_teste, char*tab_spare, char*pn_cliente, int modelo)    
{ 
	double 
		res=RES_FALHA;

	switch (pos_teste)
	{
	case 1:
		res = Montar_Difusor( MensagemFalha1 );
		break;
	case 2:
		res = Montar_LCD( MensagemFalha1 );
		break;
	case 3:
		res = Montar_LCD_Retirar_Liner( MensagemFalha1 );
		break;
	case 4:
		res = Montar_Shield( MensagemFalha1 );
		break;
	case 5:
		res = VisionVerificaMontagem(DIFUSOR, MensagemFalha1, ON );
		break;
	case 6:
		res = VisionVerificaMontagem(SHIELD, MensagemFalha1, ON );
		break;
	case 7:
		res = VisionVerificaMontagem(DISPLAY, MensagemFalha1, OFF );
		break;
	case 8:
		res = VisionVerificaMontagem(LIGHT_GUIDE_TACO, MensagemFalha1, ON );
		break;
	case 9:
		res = VisionVerificaMontagem(LIGHT_GUIDE_DISP, MensagemFalha1, ON );	
		break;
	case 10:
		res = VisionVerificaMontagem(LIGHT_GUIDE_VELO, MensagemFalha1, ON );
		 break;
	case 11:
		res = VisionVerificaMontagem(LOCALIZADOR, MensagemFalha1, ON );
		break;
	case 12:
		res = VisionVerificaMontagem(DIFUS_PONTO_1, MensagemFalha1, ON );
		break;
	case 13:
		res = VisionVerificaMontagem(DIFUS_PONTO_2, MensagemFalha1, OFF );
		break;
	case 14:
		res = VisionVerificaMontagem(DISP_LINER_PONTO_1, MensagemFalha1, OFF );
		break;
	case 15:
		res = VisionVerificaMontagem(DISP_LINER_PONTO_2, MensagemFalha1, OFF );
		break;
	case 16:
		res = VisionVerificaMontagem(DISP_LINER_PONTO_3, MensagemFalha1, OFF );
		break;
	case 17:
		res = VisionVerificaMontagem(DISP_LINER_PONTO_4, MensagemFalha1, OFF );
		break;
	case 18:
		res = Localizador_painel(MensagemFalha1);
		break;
	default:
		MessagePopup ("ERRO NA TABELA", "Teste inexistente..." );
	}
	return res;
}

//
// Função: FinalizaTestes()
//
int FinalizaTestes(double res)
{
	int 
		file,
		i,
		status;
	char 
		tmp[90],
		path[50],
		mens_falha[100]= {0};
		
	strcpy(path,CAMINHO_SONS);
	if(sair_programa)
		return 0;
	VisionIluminacao(ILUM_SUP_OFF);
	SAIDA.Liga_Ionizador 	= OFF;
	status = MoveTravaClampProduto( OFF ) ;
	leitor_ativado = OFF; 
	
	
	if( res==FALHA_GRAVE ){		
		Mensagem("Chame o técnico", MensagemFalha1, "Aperte CONTINUE para abortar", "", MENS_VERMELHO);
		do{
			Delay_thread(0.100);			
		}while( STATUS.Continua==OFF );
		ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "" );
		if(!sair_programa) sair_programa=ON;
		return FALHA_GRAVE;
	}
	if(amostra_tipo == NEGATIVE_SAMPLE && neg_samp_rejeitou)
	{
		rejeitado = OFF;
	}
	tempo_total=Timer()-tempo_inic_total_teste;
	if( rejeitado )
	{
		SAIDA.Sinaleiro_Vermelho = ON;
 		status_produto = PRODUTO_REJEITADO;
		if( res != FALHA_TIMEOUT && res != FALHA_PRODUTO_AUSENTE )
		{
			TotalRejeitados++;
	      	TotalTestados++;
		}
	}
	else
	{
	 	SAIDA.Sinaleiro_Verde = ON;
		status_produto = PRODUTO_APROVADO;
	    TotalAprovados ++;
	    TotalTestados++;
		strcat(path,"Sininho01.wav");	 
		CVI_PlaySound (path, ON );
	}
	if(amostra_tipo == TESTE)
	{
		if( cim_conectado == ON && res != FALHA_PRODUTO_AUSENTE && res != FALHA_TIMEOUT && !em_debug )
		{
			MoveOut_Build_Cim( id_prefix_board[modelo_atual_index], res, rejeitado, teste );
		}
		//Grava as estatisticas em arquivo
		file = OpenFile( ARQ_FTT , VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII );
		Fmt( tmp, "%s<%f[p0]", TotalTestados);
		WriteLine ( file, tmp, strlen(tmp) );
		Fmt( tmp, "%s<%f[p0]", TotalAprovados);
		WriteLine ( file, tmp, strlen(tmp) );
		Fmt( tmp, "%s<%f[p0]", TotalRejeitados);
		WriteLine ( file, tmp, strlen(tmp) );
		Fmt( tmp, "%s<%f[p1]", ftt);
		WriteLine ( file, tmp, strlen(tmp) );
		CloseFile ( file );
		AtualizaFTT();
		// -----------------------  Coleta Dados ----------------------------------              
		if( coletando_dados && res != FALHA_TIMEOUT && res != FALHA_PRODUTO_AUSENTE )
			ColetaDados(max_teste);	
		else if(amostra_tipo == MASTER_SAMPLE || amostra_tipo == NEGATIVE_SAMPLE)
		 	Grava_Hist_e_Status_das_Samples(max_teste);
	}
	else if(amostra_tipo == MASTER_SAMPLE || amostra_tipo == NEGATIVE_SAMPLE)
	 	Grava_Hist_e_Status_das_Samples(max_teste);
	if(criar_negat_sample)
	{
		RegistraNegatSample();
		SetCtrlVal (painel_manual, MANUAL_CRIAR_NSAMPLE, 0);
		criar_negat_sample = OFF;
	}
	i=0;
 	while(i <= max_teste)
	{
 		Teste[i].Resultado = RES_FALHA;
 		strcpy(Teste[i].Condicao ,"N.T.");
	 	i++;
 	}
	status = MensFinal( Rodape, TotalRejeitados,  tempo_total, res, Teste[teste].LimAlto, Teste[teste].LimBaixo, Teste[teste].Nome, teste );
	SetCtrlAttribute (painel_auto, AUTO_TEMPO_TOTAL, ATTR_CTRL_VAL, 0.00);
	status = SEM_ERRO;
	if( all_motions_home==OFF ){
		status = RetornaMovimentos(1, mens_falha);
	}
	if( status == SEM_ERRO ){
		// -------------------- Espera retirar Produto ------------------------------
		while( status_debounc_presenca_produto && !sair_programa && status_emergencia ) 
			Delay_thread(0.01);
	}
	else
	{
		MessagePopup( "Erro ao Retornar Movimentos", mens_falha );
		if(!sair_programa && status==FALHA_GRAVE )
			sair_programa=ON;
	}
 	SAIDA.Sinaleiro_Verde = OFF;
 	SAIDA.Sinaleiro_Vermelho = OFF;
	testar_produto	= OFF;
	modulo_em_teste = OFF;
	DiscardPanel(painel_mensagens);
	ResetBarcode(OFF);
	status_produto  = PRODUTO_OFF;
	return 0;
}

//
// Função: StatusTeste()
//
int StatusTeste( double res )
{
	static int 
		cont_retest = 0;
	int 
		status;
	char 
		tmp[90],
		path[100];
	
	if( sair_programa )
		return 0;

	if( res==FALHA_GRAVE ){
		return FALHA_GRAVE;
	}

	Rodape = "";
	retestar = OFF;
	Teste[teste].Resultado = res;
	Teste[teste].Tempo_teste = Timer()-tempo_inicial_teste;
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo) || rejeita_montagem)
	{ 
		rejeitado = ON;
		SetCtrlAttribute (painel_auto, AUTO_TXT_STATUS, ATTR_TEXT_COLOR, VAL_DK_RED);
	 	strcpy(Teste[teste].Condicao,"Rejeit");
		strcpy(path,CAMINHO_SONS);
		strcat(path,"campainha_1.wav");	 
		if(amostra_tipo != NEGATIVE_SAMPLE) 
			CVI_PlaySound (path, ON );
		Delay_thread(1.2);
		CVI_StopCurrentSound();
		if(amostra_tipo == NEGATIVE_SAMPLE && !treinar_vision)
		{  
			neg_samp_rejeitou = ON;
    		rejeitado		  = OFF ;
		}
	}
	else
	{
		SetCtrlAttribute (painel_auto, AUTO_TXT_STATUS, ATTR_TEXT_COLOR, VAL_DK_BLUE);
	 	strcpy(Teste[teste].Condicao,"Aprov");
	}  
	ReplaceTextBoxLine (painel_auto, AUTO_TXT_STATUS, 0, Teste[teste].Condicao);
	Fmt(tmp, "%s<%f[p2]", res);
	ReplaceTextBoxLine (painel_auto, AUTO_TXT_RESULTADO, 0, tmp);
	Fmt(tmp, "%s<%f[p2]", Teste[teste].Tempo_teste);
	ReplaceTextBoxLine (painel_auto, AUTO_TXT_TEMPO, 0, tmp);
	if( rejeitado )
	{
		if( TRAT_REJ == RETESTA )
		{
			if(res == FALHA_PRODUTO_AUSENTE )
				status= REJ;
			else
		    	status = FuncRejeitado(Rodape, MensagemFalha1);
		}
		else
		{
			cont_retest++;
			if(cont_retest <= 3)
				status = CONT;
			else
				status = REJ;
			
		}
	 	if (status != CONT)
		{
			aborta_testes = ON;
			rejeitado 	  = ON;
		}
	 	if (status == CONT)
	 	{
			teste = teste_retorno;
			if(rejeitado )
			{
				retestar  = ON;
				rejeitado = OFF;
			}
	 	}
	}
	Fmt(tmp, "%s<%f[p2]", Teste[teste].Tempo_teste);
	status = ReplaceTextBoxLine (painel_auto, AUTO_TXT_TEMPO, 0, tmp);
	if(!rejeitado && !retestar) 
	{
		teste ++;   
		retestar 	= OFF;
		cont_retest	= 0;
	}
	return 0;
}

//
// Função: InicializaTestes()
//
int InicializaTestes(void)
{
	char
		group_id[6];
	short
		time_out = 5;
	int
		tent,
		status;

	if(modo_samples == SEMPRE)
	{
		if(Gerenciar_Samples() < 0)
		{
			ResetBarcode(OFF);
		 	status_produto 	= PRODUTO_OFF;
			testar_produto	= OFF;
			modulo_em_teste	= OFF;
			return -1;
		}
	}
	DeleteImage (painel_auto, AUTO_PIC_INSTRUCAO);
	desabilitar_leitor_enditem = ON;
	ler_string_scanner = OFF;
	Delay_thread(0.001);
	leitor_ativado = OFF;
	strcpy(group_id, id_prefix_board[modelo_atual_index]);
	rejeita_montagem = OFF;
	tempo_inic_total_teste = tempo_inicial_teste=Timer ();
																			
    while( STATUS.BiManual == OFF && STATUS.Continua==OFF && !sair_programa )
	{
				Delay_thread(0.001);
				COR_MENSAGENS = MENS_PRETO ;
		        strcpy( mens_montagem_1, "" ) ;
				strcpy( mens_montagem_2, "" ) ;
		        strcpy( mens_montagem_3, "APERTE 'CONTINUE' OU 'BIMANUAL' PARA INICIAR" ) ;
				
				if(!status_debounc_presenca_produto )
				{
				     COR_MENSAGENS = MENS_VERMELHO ;
					 strcpy( mens_montagem_1, "CLUSTER RETIRADO DO BERÇO" ) ;
					 strcpy( mens_montagem_2, "" ) ;
					 strcpy( mens_montagem_3, "" ) ;
				     Delay_thread(0.5);
			         testar_produto	= OFF;
				     modulo_em_teste = OFF;
				     ResetBarcode(OFF);
			         return -1;
				}

	}
			
	status = MoveTravaClampProduto( ON ) ;
	if( status < 0 )
	{
	     COR_MENSAGENS = MENS_VERMELHO ;
		 strcpy( mens_montagem_1, "Erro no Acionamento das Travas do Clamp" ) ;
		 strcpy( mens_montagem_2, "" ) ;
		 strcpy( mens_montagem_3, "" ) ;
	     Delay_thread(0.5);
	     status = MoveTravaClampProduto( OFF ) ;
         testar_produto	= OFF;
	     modulo_em_teste = OFF;
	     ResetBarcode(OFF);
         return -1;

	}
	   
	if (cim_conectado)
	{
		tent = 0;
		errordesc = (char *) CA_AllocMemory(500);
	falha_conex2:
		rescim = CIMSockets__SMFmovein (cimsockets, NULL, sockID, time_out, stationID, N_Serie_pwb, &errordesc, &resultcim);
		if (resultcim < 0 || rescim <0)
		{
			if(tent == 0)
			{
				tent = 1;
				Delay_thread(0.100);
				//if(Reconect_CIM () == 0)
					goto falha_conex2;
			}
			MENS_WARNING = MENS_CIM_ERROR;
			Delay_thread(0.1);
			Fmt(mens_montagem_1,"%s<%s%s%s","Barcode ",N_Serie_pwb," inválido no CIM ou erro de ROTA");         
			//Mensagem ( "",Mensagem, errordesc, "", MENS_VERMELHO);
			strcpy(mens_montagem_2, errordesc);
			CA_FreeMemory(errordesc);
			ResetBarcode(OFF);
		 	status_produto = PRODUTO_OFF;
			testar_produto = OFF;
			modulo_em_teste	= OFF;
			status = MoveTravaClampProduto( OFF ) ;
		    return -1;
		}
		CA_FreeMemory(errordesc);
	}
 	status_produto = PRODUTO_TESTANDO;
	aborta_testes = OFF;
	retestar = OFF;
	teste = 1;
	teste_retorno = teste;

	return 0;	
}

//
// Função: Montar_Difusor()
//
double Montar_Difusor(char * mens_rejeito)
{
	int
		sair,
		botao_saida,
		tempo_imagens;
	int
	status;
	SAIDA.Liga_Ionizador   = ON ;
	botao_saida = CONT ;
	tempo_imagens = 1.5;
	Delay_thread(0.020);
	teste_retorno = teste;
	SAIDA.iluminacao_superior_max = ON;
	Delay_thread(0.020);	
	
	
	do{
		
		COR_MENSAGENS = MENS_PRETO ;
		strcpy( mens_montagem_2, "MONTE O DIFUSOR, O DISPLAY SOBRE O LIGHT GUIDE" ) ;
		strcpy( mens_montagem_3, "APERTE 'CONTINUE' PARA PROSSEGUIR" ) ;
		
		do{
			Delay_thread(0.020);	
		}while( STATUS.Continua==ON ) ;
		
		SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, "");
		strcpy( mens_rejeito, "\0" );
		strcpy(MensagemFalha1, "");
		do
		{
			sair = EsperaSairMontagem(botao_saida, "MPlate Berco.jpg", tempo_imagens, OFF); 
			if(sair != NAO_SAIR)
				break;
			sair = EsperaSairMontagem(botao_saida, "AperteContinue.jpg", tempo_imagens, OFF); 
			if(sair != NAO_SAIR)
				break;
		} while(sair == NAO_SAIR);
		if(sair == FALHA_REJ_OPERADOR || sair == FALHA_MODULO_ABORTADO || sair == FALHA_PRODUTO_AUSENTE)
		{
			return sair;
		}
		
		Delay_thread(0.100) ;
	
		
	}while( STATUS.Presenca_PCB==OFF ) ;
	
	DeleteImage (painel_auto, AUTO_PIC_INSTRUCAO);
	COR_MENSAGENS = MENS_CINZA ;
	strcpy( mens_montagem_2, "" ) ;
	strcpy( mens_montagem_3, "Aguarde..." ) ;
	if(sair_programa)
		return EXIT_PROGRAM ;

	return 1.0;
}






//
// Função: Montar_LCD_Retirar_Liner()
//
double Montar_LCD_Retirar_Liner(char * mens_rejeito)
{
	int
		sair,
		botao_saida,
		
		tempo_imagens ;
	
	if (!retestar)
		descartou_liner=0;
	
	// =============================================================================================

	botao_saida = BOT_OFF ;
	tempo_imagens = 1.5;
	rejeita_montagem = OFF ;
	teste_retorno = teste;
	
	COR_MENSAGENS = MENS_PRETO ;
	
	strcpy( mens_montagem_2, "Retire o liner INFERIOR do Display" ) ;
	strcpy( mens_montagem_3, "Coloque no Descarte " ) ;
	
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, "");
	strcpy( mens_rejeito, "\0" );
	strcpy(MensagemFalha1, "")  ;
	
	do
	{
		sair = EsperaSairMontagem(botao_saida, "Desc Liner 1.jpg", tempo_imagens, ON); 
		if(sair != NAO_SAIR)
			break;
		sair = EsperaSairMontagem(botao_saida, "Desc Liner 2.jpg", tempo_imagens, ON); 
		if(sair != NAO_SAIR)
			break;
	} while(sair == NAO_SAIR);
	if(sair == FALHA_REJ_OPERADOR || sair == FALHA_MODULO_ABORTADO || sair == FALHA_PRODUTO_AUSENTE)
	{
		return sair;
	}

	COR_MENSAGENS = MENS_CINZA ;
	
	strcpy( mens_montagem_2, "Retirado" ) ;
	strcpy( mens_montagem_3, "" ) ;
	
	do{
		Delay_thread(0.020);	
	}while( STATUS.Continua==ON ) ;
	
	// =============================================================================================
	
	if (!retestar)
		descartou_liner=0;

	botao_saida = BOT_OFF ;
	tempo_imagens = 1.5 ;
	rejeita_montagem = OFF ;
	
	COR_MENSAGENS = MENS_AZUL ;
	
	strcpy( mens_montagem_2, "Retire o liner SUPERIOR do Display" ) ;
	strcpy( mens_montagem_3, "Coloque no Descarte" ) ;
	
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, "");
	strcpy( mens_rejeito, "\0" );
	strcpy(MensagemFalha1, "");
	do
	{
		sair = EsperaSairMontagem(botao_saida, "Desc Liner 1.jpg", tempo_imagens, ON); 
		if(sair != NAO_SAIR)
			break;
		sair = EsperaSairMontagem(botao_saida, "Desc Liner 2.jpg", tempo_imagens, ON); 
		if(sair != NAO_SAIR)
			break;
	} while(sair == NAO_SAIR);
	if(sair == FALHA_REJ_OPERADOR || sair == FALHA_MODULO_ABORTADO || sair == FALHA_PRODUTO_AUSENTE)
	{
		return sair;
	}

	COR_MENSAGENS = MENS_CINZA ;
	
	strcpy( mens_montagem_2, "Retirado" ) ;
	strcpy( mens_montagem_3, "" ) ;
	
	do{
		Delay_thread(0.020);	
	}while( STATUS.Continua==ON ) ;
	
return 1.00;
}
/*-----------------------------------------------------------------------------------
                               Monta LCD e TFT
-------------------------------------------------------------------------------------*/
double Montar_LCD(char * mens_rejeito)
{
	int
		sair,
		botao_saida,
		tempo_imagens ;
	
	teste_retorno = teste;
	botao_saida = CONT;
	tempo_imagens = 1.5;
	rejeita_montagem = OFF ;
	
	COR_MENSAGENS = MENS_PRETO ;
	
	SAIDA.iluminacao_superior_max = ON;
	Delay_thread(0.020);	
	
	strcpy( mens_montagem_2, "Monte o Display - seguir orientações da IT" ) ;
	strcpy( mens_montagem_3, "APERTE 'CONTINUE' PARA PROSSEGUIR" ) ;
	do{
			Delay_thread(0.020);	
		}while( STATUS.Continua==ON ) ;

	
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, "");
	strcpy( mens_rejeito, "\0" );
	strcpy(MensagemFalha1, "");
	do
	{
		sair = EsperaSairMontagem(botao_saida, "Mont Display.jpg", tempo_imagens, OFF); 
		if(sair != NAO_SAIR)
			break;
		} while(sair == NAO_SAIR);
	if(sair == FALHA_REJ_OPERADOR || sair == FALHA_MODULO_ABORTADO || sair == FALHA_PRODUTO_AUSENTE)
	{
		return sair;
	}
	
	DeleteImage (painel_auto, AUTO_PIC_INSTRUCAO);
	COR_MENSAGENS = MENS_CINZA ;
	
	strcpy( mens_montagem_2, "Montado" ) ;
	strcpy( mens_montagem_3, "" ) ;
	

return 1.0 ;
	
}
/*-----------------------------------------------------------------------------------
                               Monta Shield
-------------------------------------------------------------------------------------*/
double Montar_Shield(char * mens_rejeito)
{
	int
		sair,
		botao_saida,
		tempo_imagens ;
	
	botao_saida = CONT;
	tempo_imagens = 1.5;
	rejeita_montagem = OFF ;
	teste_retorno = teste;
	
	COR_MENSAGENS = MENS_PRETO ;
	
	SAIDA.iluminacao_superior_max = ON;
	Delay_thread(0.020);	
	
	strcpy( mens_montagem_2, "Monte o Shield - seguir orientações da IT" ) ;
	strcpy( mens_montagem_3, "APERTE 'CONTINUE' PARA PROSSEGUIR" ) ;
	
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, "");
	strcpy( mens_rejeito, "\0" );
	strcpy(MensagemFalha1, "");
	if(retestar == OFF)
	{
		SAIDA.Sinaleiro_Verde = ON;
		Delay_thread(3.5);
		SAIDA.Sinaleiro_Verde = OFF;
	}
	do
	{
		sair = EsperaSairMontagem(botao_saida, "Mont Shield.jpg", tempo_imagens, OFF); 
		if(sair != NAO_SAIR)
			break;
		} while(sair == NAO_SAIR);
	if(sair == FALHA_REJ_OPERADOR || sair == FALHA_MODULO_ABORTADO || sair == FALHA_PRODUTO_AUSENTE)
	{
		return sair;
	}
	
	DeleteImage (painel_auto, AUTO_PIC_INSTRUCAO);
	COR_MENSAGENS = MENS_CINZA ;
	
	strcpy( mens_montagem_2, "Montado" ) ;
	strcpy( mens_montagem_3, "" ) ;
	

return 1.0 ;
	
}

/*----------------------------------------------------------------------------------------------------------------
               Verifica a montagem usando quantify - valores limites no arquivo config vision
-----------------------------------------------------------------------------------------------------------------*/
double VisionVerificaMontagem(int montagem, char * mens_rejeito, int referenciar)
{
int 
	tent             = 0,
	erro_locator     = OFF,
	treinar_template = ON,
	rej_vision       = OFF,
	max_tent         = 2;
double 
	res = 0;
char
	mens[100] = {0};
	
	
	/////////////////////////////////
	if(montagem == DIFUS_PONTO_1)
		teste_retorno = teste;
	strcpy(mens_montagem_2,   " Verificando com Sistema de Visão");
	strcpy(mens_montagem_3,   " ");
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
/////////////////////////////////////////////////////////////////////////////////////////	
	if(criar_negat_sample)
	{
		strcpy(mens_montagem_2, "CRIANDO NEGATIVE SAMPLE...");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == MASTER_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Master Sample");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Amostra Negativa");
		strcpy(mens_montagem_3, "");
	}
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
///////////////////////////////////////////////////////////////////////////////////////	
	
reteste:
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	if(retestar)
		referenciar = ON;
	switch (montagem)
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		case LOCALIZADOR:
			strcpy(mens,"Rejeitou localizador");
			res = VisionQuantify(coord_qtfy_difusor, LOCATOR_PAINEL, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem,COORD_LOCATOR_PAINEL_STR);
			break;
		case DIFUS_PONTO_1:
			strcpy(mens,"Rejeitou presença do difusor PT 1");
			res = VisionQuantify(coord_roi_difusor_pt_1, DIFUS_PONTO_1, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DIFUSOR_PT_1_STR);
			break;
		case DIFUS_PONTO_2:
			strcpy(mens,"Rejeitou presença do difusor PT 2");
			res = VisionQuantify(coord_roi_difusor_pt_2, DIFUS_PONTO_2 ,referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DIFUSOR_PT_2_STR);
			break;
		case DISP_LINER_PONTO_1:
			strcpy(mens,"Rejeitou presença Display/Liner PT 1");
			res = VisionQuantify(coord_roi_disp_liner_pt_1, DISP_LINER_PONTO_1, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISP_LINER_PT_1_STR);
			break;
		case DISP_LINER_PONTO_2:
			strcpy(mens,"Rejeitou presença Display/Liner PT 2");
			res = VisionQuantify(coord_roi_disp_liner_pt_2, DISP_LINER_PONTO_2, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISP_LINER_PT_2_STR);
			break;
		case DISP_LINER_PONTO_3:
			strcpy(mens,"Rejeitou presença Display/Liner PT 3");
			res = VisionQuantify(coord_roi_disp_liner_pt_3, DISP_LINER_PONTO_3, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISP_LINER_PT_3_STR);
			break;
		case DISP_LINER_PONTO_4:
			strcpy(mens,"Rejeitou presença Display/Liner PT 4");
			res = VisionQuantify(coord_roi_disp_liner_pt_4, DISP_LINER_PONTO_4, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISP_LINER_PT_4_STR);
			break;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		case DIFUSOR:
			strcpy(mens,"Rejeitou presença do difusor ");
			res = VisionQuantify(coord_qtfy_difusor, DIFUSOR, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DIFUSOR_STR);
			break;
		case SHIELD:
			strcpy(mens,"Rejeitou presença do shield ");
			res = VisionQuantify(coord_qtfy_shield, SHIELD, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_SHIELD_STR);
			break;
		case DISPLAY:
			strcpy(mens,"Rejeitou presença do Display ");
			res = VisionQuantify(coord_qtfy_display, DISPLAY, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISPLAY_STR);
			break;
        case LINER_SUP:
			strcpy(mens,"Rejeitou liner superior ");
			res = VisionQuantify(coord_liner_sup, LINER_SUP, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISP_LINER_PT_1_STR);
			break;
		case LINER_INF:
			strcpy(mens,"Rejeitou liner inferior ");
			res = VisionQuantify(coord_liner_inf, LINER_INF, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, COORD_ROI_DISP_LINER_PT_2_STR);
			break;
		case LIGHT_GUIDE_TACO:
			strcpy(mens,"Rejeitou light Guide TACO ");
			res = Vision_Template_Forma_e_Cor(dados_cfg_vision ,&erro_locator, referenciar, TEMPLATE_LGUIDE_TACO, 700, coord_temp_light_esq, ON, ON, treinar_template);
		    break;
		case LIGHT_GUIDE_DISP:
			strcpy(mens,"Rejeitou light Guide DISPLAY ");
			res = Vision_Template_Forma_e_Cor(dados_cfg_vision ,&erro_locator, referenciar, TEMPLATE_LGUIDE_DISP, 700, coord_temp_light_disp, ON, ON, treinar_template);
		    break;
	    case LIGHT_GUIDE_VELO:
			strcpy(mens,"Rejeitou light Guide VELOCIMETRO ");
			res = Vision_Template_Forma_e_Cor(dados_cfg_vision ,&erro_locator, referenciar, TEMPLATE_LGUIDE_VELO, 700, coord_temp_light_dir, ON, ON, treinar_template);
		    break;
	}			
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
	{
		rej_vision = ON;
		referenciar = ON;
	}
	else
	{
		rej_vision = OFF;
	}
	if(rej_vision && tent < max_tent)
	{
		tent++;
		goto reteste;
	}
	if(rej_vision )
		Fmt( mens_rejeito, "%s<%s%s%f[p2]%s%f[p2]%s%f[p2]",mens, "Min = ",Teste[teste].LimBaixo, " Max = ",Teste[teste].LimAlto, " Res = ", res ) ;
Error:
return res;
}
/*----------------------------------------------------------------------------------------------------------------
               Verifica se as travas do aplique foi montado corretamente
-----------------------------------------------------------------------------------------------------------------*/
double VisionVerificaTravaAplique(char * mens_rejeito)
{
int
	status          = 0,
	max_tent 		= 2,
	rej_vision      = OFF,
	erro_locator    = OFF,
	tent = 0;
double 
	res=0.00;

	/////////////////////////////////
	if(criar_negat_sample)
	{
		strcpy(mens_montagem_2, "CRIANDO NEGATIVE SAMPLE...");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == MASTER_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Master Sample");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Amostra Negativa");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == TESTE)
	{
		strcpy(mens_montagem_2,   " Verificando o aplique");
		strcpy(mens_montagem_3,   " ");
	}
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
reteste:
	rej_vision = OFF;
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	 status = Vision_Locator_Main(&erro_locator, ON);
	if(status<0 || erro_locator != OFF)
	{
		strcpy(mens_rejeito,"Erro na Visão para detecção do Aplique. Erro de Localizador");
		res = -1;
		goto Error;
	}
	//Verifica trava esquerda do aplique
	 res = Vision_2_Edge(coord_trava_aplique_esq, OFF, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem);
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
	{
		rej_vision = ON;
		strcpy(mens_rejeito,"Rejeitou teste de Visão para a trava esquerda do aplique  ");
	}
	else
	{
		rej_vision = OFF;
	}
	if(rej_vision && tent < max_tent)
	{
		tent++;
		goto reteste;
	}
	if(rej_vision)
		goto Error;
	//Verifica trava direita do aplique 
	 res = Vision_2_Edge(coord_trava_aplique_dir, OFF, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem);
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
	{
		rej_vision = ON;
		strcpy(mens_rejeito,"Rejeitou teste de Visão para a trava direita do aplique ");
	}
	if(rej_vision && tent < max_tent)
	{
		tent++;
		goto reteste;
	}
	if(rej_vision)
		goto Error;

Error:	
return res;
}
/*----------------------------------------------------------------------------------------------------------------
               Verifica o barcode do Aplique e do display
-----------------------------------------------------------------------------------------------------------------*/
double VisionVerificaBarcodeAplique(char * mens_rejeito, int referenciar)
{
int 
	tent = 0,
	erro_locator = OFF,
	max_tent = 2;
double 
	res = 0;
	/////////////////////////////////
	if(criar_negat_sample)
	{
		strcpy(mens_montagem_2, "CRIANDO NEGATIVE SAMPLE...");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == MASTER_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Master Sample");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Amostra Negativa");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == TESTE)
	{
		strcpy(mens_montagem_2,   " Verificando o barcode Aplique - com Sistema de Visão");
		strcpy(mens_montagem_3,   " ");
	}
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
reteste:
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	//Verifica barcode Aplique
	res = VisionBarcodeReader(coord_roi_barcode_aplique, referenciar, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, dados_cfg_vision.contraste_montagem, dados_cfg_vision.gamma_montagem);
	if(res == 1)
	{
		if((strcmp(barcode_lido, id_aplique[modelo_atual_index]) == 0))
			res = 1;
		else if(tent<max_tent)
		{
			tent++;
			goto reteste;
		}
		else
		{
			res = 0;
			strcpy(mens_rejeito,"Rejeitou teste de Visão para modelo do aplique ");
			goto Error;
		}
	}
	else
	{
		goto Error;	
	}
	
Error:	
return res;
}
/*----------------------------------------------------------------------------------------------------------------
               Verifica o barcode do display
-----------------------------------------------------------------------------------------------------------------*/
double VisionVerificaBarcodeLCD(char * mens_rejeito, int referenciar)
{
int 
	tent = 0,
//	erro_locator = OFF,
	max_tent = 2;
double 
	res = 0;
	/////////////////////////////////
	if(criar_negat_sample)
	{
		strcpy(mens_montagem_2, "CRIANDO NEGATIVE SAMPLE...");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == MASTER_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Master Sample");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Amostra Negativa");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == TESTE)
	{
		strcpy(mens_montagem_2,   " Verificando o barcode Display");
		strcpy(mens_montagem_3,   " ");
	}
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
reteste:
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	//Verifica barcode Display
	res = VisionBarcodeReader(coord_roi_barcode_display, referenciar, dados_cfg_vision.camera_shutter_barcode_lcd, dados_cfg_vision.camera_gain_barcode_lcd, dados_cfg_vision.contraste_barcode_lcd, dados_cfg_vision.gamma_barcode_lcd);
	if(res == 1)
	{
		if((strcmp(barcode_lido, id_display[modelo_atual_index]) == 0))
			res = 1;
		else if(tent<max_tent)
		{
			tent++;
			goto reteste;
		}
		else
		{
			res = 0;
			strcpy(mens_rejeito,"Rejeitou teste de Visão para modelo do display ");
			goto Error;
		}
	}			
	
Error:	
return res;
}
/*----------------------------------------------------------------------------------------------------------------
       Verifica se os pinos do LCD estão aparecendo na parte inferior
-----------------------------------------------------------------------------------------------------------------*/
double VisionVerificaMontagemPinsLCD(char * mens_rejeito)
{
int 
	status			= 0, 
	max_tent 		= 2,
	rej_vision,
	referenciar     = ON,
	erro_locator    = OFF,
	tent = 0;
double 
	res=0.00;

	 
	/////////////////////////////////
	tent = 0;
	strcpy(MensagemFalha1, "");
	if(!retestar)
		descartou_liner = OFF;
	strcpy(mens_montagem_2,   " Testando os pinos do display");
	strcpy(mens_montagem_3,   " ");
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	COR_MENSAGENS = MENS_AZUL;
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
retest_vision:
	rej_vision = OFF;
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	status = Vision_Locator_Pins_Lcd(&erro_locator, ON);
	if(status<0 || erro_locator != OFF)
	{
		strcpy(mens_rejeito,"Erro na Visão para deteção do Display. Erro de Localizador");
		res = -1;
		goto Error;
	}
	if(FERRAM_VER_PINS_DISP == FERR_EDGE)
	{
		res = Vision_Line_Edge(coord_roi_lcd_pins, referenciar, dados_cfg_vision.camera_shutter_pins_display, dados_cfg_vision.camera_gain_pins_display);
	}
	else
	{
		res = Vision_Quant_PinsDisp(coord_roi_lcd_pins, referenciar, dados_cfg_vision.camera_shutter_pins_display, dados_cfg_vision.camera_gain_pins_display);
	}
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
	{
		rej_vision = ON;
		if(strlen(MensagemFalha1) == 0)
			strcpy(MensagemFalha1,"Rejeitou teste de Visão para os pinos do LCD ");
	}
	if(rej_vision && tent < max_tent)
	{
		tent++;
		goto retest_vision;
	}
Error:	
return res;
}
/******************************** Medidas_Travas_Conector_J2901 ************************************************************************/
double Medidas_Travas_Conector_J2901(char * mens_rejeito)
{
int status	= 0, 
	sair	= 0,
	aprovado_montagem,
	erro_locator 	= OFF,
	nova_referencia = OFF,
	tent 			= 0,
	tolerancia_template = 700;
double 
	res = 1,
	res_esq=0,
	t;


	COR_MENSAGENS = MENS_AZUL;
	strcpy(mens_montagem_2,"Testando os conectores com vision");
	strcpy(mens_montagem_3,"");
	strcpy(mens_rejeito,"");
	aprovado_montagem	= OFF;
	
	/////////////////////////////////
	
retest_trava:
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	strcpy(mens_rejeito, "");
	
	res = Vision_Measure_Distance_Edges(imagem_trabalho, coord_line_esq_j5, LOCATOR_PAINEL, ON, "COORD_LINE_TRAVAESQ_J5=");
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
	{
		strcpy(mens_rejeito, "Falha no posicionamento da TRAVA ESQUERDA do J2901 - Bezel");
		if(res == -2)
			strcpy(mens_rejeito, "Falha no parametro pixel por mm");
		else if(res == -1)
			strcpy(mens_rejeito, "Falha no sistema de visão");
	}
	else
	{
		res_esq = res ;
		res = Vision_Measure_Distance_Edges(imagem_trabalho, coord_line_dir_j5, LOCATOR_PAINEL, OFF, "COORD_LINE_TRAVADIR_J5=");
		if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
			strcpy(mens_rejeito, "Falha no posicionamento da TRAVA DIREITA do J2901 - Bezel");
	}
	if ((res > Teste[teste].LimAlto) | (res < Teste[teste].LimBaixo))
	{
		if(tent > 2)
		{
			goto falha_vision;
		}
		else
		{
			tent++;
			goto retest_trava;
		}
	}
	ProcessDrawEvents ();
	return ((res + res_esq)/2);

	
falha_vision:

return RES_FALHA;
}
/*----------------------------------------------------------------------------------------------------------------
               Determina o localizador
-----------------------------------------------------------------------------------------------------------------*/
double Localizador_painel(char * mens_rejeito)
{
int
	status          = 0,
	max_tent 		= 2,
	erro_locator    = OFF,
	tent = 0;
double 
	res=0.00;

	/////////////////////////////////
	if(criar_negat_sample)
	{
		strcpy(mens_montagem_2, "CRIANDO NEGATIVE SAMPLE...");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == MASTER_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Master Sample");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE)
	{
		strcpy(mens_montagem_2, "Validando Amostra Negativa");
		strcpy(mens_montagem_3, "");
	}
	else if(amostra_tipo == TESTE)
	{
		strcpy(mens_montagem_2,   " Iniciando o sistema de visão - localizador");
		strcpy(mens_montagem_3,   " ");
	}
	strcpy(MensagemFalha1,"");
	strcpy(mens_rejeito,"");
	SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, ""); 
	
reteste:
	if(strcmp(id_arq_vision[modelo_atual_index], arq_vision_ant) != 0 || treinar_vision)
	{
		if(ConfigVision(&dados_cfg_vision, id_arq_vision[modelo_atual_index]) != 0)
		{
			strcpy(mens_rejeito, "Erro na leitura do arquivo de CFG do vision...");
			return 0;
		}
	}
	status = Vision_Locator_Main(&erro_locator, ON);
	if(status<0 || erro_locator != OFF)
	{
		strcpy(mens_rejeito,"Erro de Localizador do painel");
		res = -1;
	}
	else
	{
		res = 1;
	}
	if(reparam_rois == REPARAM_PAINEL_LOC)
			reparam_rois = NAO_REPARAMET;

Error:	
return res;
}
