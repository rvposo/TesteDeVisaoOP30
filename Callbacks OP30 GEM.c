//#include "NIIMAQdx.h"
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include "Main OP30 GEM.h"
#include "Auto OP30 GEM.h"
#include "Manual OP30 GEM.h"
#include "Vision Scripts OP30 Cluster GEM.h"
#include "Master_e_Negative_Samples.h"

//
int CVICALLBACK ModelosCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			ReadChoicedIndexFile ();
			break;
		}
	return 0;
}

// Função: ESC_BAR() 
//
int CVICALLBACK ESC_BAR (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(painel_logo) ;
			break;
	}
	return 0;
}

//
// Função: LeNSerie() 
//
int CVICALLBACK LeNSerie (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char
		aux[50];

	switch (event)
	{
		case EVENT_COMMIT:
			
			if( !sair_programa )
			{
				GetCtrlVal (panel, control, aux);
				if(strlen(aux) > 1)
					leu_serial_scanner_manual = ON; //Seta flag leu com o leitor manual
			}	
			break;
			
	}
	return 0;
}

//
// Função: Manual_Bar() 
//
void CVICALLBACK Manual_Bar (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	modo_manual = ON;	
	DisplayPanel (painel_manual);
	modo_thread=ESCRITA_E_LEITURA ;
	HidePanel (painel_auto);
}

//
// Função: Version_Bar() 
//
void CVICALLBACK Version_Bar (int menuBar, int menuItem, void *callbackData,
		int panel)
{
 	SetCtrlVal (painel_logo,LOGO_BAR_VERSAO , VERSAO); 
	InstallPopup( painel_logo ) ;
}

//
// Função: ZeraProducao() 
//
int CVICALLBACK ZeraProducao (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			Zera();
			SetActiveCtrl (painel_auto, AUTO_TXT_SERIE);
			break;
		}
	return 0;
}

//
// Função: RejeitaMontagem() 
//
int CVICALLBACK  RejeitaMontagem (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{

	switch (event){
		
		case EVENT_COMMIT:
			rejeita_montagem = ON;
			break;
	}
	return 0;
}

//
// Função: Quit_Bar() 
//
void CVICALLBACK Quit_Bar (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	sair_programa = ON;
	QuitUserInterface (EXIT_PROGRAM);

}

//
// Função: CallBack_TEXT() 
//
int CVICALLBACK CallBack_TEXT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	return 0;
}

//
// Função: ColetaDados_Bar() 
//
void CVICALLBACK ColetaDados_Bar (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	GetMenuBarAttribute( menuBar, MENUBAR_MENU2_ITEM1_2, ATTR_CHECKED, &coletando_dados ) ;
	coletando_dados ^= 1 ;
	SetMenuBarAttribute( menuBar, MENUBAR_MENU2_ITEM1_2, ATTR_CHECKED, coletando_dados ) ;
}


//
// Função: Manual_Fim() 
//
int CVICALLBACK Manual_Fim (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			HidePanel(painel_manual);
			DisplayPanel (painel_auto);
			SetCtrlAttribute (painel_auto, AUTO_TXT_SERIE , ATTR_DIMMED, 0);
			SetActiveCtrl (painel_auto, AUTO_TXT_SERIE);
			modo_manual=OFF;
			break;
		}
	return 0;
}

//
// Função: Manual_ThreadSistema() 
//
int CVICALLBACK Manual_ThreadSistema (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		valor;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &valor);
			switch(valor)
			{
				case 0:
					modo_thread=PARAR_ESC_LEIT;
					break;
				case 1:
					modo_thread=LEITURA_APENAS;
					break;
				case 2:
					modo_thread=ESCRITA_E_LEITURA;
					break;
				case 3:
					modo_thread=ESCRITA_APENAS;
					break;
			}
			break;
	}
	return 0;
}

////////////////////////////////////////////////
//       Acoes Segurança/Botoeiras
////////////////////////////////////////////////

//
// Função: Manual_HabitaEmergencia() 
//
int CVICALLBACK Manual_HabitaEmergencia (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.HabitaEmergencia = acao;
			break;
	}
	return 0;
}

//
// Função: Manual_ResetaEmergencia() 
//
int CVICALLBACK Manual_ResetaEmergencia (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.ResetaEmergencia = acao;
			break;
	}
	return 0;
}

//
// Função: Manual_ResetarEmergencia() 
//
int CVICALLBACK Manual_ResetarEmergencia (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
	
			ResetarEmergencia(OFF);
			break;
		}
	return 0;
}


//////////////////////////////////
//       Acoes Recursos
//////////////////////////////////

//
// Função: Manual_Sol_Descarte_Liner() 
//
int CVICALLBACK Manual_Sol_Descarte_Liner (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.Sol_Descarte_Liner = acao;
			break;
	}
	return 0;
}

//
// Função: Manual_Liga_Ionizador() 
//
int CVICALLBACK Manual_Liga_Ionizador (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.Liga_Ionizador = acao;
			break;
	}
	return 0;
}


//
// Função: Manual_Iluminacao_Vision_Superior() 
//
int CVICALLBACK Manual_Iluminacao_Vision_Superior (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.iluminacao_superior_max = acao;
			break;
	}
	return 0;
}


/////////////////////////////////////////////
//       Movimentos Manuais Atuadores
/////////////////////////////////////////////

//
// Função: Manual_TravaClamp() 
//
// Descrição: Executa o movimento manual da Trava do Clamp.
//
int CVICALLBACK Manual_TravaClamp (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao   = 0;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, control, &acao);
			MoveTravaClampProduto(acao);
	}
	return 0;
}
/******************* RejRemoto() **************************/
int CVICALLBACK RejRemoto (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
								   
	switch (event)
		{
		case EVENT_COMMIT:
			rej_remoto = ON;
			break;
		}
return 0; 
}
/*******************ContRemoto() **************************/
int CVICALLBACK ContRemoto (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
								   
	switch (event)
		{
		case EVENT_COMMIT:
			cont_remoto = ON;
			break;
		}
return 0; 
}
/*******************BimanualRemoto() **************************/
int CVICALLBACK BimanualRemoto (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
								   
	switch (event)
		{
		case EVENT_COMMIT:
			bimanual_remoto = ON;
			SetActiveCtrl (panel, AUTO_TXT_SERIE);
			break;
		}
return 0; 
}
//*********************************************************************************************************
int CVICALLBACK CriarNegatSample (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int acao;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			if(acao)
			{
				if(ConfirmPopup ("Confirmar criação de Amostra Negativa", 
					"Deseja realmente criar uma Amostra Negativa? Após a criação eventualmente a mesma poderá ser pedida pela máquina.") == 0)
				{
					SetCtrlVal (panel, control, 0);
					return 0;
				}
				criar_negat_sample = ON;
			}
			else
			{
				//if(!modulo_em_teste)
				//	Mensagem( "PRONTO PARA TESTES.","Coloque o cluster no fixture", "   ", "", "azul");
				criar_negat_sample = OFF;
			}
			break;
	}
	return 0;
}
//*********************************************************************************************************
int CVICALLBACK RedefinirPeriodicidade (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
				
			if(ConfirmPopup ("Escala para Negative Sample", 
				 "Realmente deseja redefinir a escala semanal para Negative Samples? ")!=1)
				return -1;
			if(RegistSeqNegSamp_Semanal() < 0)
			{
				MessagePopup ("Erro ao criar arquivo de periodicidade", 
					"Houve um erro ao tentar criar arquivo de periodicidade para Negative Sample semanal"); 
				return -1;
			}  
			MessagePopup ("Sucesso!!", 
				"Escala da programação semanal das Negative Samples redefinida"); 
			break;
	}
	return 0;
}

int CVICALLBACK CopiarTabelasRede (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
char 
	arq_tabs_rede[256], 
	cmd_str[300],
	tabela[200];
int
	atualizacao_tabs = -1;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//------------ Atualiza tabelas local a partir do driver de rede ----------------------
			strcpy(arq_tabs_rede,  PASTA_TAB_REDE);
			SetBreakOnLibraryErrors (0);
			Fmt(cmd_str,"%s<%s%s",arq_tabs_rede, "\\*.*"); 
			atualizacao_tabs = CopyFile (cmd_str, PASTA_TAB_LOCAIS);
			if(atualizacao_tabs == 0)
			{
				strcat(arq_tabs_rede,  TABELAS_REDE_VERSAO);
			//	Fmt(cmd_str,"%s<%s%s",arq_tabs_rede, "\\CFG\\*.*");
			//	Fmt(tabela, "%s<%s%s",PASTA_TAB_LOCAIS, "\\CFG");
			//	atualizacao_tabs = CopyFile (cmd_str, tabela);
				if(atualizacao_tabs == 0)
				{
					Fmt(cmd_str,"%s<%s%s",arq_tabs_rede, "\\TST\\*.*"); 
					Fmt(tabela, "%s<%s%s",PASTA_TAB_LOCAIS, "\\TST");
					atualizacao_tabs = CopyFile (cmd_str, tabela);
					if(atualizacao_tabs == 0)
						atualizacao_tabs = 1;
				}
			}
			SetBreakOnLibraryErrors (1);
			if(atualizacao_tabs != 1)
			{
				if(ConfirmPopup ("Falha na cópia de tabelas a partir da rede", 
					"Houve um falha na atualização das tabelas de testes e configurações a partir do servidor.\nNÃO é recomendável trabalhar com esta falha. Você deseja prosseguir?")==0)
				{
					return -1 ;		
				}
				atualizacao_tabs = 0;
			}

			break;
	}
	return 0;
}
//*********************************************************************************************************
int CVICALLBACK Reparam_ROIs(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int tipo_reparam,
	reparam_locator,
	status = 0;

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &tipo_reparam);
			switch(tipo_reparam)
			{
				case 0:
					reparam_rois = NAO_REPARAMET;
					break;
				case 1:
					break;
				case 2:
					reparam_rois = REPARAM_ROIS;
					break;
				case 3:
					reparam_rois = REPARAM_DISPLAY_ROIs;
					break;
				case 4:
					reparam_rois = REPARAM_SHIELD_ROIs;
					break;
				case 5:
					reparam_rois = REPARAM_DIFUSOR_ROIs;
					break;
				case 6:
					reparam_rois = REPARAM_LINER_ROIs;
					break;
				case 7:
					reparam_rois = REPARAM_PAINEL_LOC;
					break;
				default:
					reparam_rois = NAO_REPARAMET;
					break;
			}
			break;
	}
	return 0;
}

int CVICALLBACK Manual_sinalizador_verde (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.Sinaleiro_Verde = acao;

			break;
	}
	return 0;
}

int CVICALLBACK Manual_sinalizador_vermelho (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int
		acao;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.Sinaleiro_Vermelho = acao;

			break;
	}
	return 0;
}

int CVICALLBACK Manual_buzzer (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int
		acao;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &acao);
			SAIDA.Sinaleiro_Buzzer = acao;

			break;
	}
	return 0;
}
