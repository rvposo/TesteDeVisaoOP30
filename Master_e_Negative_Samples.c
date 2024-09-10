#include <userint.h>
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include "Periodicidade.h"
#include "Master_e_Negative_Samples.h"
#include "Main OP30 GEM.h"
#include "Auto OP30 GEM.h"


////////////// Função VerFreqSamples //////////////////
int	VerFreqSamples(void)
{
long Size;
int  
	cont, 
	mes, 
	dia, 
	ano, 
	hour;
char 
	*Hora, 
	Nome[20]="" ; 
	
		strcpy(Data, DateStr());
		Data_e_Hora ( Data, Dtimer, DmesI ) ;
		//verifica durante 50 minutos a mudança de turno e se necessário zera samples 
		if(freq_solicit_samples == TURNO || freq_solicit_samples == TURNO_RESET)
		{
			if((horas == HORA_TURNO_1  && minutos <= 59) || 
				(horas == HORA_TURNO_2 && minutos <= 59) || 
				(horas == HORA_TURNO_3 && minutos <= 59)) 
			{
				if(verificado_freq_sample == OFF)
				{
					verificado_freq_sample = ON;
					return Deleta_Status_das_Samples();
				}
			}
			else
			{
				verificado_freq_sample = OFF;
			}
		}
		else //freq_solicit_samples == DIARIA
		{
				if(horas == HORA_TURNO_1  && minutos <= 59) 
				{
					if(verificado_freq_sample == OFF)
					{
						verificado_freq_sample = ON;
						return Deleta_Status_das_Samples();
					}
				}
				else
				{
					verificado_freq_sample = OFF;
				}
		}
return 0;
}

////////////// Função Verifica_Status_das_Samples //////////////////
int	Verifica_Status_das_Samples(void)
{
char  nome_arquivo[200], 
	  sample_status[2000];
int   file_handle,
 	  i,
	  sair,
	  index;
long 	Size;

	///////////////  VERIFICA STATUS DO MASTER SAMPLE ///////////////////////
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, "Samples_");
	strcat(nome_arquivo, DateStr());

	SetBreakOnLibraryErrors (0);
	file_handle= OpenFile (nome_arquivo, VAL_READ_ONLY,VAL_TRUNCATE,VAL_ASCII);
	SetBreakOnLibraryErrors (1); 
	
	master_sample_ok = OFF;
	negativ_sample_ok= OFF;
	if (file_handle != -1)
	{
		ReadFile (file_handle, sample_status, 990);
		if (strstr (sample_status, STR_MAST_SAMP_OK) != NULL)
			master_sample_ok = ON;
		if (strstr (sample_status, STR_NEG_SAMP_OK) != NULL)
			negativ_sample_ok = ON;
		
		CloseFile (file_handle); 
	}
	SetCtrlVal (painel_auto, AUTO_LED_MSAMPLE, master_sample_ok);
	SetCtrlVal (painel_auto, AUTO_LED_NEGSAMPLE, negativ_sample_ok);
	
	if(!negativ_sample_ok)
	{
		index_neg_samp_testando = 0;
		if(Le_NegSamp_a_testar()<0)
			return -1;
	}
	return 0;
}


////////////// Função Deleta_Status_das_Samples //////////////////
int	Deleta_Status_das_Samples(void)
{
char  nome_arquivo[200], 
	  sample_status[2000];
int   file_handle,
 	  i,
	  sair,
	  index;
long 	Size;

	///////////////  DELETA STATUS DO MASTER SAMPLE ///////////////////////
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, "Samples_");
	strcat(nome_arquivo, DateStr());
	SetBreakOnLibraryErrors (0);
	DeleteFile (nome_arquivo);
	SetBreakOnLibraryErrors (1);
	Delay(0.3);
	return Verifica_Status_das_Samples();
}
//////////////////////////////////////////////  FUNÇÃO Grava_Hist_e_Status_das_Samples //////////////////////////////////////////
void Grava_Hist_e_Status_das_Samples(int Max_Test)
{
	long 	Size;
    int  	Arquivo_Coleta , 
			cont, 
			Mes, 
			Dia, 
			Ano, 
			status;
    char 	Nome[100]="" , 
			dia[4]="", 
			mes[4]="", 
			ano[4]="" , 
			Buf[5000], 
			Diretorio[45];
	static char	modelo_ant[20]="xxx";
   
		//----------------------------------------------------------------------------------------		
		strcpy(Buf, CAMINHO_SAMPLE);
		strcat(Buf, "Samples_");
		strcat(Buf, DateStr());
		status = GetFileInfo(Buf , &Size);
		if (status == 1)
		  Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_APPEND , VAL_ASCII );
		else   
		  Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );
		
		if(amostra_tipo == MASTER_SAMPLE && status_produto == PRODUTO_APROVADO)
		{
			status = WriteLine(Arquivo_Coleta , STR_MAST_SAMP_OK , StringLength(STR_MAST_SAMP_OK));
			master_sample_ok = ON;
			
		}
		else if(amostra_tipo == NEGATIVE_SAMPLE)
		{
			if(!neg_samp_rejeitou)
			{
				MessagePopup ("Erro ao testar Negative Sample", "Não foi detectado falhas na Negative Sample");
			}
			if(!criar_negat_sample && neg_samp_rejeitou)
			{
				if(VerConsistNegatSample() == SEM_ERRO)
				{
					index_neg_samp_testando++;
					if(neg_samples_a_ser_test[index_neg_samp_testando] == -1 && !negativ_sample_ok)
					{
						if(LerDiaAtual(0,1) == SEM_ERRO) //le o dia atual - parametros = (0== Não carrega NegSamp a testar, 1==Indexar dia atual)
						{
								status = WriteLine(Arquivo_Coleta , STR_NEG_SAMP_OK , StringLength(STR_NEG_SAMP_OK));               
								negativ_sample_ok = ON;
						}
					}
				}
			}
		}
		CloseFile (Arquivo_Coleta) ;
		//----------------------------------------------------------------------------------------		
		GetSystemDate ( &Mes, &Dia, &Ano);// faz o nome do arquivo 
		if(status_produto == PRODUTO_APROVADO)
			sprintf(Buf,"%sHist_%d-%d-%d.txt",CAMINHO_SAMPLE, Dia,Mes,Ano);
		else
			sprintf(Buf,"%sHist_%d-%d-%d.txt",CAMINHO_SAMPLE, Dia,Mes,Ano);
		status = GetFileInfo(Buf , &Size);
		if (status == 1)
		  Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_APPEND , VAL_ASCII );
		else   
		  Arquivo_Coleta  = OpenFile(Buf , VAL_WRITE_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );

		if(amostra_tipo == MASTER_SAMPLE)
			strcpy(Buf,"-- Master Sample --");
		else
			strcpy(Buf,"-- Negative Sample --");
		status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
		if(status_produto == PRODUTO_APROVADO)
		{
			Fmt( Buf , "%s<%s%s%s%s Id: %s", "Serial = ", N_Serie_pwb, "  - APROVADO  -  Operador(a): ", nomeCADASTRO, sprontuario ) ;            
		}
		else
		{
			Fmt( Buf , "%s<%s%s%s%s Id: %s", "Serial = ", N_Serie_pwb, "  - REPROVADO  -  Operador(a): ", nomeCADASTRO, sprontuario ) ;            
		}
		status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
		
		strcpy(Buf,STR_COMENT);                                           
		status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
		for (cont = 1 ; cont <= Max_Test ; cont++)
		{ 
			strcpy(Buf,Teste[cont].Nome);
	   	    Fmt(Buf , "%s<%s%s\t" , Buf ,",");
			if(Teste[cont].LimBaixo <= 9999)
	   	    	Fmt(Buf , "%s<%s%f[p2]\t" , Buf , Teste[cont].LimBaixo);	 
			else
	   	    	Fmt(Buf , "%s<%s%f[p1]\t" , Buf , Teste[cont].LimBaixo);	 
	   	    Fmt(Buf , "%s<%s%s" , Buf ,",");
			if(Teste[cont].Resultado <= 9999)
   	    		Fmt(Buf , "%s<%s%f[p2]\t" , Buf , Teste[cont].Resultado);
			else
   	    		Fmt(Buf , "%s<%s%f[p1]\t" , Buf , Teste[cont].Resultado);
			Fmt(Buf , "%s<%s%s" , Buf ,",");
			if(Teste[cont].LimAlto <= 9999)
	   	    	Fmt(Buf , "%s<%s%f[p2]\t" , Buf , Teste[cont].LimAlto);	 
			else
	   	    	Fmt(Buf , "%s<%s%f[p1]\t" , Buf , Teste[cont].LimAlto);	 
	   	    Fmt(Buf , "%s<%s%s" , Buf ,",");
	   	    Fmt(Buf , "%s<%s%s" , Buf , Teste[cont].Unidade);	 
	   	    Fmt(Buf , "%s<%s%s\t" , Buf ,",");
   	    	Fmt(Buf , "%s<%s%s" , Buf , Teste[cont].Condicao);
			Fmt(Buf , "%s<%s%s\t" , Buf ,",");
	   	    Fmt(Buf , "%s<%s%f[p3]" , Buf , Teste[cont].Tempo_teste);
	   	    Fmt(Buf , "%s<%s%s" , Buf ," s");
	        status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
   	    }
        Fmt(Buf , "%s<%s%s" , "Cluster SW level = ",cluster_sw_version);
        status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
        Fmt(Buf , "%s<%s%s" , "Versão SW de teste = ",VERSAO);
        status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
        Fmt(Buf , "%s<%s%f[p2]" , "Tempo de teste = ",tempo_total);
        status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf));
        Data_e_Hora (Data, Dtimer, DmesI);
        Fmt( Buf, "%s<%s%s - %s" , "Data = ", DateStr(), TimeStr ());
        status = WriteLine(Arquivo_Coleta , Buf , StringLength(Buf)); 
		status = CloseFile (Arquivo_Coleta);           
}

//////////////////////////////////////////////  FUNÇÃO RegistraNegatSample //////////////////////////////////////////
int RegistraNegatSample(void)
{
char  	nome_arquivo[200],
		nome_neg_sample[MAX_LEN_NSAMPLE+1],
		Buf[2000];
int   	file_handle,
		status,
		sair,
		neg_sample_present;
long 	Size;

	if(!neg_samp_rejeitou)
	{
		MessagePopup ("Erro ao criar Negative Sample", 
			"Não pôde criar Negative Sample. Veja log de dados e observe se:\n 1-Negative Sample Aprovou \n 2-Negative Sample não realizou testes obrigatórios para o modelo"); 
		return -1;
	}
	tent_nome:
	status = PromptPopup ("Nome da Negative Sample", 
		"Defina um nome para esta Negative Sample", nome_neg_sample, MAX_LEN_NSAMPLE);
	if(status != 0)
	{
		if(ConfirmPopup ("Nome para Negative Sample", 
			 "Necessario definir um nome para Negative Sample. \n Deseja voltar e tentar novamente? ")==1)
			goto tent_nome;;
		
		return -1;
	}
	if(strlen(nome_neg_sample) < 1 || strlen(nome_neg_sample)>MAX_LEN_NSAMPLE)
	{
		MessagePopup ("Erro no nome da Negative Sample", 
			"Nome inválido para a Negative Sample"); 
		goto tent_nome;
	
	}
	periodicidade = 0;
	//panel_periodo = LoadPanel (painel_auto, "Periodicidade.uir", PERIO_NSAM); 
	//if(panel_periodo < 0)
	//{
	//	MessagePopup ("Erro ao tentar definir periodo para a negative sample", 
	//		"Painel inválido para periodo da Negative Sample"); 
	//	return -1;
	//}
	//DisplayPanel (panel_periodo);
	
	//while(periodicidade==0) // 1==Semanal e 2=Diaria
	//	Delay_thread(0.1);
	//DiscardPanel (panel_periodo);
	status = PromptPopup ("Defina periodicidade", 
		"Defina um periodo para esta Negative Sample - 'S' para Semanal ou 'D' para diaria", Buf, 1);
	StringUpperCase (Buf);
	if(Buf[0] != 'S' && Buf[0] != 'D')
	{
		MessagePopup ("Erro ao definir periodicidade", 
			"Erro ao tentar definir periodo para a negative sample"); 
		goto tent_nome;
	
	}
	if(Buf[0] == 'S')
		periodicidade = SEMANAL;
	else
		periodicidade = DIARIO;
	///////////////  VERIFICA arquivo de Negative Samples ///////////////////////
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, ARQ_N_SAMPL_REGIST);
	status = GetFileInfo(nome_arquivo , &Size);
	if (status == 1)
	  file_handle  = OpenFile(nome_arquivo , VAL_WRITE_ONLY , VAL_APPEND , VAL_ASCII );
	else   
	  file_handle  = OpenFile(nome_arquivo , VAL_WRITE_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );
	if(file_handle < 0)
	{
		MessagePopup ("Erro ao criar Negative Sample", 
			"Não pôde registrar Negative Sample. Houve um erro abrir arquivo de registro de Negative Samples"); 
		CloseFile (file_handle);           
		return -1;
	}
	if (status == 1)
	{
		strcpy(Buf,STR_COMENT);
		status = WriteLine(file_handle , Buf , StringLength(Buf));
	}
	else
	{
		strcpy(Buf,"////////// ARQUIVO DE REGISTRO DAS NEGATIVE SAMPLES //////////");
	    status = WriteLine(file_handle , Buf , StringLength(Buf));
		strcpy(Buf,"/////////// ATENÇÃO!!!! NÃO MODIFIQUE ESTE ARQUIVO ///////////");
	    status = WriteLine(file_handle , Buf , StringLength(Buf));
		strcpy(Buf,STR_COMENT);
		status = WriteLine(file_handle , Buf , StringLength(Buf));
	}
	
	////////// registra Serial Number ///////////////////
	strcpy(Buf,STR_SERIAL_NEG_SAMPLE);
	strcat(Buf,N_Serie_pwb);
	strcat(Buf,",");
	strcat(Buf,nome_neg_sample);
	strcat(Buf,",");
	if(periodicidade == SEMANAL)
		strcat(Buf,STR_NEG_SAMPLE_SEMANA);
	else
		strcat(Buf,STR_NEG_SAMPLE_DIARIA);
	status = WriteLine(file_handle , Buf , StringLength(Buf));
	////////// registra Numero dos Testes Executados ////
	strcpy(Buf,"Num Teste ,");
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(cont == (max_teste))
			Fmt(Buf , "%s<%s%i" , Buf ,Teste[cont].Numero);
		else
			Fmt(Buf , "%s<%s%i%s" , Buf ,Teste[cont].Numero, ",");
	}
	status = WriteLine(file_handle , Buf , StringLength(Buf));
	////////// registra Status /////////////////////////
	strcpy(Buf, "Status ,");
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(cont == (max_teste))
			Fmt(Buf , "%s<%s%s" , Buf , Teste[cont].Condicao);
   	    else
			Fmt(Buf , "%s<%s%s%s" , Buf , Teste[cont].Condicao,",");
    }
    status = WriteLine(file_handle , Buf , StringLength(Buf));
	
	////////// registra Resultados /////////////////////
	strcpy(Buf, "Resultados ,");
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(cont == (max_teste))
			Fmt(Buf , "%s<%s%f[p2]" , Buf , Teste[cont].Resultado);
   	    else
			Fmt(Buf , "%s<%s%f[p2]%s" , Buf , Teste[cont].Resultado,",");
    }
    status = WriteLine(file_handle , Buf , StringLength(Buf));
	
	////////// registra Lim Inf ////////////////////////
	strcpy(Buf,"Lim Inf ,");
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(cont == (max_teste))
			Fmt(Buf , "%s<%s%f[p2]" , Buf ,Teste[cont].LimBaixo);
		else
			Fmt(Buf , "%s<%s%f[p2]%s" , Buf ,Teste[cont].LimBaixo,",");
    }
	status = WriteLine(file_handle , Buf , StringLength(Buf));
	
	////////// registra Lim Sup ////////////////////////
	strcpy(Buf,"Lim Sup ,");
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(cont == (max_teste))
   	    	Fmt(Buf , "%s<%s%f[p2]" , Buf ,Teste[cont].LimAlto);
		else
   	    	Fmt(Buf , "%s<%s%f[p2]%s" , Buf ,Teste[cont].LimAlto, ",");
    }
    status = WriteLine(file_handle , Buf , StringLength(Buf));
	
    Fmt(Buf , "%s<%s%f[p2]" , "Tempo de teste = ",tempo_total);
    status = WriteLine(file_handle , Buf , StringLength(Buf));
    Fmt(Buf , "%s<%s%s" , "Cluster SW level = ",cluster_sw_version);
    status = WriteLine(file_handle , Buf , StringLength(Buf));

	Fmt(Buf , "%s<%s%s" , "Versão SW de teste = ",VERSAO);
	status = WriteLine(file_handle , Buf , StringLength(Buf));
    Data_e_Hora (Data, Dtimer, DmesI);
    Fmt( Buf, "%s<%s%s - %s" , "Data = ", DateStr(), TimeStr ());
    status = WriteLine(file_handle , Buf , StringLength(Buf)); 
				
	status = CloseFile (file_handle);           
	if(status == -1)
	{
		MessagePopup ("Erro no registro do Negative Sample", 
			"Houve um erro ao fechar arquivo de registro de Negative Samples"); 
		return -1;
	}
	
	if(RegistSeqNegSamp_Semanal() < 0)
	{
		MessagePopup ("Erro ao criar arquivo de periodicidade", 
			"Houve um erro ao tentar criar arquivo de periodicidade para Negative Sample semanal"); 
		return -1;
	}
	
	return 0;
}

//////////////////////////////////////////////  FUNÇÃO AutorizaCriarNegatSample //////////////////////////////////////////
int AutorizaCriarNegatSample(void)
{
char  	nome_arquivo[200], 
		sample_status[1000];
int   	file_handle,
		status,
		sair,
		neg_sample_present;
long 	Size;


	if(amostra_tipo != NEGATIVE_SAMPLE)
	{
			MessagePopup ("Erro ao criar Negative Sample", 
				"Não pode criar Negative Sample - Modelo não identificado como Amostra Negativa");
			return -1;
	}
	///////////////  VERIFICA arquivo de Negative Samples ///////////////////////
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, ARQ_N_SAMPL_REGIST);
	status = GetFileInfo(nome_arquivo , &Size);
	if (status == 1)
	{
		//////// Verifica se registro já existe ////////
		sair = OFF;
		neg_sample_present = OFF;
	  	file_handle  = OpenFile(nome_arquivo , VAL_READ_ONLY , VAL_APPEND , VAL_ASCII );
		do
		{
			status = ReadLine (file_handle, sample_status, 990 );
			if(status == -2)//fim do arquivo
				sair = ON;
			if(strstr(sample_status, N_Serie_pwb) != NULL)
			{
				neg_sample_present = ON;
			}
		}while(!sair && !neg_sample_present);
		CloseFile (file_handle);
		if(neg_sample_present)
		{
			MessagePopup ("Erro ao criar Negative Sample", 
				"Não pode criar Negative Sample pois este número de série ja está registrado");
			return -2;
		}
	}
	
	return 0;
}

//////////////////////////////////////////////  FUNÇÃO Gerenciar_Samples //////////////////////////////////////////
int Gerenciar_Samples(void)
{
int status;

	if(!master_sample_ok && amostra_tipo != MASTER_SAMPLE)
	{
		MessagePopup ( " CLUSTER PADRÃO", "MASTER SAMPLE NÃO PASSOU HOJE!" ) ;
		return -1;                        
	}
	else if(!negativ_sample_ok && amostra_tipo == TESTE )
	{
		MessagePopup ( " AMOSTRA NEGATIVA", "AMOSTRA NEGATIVA NÃO PASSOU HOJE!" ) ;
		return -2;                        
	}
	else if(!negativ_sample_ok && amostra_tipo == NEGATIVE_SAMPLE && strstr(ident_da_sample, N_Serie_pwb) == NULL && !criar_negat_sample)
	{
		MessagePopup ( "ERRO SEQUEÊNCIA AMOSTRA NEGATIVA", "ESTA NÃO É A AMOSTRA NEGATIVA SOLICITADA! \n FAVOR INSERIR AMOSTRA NEGATIVA CORRETA" ) ;
		return -3;                        
	}
	/////////////////////////////////////////
	if(criar_negat_sample)
	{
		if(AutorizaCriarNegatSample() < 0)
			return -4;
	}
	else if(amostra_tipo == NEGATIVE_SAMPLE)
	{
		if((status=AutorizaTestarNegSample()) < 0)
		{
			switch(status)
			{
				case NSAMPLE_ARQ_REG:
					MessagePopup ("Falha arquivo registro p/ Amostra Negativa", "Não foi encontrado o arquivo de registro das Amostras Negativas" ) ;
					break;
				case NSAMPLE_SEM_REG:
					MessagePopup ("Amostra Negativa não é válida", "Esta Negative Sample não está registrada" ) ;
					break;
				case NSAMPL_ERR_TABS:
					MessagePopup ("Amostra Negativa não é válida", "A tabela de testes da Negative Sample está diferente da tabela do registro" ) ;
					break;
				case NSAMPLE_ARQ_LER:
					MessagePopup ("Amostra Negativa não é válida", "Falha na leitura dos registros da Negative Sample" ) ;
					break;
				default:
					break;
			}
			return -5;                        
		}
	}
return SEM_ERRO;
}

//////////////////////////////////////////////  FUNÇÃO AutorizaTestarNegSample //////////////////////////////////////////
int AutorizaTestarNegSample(void)
{
char  	nome_arquivo[200], 
		linha_da_tabela[2000],
		aux[50] = {0};
int   	file_handle,
		status,
		sair,
		neg_sample_present,
		erro_confronto,
		index_ant,
		index_post;
long 	Size;

	///////////////  VERIFICA arquivo de Negative Samples ///////////////////////
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, ARQ_N_SAMPL_REGIST);
	status = GetFileInfo(nome_arquivo , &Size);
	if (status != 1)
		 return  NSAMPLE_ARQ_REG;
	//////// Verifica se registro já existe ////////
	sair = OFF;
	neg_sample_present = OFF;
  	file_handle  = OpenFile(nome_arquivo , VAL_READ_ONLY , VAL_APPEND , VAL_ASCII );
	do
	{
		status = ReadLine (file_handle, linha_da_tabela, 1990 );
		if(status == -2)//fim do arquivo
			sair = ON;
		if(strstr(linha_da_tabela, N_Serie_pwb) != NULL)
		{
			neg_sample_present = ON;
		}
	}while(!sair && !neg_sample_present);
	
	if(!neg_sample_present)
	{
		CloseFile (file_handle);
		return NSAMPLE_SEM_REG;
	}
	
	///// Le registros da N.Sample e compara com tabela de testes - Para autorização de testes confronta apenas numeros dos testes  ///////////////
 	status = ReadLine(file_handle,linha_da_tabela , 1990);//le 2° linha após num serie - onde devem estar os numeros dos testes da N. Sample
 	//status = ReadLine(file_handle,linha_da_tabela , 1990);//le 3° linha após num serie - onde devem estar os Status dos testes
 	//status = ReadLine(file_handle,linha_da_tabela , 1990);//le 4° linha após num serie - onde devem estar os Resultados dos testes
 	//status = ReadLine(file_handle,linha_da_tabela , 1990);//le 5° linha após num serie - onde devem estar os Limites Inferiores dos testes
 	//status = ReadLine(file_handle,linha_da_tabela , 1990);//le 5° linha após num serie - onde devem estar os Limites Superiores dos testes
	CloseFile (file_handle);
	index_ant = FindPattern (linha_da_tabela, 0 ,-1 , ",", 0, 0);
	if(index_ant > 0)
	{
		for (cont = 1 ; cont <= max_teste ; cont++)
		{ 
			if(cont != (max_teste))
			{
				index_post = FindPattern (linha_da_tabela, index_ant+1 ,-1 , ",", 0, 0);
				if(index_post > 0)
				{
					CopyString (aux, 0, linha_da_tabela, index_ant+1, index_post-(index_ant+1));
					index_ant = index_post;
				}
			}
			else
			{
				CopyString (aux, 0, linha_da_tabela, index_ant+1, -1);
				
			}
			TestNSample[cont].Numero = atoi(aux);
	    }
	}
	if(index_ant < 0 || index_post < 0)
	{
		return NSAMPLE_ARQ_LER;
	}

	
	
 		/*{
 	   		Scan(linha_da_tabela, "%s>%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]%c%s[t44]",num_teste , virgula,
 				Teste[i].Nome, virgula, lim_baixo, virgula, lim_alto, virgula, Teste[i].Unidade, virgula, reteste);
 			Teste[i].Numero = atoi(num_teste);	
 			Teste[i].LimBaixo = atof(lim_baixo);
 			Teste[i].LimAlto  = atof(lim_alto);
 			Teste[i].Reteste = atoi(reteste);
 			if (Teste[i].Numero>0) i++;	//elimina testes com simbolo negativo								
 			max_teste=i;
 		}
    	CloseFile (arquivo);
    	if (arquivo>0) arquivo = SEM_ERRO; */
	erro_confronto = OFF;
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(Teste[cont].Numero != TestNSample[cont].Numero )
		{
			erro_confronto = ON;
			break;
		}
    }
	if(erro_confronto)
		return NSAMPL_ERR_TABS;
	
	return SEM_ERRO;
}

 
//////////////////////////////////////////////  FUNÇÃO VerConsistNegatSample //////////////////////////////////////////
int VerConsistNegatSample(void)
{
char  	nome_arquivo[200], 
		linha_da_tabela[2000],
		aux[300];
int   	file_handle,
		status,
		sair,
		neg_sample_present,
		erro_confronto,
		index_ant,
		index_post;
long 	Size;

	///////////////  VERIFICA arquivo de Negative Samples ///////////////////////
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, ARQ_N_SAMPL_REGIST);
	status = GetFileInfo(nome_arquivo , &Size);
	if (status != 1)
	{
		MessagePopup ("Erro ler registro Negative Samples", "Erro ao abrir arquivo de Negative Samples");
		return  -1;
	}
	//////// Tenta localizar registro ////////
	sair = OFF;
	neg_sample_present = OFF;
  	file_handle  = OpenFile(nome_arquivo , VAL_READ_ONLY , VAL_APPEND , VAL_ASCII );
	do
	{
		status = ReadLine (file_handle, linha_da_tabela, 1990 );
		if(status == -2)//fim do arquivo
			sair = ON;
		if(strstr(linha_da_tabela, N_Serie_pwb) != NULL)
		{
			neg_sample_present = ON;
		}
	}while(!sair && !neg_sample_present);
	
	if(!neg_sample_present)
	{
		CloseFile (file_handle);
		MessagePopup ("Erro ler registro Negative Samples", "Não conseguiu localizar o Serial da Negative Samples nos registros");
		return -2;
	}
	
	///// Le registros da N.Sample e compara com Status dos testes realizados - Por enquanto confronta apenas Status dos testes  ///////////////
 	status = ReadLine(file_handle,linha_da_tabela , 1990);//le 2° linha após num serie - onde devem estar os numeros dos testes da N. Sample
 	status = ReadLine(file_handle,linha_da_tabela , 1990);//le 3° linha após num serie - onde devem estar os Status dos testes
	index_ant = FindPattern (linha_da_tabela, 0 ,-1 , ",", 0, 0);
	if(index_ant > 0)
	{
		for (cont = 1 ; cont <= max_teste ; cont++)
		{ 
			if(cont != (max_teste))
			{
				index_post = FindPattern (linha_da_tabela, index_ant+1 ,-1 , ",", 0, 0);
				if(index_post > 0)
				{
					CopyString (TestNSample[cont].Condicao, 0, linha_da_tabela, index_ant+1, index_post-(index_ant+1));
					index_ant = index_post;
				}
			}
			else
			{
				CopyString (TestNSample[cont].Condicao, 0, linha_da_tabela, index_ant+1, -1);
			}
	    }
	}
	if(index_ant < 0 || index_post < 0)
	{
		CloseFile (file_handle);
		MessagePopup ("Erro ao validar Negative Samples", "Falha ao ler dados de registro da Negative Samples");
		return -3;
	}
 	status = ReadLine(file_handle,linha_da_tabela , 1990);//le 4° linha após num serie - onde devem estar os Resultados dos testes
 	//status = ReadLine(file_handle,linha_da_tabela , 1990);//le 5° linha após num serie - onde devem estar os Limites Inferiores dos testes
 	//status = ReadLine(file_handle,linha_da_tabela , 1990);//le 5° linha após num serie - onde devem estar os Limites Superiores dos testes
	CloseFile (file_handle);

	erro_confronto = OFF;
	for (cont = 1 ; cont <= max_teste ; cont++)
	{ 
		if(strcmp(Teste[cont].Condicao,  TestNSample[cont].Condicao) != 0)
		{
			erro_confronto = ON;
			break;
		}
    }
	if(erro_confronto)
	{
		strcpy(aux , "Resultados dos testes DIFEREM dos dados de registro da Negative Samples\n");
		strcat(aux, "Teste que falhou = "); 
		strcat(aux, Teste[cont].Nome);
		MessagePopup ("Erro ao validar Negative Samples" , aux );
		return -4;
	}
	return SEM_ERRO;
}

//////////////////////////////////////////////  FUNÇÃO DefNegSamp_Semanal //////////////////////////////////////////
int RegistSeqNegSamp_Semanal(void)
{
int quant_semanal,
	quant_diarias,
	erro = 0,
	i;

	if(LerNegSamplesRegistradas() < 0)
		return-1;
	///// verifica quantas são semanais /////////
	quant_semanal 	= 0;
	erro 			= 0;
	while (neg_samples_semanal[quant_semanal] != -1)
	{
		quant_semanal++;
		if(quant_semanal >= MAX_NUM_NEG_SAMPLES)
		{
			erro = 1;
			break;
		}
	}
	/// verifica se existem e quantas são diarias ////////
	quant_diarias = 0;
	while (neg_samples_a_ser_test[quant_diarias] != -1)//as diarias já estão carregadas como "a ser testadas"
	{
		quant_diarias++;
		if(quant_diarias >= MAX_NUM_NEG_SAMPLES)
		{
			erro = 1;
			break;
		}
	}
	if((quant_semanal + quant_diarias) < 1 || erro)
	{
		MessagePopup ("Erro na quantidade de Negative Samples",
					  "Erro na quantidade de Amostra Negativas a serem verificadas diariamente");
		return -2;
	}
	return DefSeqNegSample_DiariaSemanal();
}


//////////////////////////////////////////////  FUNÇÃO DefSeqNegSample_DiariaSemanal //////////////////////////////////////////
int DefSeqNegSample_DiariaSemanal(void)
{
int arq_temp_dia_1,
	arq_temp_dia_2,
	arq_temp_dia_3,
	arq_temp_dia_4,
	arq_temp_dia_5,
	arq_seq_semanal,
	quant_diarias,
	quant_semanal,
	dia,
	seman_incomplet,
	status, erro_encont;
char  nome_arquivo_1[210],
	  nome_arquivo_2[210],
	  nome_arquivo_3[210],
	  nome_arquivo_4[210],
	  nome_arquivo_5[210],
	  nome_arquivo_sem[210],
	  aux[210]; 

	erro_encont = SEM_ERRO;
	/////// Monta estrutura e divide semana em 5 dias /////////////////////////////////
	////////////// cria arquivos temporários //////////////////////////////////////////
	strcpy(nome_arquivo_1, CAMINHO_SAMPLE);
	strcat(nome_arquivo_1, "Temp_dia_1.txt");
	arq_temp_dia_1  = OpenFile(nome_arquivo_1 , VAL_READ_WRITE , VAL_TRUNCATE , VAL_ASCII );
	if(arq_temp_dia_1 < 0)
	{
		CloseFile (arq_temp_dia_1); 
		MessagePopup ("Erro ao tentar criar arquivos", "Houve um erro ao tentar criar arquivo temporário de sequencia para Negative Sample");
		return -1;
	}
	strcpy(nome_arquivo_2, CAMINHO_SAMPLE);
	strcat(nome_arquivo_2, "Temp_dia_2.txt");
	arq_temp_dia_2  = OpenFile(nome_arquivo_2 , VAL_READ_WRITE , VAL_TRUNCATE , VAL_ASCII );
	
	strcpy(nome_arquivo_3, CAMINHO_SAMPLE);
	strcat(nome_arquivo_3, "Temp_dia_3.txt");
	arq_temp_dia_3  = OpenFile(nome_arquivo_3 , VAL_READ_WRITE , VAL_TRUNCATE , VAL_ASCII );
	
	strcpy(nome_arquivo_4, CAMINHO_SAMPLE);
	strcat(nome_arquivo_4, "Temp_dia_4.txt");
	arq_temp_dia_4  = OpenFile(nome_arquivo_4 , VAL_READ_WRITE , VAL_TRUNCATE , VAL_ASCII );
	
	strcpy(nome_arquivo_5, CAMINHO_SAMPLE);
	strcat(nome_arquivo_5, "Temp_dia_5.txt");
	arq_temp_dia_5  = OpenFile(nome_arquivo_5 , VAL_READ_WRITE , VAL_TRUNCATE , VAL_ASCII );
	//////Grava os Negat Samples diarios /////////
	quant_diarias = 0;
	while (neg_samples_a_ser_test[quant_diarias] != -1)//as diarias já estão carregadas como "a ser testadas"
	{
		strcpy(aux, negat_samples_registr[neg_samples_a_ser_test[quant_diarias]].serial);
		strcat(aux, ",");
		strcat(aux, negat_samples_registr[neg_samples_a_ser_test[quant_diarias]].nome);
        status = WriteLine(arq_temp_dia_1 , aux , StringLength(aux));
        status = WriteLine(arq_temp_dia_2 , aux , StringLength(aux));
        status = WriteLine(arq_temp_dia_3 , aux , StringLength(aux));
        status = WriteLine(arq_temp_dia_4 , aux , StringLength(aux));
        status = WriteLine(arq_temp_dia_5 , aux , StringLength(aux));
		quant_diarias++;
	}
	//////Grava os Negat Samples Semanal /////////
	quant_semanal 	= 0;
	dia 			= 0;
	if(quant_diarias == 0)
		seman_incomplet = ON;
	else
		seman_incomplet = OFF;
completar_semana:
	while (neg_samples_semanal[quant_semanal] != -1)
	{
		strcpy(aux, negat_samples_registr[neg_samples_semanal[quant_semanal]].serial);
		strcat(aux, ",");
		strcat(aux, negat_samples_registr[neg_samples_semanal[quant_semanal]].nome);
		switch(dia)
		{
			case 0:
        		status = WriteLine(arq_temp_dia_1 , aux , StringLength(aux));
				dia = 1;
				break;
			case 1:
        		status = WriteLine(arq_temp_dia_2 , aux , StringLength(aux));
				dia = 2;
				break;
			case 2:
        		status = WriteLine(arq_temp_dia_3 , aux , StringLength(aux));
				dia = 3;
				break;
			case 3:
        		status = WriteLine(arq_temp_dia_4 , aux , StringLength(aux));
				dia = 4;
				break;
			case 4:
        		status = WriteLine(arq_temp_dia_5 , aux , StringLength(aux));
				dia = 0;
				seman_incomplet = OFF;
				break;
				
		}
		quant_semanal++;
	}
	if(seman_incomplet)
	{
		if(dia == 0)//não achou nenhuma N.Sample diaria ou semanal
		{
			MessagePopup ("Erro ao criar arquivo semanal", "Não conseguiu criar sequencia semanal temporária para Negative Sample");
			erro_encont = ON;
			goto erro;
		}
		quant_semanal = 0;
		goto completar_semana;
	}
	
	//////// copia dados dos arquivos temporários para arquivo semanal /////////
	strcpy(nome_arquivo_sem, CAMINHO_SAMPLE);
	strcat(nome_arquivo_sem, ARQ_SEQ_NSAM_SEM);
	arq_seq_semanal  = OpenFile(nome_arquivo_sem , VAL_READ_WRITE , VAL_TRUNCATE , VAL_ASCII );
	strcpy(aux,"////////// ARQUIVO DE SEQUÊNCIA SEMANAL PARA NEGATIVE SAMPLES //////////");
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	strcpy(aux,"/////////// ATENÇÃO!!!! NÃO MODIFIQUE ESTE ARQUIVO /////////////////////");
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	//////////    dia 1 //////
    status = WriteLine(arq_seq_semanal , STR_COMENT , StringLength(STR_COMENT));
	strcpy(aux,STR_DIA_1);
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	SetFilePtr (arq_temp_dia_1, 0, 0);
	while(ReadLine (arq_temp_dia_1,aux , 200) > 0)
	{
        status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	}
	//////////    dia 2 //////
    status = WriteLine(arq_seq_semanal , STR_COMENT , StringLength(STR_COMENT));
	strcpy(aux,STR_DIA_2);
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	SetFilePtr (arq_temp_dia_2, 0, 0);
	while(ReadLine (arq_temp_dia_2,aux , 200) > 0)
	{
        status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	}
	//////////    dia 3 //////
    status = WriteLine(arq_seq_semanal , STR_COMENT , StringLength(STR_COMENT));
	strcpy(aux,STR_DIA_3);
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	SetFilePtr (arq_temp_dia_3, 0, 0);
	while(ReadLine (arq_temp_dia_3,aux , 200) > 0)
	{
        status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	}
	//////////    dia 4 //////
    status = WriteLine(arq_seq_semanal , STR_COMENT , StringLength(STR_COMENT));
	strcpy(aux,STR_DIA_4);
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	SetFilePtr (arq_temp_dia_4, 0, 0);
	while(ReadLine (arq_temp_dia_4,aux , 200) > 0)
	{
        status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	}
	//////////    dia 5 //////
    status = WriteLine(arq_seq_semanal , STR_COMENT , StringLength(STR_COMENT));
	strcpy(aux,STR_DIA_5);
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	SetFilePtr (arq_temp_dia_5, 0, 0);
	while(ReadLine (arq_temp_dia_5,aux , 200) > 0)
	{
        status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	}
	///////// Define dia atual ////////
    status = WriteLine(arq_seq_semanal , STR_COMENT , StringLength(STR_COMENT));
	strcpy(aux,STR_DIA_ATUAL);
	strcat(aux,"1,");
	strcat(aux,DateStr());
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	strcpy(aux,"FIM");
    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
	
erro:
	/////// Fecha e elimina os arquivos temporários ///////////////////////////	
	CloseFile (arq_temp_dia_1); 
	CloseFile (arq_temp_dia_2); 
	CloseFile (arq_temp_dia_3); 
	CloseFile (arq_temp_dia_4); 
	CloseFile (arq_temp_dia_5);
	CloseFile (arq_seq_semanal);
	DeleteFile (nome_arquivo_1);
	DeleteFile (nome_arquivo_2);
	DeleteFile (nome_arquivo_3);
	DeleteFile (nome_arquivo_4);
	DeleteFile (nome_arquivo_5);
	return erro_encont;
}

////////////////// Função Le_NegSamp_a_testar() ///////////////////////////////////////////
int Le_NegSamp_a_testar(void)
{
	
	if(LerNegSamplesRegistradas() < 0)
		return -1;
	///////	Le as Negative Samples a serem testados no dia /////////
	if(LerDiaAtual(1,0) < 0) //parametros = (1==Carrega NegSamp a testar, 0==Não indexar dia atual)
		return -2;
	
	return 0;
	
}

/////////////// Função LerDiaAtual////////////////////////////////////////////////////////
int LerDiaAtual(int carregar, int indexar)
{
int erro = OFF,
	ns_test,
	ns_seman,
	sair,
	arq_seq_semanal,
	index,
	i,
	status,
	dia_atual,
	position,
	posit_arq_dia_atual,
	ler_dados;
char  nome_arquivo[200], 
	  sample_status[210],
	  aux[200],
	  dia_inic[200],
	  dia_fim[200],
	  str_dia_atual[200];
long Size;

	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, ARQ_SEQ_NSAM_SEM);
	arq_seq_semanal  = OpenFile(nome_arquivo , VAL_READ_WRITE , VAL_OPEN_AS_IS , VAL_ASCII );
	//// procura o dia atual e compara data com a data de hoje -- caso datas não sejam iguais incrementará depois o dia atual
	dia_atual 	= 0;
	erro 		= 0;
	while(ReadLine (arq_seq_semanal,sample_status , 200) > 0)
	{
		if(strstr(sample_status, STR_DIA_ATUAL) != NULL)
		{
			CopyString (aux, 0, sample_status, strlen(STR_DIA_ATUAL), 1);
			dia_atual = atoi(aux);
			if(dia_atual >= 1 && dia_atual <= 5)
			{
				posit_arq_dia_atual =  SetFilePtr (arq_seq_semanal, 0, 1);
				posit_arq_dia_atual = posit_arq_dia_atual- (strlen(sample_status)+2);//retorna o ponteiro de arquivo para posição a ser alterada
				strcpy(str_dia_atual,sample_status);
				break;
			}
			else
				dia_atual = 0;
		}
	}
	if(dia_atual == 0)
	{
		MessagePopup ("Erro ao procurar dia atual no arquivo semanal", 
			"Não conseguiu encontrar dia atual na sequencia semanal para Negative Sample");
		erro = 1;
		CloseFile (arq_seq_semanal); 
		return -1;
		
	}
	
	switch(dia_atual)
	{
		case 1:
			strcpy(dia_inic, STR_DIA_1);
			strcpy(dia_fim,  STR_DIA_2);
			break;
		case 2:
			strcpy(dia_inic, STR_DIA_2);
			strcpy(dia_fim,  STR_DIA_3);
			break;
		case 3:
			strcpy(dia_inic, STR_DIA_3);
			strcpy(dia_fim,  STR_DIA_4);
			break;
		case 4:
			strcpy(dia_inic, STR_DIA_4);
			strcpy(dia_fim,  STR_DIA_5);
			break;
		case 5:
			strcpy(dia_inic, STR_DIA_5);
			strcpy(dia_fim,  STR_DIA_ATUAL);
			break;
	}
	if(carregar)
	{
			position = SetFilePtr (arq_seq_semanal, 0, 0);
			for(i=0;i < MAX_NUM_NEG_SAMPLES; i++)
			{
				neg_samples_a_ser_test[i]=-1;
			}
			ns_test 	= 0;
			ler_dados   = 0;
			while(ReadLine (arq_seq_semanal, sample_status , 200) > 0)//encontra as Neg Samples do dia
			{
				if(strstr(sample_status, dia_fim) != NULL)
					break;
		
				if(strstr(sample_status, dia_inic) != NULL )
				{
					ler_dados = ON;
					continue;
				}
				if(strcmp(sample_status, STR_COMENT) ==  0)
				{
					continue;
				}
				if(ler_dados)
				{
					for(i=0;i <= MAX_NUM_NEG_SAMPLES;i++)
					{
						if(strstr(sample_status, negat_samples_registr[i].serial) != NULL)
						{
							neg_samples_a_ser_test[ns_test] = i;
							ns_test++;
							break;
						}
					}
					if(i >= MAX_NUM_NEG_SAMPLES)
					{
						MessagePopup ("Erro na definição das Negative Samples",
							"Erro ao tentar definir a escala de Amostra Negativas a serem testadas hoje\n Uma Negative Sample que deveria ser escalada para testes não foi encontrada no arquivo de Registro das Amostras Negativas");
						erro = 1;
						break;
					}
				}
			}
	}
	if(indexar && !erro)
	{
		if(strstr(str_dia_atual, DateStr()) == NULL)//indexara apenas se dia da semana for diferente
		{
			position = SetFilePtr (arq_seq_semanal, posit_arq_dia_atual, 0);
			dia_atual++;
			if(dia_atual > 5)
				dia_atual = 1;
			Fmt(aux,"%s<%s%i,%s", STR_DIA_ATUAL,dia_atual, DateStr());
		    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
			strcpy(aux,"FIM");
		    status = WriteLine(arq_seq_semanal , aux , StringLength(aux));
		}
	}
	CloseFile (arq_seq_semanal); 
	if(erro)
		return -2;
	return 0;
}

//////////////////////////////////////////////  FUNÇÃO LerNegSamplesRegistradas //////////////////////////////////////////
int LerNegSamplesRegistradas(void)
{
int erro = OFF,
	ns_test,
	ns_seman,
	sair,
	file_handle,
	index,
	index_post,
	i,
	status;
char  nome_arquivo[200], 
	  sample_status[2000];
long Size;

	///////////////  Le arquivo de Negative Samples ///////////////////////
	for(i=0;i < MAX_NUM_NEG_SAMPLES; i++)
	{
		strcpy(negat_samples_registr[i].serial,"");
		strcpy(negat_samples_registr[i].nome,"");
		neg_samples_a_ser_test[i]=-1;
		neg_samples_semanal[i]=-1;
	}
	
	quant_neg_samp_regist 	= 0;
	sair 					= OFF;
	strcpy(nome_arquivo, CAMINHO_SAMPLE);
	strcat(nome_arquivo, ARQ_N_SAMPL_REGIST);
	status = GetFileInfo(nome_arquivo , &Size);
	
	if (status == 1)
	{
		
	  	file_handle  = OpenFile(nome_arquivo , VAL_READ_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );
		ns_test	=0;
		ns_seman=0;
		do
		{
			status = ReadLine (file_handle, sample_status, 1990 );
			if(status == -2)//fim do arquivo
				sair = ON;
			//StringUpperCase ();
			if(strstr(sample_status, STR_SERIAL_NEG_SAMPLE) != NULL)
			{
				
				index = FindPattern (sample_status, 0 ,-1 , ",", 0, 0);
				CopyString (negat_samples_registr[quant_neg_samp_regist].serial, 0, sample_status, strlen(STR_SERIAL_NEG_SAMPLE), index-(strlen(STR_SERIAL_NEG_SAMPLE)));
				if(index > 0)
				{
					index_post = FindPattern (sample_status, index+1 ,-1 , ",", 0, 0);
					CopyString (negat_samples_registr[quant_neg_samp_regist].nome, 0, sample_status, index+1, index_post-(index+1));
					if(strlen(negat_samples_registr[quant_neg_samp_regist].nome) > 0 &&  
						strlen(negat_samples_registr[quant_neg_samp_regist].nome) <= MAX_LEN_NSAMPLE)
					{
						if(strstr(sample_status, STR_NEG_SAMPLE_DIARIA) != NULL)
						{
							neg_samples_a_ser_test[ns_test] = quant_neg_samp_regist;
							ns_test++;
						}
						else
						{
							neg_samples_semanal[ns_seman] = quant_neg_samp_regist;
							ns_seman++;
						}
						quant_neg_samp_regist++;
					}
					else
					{
						MessagePopup ("Encontrado registro de amostra negativa corrompido",
									  "Ao menos um registro de Amostra Negativa está corrompido-Nome da Amostra inválido");
						erro = ON;
						break;
					}
				}
				else
				{
					MessagePopup ("Encontrado registro de amostra negativa corrompido",
								  "Ao menos um registro de Amostra Negativa está corrompido");
					erro = ON;
					break;
				}
			}
		}while(!sair && quant_neg_samp_regist < MAX_NUM_NEG_SAMPLES);
		CloseFile (file_handle); 
	}
	if(erro)
	{
		quant_neg_samp_regist 		= 0;
		neg_samples_a_ser_test[0] 	=-1;
		return -1;
	}
	return 0;
}
//////////////////////////////////////////////  FUNÇÃO LeDescTipo_AmosNeg() //////////////////////////////////////////
int LeDescTipo_AmosNeg(void)
{
char
	aux[50],
	prefix[10];
int
	i;
		
	for(i=0; i < MAX_NUM_NEG_SAMPLES; i++)
	{
		CopyString(prefix, 0, negat_samples_registr[i].serial, 0, TAMANHO_PREFIXO_PWB);
		DefModel(prefix);
		//strcpy(negat_samples_registr[i].descricao_modelo, descricao_modelo);
	}
	
return 0;
}
// *********************** GravaTipoAtual *****************************
int GravaTipoAtual(void)
{
int file;
	
	file = OpenFile( ARQ_TIPO_ATUAL , VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII ) ;
	if(file < 0)
		return -1;
	WriteLine ( file, id_tipo[modelo_atual_index], strlen(id_tipo[modelo_atual_index]) ) ;
	strcpy(descricao_modelo_atual, id_tipo[modelo_atual_index]);
	CloseFile ( file );
return 1;
}

// *********************** LeTipoAtual *****************************
int LeTipoAtual(void)
{
int file,
	status;
	
	strcpy(descricao_modelo_atual,"");
	file = OpenFile( ARQ_TIPO_ATUAL , VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII ) ;
	if(file < 0)
		return -1;
	status = ReadLine (file,descricao_modelo_atual , -1);
	if(status < 0 || strlen(descricao_modelo_atual) < 2)
	{
		strcpy(descricao_modelo_atual,"");
		CloseFile ( file );
		return -2;
	}
	CloseFile ( file );
	
return 1;
}

