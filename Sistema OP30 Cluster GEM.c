
#include "Windows.h"
#include <formatio.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include <rs232.h>
#include "Main OP30 GEM.h"
#include "Auto OP30 GEM.h"
#include "Manual OP30 GEM.h"
#include "Global Error Tracking.h" 
//
// Função: ThreadSistema()
//
int CVICALLBACK ThreadSistema (void *parametro)
{

	int
		status = 0,
		tamanho;
 	static int
		i2c_sist_err_quant = 0;

	////////////////////////////////
	//SetSleepPolicy (MODO_SLEEP);
	CmtSetCurrentThreadPriority (THREAD_PRIORITY_HIGHEST);
	SetSleepPolicy (VAL_SLEEP_SOME);

	while (!finalizar_thread_sistema)
	{
		switch(modo_thread)
		{
			case LEITURA_APENAS:
				status = Le_Entradas(END_CARTAO_1_ENTRADA);
				CheckI2C_err
				break;
			case ESCRITA_APENAS:
				status = AtualizaCartaoSaida(END_CARTAO_3_SAIDA);
				CheckI2C_err
				STATUS.Continua = OFF;
				STATUS.Rejeita = OFF;
				motions_clear_to_move = OFF;
				//status_bimanual = OFF;
				break;

			case ESCRITA_E_LEITURA:
				Status_Seguranca();
				status = Le_Entradas(END_CARTAO_1_ENTRADA);
				CheckI2C_err
				status = AtualizaCartaoSaida(END_CARTAO_3_SAIDA);
				CheckI2C_err
				
				if( habilita_leitor_barcode == ON ){
					Habilita_scanner_DM();
					habilita_leitor_barcode = OFF ;
				}				
				if  (disable_leitor_barcode == ON){
					Desliga_scanner_DM ();
					disable_leitor_barcode = OFF ;
				}				
	
				if( ler_string_scanner == ON )
				{
					tamanho = GetInQLen (comm_scanner);
					if(tamanho  > 0 )
					{
						strcpy(strGotScanner, "\0" );

						Delay(0.005);
						for(tamanho =0; tamanho <= 15; tamanho++)
						{
							strGotScanner[tamanho] = 0;
						}
						status = ComRdTerm (comm_scanner, strGotScanner, 20, 13 );
						leitor_ativado = OFF;
						tempo_scanner_ligado   		= 0; 

						Delay(0.005);
						FlushInQ (comm_scanner);
					}
				}
				break;

			case PARAR_ESC_LEIT:
				STATUS.Continua = OFF;
				STATUS.Rejeita = OFF;
				motions_clear_to_move = OFF;
				//status_bimanual = OFF;
				break;
		}
		if(STATUS.Descarte_Liner && barcode_Enditem_lido)
		{
			SAIDA.Sol_Descarte_Liner = ON ;
			tempo_Vortex = Timer();
		}
		if(SAIDA.Sol_Descarte_Liner){
			if( Timer()-tempo_Vortex>2.00 ){
				SAIDA.Sol_Descarte_Liner = OFF ;
			}
		}
		
		ProcessSystemEvents ();
	}
	return 0;
}

//
// Função: EsperaSairMontagem()
//
int EsperaSairMontagem(int botao_saida, char* image_file, double tempo_imagens, int descartar_liner)
{
	char
		path[200];
	double
		t;
	int
		sair,
		bot_saida=0;

	////////////////////////////////
	strcpy(path,CAMINHO_TEMPLATES);
	strcat(path, image_file);
	DisplayImageFile (painel_auto, AUTO_PIC_INSTRUCAO, path);
	ProcessDrawEvents ();

	if(descartar_liner == OFF)
		descartou_liner = ON ; // default  - NÃO monitorar descarte

	sair= NAO_SAIR;
	t 	= Timer();
	while(Timer()-t < tempo_imagens && sair == NAO_SAIR)
	{
		Delay_thread(0.010);
		if(sair_programa || status_emergencia == OFF)
			sair = FALHA_MODULO_ABORTADO;
		if( descartar_liner && sair == NAO_SAIR && !descartou_liner )
		{
			if(VerificaDescarteLiner() == 1)
			{
				COR_MENSAGENS = MENS_PRETO;
				descartou_liner = ON ;
			}
		}
		if(botao_saida == CONT)
			bot_saida = STATUS.Continua;
		else if( botao_saida == CONT_REJ )
			bot_saida = STATUS.BiManual;
		else
			bot_saida = ON;
		if(rejeita_montagem == ON || !status_debounc_presenca_produto || (bot_saida==ON && descartou_liner))
		{
			Delay_thread(0.050); // debouncing
			
			if(botao_saida == CONT)
				bot_saida = STATUS.Continua;
			else if(botao_saida == CONT_REJ)
				bot_saida = STATUS.BiManual;
			else
				bot_saida = ON;
			
			if(rejeita_montagem == ON || !status_debounc_presenca_produto || (bot_saida == ON && descartou_liner))
			{
				if(rejeita_montagem)
				{
					COR_MENSAGENS= MENS_VERMELHO;
					strcpy(MensagemFalha1,"Produto rejeitado pelo operador.");
					SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, MensagemFalha1);
					sair = FALHA_REJ_OPERADOR;
				}
				else if(!status_debounc_presenca_produto)
				{
					COR_MENSAGENS= MENS_VERMELHO;
					strcpy(mens_montagem_2,"Produto rejeitado !!");
					strcpy(mens_montagem_3,"Sequencia de operação incorreta - Removido do Berço");
					strcpy(MensagemFalha1, "Subconjunto removido do berço de montagem" );
					sair = FALHA_PRODUTO_AUSENTE;
				}
				else if(bot_saida == ON && descartou_liner)
				{
					SetCtrlAttribute (painel_auto, AUTO_MENS_FALHA, ATTR_CTRL_VAL, "");
					COR_MENSAGENS= MENS_AZUL;
					ProcessDrawEvents ();
					sair = SAIR_LOOP_SEM_ERRO;
				}
			}
		}
	}
	return sair;
}

/*********************************** VerificaDescarteLiner ********************************************************/
int VerificaDescarteLiner(void)
{
double 
	t,
	tempo_deboucing = 0.2, //seg
	tempo_min_sugar = 1.0; //seg	 2 seg 

	///////////////////////////////
	if(STATUS.Descarte_Liner == OFF)
		return 0;
	SAIDA.Sol_Descarte_Liner = ON;
	t = Timer();
	while(STATUS.Descarte_Liner == ON && !sair_programa && status_emergencia)
	{
		Delay_thread(0.010);
	}
	if(Timer() - t < tempo_deboucing)
	{
		SAIDA.Sol_Descarte_Liner = OFF;
		return 0;
	}
	while(Timer() - t < tempo_min_sugar)
	{
		Delay_thread(0.010);
	}
	SAIDA.Sol_Descarte_Liner = OFF;
	
	return 1;
}

//
// Função: IniciaPortasDeSaida()
// 
// Descrição: Inicia as portas de saída das placas.
//
int IniciaPortasDeSaida(char *mens_erro)
{

	int status;
	
	// Inicializa a porta de saída 'CARTAO_3'
	Delay_thread(0.15);
	status = EnviaCommand_C_ToPIC (END_CARTAO_3_SAIDA);//Placa  0x42
	status = EnviaCommand_C_ToPIC (END_CARTAO_3_SAIDA);//Placa  0x42
	if(status==0)
	{
		status = EnviaCommand_A_ToPIC (END_CARTAO_3_SAIDA);
		portGP0[(END_CARTAO_3_SAIDA-0x40)] = 0x00;
		portGP1[(END_CARTAO_3_SAIDA-0x40)] = 0x00;
	}
	if(status != 0)
	{
		strcpy(mens_erro, "Cartão de SAIDA('CARTAO_3') endereço 0x42 não respondeu!\rErro na comunicação I2C do Sistema" );
		return FALHA;
	}
	else
	{
		AtualizaCartaoSaida(END_CARTAO_3_SAIDA);
	}
	
	
	return 0;
}

//
// Função: Le_Entradas()
// 
// Descrição: Responsável pela leitura dos cartões de entrada.
//
int	Le_Entradas(int endereco_cartao)
{
	
	int 
		porta_a = 0,
		porta_b = 0,
		status, 
		a;
	porta 
		matrix;
		
 	if(global_error.i2c_sistem_err == ON)
	return -1;

	status = LeWORDfromMCP23017 (endereco_cartao, &porta_a, &porta_b);
	if(status <0)
	{
		matrix.bit_a0 = 0;
		matrix.bit_a1 = 0;
		matrix.bit_a2 = 0;
		matrix.bit_a3 = 0;
		matrix.bit_a4 = 0;
		matrix.bit_a5 = 0;
		matrix.bit_a6 = 0;
		matrix.bit_a7 = 0;
		///
		matrix.bit_b0 = 0;
		matrix.bit_b1 = 0;
		matrix.bit_b2 = 0;
		matrix.bit_b3 = 0;
		matrix.bit_b4 = 0;
		matrix.bit_b5 = 0;
		matrix.bit_b6 = 0;
		matrix.bit_b7 = 0;

	}
	else
	{
		a = porta_a & 0x1  	;matrix.bit_a0 = !(a/0x1);
		a = porta_a & 0x2  	;matrix.bit_a1 = !(a/0x2);
		a = porta_a & 0x4  	;matrix.bit_a2 = !(a/0x4);
		a = porta_a & 0x8  	;matrix.bit_a3 = !(a/0x8);
		a = porta_a & 0x10 	;matrix.bit_a4 = !(a/0x10);
		a = porta_a & 0x20 	;matrix.bit_a5 = !(a/0x20); 
		a = porta_a & 0x40 	;matrix.bit_a6 = !(a/0x40); 
		a = porta_a & 0x80 	;matrix.bit_a7 = !(a/0x80);
		///
		a = porta_b & 0x1  ;matrix.bit_b0 = !(a/0x1);
		a = porta_b & 0x2  ;matrix.bit_b1 = !(a/0x2);
		a = porta_b & 0x4  ;matrix.bit_b2 = !(a/0x4);
		a = porta_b & 0x8  ;matrix.bit_b3 = !(a/0x8);
		a = porta_b & 0x10 ;matrix.bit_b4 = !(a/0x10);
		a = porta_b & 0x20 ;matrix.bit_b5 = !(a/0x20); 
		a = porta_b & 0x40 ;matrix.bit_b6 = !(a/0x40); 
		a = porta_b & 0x80 ;matrix.bit_b7 = !(a/0x80);
	}

	switch(endereco_cartao)
	{

		case END_CARTAO_1_ENTRADA:
			STATUS.Trava_Avancado_1 = matrix.bit_a0;
			STATUS.Trava_Avancado_2 = matrix.bit_a1;
			STATUS.Presenca_PCB = matrix.bit_a2;
			STATUS.Combitac_Encaixado = matrix.bit_a3;
			STATUS.Descarte_Liner = matrix.bit_a4;
			STATUS.Spare_in_105 = matrix.bit_a5;
			STATUS.Spare_in_106 = matrix.bit_a6;
			STATUS.Spare_in_107 = matrix.bit_a7;
			///
			STATUS.Spare_in_108 = matrix.bit_b0;
			STATUS.Spare_in_109 = matrix.bit_b1;
			STATUS.Spare_in_110 = matrix.bit_b2;
			STATUS.Spare_in_111 = matrix.bit_b3;
			STATUS.BiManual     = matrix.bit_b4;
			STATUS.Rejeita      = matrix.bit_b5;
			STATUS.Continua     = matrix.bit_b6;
			STATUS.Emergencia   = matrix.bit_b7;
			break;

		default:
			return -1;
	}
	
	return status ;
	
}

//
// Função: AtualizaCartaoSaida()
// 
// Descrição: Envia informações para os cartões de saída das placas.
//
int	AtualizaCartaoSaida(int endereco_cartao)
{
	char 
		buffer[40], 
		aux_send[20];
	porta 
		matrix = {0};
	int
		saida1_cartao_3_anterior=999, saida2_cartao_3_anterior=999,
		saida1_cartao_4_anterior=999, saida2_cartao_4_anterior=999,
		saida1, 
		saida2,
		status = -1;
	
		if(global_error.i2c_sistem_err == ON)
		return -1;

	switch (endereco_cartao)
	{
		
		case END_CARTAO_3_SAIDA:  //0x42
			matrix.bit_a0 = SAIDA.iluminacao_superior_max;
			matrix.bit_a1 = SAIDA.iluminacao_superior_med;
			matrix.bit_a2 = SAIDA.iluminacao_superior_min;
			matrix.bit_a3 = SAIDA.Liga_Ionizador;
			matrix.bit_a4 = SAIDA.Spare_in_304 ;
			matrix.bit_a5 = SAIDA.Sol_Trava_Produto;
			matrix.bit_a6 = SAIDA.Sol_Combitac;
			matrix.bit_a7 = SAIDA.Sinaleiro_Verde;				  
			///
			matrix.bit_b0 = SAIDA.Sinaleiro_Vermelho;
			matrix.bit_b1 = SAIDA.Sinaleiro_Buzzer;
			matrix.bit_b2 = SAIDA.Spare_in_310;
			matrix.bit_b3 = SAIDA.Spare_in_311;
			matrix.bit_b4 = SAIDA.Spare_in_312;
			matrix.bit_b5 = SAIDA.Sol_Descarte_Liner;
			matrix.bit_b6 = SAIDA.ResetaEmergencia;
			matrix.bit_b7 = SAIDA.HabitaEmergencia;
			
			break;
		
		default:
			break;						  
	}									  

	saida1 =  matrix.bit_a0 * 0x1			;
	saida1 =  saida1 + matrix.bit_a1 * 0x2	;
	saida1 =  saida1 + matrix.bit_a2 * 0x4	;
	saida1 =  saida1 + matrix.bit_a3 * 0x8	;
	saida1 =  saida1 + matrix.bit_a4 * 0x10	;
	saida1 =  saida1 + matrix.bit_a5 * 0x20	;
	saida1 =  saida1 + matrix.bit_a6 * 0x40	;
	saida1 =  saida1 + matrix.bit_a7 * 0x80	;
	
	saida2 =  matrix.bit_b0 * 0x1 			;
	saida2 =  saida2 + matrix.bit_b1 * 0x2	;
	saida2 =  saida2 + matrix.bit_b2 * 0x4	;
	saida2 =  saida2 + matrix.bit_b3 * 0x8	;
	saida2 =  saida2 + matrix.bit_b4 * 0x10	;
	saida2 =  saida2 + matrix.bit_b5 * 0x20	;
	saida2 =  saida2 + matrix.bit_b6 * 0x40	;
	saida2 =  saida2 + matrix.bit_b7 * 0x80	;
	
	switch (endereco_cartao)
	{

		case END_CARTAO_3_SAIDA:  //0x42
			if(saida1 != saida1_cartao_3_anterior || saida2 != saida2_cartao_3_anterior)
			{
				strcpy(buffer, "B");
				Fmt(aux_send, "%s<%s%i", "000",endereco_cartao);
				CopyString (aux_send, 0, aux_send,strlen(aux_send)-3 , 3);
				strcat(buffer, aux_send);

				Fmt(aux_send, "%s<%s%i", "000",saida1);
				CopyString (aux_send, 0, aux_send,strlen(aux_send)-3 , 3);
				strcat(buffer, aux_send);

				Fmt(aux_send, "%s<%s%i", "000",saida2);
				CopyString (aux_send, 0, aux_send,strlen(aux_send)-3 , 3);
				strcat(buffer, aux_send);
			  	status = EscreveMCP23017byPIC ( buffer, &saida1_cartao_3_anterior, &saida2_cartao_3_anterior );
			}
			break;

		case END_CARTAO_4_SAIDA:  //0x44
			if(saida1 != saida1_cartao_4_anterior || saida2 != saida2_cartao_4_anterior)
			{
				strcpy(buffer, "B");
				Fmt(aux_send, "%s<%s%i", "000",endereco_cartao);
				CopyString (aux_send, 0, aux_send,strlen(aux_send)-3 , 3);
				strcat(buffer, aux_send);

				Fmt(aux_send, "%s<%s%i", "000",saida1);
				CopyString (aux_send, 0, aux_send,strlen(aux_send)-3 , 3);
				strcat(buffer, aux_send);

				Fmt(aux_send, "%s<%s%i", "000",saida2);
				CopyString (aux_send, 0, aux_send,strlen(aux_send)-3 , 3);
				strcat(buffer, aux_send);
			  	status = EscreveMCP23017byPIC ( buffer, &saida1_cartao_4_anterior, &saida2_cartao_4_anterior );
			}
			break;

			default:
			break;						  
	}

	return status;
}

//
// Função: Status_Seguranca()
//
void Status_Seguranca(void)
{
static int
	cont_emergencia	= 0,
	cont_vortex = 0;
static double
	tempo_presenca = 0;
#define  MAX_VORTEX  10

	/////// status da emergência ///////
	if( !STATUS.Emergencia )
		cont_emergencia++;
	else
		cont_emergencia = 0;
	if(cont_emergencia >= DEBOUNCING_EMERGEN)
	{
		status_emergencia = OFF;
		cont_emergencia = DEBOUNCING_EMERGEN;
	}
	else
		status_emergencia = ON;
	
	
	/////// status debouncing presença ///////
	if( STATUS.Presenca_PCB == ON )
	{
		tempo_presenca = Timer();
		status_debounc_presenca_produto = ON ;
	}
	if(status_debounc_presenca_produto && STATUS.Presenca_PCB  == OFF && (Timer()-tempo_presenca) >  TEMPO_MAX_AUSENCIA )
	{
		status_debounc_presenca_produto = OFF;
	}
	/////////////////////////////////////////////////////////////////
	if(modulo_em_teste == ON && status_produto != PRODUTO_TESTANDO && STATUS.Descarte_Liner == ON)//produto está sendo montado no berço 
	{
		SAIDA.Sol_Descarte_Liner = ON;
		cont_vortex = MAX_VORTEX;
	}
	else
	{
		if(SAIDA.Sol_Descarte_Liner == ON && cont_vortex > 0 && status_produto != PRODUTO_TESTANDO)	
		{
			cont_vortex--;
			if(cont_vortex <= 0)
			{
				SAIDA.Sol_Descarte_Liner = OFF;
				cont_vortex = 0;
			}
		}
	}
	
	/////// verifica home position ///////
	if( (STATUS.Trava_Avancado_1==OFF && STATUS.Trava_Avancado_2==OFF ) 
		)
		all_motions_home = ON;
	else
		all_motions_home = OFF;
	
	/////// motions clear ///////
	if( status_emergencia )
		motions_clear_to_move = ON;
	else
		motions_clear_to_move = OFF;
}

//
// Função: ResetarEmergencia()
//
int ResetarEmergencia(int ver_consistencia)
{
	if(ver_consistencia)
	{
		if(modo_thread != ESCRITA_E_LEITURA)
			return FALHA;
		SAIDA.ResetaEmergencia = OFF;
		SAIDA.HabitaEmergencia = OFF;
   		Delay_thread(0.4);
		SAIDA.HabitaEmergencia = ON;
   		Delay_thread(0.5);
		if(STATUS.Emergencia != OFF)
			return FALHA;
	}
	if(SAIDA.HabitaEmergencia != ON)
	{
		SAIDA.HabitaEmergencia = ON;
   		Delay_thread(0.5);
	}
	SAIDA.ResetaEmergencia = ON;
	Delay_thread(0.5);
	SAIDA.ResetaEmergencia = OFF;
   	Delay_thread(0.6);
	return SEM_ERRO;
}

//
// Função: LeBotoeira()
//
int LeBotoeira(void)
{
	double
		t = Timer();
	int
		botao_acionado = OFF,
		cont = BOT_OFF,
		rej = BOT_OFF;
		
	do
	{
		Delay_thread(0.01);
		rej = STATUS.Rejeita;
		cont = STATUS.Continua;
		if (cont == BOT_ON || rej == BOT_ON || rejeita_montagem == ON )             // Oper. apertou algum botao
			botao_acionado = ON;
	}
	while(((Timer() - t) < 0.3) && (botao_acionado == OFF));// Espera apertar bot. ate 300ms

	if (botao_acionado	== OFF)
	{
		// Time out para botoeira
		ProcessDrawEvents();                                 // Desenha tela apos timeout
		return BOT_TIMEOUT;
	}

	//------------------------- Verifica qual botao, espera soltar ou sai se tempo > 1.9------
	cont = OFF;
	rej = OFF;
	t = Timer();
	while ((botao_acionado	== ON) && (Timer() - t < 1.9))
	{
		Delay_thread(0.1);
		botao_acionado = OFF;
		if (STATUS.Rejeita == BOT_ON  || rejeita_montagem == ON)   // Verifica botao de REJEITA
		{
			rej = REJ;
			botao_acionado = ON;
		}
		if (STATUS.Continua == BOT_ON)  // Verifica botao de CONT
		{
			cont = CONT;
			botao_acionado = ON;
		}
	}// Fim do laco while
	//-------------------- Retorna Botao Lido -------------------------------
	if(!status_debounc_presenca_produto && rejeitado)//Retirou o produto
		return FALHA_PRODUTO_AUSENTE;

	if (rej && cont)
		return CONT_REJ;
	else if (rej)
		return REJ;
	else if (cont)
		return CONT;
	else
		return BOT_OFF;
}

//
// Função: VerificaNoButton()
//
int VerificaNoButton(void)
{
	if(STATUS.Continua == OFF && STATUS.Rejeita == OFF)
		return SEM_ERRO;
	MENS_WARNING = MENS_BOT_TRAVADO;
	do
	{
		Delay_thread(0.05);
	}
	while((STATUS.Continua != OFF || STATUS.Rejeita != OFF) && status_emergencia && !sair_programa);
	if(STATUS.Continua == OFF && STATUS.Rejeita == OFF)
		return SEM_ERRO;
	return FALHA;
}

//
// Função: Delay_thread()
//
void Delay_thread(double segundos)
{
	double
		t = Timer();

	while(Timer() - t < segundos)
		ProcessSystemEvents ();
}

//
// Função: EnviaCommand_A_ToPIC()
//
int EnviaCommand_A_ToPIC ( int Add )
{
	int
		x,
		status;
	char
		aux_send[50],
		receive[100]= {0};

	///////////////////////////////////////////
	strcpy( aux_send, "A");
	strcat( aux_send, itostr( Add, 3 ));
	strcat( aux_send, "\r");
	x = strlen(aux_send);
	status = ComWrt( comm_sistema, aux_send, x );
	if (status != x )
	{
		global_error.status = -1;
		strcpy(global_error.global_error_desc, "Falha ao enviar string ao PIC");	
		strcpy(global_error.funcao_error_name, "ComWrt() - chamada por EnviaCommand_A_ToPIC()");
		GlobalErrDetect();
		return global_error.status;
	}		
	global_error.status = EsperaRetornoPIC(receive);
	if(global_error.status != SEM_ERRO)
	{
		global_error.status = -2;
		Fmt(global_error.funcao_error_name, "%s<%s%x ", "EsperaRetornoPIC() - chamado por EnviaCommand_A_ToPIC() - ADD=", Add);
		GlobalErrDetect();
		return global_error.status;
	}
	if(strcmp(receive,"A") != 0)
	{
		global_error.status = -3;
		strcpy(global_error.global_error_desc, "Resposta do PIC faltou carater 'A'");	
		Fmt(global_error.funcao_error_name, "%s<%s%x ", "EnviaCommand_A_ToPIC() - ADD=", Add);
		GlobalErrDetect();
		return global_error.status;
	}

	return SEM_ERRO;
}

//
// Função: EnviaCommand_C_ToPIC()
//
int EnviaCommand_C_ToPIC ( int Add )
{
	int
		x,
		status;
	char
		aux_send[50],
		receive[100]= {0};

	/////////////////////////////////////////
	strcpy( aux_send, "C");
	strcat( aux_send, itostr( Add, 3 ) );
	strcat( aux_send, "\r");
	x = strlen(aux_send);
	status = ComWrt( comm_sistema, aux_send, x );
	if (status != x )
	{
		global_error.status = -1;
		strcpy(global_error.global_error_desc, "Falha ao enviar string ao PIC");	
		strcpy(global_error.funcao_error_name, "ComWrt() - chamada por EnviaCommand_C_ToPIC()");
		GlobalErrDetect();
		return global_error.status;
	}		
	global_error.status = EsperaRetornoPIC(receive);
	if(global_error.status != SEM_ERRO)
	{
		global_error.status = -2;
		Fmt(global_error.funcao_error_name, "%s<%s%x ", "EsperaRetornoPIC() - chamado por EnviaCommand_C_ToPIC() - ADD=", Add);
		GlobalErrDetect();
		return global_error.status;
	}
	if(strcmp(receive,"C") != 0)
	{
		global_error.status = -3;
		strcpy(global_error.global_error_desc, "Resposta do PIC faltou carater 'C'");	
		Fmt(global_error.funcao_error_name, "%s<%s%x ", "EnviaCommand_C_ToPIC() - ADD=", Add);
		GlobalErrDetect();
		return global_error.status;
	}

	return SEM_ERRO;
}


//
// Função: EscreveMCP23017byPIC()
//
int EscreveMCP23017byPIC ( char *send_data, int *GP0, int *GP1 )
{
	int
		x,
		y,
		k=0,
		k_ant,
		status;
	char
		receive[50],
		aux_resp[20],
		strGP0[10],
		strGP1[10];

	///////////////////////////////////////
	strcat(send_data, "\r");
	x = strlen(send_data);
	status = ComWrt( comm_sistema, send_data, x );
	if (status != x )
	{
		global_error.status = -1;
		strcpy(global_error.global_error_desc, "Falha ao enviar string ao PIC");	
		strcpy(global_error.funcao_error_name, "ComWrt() - chamada por LeWORDfromMCP23017()");
		GlobalErrDetect();
		return global_error.status;
	}		
	global_error.status = EsperaRetornoPIC(receive);
	if(global_error.status != SEM_ERRO)
	{
		global_error.status = -2;
		strcpy(global_error.funcao_error_name, "EsperaRetornoPIC() - chamado por EscreveMCP23017byPIC()");
		GlobalErrDetect();
		return global_error.status;
	}
	k = FindPattern (receive, 0, -1, ";", 0, 0);
	if(k < 0)
	{
		global_error.status = -3;
		strcpy(global_error.global_error_desc, "Não encontrou Pattern ';' ");	
		strcpy(global_error.funcao_error_name, "FindPattern() - chamado por EscreveMCP23017byPIC()");
		GlobalErrDetect();
		return global_error.status;
	}
	if(k >= sizeof(aux_resp))
	{
		global_error.status = -4;
		strcpy(global_error.global_error_desc, "Index 'k' > tamanho da Target String ");	
		strcpy(global_error.funcao_error_name, "FindPattern() - chamado por EscreveMCP23017byPIC(1) - 1");
		GlobalErrDetect();
		return global_error.status;
	}
	CopyString (aux_resp, 0 , receive, 0, k);
	if(strcmp(aux_resp,"B") != 0 || strlen(receive)< 6)
	{
		global_error.status = -5;
		strcpy(global_error.global_error_desc, "Resposta do PIC curta ou faltou carater 'B'");	
		strcpy(global_error.funcao_error_name, "EscreveMCP23017byPIC()");
		GlobalErrDetect();
		return global_error.status;
	}
	k_ant=k+1;
	k = FindPattern (receive, k+1, -1, ";", 0, 0);
	if(k < 0)
	{
		global_error.status = -6;
		strcpy(global_error.global_error_desc, "Não encontrou Pattern ';' ");	
		strcpy(global_error.funcao_error_name, "FindPattern() - chamado por EscreveMCP23017byPIC() - 2");
		GlobalErrDetect();
		return global_error.status;
	}
	CopyString (strGP0, 0 , receive, k_ant, k-k_ant);
	k_ant=k+1;
	k = FindPattern (receive, k+1, -1, ";", 0, 0);
	if(k < 0)
	{
		global_error.status = -7;
		strcpy(global_error.global_error_desc, "Não encontrou Pattern ';' ");	
		strcpy(global_error.funcao_error_name, "FindPattern() - chamado por EscreveMCP23017byPIC() - 3");
		GlobalErrDetect();
		return global_error.status;
	}
	CopyString (strGP1, 0 , receive, k_ant, k-k_ant);
	Scan ( strGP0, "%s>%i", &x);
	if( x<0 ) 
		x += 256;
	*GP0 = x;
	Scan ( strGP1, "%s>%i", &y);
	if( y<0 ) 
		y += 256;
	*GP1 = y;
	
	return SEM_ERRO;
}


//
// Função: LeWORDfromMCP23017()
//
int LeWORDfromMCP23017 ( int Add, int *PortA, int *PortB )
{
	int
		status,
		x,
		y,
		k=0,
		k_ant;
	char
		aux_send[50],
		receive[50],
		aux_resp[10],
		strGP0[10],
		strGP1[10];

	///////////////////////////////////
	FlushInQ (comm_sistema);
	strcpy( aux_send, "D" );
	strcat( aux_send, itostr( Add, 3 ) );
	strcat( aux_send, "\r");
	x = strlen(aux_send);
	status = ComWrt( comm_sistema, aux_send, x );
	if (status == x )
	{
		global_error.status = EsperaRetornoPIC( receive );
		if(global_error.status != SEM_ERRO)
		{
			global_error.status = -1;
			strcpy(global_error.funcao_error_name, "EsperaRetornoPIC() - chamado por LeWORDfromMCP23017()");
			GlobalErrDetect();
			return global_error.status;
		}

		k = FindPattern (receive, 0, -1, ";", 0, 0);
		if(k < 0)
		{
			global_error.status = -2;
			strcpy(global_error.global_error_desc, "Não encontrou Pattern ';' ");	
			strcpy(global_error.funcao_error_name, "FindPattern() - chamado por LeWORDfromMCP23017()");
			GlobalErrDetect();
			return global_error.status;
		}
		if(k >= sizeof(aux_resp))
		{
			global_error.status = -3;
			strcpy(global_error.global_error_desc, "Index 'k' > tamanho da Target String ");	
			strcpy(global_error.funcao_error_name, "FindPattern() - chamado por LeWORDfromMCP23017(1) - 1");
			GlobalErrDetect();
			return global_error.status;
		}
		CopyString (aux_resp, 0 , receive, 0, k);
		if(strcmp(aux_resp,"R") != 0 || strlen(receive)< 6)
		{
			global_error.status = -4;
			strcpy(global_error.global_error_desc, "Resposta do PIC curta ou faltou carater 'R'");	
			strcpy(global_error.funcao_error_name, "LeWORDfromMCP23017()");
			GlobalErrDetect();
			return global_error.status;
		}
		
		k_ant=k+1;
		k = FindPattern (receive, k+1, -1, ";", 0, 0);
		if(k < 0)
		{
			global_error.status = -5;
			strcpy(global_error.global_error_desc, "Não encontrou Pattern ';' ");	
			strcpy(global_error.funcao_error_name, "FindPattern() - chamado por LeWORDfromMCP23017() - 2");
			GlobalErrDetect();
			return global_error.status;
		}
		CopyString (strGP0, 0 , receive, k_ant, k-k_ant);
		k_ant=k+1;
		k = FindPattern (receive, k+1, -1, ";", 0, 0);
		if(k < 0)
		{
			global_error.status = -6;
			strcpy(global_error.global_error_desc, "Não encontrou Pattern ';' ");	
			strcpy(global_error.funcao_error_name, "FindPattern() - chamado por LeWORDfromMCP23017() - 3");
			GlobalErrDetect();
			return global_error.status;
		}
		CopyString (strGP1, 0 , receive, k_ant, k-k_ant);
		Scan ( strGP0, "%s>%i", &x);
		if( x<0 ) 
			x += 256;
		Scan ( strGP1, "%s>%i", &y);
		if( y<0 ) 
			y += 256;
		*PortA = x;
		*PortB = y;
		return SEM_ERRO;
	
	}
	global_error.status = -7;
	strcpy(global_error.global_error_desc, "Falha ao enviar string ao PIC");	
	strcpy(global_error.funcao_error_name, "ComWrt() - chamada por LeWORDfromMCP23017()");
	GlobalErrDetect();
	return global_error.status;
}


//
// Função: EsperaBimanual()
//
int EsperaBimanual(void)
{
	if(STATUS.BiManual)
		return SEM_ERRO;
	COR_MENSAGENS= MENS_VERMELHO;
	strcpy(mens_montagem_2, "MANTENHA BIMANUAL PRESSIONADO");
	strcpy(mens_montagem_3, " ");
	do
	{
		Delay_thread(0.05);
	}
	while(!STATUS.BiManual && status_emergencia && !sair_programa);
	if(STATUS.BiManual)
		return SEM_ERRO;
	return FALHA;
}
// Função: EsperaRetornoPIC()
//
int EsperaRetornoPIC( char *bRetorno )
{
	int
		decimal;
	double
		t,
		time_out = 0.100;

	
	///////////////////////////////////////
	SetComTime (comm_sistema, time_out); //set timeout da COM
	decimal = 0;
	t = Timer();
	while( decimal == 0 &&  (Timer() - t < time_out) )
		decimal = GetInQLen ( comm_sistema);

	if ( decimal == 0 )
	{
		strcpy(global_error.global_error_desc, "Timeout - PIC não respondeu-1");	
		return -1;
	}
	decimal = ComRdTerm (comm_sistema, bRetorno, TAM_MAX_RESP_PIC, '\r' );
	if(decimal < (TAM_MAX_RESP_PIC-1))
	{
		bRetorno[decimal] = 0;
	}
	if(decimal < 0)
	{
		strcpy(global_error.global_error_desc, "Timeout - PIC não respondeu-2");	
		return -2;
	}
	if(strlen(bRetorno) >= (TAM_MAX_RESP_PIC-1))
	{
		strcpy(global_error.global_error_desc, "Resposta PIC excedeu tamanho do buffer");	
		return -3;
	}
	return SEM_ERRO;
}


//
// Função: MoveTravaClampProduto()
//
// Descrição: Utilizada para executar o movimento da Trava do Clamp.
//
int MoveTravaClampProduto (int acao)
{
	
	// == Inicializa variaveis ==
	double
		t=Timer(),
		timeOut = 3.0;


	if (acao == ON)
	{
		// Verifica se a Trava do Clamp já esta avançada 
		if ( STATUS.Trava_Avancado_1==OFF || STATUS.Trava_Avancado_2==OFF )
		{

			// Verificação de segurança
			if(modo_manual && motions_clear_to_move == OFF)
			{
				MessagePopup ("Condição incorreta", "Libere a cortina para mover a Trava do Clamp!");
				return FALHA;
			}
			while (motions_clear_to_move == OFF && status_emergencia && !sair_programa)
			{
				MENS_WARNING = MENS_CORTINA;
				Delay_thread(0.010);
			}
			if(!status_emergencia  || sair_programa)
				return EMERG_ACIONADA;
      
			t = Timer();

			//
			// Comando para avançar a Trava do Clamp
			//
			SAIDA.Sol_Trava_Produto = ON ; 
			 
			// Aguarda conclusão do avanço
			do
			{
				Delay_thread(0.010);
			}
			while( ( STATUS.Trava_Avancado_1==OFF || STATUS.Trava_Avancado_2==OFF ) && Timer()-t <= timeOut && status_emergencia && !sair_programa );

			// Tratamento de falhas
			if(!status_emergencia  || sair_programa)
				return EMERG_ACIONADA;

			if( Timer()-t > timeOut )
			{
			  if(modo_manual) 
				{	
					MessagePopup("Condição incorreta", "TimeOut na Trava do Clamp!");
				    return FALHA_TIMEOUT;
				}	
			  else
				return FALHA;
					

			}

			Delay_thread(0.010);
		//	SAIDA.Sol_Trava_Produto = OFF ; 
		    SAIDA.Sol_Trava_Produto = ON ;
		}
	}
	else if (acao == OFF)
	{
		// Verifica se a Trava do Clamp já esta recuada .
		if ( STATUS.Trava_Avancado_1==ON || STATUS.Trava_Avancado_2==ON ) 
		{
			// Verificação de segurança
			if(modo_manual && motions_clear_to_move == OFF)
			{
				MessagePopup ("Condição incorreta", "Libere a cortina para mover a Trava do Clamp!");
				return FALHA;
			}
			while (motions_clear_to_move == OFF && status_emergencia && !sair_programa)
			{
				MENS_WARNING = MENS_CORTINA;
				Delay_thread(0.010);
			}
			if(!status_emergencia  || sair_programa)
				return EMERG_ACIONADA;
			
			t = Timer();
			
			//
			// Comando para recuar a Trava do Clamp
			//
			SAIDA.Sol_Trava_Produto = OFF ; 
			 
			// Aguarda conclusão do recuo
			do
			{
				Delay_thread(0.010);
			}
			while (	( STATUS.Trava_Avancado_1==ON || STATUS.Trava_Avancado_2==ON ) && Timer()-t <= timeOut && status_emergencia && !sair_programa);

			// Tratamento de falhas  
			if(!status_emergencia  || sair_programa)
				return EMERG_ACIONADA;

			if( Timer()-t > timeOut )
			{
				if(modo_manual) 
				MessagePopup("Condição incorreta", "TimeOut na Trava do Clamp!");
				return FALHA_TIMEOUT;
			}

			SAIDA.Sol_Trava_Produto = OFF ;
			Delay_thread(0.010);
			
		}
	}

	// tratamento de falha por Timeout
	if(Timer() - t >timeOut)
		return FALHA;
	else
		return SEM_ERRO;
		
}
