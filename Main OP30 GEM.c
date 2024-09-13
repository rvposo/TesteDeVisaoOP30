/* GM OP30 Patch 0.4.5 - Detecção do Guia de Montagem */  

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
#include "Global Error Tracking.h"
 
//
// Função: main()
//
int main (int argc, char *argv[])
{

	int status, 
		another_inst;

	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */

	status = CheckForDuplicateAppInstance (DO_NOT_ACTIVATE_OTHER_INSTANCE, &another_inst);
	if( another_inst )
		return -1;

	resp_itostr = (char *)malloc(30);
	resp_str_mid = (char *)malloc(100);
	////////////// inicializa geral /////////////////////

	if(InicializaMaquina() < 0)
		return -1;

	////////////// cria pool de 2 threads(sistema, ciclo p/ carga 1 //////////////
	CmtNewThreadPool (2, &PoolHandle);

	////////////// inicia thread sistema //////////////
	CmtScheduleThreadPoolFunction (PoolHandle, ThreadSistema, (void*)0, &sistema_threadID);

	ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "");
	SetCtrlAttribute (painel_auto, AUTO_TXT_SERIE, ATTR_DIMMED, 0);
	
	////////////// loop principal //////////////
	LoopPrincipal();

	//////////////////// FIM ////////////////////////////
	return 0;
}

//
// Função: LoopPrincipal()
//
int LoopPrincipal(void)
{
	int
		status;
	static double
		tempo_nao_prioridade = 0;
	char
		mens_falha[100]= {0};

	/////////////////////////////////////////////////
	if( HomePosition(mens_falha) < 0 )
	{
		MessagePopup( "Erro Home Position", mens_falha );
		return -1;
	}

	ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM , "\0");
	SetCtrlAttribute (painel_auto, AUTO_TXT_SERIE, ATTR_DIMMED, 0);
	SetCtrlAttribute (painel_auto, AUTO_TXT_MAQUINA, ATTR_CTRL_VAL, stationID);

	SetActiveCtrl( painel_auto, AUTO_TXT_SERIE );

	////////////// inicia loop principal - eventos de UIR e relógio/////////////////
	SetSleepPolicy (MODO_SLEEP);
	do
	{
		ProcessSystemEvents ();
		if(Timer()-tempo_nao_prioridade > 0.5)
		{
			if(modo_manual)
			{
				Atualiza_Manual();
			}
			else
			{
				if(!status_emergencia)
				{
					if( HomePosition(mens_falha) < 0 )
					{
						MessagePopup( "Erro Home Position", mens_falha );
						return -1;
					}
				}
				if(TotalTestados_ant != TotalTestados)
					AtualizaFTT();

				Verifica_Barcodes();
				ProntoParaTeste();
				MensagensAuto();
			}
			tempo_nao_prioridade=Timer();
		}
		if( testar_produto  && !modulo_em_teste )
		{
			// Inicia thread ciclo de testes
			modulo_em_teste = ON;
			status=CmtScheduleThreadPoolFunction (PoolHandle, CicloAutomatico, (void*)0, &ciclo_threadID);
		}
	} while(!sair_programa);

	//// Finaliza loop principal ////
	Finaliza_Loop_Principal();
	return 0;
}


//
// Função: CicloAutomatico()
//
int CVICALLBACK CicloAutomatico (void *parametro)
{
	double
		res=FALHA;

	SetSleepPolicy (MODO_SLEEP);
	if(InicializaTestes() < 0)
		return -1;
	// -------------  Executa sequência dos testes -------------
	while (teste <= max_teste  && !aborta_testes && !sair_programa && !retornar_movimentos && res!=FALHA_GRAVE)
	{
		if(PresetaTeste() != RES_FALHA)
		{
			res = ExecutaTestes(pos_teste, "", "", modelo);
		}
		else
			res = RES_FALHA;

		StatusTeste(res);
	}
	FinalizaTestes(res);

	return 0;
}

//
// Função: Reconect_CIM()
//
int Reconect_CIM(void)
{
	int
		rescim;

	CIMSockets__SMFclose_sock (cimsockets, NULL, sockID, 2, NULL);
	Delay_thread(0.1);

	rescim = CIMSockets_New_SMF (NULL, 0, LOCALE_NEUTRAL, 0, &cimsockets);//"CIM Sockets - SMF","Criando Object Handle";
	if(rescim < 0 || cimsockets < 0)
	{
		return -1;
	}
	rescim = CIMSockets__SMFopen_sock (cimsockets, NULL, host, service, 3 , &sockID);//CIM Sockets - SMF", Opening Socket
	if (rescim < 0 || sockID < 0)
	{
		return -2;
	}
	rescim = CIMSockets__SMFsync_sock (cimsockets, NULL, sockID, stationID, 3 , &timestamp);
	if (rescim < 0)
	{
		return -3;
	}
	if (timestamp < 0)
	{
		return -4;
	}
	Delay_thread(0.1);

	return 0;
}
/***********************************************************************************************/
void Habilita_scanner_DM(void)
{
char 
	send_data[100];

	strcpy(send_data,"LON");
	strcat(send_data,"\r");
	ComWrt (comm_scanner, send_data, strlen(send_data));//habilita leitura de codigo de barras 
	scanner_habilitado_DM = ON;
	tempo_scanner_ligado_DM = Timer();
	FlushInQ(comm_scanner);
}

/***********************************************************************************************/
void Desliga_scanner_DM(void)
{
   
	char 
	send_data[100];

	strcpy(send_data,"LOFF");
	strcat(send_data,"\r");
	ComWrt (comm_scanner, send_data, strlen(send_data));//desabilita leitura de codigo de barras 
	scanner_habilitado_DM = OFF;
	FlushInQ(comm_scanner);
	
}
/******************************* LeProducaoHora() ***************************************************************/

//
// Função: CarregaTabela()
//
int CarregaTabela(void) //coloca valores da tab na estrut. Teste
{
	int
		i = 0,
		arquivo = 0;
	char
		lim_alto[15],
		lim_baixo[15],
		virgula[5],
		num_teste[25],
		reteste	[15],
		linha_da_tabela[100]="",
		versao[80];

	arquivo = OpenFile (id_tab_testes[modelo_atual_index], VAL_READ_ONLY, VAL_APPEND, VAL_ASCII);
	if(arquivo < 0)
	{
		MessagePopup ("Erro ao tentar carregar tabela de testes",
					  "Falha ao tentar ler arquivo de tabela de testes...");
		return -1;
	}
	ReadLine ( arquivo, versao , 70 ); //le 1° linha
	i=1;
	while (ReadLine(arquivo,linha_da_tabela , 95) > 0)//le 2° linha em diante
	{
		if(strlen(linha_da_tabela) <10)
			break;
		Scan(linha_da_tabela, "%s>%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]",num_teste , virgula,
		Teste[i].Nome, virgula, lim_baixo, virgula, lim_alto, virgula, Teste[i].Unidade, virgula, reteste);
		Teste[i].Numero 	= atoi(num_teste);
		Teste[i].LimBaixo 	= atof(lim_baixo);
		Teste[i].LimAlto  	= atof(lim_alto);
		Teste[i].Reteste 	= atoi(reteste);
		if (Teste[i].Numero>0)
			i++;	//elimina testes com simbolo negativo
	}
	max_teste = i-1;
	CloseFile (arquivo);
	if (arquivo>0)
		arquivo = SEM_ERRO;
	return arquivo;
}

//
// Função: ResetBarcode()
//
void ResetBarcode(int bcr_enditem_pronto)
{
char aux[100];

	if( bcr_enditem_pronto )
	{
		SetCtrlAttribute (painel_auto,AUTO_SERIAL_ENDITEM, ATTR_CTRL_VAL , N_Serie_pwb);
		strcpy(aux, id_tipo[modelo_atual_index]);
		SetCtrlAttribute (painel_auto, AUTO_DESCRICAO_MODELO, ATTR_CTRL_VAL, aux);
		SetCtrlAttribute (painel_auto, AUTO_TABELA, ATTR_CTRL_VAL, id_tab_testes[modelo_atual_index]);
		SetCtrlAttribute (painel_auto, AUTO_PN_VISTEON, ATTR_CTRL_VAL, id_pn_visteon[modelo_atual_index]);
		SetCtrlAttribute (painel_auto, AUTO_AMOSTRA, ATTR_CTRL_VAL, id_tipo_amostra[modelo_atual_index]);
		strcpy(aux, id_display[modelo_atual_index]);
		if( strcmp(id_display[modelo_atual_index], "M") == 0)
			strcat(aux, "-VERM");
		else
			strcat(aux, "-AMARE");
		SetCtrlAttribute (painel_auto, AUTO_DISPLAY, ATTR_CTRL_VAL, aux);
		bcr_resetado = ON;
		return;
	}
	SetCtrlVal (painel_auto, AUTO_DESCRICAO_MODELO, "");
	SetCtrlVal (painel_auto, AUTO_TABELA, "");
	SetCtrlVal (painel_auto, AUTO_PN_VISTEON, "");
	SetCtrlAttribute (painel_auto, AUTO_AMOSTRA, ATTR_CTRL_VAL, "");
	strcpy(N_Serie_pwb,"");
	SetCtrlAttribute (painel_auto,AUTO_SERIAL_ENDITEM, ATTR_CTRL_VAL, "");
	ResetTextBox (painel_auto, AUTO_TXT_NOME, "");
	ResetTextBox (painel_auto, AUTO_TXT_STATUS, "");
	ResetTextBox (painel_auto, AUTO_TXT_RESULTADO , "");
	ResetTextBox (painel_auto, AUTO_TXT_TESTE, "");
	ResetTextBox (painel_auto, AUTO_TXT_TEMPO, "");
	ResetTextBox (painel_auto, AUTO_LIM_INF, "");
	ResetTextBox (painel_auto, AUTO_LIM_SUP, "");
	ResetTextBox (painel_auto, AUTO_TXT_UNIDADE, "");
	barcode_Enditem_lido=OFF;
	rejeitado		= OFF;
	retestar		= OFF;
	testar_produto 	= OFF;
	bcr_resetado	= ON;
	// inserido
	tamanho_enditem_lido = 0 ;
	
}

//
// Função: SetEmDebug()
//
int SetEmDebug(void)
{
	char
		comando[50];
	int
		status;

	EnableBreakOnLibraryErrors ();
	CopyString(comando, 0, BARCODE_PADRAO_PWB, 0, TAMANHO_PREFIXO_PWB);
	strcpy(id_prefix_board[modelo_atual_index], comando);
	//status 			= DefModel(comando);
	//status 			= CarregaTabela(); //carrega tab de testes
	if(modo_manual)
		return 0;
	cim_conectado = OFF;

	return 0;
}

//
// Função: LeBarcode()
//
int LeBarcode(void)
{
	char
		prefix 	[12]={0},
		aux		[70]={0},
		aux2	[50]={0};
	int
		n 		= 0,
		i 		= 0,
		j 		= 0,
		status;
	char
		path_som[50] = {0};

	GetCtrlVal (painel_auto, AUTO_TXT_SERIE, aux);
	ResetTextBox (painel_auto, AUTO_TXT_SERIE, "");
	do
	{
		if(aux[i] != 32)//elimina espacos
		{
			aux2[j] = aux[i];
			j++;
			aux2[j+1]=0;
		}
		i++;
	}while(aux[i] != 0 && i < 20);
	leu_serial_scanner_manual = OFF; //reseta flag do leitor manual
	n = StringLength (aux2);
	StringUpperCase (aux2);
	if(strcmp(aux2, N_Serie_EndItem_anterior) == 0)
	{
		ResetTextBox (painel_auto, AUTO_TXT_SERIE, "DUPLICADO");
		return 2;
	}
	if (strcmp (aux2, "CIMON")==0)//conecta o cim
	{
		SetPanelAttribute (painel_auto, ATTR_BACKCOLOR,0x0000CCCC ); //Seta a cor do background para o padrão 
		cim_conectado = 1;
		SetCtrlVal (painel_auto, AUTO_TXT_SERIE, "\0");
		MENS_WARNING = MENS_CIM_ON;
		SetCtrlVal (painel_auto, AUTO_LED_CIM, ON);
		SetCtrlAttribute (painel_auto, AUTO_LED_CIM, ATTR_LABEL_TEXT, "CIM Conectado");
		Delay_thread(0.01);
		return 2; 
	}
	else if (strcmp (aux2, "CIMOFF")==0)//desconecta o cim
	{
		SetPanelAttribute (painel_auto, ATTR_BACKCOLOR,0x00FF0000 ); //Seta a cor do background para vermelho
		cim_conectado = 0;
		SetCtrlVal (painel_auto, AUTO_TXT_SERIE, "\0");
		MENS_WARNING = MENS_CIM_OFF;
		SetCtrlVal (painel_auto, AUTO_LED_CIM, OFF);
		SetCtrlAttribute (painel_auto, AUTO_LED_CIM, ATTR_LABEL_TEXT, "CIM Desconectado");
		Delay_thread(0.01);
		return 2;
	}
	else if (strcmp (aux2, "FIM")==0)//finaliza o programa
	{
		sair_programa=ON;
		return 3;
	}
	n = StringLength (aux2);
	if( n != TAMANHO_BARCODE_PWB)
		return n;
	
	///////////////////// EndItem /////////////////////////////////
	if((n == TAMANHO_BARCODE_PWB || (strstr(aux2, PADRAO_PREFIX_PWB) != NULL)))
	{
		if(n == TAMANHO_BARCODE_PWB)
			CopyString(prefix, 0, aux2, 0, TAMANHO_PREFIXO_PWB);
		status = SolicitaAtualizarTabela();
		if(strcmp(id_prefix_board[modelo_atual_index], prefix) != 0 || !cim_conectado || status == 0 || max_teste < 2)
		{
			
			strcpy(id_prefix_board[modelo_atual_index], prefix);
			status = DefModel(prefix);
			if(status != SEM_ERRO)
			{
				if(status == FALHA_MODELO_NAO_ENCONTRADO)
					MENS_WARNING = MENS_MODEL_CADAST_ERROR;
				else
					MENS_WARNING = MENS_MODEL_FILE_ERROR;
				SetActiveCtrl (painel_auto, AUTO_TXT_SERIE);
				Delay_thread(0.01);
				return 2 ;//erro nas cargas de tabela   
			}
			if(strcmp(id_prefix_board[modelo_atual_index], id_prefix_board[modelo_atual_index]) != 0)
			{
				MessagePopup ("Erro na seleção de modelos",
							  "Placa PWB não casa com modelo atual selecionado...");
				return -1;
			}
			status = CarregaTabela(); //carrega tab de testes
			if(status != SEM_ERRO)//erro na carga de tabela
			{
				MENS_WARNING = MENS_TEST_TAB_ERROR;
				Delay_thread(0.01);
				return 2 ;//erro nas cargas de tabela   
			}
			barcode_Enditem_lido = ON;  
		}
		else
		{
			barcode_Enditem_lido = ON;  
		}
		strcpy(N_Serie_pwb, aux2);
		strcpy(MensagemFalha1,"");
		tempo_bcr_enditem=Timer();
	}
	//aciona o beep para indicar leitura do barcode
	if(barcode_Enditem_lido && !modulo_em_teste)
	{
		strcpy(path_som,CAMINHO_SONS);
		strcat(path_som,"beep-07.wav");
		CVI_PlaySound (path_som, ON );
		Delay_thread(1.00);
		CVI_StopCurrentSound ();
	}		
	
	return n;
}

//
// Função: Zera()
//
void Zera(void)
{
	int
		file;
	char
		tmp[90];

	TotalRejeitados = 0;
	TotalAprovados = 0;
	TotalTestados = 0;
	SetCtrlVal (painel_auto, AUTO_TXT_TESTADOS, "0");
	SetCtrlVal (painel_auto, AUTO_TXT_APROVADOS, "0");
	SetCtrlVal (painel_auto, AUTO_TXT_REJEITADOS, "0");
	SetCtrlVal (painel_auto, AUTO_TXT_FTT, "0.0");
	//grava as estatisticas em arquivo
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
}

//
// Função: SolicitaAtualizarTabela()
//
// Descrição: verifica se o arquivo de solicitação de atualização de tabela esta presente
//
int SolicitaAtualizarTabela(void)
{
	int
		status = -1,
		arquivo = 0,
		tamanho = 0;
	char
		arq_nome[40] = {0};

	Fmt(arq_nome, "%s<%s%s%s", PASTA_REDE_CTR, stationID, ".txt");
	arquivo = GetFileInfo (arq_nome, &tamanho);
	if(arquivo)
		status = remove (arq_nome);

	return status;
}

//
// Função: ColetaDados()
//
void ColetaDados(int Max_Test)
{
	long
		Size;
	int
		Arquivo_Coleta,
		cont,
		Mes,
		Dia,
		Ano,
		status;
	char
		*Data,
		*Hora,
		dia[4]="",
		mes[4]="",
		ano[4]="",
		*Buf;

	Data = DateStr();
	Hora = TimeStr();
	CopyString (dia, 0, Data, 3, 2);
	CopyString (mes, 0, Data, 0, 2);
	CopyString (ano, 0, Data, 8, 2);
	Buf = (char *)malloc(20000);
	GetSystemDate ( &Mes, &Dia, &Ano);// faz o nome do arquivo
	if(status_produto == PRODUTO_APROVADO)
		sprintf(Buf,"%sHist_%d-%d-%d.txt",CAMINHO_APROVADOS, Dia,Mes,Ano);
	else
		sprintf(Buf,"%sHist_%d-%d-%d.txt",CAMINHO_REJEITOS, Dia,Mes,Ano);
	status = GetFileInfo(Buf , &Size);
	if (status == 1)
		Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_APPEND , VAL_ASCII );
	else
		Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );
	if(Arquivo_Coleta < 0)
	{
		MessagePopup ("Erro ao tentar coletar dados de Histórico",
					  "Falha ao tentar abrir arquivo de coleta de Histórico");
	}
	strcpy(Buf,"-----------------------------------------------------------------------------");
	status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
	if(status_produto == PRODUTO_APROVADO)
	{
		Fmt( Buf , "%s<%s%s", N_Serie_pwb, "  - APROVADO");
	}
	else
	{
		Fmt( Buf , "%s<%s%s", N_Serie_pwb, "  - REPROVADO");
	}
	status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
	for (cont = 1; cont <= Max_Test; cont++)
	{
		strcpy(Buf,Teste[cont].Nome);
		Fmt(Buf , "%s<%s%s" , Buf ,",");
		if(Teste[cont].LimBaixo <= 999)
			Fmt(Buf , "%s<%s%f[p2]\t" , Buf , Teste[cont].LimBaixo);
		else
			Fmt(Buf , "%s<%s%f[p1]\t" , Buf , Teste[cont].LimBaixo);
		Fmt(Buf , "%s<%s%s" , Buf ,",");
		if(Teste[cont].Resultado <= 999)
			Fmt(Buf , "%s<%s%f[p2]\t" , Buf , Teste[cont].Resultado);
		else
			Fmt(Buf , "%s<%s%f[p1]\t" , Buf , Teste[cont].Resultado);
		Fmt(Buf , "%s<%s%s" , Buf ,",");
		if(Teste[cont].LimAlto <= 999)
			Fmt(Buf , "%s<%s%f[p2]\t" , Buf , Teste[cont].LimAlto);
		else
			Fmt(Buf , "%s<%s%f[p1]\t" , Buf , Teste[cont].LimAlto);
		Fmt(Buf , "%s<%s%s" , Buf ,",");
		Fmt(Buf , "%s<%s%s" , Buf , Teste[cont].Unidade);
		Fmt(Buf , "%s<%s%s" , Buf ,",");
		Fmt(Buf , "%s<%s%s" , Buf , Teste[cont].Condicao);
		Fmt(Buf , "%s<%s%s\t" , Buf ,",");
		Fmt(Buf , "%s<%s%f[p3]" , Buf , Teste[cont].Tempo_teste);
		Fmt(Buf , "%s<%s%s" , Buf ," s");
		status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
	}
	Fmt(Buf , "%s<%s%s" , "Versão SW de teste = ",VERSAO);
	status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
	Fmt(Buf , "%s<%s%f[p2]" , "Tempo de teste = ",tempo_total);
	status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
	Data_e_Hora (Data, Dtimer, DmesI);
	Fmt( Buf, "%s<%s%s - %s" , "Data = ", Data, Dtimer);
	status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
	status = CloseFile (Arquivo_Coleta);
	if(coletando_dados == CPK || coletando_dados == HISTOR_CPK)//
	{
		GetSystemDate ( &Mes, &Dia, &Ano);// faz o nome do arquivo
		if(status_produto == PRODUTO_APROVADO)
			sprintf(Buf,"%sCPK_%d-%d-%d.txt",CAMINHO_CPK_APROV, Dia,Mes,Ano);
		else
			sprintf(Buf,"%sCPK_%d-%d-%d.txt",CAMINHO_REJEITOS, Dia,Mes,Ano);
		status = GetFileInfo(Buf , &Size);
		if (status == 1)
			Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_APPEND , VAL_ASCII );
		else
			Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );
		if(Arquivo_Coleta < 0)
		{
			MessagePopup ("Erro ao tentar coletar dados de CPK",
						  "Falha ao tentar abrir arquivo de coleta de CPK");
		}
		if(!status)//1º vez gravando o arquivo - cria cabeçalho
		{
			Fmt(Buf , "%s<%s%s" , "Versão SW de teste = ",VERSAO);
			status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));


			strcpy(Buf,"Serial ,");
			for (cont = 1; cont <= Max_Test; cont++)
			{
				//RemoveSurroundingWhiteSpace(teste.nome[i]);
				Fmt(Buf , "%s<%s%s%s" , Buf ,Teste[cont].Nome, ",");

			}
			status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));

			strcpy(Buf,"Lim Sup ,");
			for (cont = 1; cont <= Max_Test; cont++)
			{
				Fmt(Buf , "%s<%s%f[p2]%s" , Buf ,Teste[cont].LimAlto, ",");
			}
			status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));

			strcpy(Buf,"Lim Inf ,");
			for (cont = 1; cont <= Max_Test; cont++)
			{
				Fmt(Buf , "%s<%s%f[p2]%s" , Buf ,Teste[cont].LimBaixo,",");
			}
			status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));

		}
		strcpy(Buf,N_Serie_pwb);
		strcat(Buf, ",");
		for (cont = 1; cont <= Max_Test; cont++)
		{
			Fmt(Buf , "%s<%s%f[p2]%s" , Buf , Teste[cont].Resultado,",");
		}
		status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
		status = CloseFile (Arquivo_Coleta);
	}
	free (Buf);
}

//
// Função: MoveOut_Build_Cim()
//
void MoveOut_Build_Cim(char *prefixo, double Res, int Rej, int Test)
{
	int
		tent;
	char
		unit_result[3],
		group_id[6],
		version[6]="",
		sTest[6]="";
	short
	timeout=3;

	Fmt(sTest,"%s<%i",Test);
	strcpy(version  , "V2.00");
	strcpy(group_id, prefixo);
	if(Rej == 1)
		strcpy(unit_result, "FA");
	else
		strcpy(unit_result, "PA");

	tent = 0;
falha_conex1:
	rescim = CIMSockets__SMFmoveout (cimsockets, NULL, sockID, timeout, stationID, N_Serie_pwb, unit_result, &resultcim);
	if (resultcim < 0 || rescim <0)
	{
		if(tent == 0)
		{
			tent 	= 1;
			if(Reconect_CIM () == 0)
				goto falha_conex1;
		}
	}
	if(Rej)
	{
		rescim = CIMSockets__SMFenvtest (cimsockets, NULL, sockID, timeout,
										 stationID, N_Serie_pwb, group_id, version, sTest, unit_result, Res, 1, &resultcim);
	}
	else
	{
		strcpy(N_Serie_EndItem_anterior, N_Serie_pwb);
	}

	return;
}

//
// Função: AtualizaFTT()
//
void AtualizaFTT(void)
{
	char
		tmp[90];

	//atualiza estatistica
	Fmt(tmp , "%s<%f[p0]" , TotalTestados);
	SetCtrlAttribute (painel_auto, AUTO_TXT_TESTADOS, ATTR_CTRL_VAL,tmp);
	Fmt(tmp , "%s<%f[p0]" , TotalRejeitados);
	SetCtrlAttribute (painel_auto, AUTO_TXT_REJEITADOS, ATTR_CTRL_VAL,tmp);
	Fmt(tmp , "%s<%f[p0]" , TotalAprovados);
	SetCtrlAttribute (painel_auto, AUTO_TXT_APROVADOS, ATTR_CTRL_VAL,tmp);
	if(TotalTestados == 0)//evitar divisão por zero
		ftt = 0;
	else
		ftt = (TotalAprovados / TotalTestados) * 100.00;
	Fmt(tmp , "%s<%f[p1]%s" , ftt , "%");
	SetCtrlAttribute (painel_auto, AUTO_TXT_FTT, ATTR_CTRL_VAL,tmp);
	//SetCtrlAttribute (painel_auto,AUTO_COLETA ,ATTR_CTRL_VAL ,coletando_dados);
	TotalTestados_ant = TotalTestados;
}

//
// Função: PresetaTeste()
//
int PresetaTeste(void)
{
	char
		tmp[90];

	status_produto 		= PRODUTO_TESTANDO;
	tempo_inicial_teste = Timer();
	rejeitado 			= OFF;
	rejeita_montagem 	= OFF;
	pos_teste			= Teste[teste].Numero; //pega eqval. a postest,vetor começa com zero(deve-se subtrair 1)
	strcpy(mens_montagem_2,"");
	strcpy(mens_montagem_3,"");
	strcpy(mens_montagem_falha,"");
	strcpy(MensagemFalha1,"");
	Fmt(tmp , "%s[w3]<%d" , teste);
	InsertTextBoxLine (painel_auto, AUTO_TXT_TESTE, 0, tmp);
	InsertTextBoxLine (painel_auto, AUTO_TXT_NOME, 0, Teste[teste].Nome);
	InsertTextBoxLine (painel_auto, AUTO_TXT_STATUS, 0, "...");
	InsertTextBoxLine (painel_auto,AUTO_TXT_RESULTADO , 0, "testando..");
	Fmt(tmp, "%s<%f[p2]", Teste[teste].LimAlto);
	InsertTextBoxLine (painel_auto, AUTO_LIM_SUP, 0, tmp);
	Fmt(tmp, "%s<%f[p2]", Teste[teste].LimBaixo);
	InsertTextBoxLine (painel_auto, AUTO_LIM_INF, 0, tmp);
	InsertTextBoxLine (painel_auto, AUTO_TXT_UNIDADE, 0, Teste[teste].Unidade);
	InsertTextBoxLine (painel_auto, AUTO_TXT_TEMPO, 0, "...");

	if (!status_emergencia)
		return RES_FALHA;

	return 0;
}

