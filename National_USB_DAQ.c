
#include <utility.h>
#include <ansi_c.h>
#include <analysis.h>
#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include <stdio.h>  

#include "DAQ_MX.h"

#include "Main OP30 GEM.h"

int status = 0, 
	sampsPerChan_daq = 1000, 
	tamanho = 4000, 	//4000
	tamanho_real,
	acao_digital;

double 	time_out = 3,
		leituras[32000],   // 10000
		leituras_2[64000]; // 20000

int EscDigital6009(char *canal_string, uInt32 valor_escrever, int tipo_escrita);

//************************* Função  init_NI_daq ************************************************************************
int init_NI_daq (void)
{
	
	status = DAQmxCreateTask("",&entradas_analogicas_handler); 	
	status = DAQmxCreateTask("",&saidas_analogicas_handler);
	status = DAQmxCreateAIVoltageChan(entradas_analogicas_handler,canal_AD_DAQ_string,"",DAQmx_Val_Diff, -10, 10, DAQmx_Val_Volts,NULL);
	status = DAQmxCreateAOVoltageChan(saidas_analogicas_handler,canal_DA_DAQ_string,"", MIN_TENSAO_DA, MAX_TENSAO_DA, DAQmx_Val_Volts,NULL);
	
	return 0;
}
//************************* Função  stop_NI_daq ************************************************************************
int stop_NI_daq (void)
{
	DAQmxStopTask(entradas_analogicas_handler);
	DAQmxStopTask(saidas_analogicas_handler);
	DAQmxClearTask(entradas_analogicas_handler);
	DAQmxClearTask(saidas_analogicas_handler);
	return 0;
}

//************************* Função  init_NI_4461 ************************************************************************
int init_NI_4461 (void)
{
	stop_NI_4461();
	Delay(0.1);
	status = DAQmxCreateTask("",&entrada_4461_handler_0); 	
	status = DAQmxCreateTask("",&saida_4461_handler_0); 	
	status = DAQmxCreateAIVoltageChan(entrada_4461_handler_0,canal_AD_4461_0_string,"",DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts,NULL);
	status = DAQmxCreateAOVoltageChan(saida_4461_handler_0,canal_DA_4461_0_string,"", MIN_TENSAO_DA4461, MAX_TENSAO_DA4461, DAQmx_Val_Volts,NULL);

	status = DAQmxCreateTask("",&entrada_4461_handler_1); 	
	status = DAQmxCreateTask("",&saida_4461_handler_1); 	
	status = DAQmxCreateAIVoltageChan(entrada_4461_handler_1,canal_AD_4461_1_string,"",DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts,NULL);
	//status = DAQmxCreateAIVoltageRMSChan (entrada_4461_handler_1, canal_AD_4461_1_string, "", DAQmx_Val_Cfg_Default, -5.0, 5.0, DAQmx_Val_Volts, NULL);
	status = DAQmxCreateAOVoltageChan(saida_4461_handler_1,canal_DA_4461_1_string,"", MIN_TENSAO_DA4461, MAX_TENSAO_DA4461, DAQmx_Val_Volts,NULL);

	
	return 0;
}
//************************* Função  stop_NI_4461 ************************************************************************
int stop_NI_4461 (void)
{
	DAQmxStopTask(entrada_4461_handler_0);
	DAQmxStopTask(saida_4461_handler_0);
	DAQmxClearTask(entrada_4461_handler_0);
	DAQmxClearTask(saida_4461_handler_0);
	
	DAQmxStopTask(entrada_4461_handler_1);
	DAQmxStopTask(saida_4461_handler_1);
	DAQmxClearTask(entrada_4461_handler_1);
	DAQmxClearTask(saida_4461_handler_1);
	
	return 0;
}
//************************* Função  SetSampleClockRate ************************************************************************
int SetSampleClockRate(TaskHandle taskHandle, float64 desiredFreq, int32 sampsPerBuff, float64 cyclesPerBuff, float64 *desiredSampClkRate, float64 *sampsPerCycle, float64 *resultingSampClkRate, float64 *resultingFreq)
{
	
	*sampsPerCycle = sampsPerBuff / cyclesPerBuff;
	*desiredSampClkRate = desiredFreq * sampsPerBuff / cyclesPerBuff;
	status = DAQmxSetTimingAttribute(taskHandle,DAQmx_SampClk_Rate,*desiredSampClkRate);
	status = DAQmxGetTimingAttribute(taskHandle,DAQmx_SampClk_Rate,resultingSampClkRate);
	*resultingFreq = *resultingSampClkRate / *sampsPerCycle;
	return status;
}


//************************* Função  GenSineWave ************************************************************************
void GenSineWave(int numElements, double amplitude, double frequency, double *phase, double sineWave[])
{
	int i=0;

	for(;i<numElements;++i)
		sineWave[i] = sqrt(2)*amplitude*sin(PI/180.0*(*phase+360.0*frequency*i));
	*phase = fmod(*phase+frequency*360.0*numElements,360.0);
}
//************************* Função  CenterInRange ************************************************************************
void CenterInRange(const double inputArray[], int numElements, double upper, double lower, double outputArray[])
{
	int i=0;
	double	shift=(upper+lower)/2.0;
	
	for(;i<numElements;++i)
		outputArray[i] = inputArray[i] + shift;
}
//************************* Função  VmedioSEnd2canais ************************************************************************
double VmedioSEnd2canais (int canalA, int canalB)
{
double resA, resB, res;

	resA = 2*Vmedio_NIdaq_1ch(canalA, -5, 5, TERMIN_CONF_SING_ENDED);  //CANAL A  = sinal +
	resB = 2*Vmedio_NIdaq_1ch(canalB, -5, 5, TERMIN_CONF_SING_ENDED); //CANAL B = sinal -
	res  = resA - resB;
return res;
}
//************************* Função  Vmedio_NIdaq_1ch ************************************************************************
double Vmedio_NIdaq_1ch (int canal, double lim_min, double lim_max, int terminal_conf)
{
char canal_nome[100]={0};
double valor = 0;

		///DAQmxStopTask(entrada_anal_handler_vmedio);   
		DAQmxClearTask(entrada_anal_handler_vmedio);
		switch(canal)
		{
			case 0:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai0");
				strcpy(canal_nome,"canal_0");
				break;
			case 1:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai1");
				strcpy(canal_nome,"canal_1");
				break;
			case 2:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai2");
				strcpy(canal_nome,"canal_2");
				break;
			case 3:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai3");
				strcpy(canal_nome,"canal_3");
				break;
			case 4:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai4");
				strcpy(canal_nome,"canal_4");
				break;
			case 5:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai5");
				strcpy(canal_nome,"canal_5");
				break;
			case 6:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai6");
				strcpy(canal_nome,"canal_6");
				break;
			case 7:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai7");
				strcpy(canal_nome,"canal_7");
				break;
		}
		status = DAQmxCreateTask("",&entrada_anal_handler_vmedio);
		if(terminal_conf == TERMIN_CONF_SING_ENDED)
			status = DAQmxCreateAIVoltageChan(entrada_anal_handler_vmedio,canal_AD_DAQ_string, canal_nome, DAQmx_Val_RSE,lim_min,lim_max,DAQmx_Val_Volts,NULL);
		else
			status = DAQmxCreateAIVoltageChan(entrada_anal_handler_vmedio,canal_AD_DAQ_string, canal_nome, DAQmx_Val_Diff,lim_min,lim_max,DAQmx_Val_Volts,NULL);
		DAQmxReadAnalogF64 (entrada_anal_handler_vmedio, sampsPerChan_daq, time_out, DAQmx_Val_GroupByChannel, leituras, tamanho,
                &tamanho_real, NULL);
    	Mean (leituras, sampsPerChan_daq, &valor);
		DAQmxStopTask(entrada_anal_handler_vmedio);
		DAQmxClearTask(entrada_anal_handler_vmedio);
	return valor;
}



//************************* Função  Vmedio_daq6009_4ch ************************************************************************
int Vmedio_daq6009_4ch (double lim_min, double lim_max, double *canal0, double *canal1,double *canal2,double *canal3)
{
char canal_nome[100];
double valor;


		//ESTA FUNÇÃO ESTA COM BUG - NÃO É USADA NOS TESTES
		
		
		
		DAQmxStopTask(entradas_analogicas_handler);
		DAQmxClearTask(entradas_analogicas_handler);
		Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai0:3");
		strcpy(canal_nome,"canal_todos");
		status = DAQmxCreateTask("",&entradas_analogicas_handler); 	
		status = DAQmxCreateAIVoltageChan(entradas_analogicas_handler,canal_AD_DAQ_string, canal_nome, DAQmx_Val_Diff,lim_min,lim_max,DAQmx_Val_Volts,NULL);
		//DAQmxReadAnalogF64 (entradas_analogicas_handler, sampsPerChan, time_out, DAQmx_Val_GroupByChannel, leituras, tamanho,
        //        &tamanho_real, NULL);
		DAQmxReadAnalogF64 (entradas_analogicas_handler, 1000, 10, DAQmx_Val_GroupByChannel, leituras, 4000,
                &tamanho_real, NULL);
    	Mean (leituras, sampsPerChan_daq, &valor);
		*canal0 = valor;
		
		Subset1D (leituras, tamanho, sampsPerChan_daq, sampsPerChan_daq, leituras_2);
    	Mean (leituras_2, sampsPerChan_daq, &valor);
		*canal1 = valor;

		Subset1D (leituras, tamanho, sampsPerChan_daq*2, sampsPerChan_daq, leituras_2);
    	Mean (leituras_2, sampsPerChan_daq, &valor);
		*canal2 = valor;

		Subset1D (leituras, tamanho, sampsPerChan_daq*3, sampsPerChan_daq, leituras_2);
    	Mean (leituras_2, sampsPerChan_daq, &valor);
		*canal3 = valor;
		
		
		
		
		DAQmxStopTask(entradas_analogicas_handler);
		DAQmxClearTask(entradas_analogicas_handler);

	return 0;
}


//************************************ Atualiza_DA_DAQ6009 *************************************************				
int Atualiza_DA_DAQ6009 (double tensao_ch0, double tensao_ch1)
{
int32 samples_escrita_per_channel= 1;
bool32 auto_start = 1, data_layout = DAQmx_Val_GroupByChannel;
float64 time_out_escrita = 1,//1 segundo
		tensao[2];

	tensao[0] = tensao_ch0;
	tensao[1] = tensao_ch1;
	DAQmxWriteAnalogF64 (saidas_analogicas_handler, samples_escrita_per_channel, auto_start, time_out_escrita,
             data_layout, tensao, &tamanho_real, NULL);			   
	return 0;
}

//************************************ InitDigital6210 *************************************************				
int InitDigital6210(void)
{
	Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port1");
	status = DAQmxCreateTask("",&portas_digitais_handler); 	
	status = DAQmxCreateDOChan(portas_digitais_handler, canal_dig_string,"", DAQmx_Val_ChanForAllLines);
	
return 0;
}
//************************************ InitDigital6210 *************************************************				
int StopDigital6210(void)
{
	DAQmxStopTask(portas_digitais_handler);
	DAQmxClearTask(portas_digitais_handler);

return 0;
}
//************************************ EscDigital6210_Byte *************************************************				
int EscDigital6210_Byte(uInt32 valor_escrever)
{
int32 samples_escrita_per_channel= 1;
bool32 auto_start = 1, data_layout = DAQmx_Val_GroupByChannel;
float64 time_out_escrita = 1;//1 segundo 
int32	escrito; 
// uInt8 valor_bit;
	
	
	status = DAQmxWriteDigitalU32(portas_digitais_handler, samples_escrita_per_channel,
		auto_start, time_out_escrita, data_layout, &valor_escrever, &escrito, NULL);
	return 0;
}


//************************************ EscDigital6009_Byte *************************************************				
int EscDigital6009_Byte(int porta, uInt32 valor_escrever)
{
int32 samples_escrita_per_channel= 1;
bool32 auto_start = 1, data_layout = DAQmx_Val_GroupByChannel;
float64 time_out_escrita = 1;//1 segundo 
int32	escrito; 
// uInt8 valor_bit;
char canal_dig_string[100];
	
	if(porta == 0)
			Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0");
	else
			Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port1");
	
	status = DAQmxCreateTask("",&portas_digitais_handler); 	
	status = DAQmxCreateDOChan(portas_digitais_handler, canal_dig_string,"", DAQmx_Val_ChanForAllLines);
	status = DAQmxWriteDigitalU32(portas_digitais_handler, samples_escrita_per_channel,
		auto_start, time_out_escrita, data_layout, &valor_escrever, &escrito, NULL);
	DAQmxStopTask(portas_digitais_handler);
	DAQmxClearTask(portas_digitais_handler);
	return 0;
}
//************************************ EscDigital6009_Bit *************************************************				
int EscDigital6009_Bit(int porta, int linha, uInt32 valor_escrever)
{
int32 samples_escrita_per_channel= 1;
bool32 auto_start = 1, data_layout = DAQmx_Val_GroupByChannel;
float64 time_out_escrita = 1;//1 segundo 
int32	escrito; 
uInt8 valor_bit;
	
	if(porta == 0)
	{
		switch(linha)
		{
			case 0:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line0");
				break;
			case 1:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line1");
				break;
			case 2:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line2");
				break;
			case 3:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line3");
				break;
			case 4:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line4");
				break;
			case 5:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line5");
				break;
			case 6:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line6");
				break;
			case 7:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port0/line7");
				break;
		}
	}
	else
	{
		switch(linha)
		{
			case 0:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port1/line0");
				break;
			case 1:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port1/line1");
				break;
			case 2:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port1/line2");
				break;
			case 3:
				Fmt(canal_dig_string, "%s<%s%s",DAQ_string,"/port1/line3");
				break;
		}
	}
	status = DAQmxCreateTask("",&portas_digitais_handler); 	
	valor_bit = (uInt8)valor_escrever;
	status = DAQmxCreateDOChan(portas_digitais_handler, canal_dig_string,"", DAQmx_Val_ChanForAllLines);
	status = DAQmxWriteDigitalLines(portas_digitais_handler, samples_escrita_per_channel,
		auto_start, time_out_escrita, data_layout, &valor_bit, &escrito, NULL);
	DAQmxStopTask(portas_digitais_handler);
	DAQmxClearTask(portas_digitais_handler);
	return 0;
}


//***********************************double *FormaOnda_NI_daq_1ch *************************************************				
int FormaOnda_NI_daq_1ch (double lim_min, double lim_max,int canal, int leit_por_seg, int num_leit, double *leituras)
{
char canal_nome[100];
//int32       error=0;
TaskHandle  taskHandle=0;
int32       numRead;
uInt32      numChannels;
// int         log;

		resultingSampClkRate = leit_por_seg;
		
		switch(canal)
		{
			case 0:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai0");
				strcpy(canal_nome,"canal_0");
				break;
			case 1:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai1");
				strcpy(canal_nome,"canal_1");
				break;
			case 2:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai2");
				strcpy(canal_nome,"canal_2");
				break;
			case 3:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai3");
				strcpy(canal_nome,"canal_3");
				break;
			case 4:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai4");
				strcpy(canal_nome,"canal_4");
				break;
			case 5:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai5");
				strcpy(canal_nome,"canal_5");
				break;
			case 6:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai6");
				strcpy(canal_nome,"canal_6");
				break;
			case 7:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai7");
				strcpy(canal_nome,"canal_7");
				break;
			default:
				Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai7");
				strcpy(canal_nome,"canal_7");
				break;
		}
	
		status=DAQmxCreateTask("",&taskHandle);
		if (status <0)
			return status;
		
		status=DAQmxCreateAIVoltageChan(taskHandle,canal_AD_DAQ_string,canal_nome,DAQmx_Val_Cfg_Default,lim_min,lim_max,DAQmx_Val_Volts,NULL);
		if (status <0)
			return status;
	
		status=DAQmxCfgSampClkTiming(taskHandle,"",leit_por_seg, DAQmx_Val_Rising,DAQmx_Val_FiniteSamps,num_leit);
		if (status <0)
			return status;
	
		status=DAQmxGetTaskAttribute(taskHandle,DAQmx_Task_NumChans,&numChannels);
		if (status <0)
			return status;
	
		status=DAQmxStartTask(taskHandle);
		if (status <0)
			return status;
	
		status=DAQmxReadAnalogF64(taskHandle,num_leit,time_out,DAQmx_Val_GroupByChannel,leituras,leit_por_seg,&numRead,NULL);
		if (status <0)
			return status;

		if( taskHandle!=0 ) 
		{
			DAQmxStopTask(taskHandle);
			DAQmxClearTask(taskHandle);
	  	}

		return 0;
}
//************************************int FormaOnda_daq6009_4ch *************************************************				
int FormaOnda_daq6009_4ch (double lim_min, double lim_max, int leit_por_seg,
	int num_leit, double *canal0, double *canal1, double *canal2,double *canal3) 
{
char canal_nome[100];
		strcpy(canal_AD_DAQ_string, "Dev1/ai0:3");
		strcpy(canal_nome,"canal_todos");
		status = DAQmxCreateTask("",&entradas_analogicas_handler); 	
		status = DAQmxCreateAIVoltageChan(entradas_analogicas_handler,canal_AD_DAQ_string, canal_nome, DAQmx_Val_Diff,lim_min,lim_max,DAQmx_Val_Volts,NULL);
		DAQmxReadAnalogF64 (entradas_analogicas_handler, leit_por_seg, time_out, DAQmx_Val_GroupByChannel, leituras, num_leit,
                &tamanho_real, NULL);
return 0;
}

//************************* Função  Inicia8canaisAD ************************************************************************
int Inicia8canaisAD (void)
{
char canal_nome[100];
//double valor;

		SetBreakOnLibraryErrors (0);
		status=DAQmxStopTask(entradas_analogicas_handler);
		status=DAQmxClearTask(entradas_analogicas_handler);
		Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"Dev1/ai0:3");  // ai0:7
		strcpy(canal_nome,"canal_todos");
		status = DAQmxCreateTask("",&entradas_analogicas_handler); 
		status = DAQmxCreateAIVoltageChan(entradas_analogicas_handler, canal_AD_DAQ_string, "", DAQmx_Val_Diff, -10.0, 10.0, DAQmx_Val_Volts, NULL ) ;

	return status;
}


//************************* Função  Vmedio_daq6210_8ch ************************************************************************
int LeRapido_daq6210_8ch (double lim_min, double lim_max, double *canal0, double *canal1,double *canal2,double *canal3
	,double *canal4, double *canal5, double *canal6, double *canal7)
{
double valor;
int quantos=100;
		DAQmxReadAnalogF64 (entradas_analogicas_handler, quantos, time_out, DAQmx_Val_GroupByChannel, leituras, quantos*8,
                &tamanho_real, NULL);
    	Mean (leituras, quantos, &valor);
		*canal0 = valor;
		Subset1D (leituras, tamanho*8, quantos, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal1 = valor;

		Subset1D (leituras, tamanho*8, quantos*2, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal2 = valor;

		Subset1D (leituras, tamanho*8, quantos*3, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal3 = valor;
		
		Subset1D (leituras, tamanho*8, quantos*4, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal4 = valor;
		
		Subset1D (leituras, tamanho*8, quantos*5, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal5 = valor;
		
		Subset1D (leituras, tamanho*8, quantos*6, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal6 = valor;
		
		Subset1D (leituras, tamanho*8, quantos*7, quantos, leituras_2);
    	Mean (leituras_2, quantos, &valor);
		*canal7 = valor;
	return 0;
}

//************************* Função  IniciaCanaisAD_Corrente ************************************************************************
int IniciaCanaisAD_Corrente (void)
{
char canal_nome[100];
//double valor;

		SetBreakOnLibraryErrors (0);
		DAQmxStopTask(entradas_analogicas_handler);
		DAQmxClearTask(entradas_analogicas_handler);
		Fmt(canal_AD_DAQ_string, "%s<%s%s",DAQ_string,"/ai0");
		strcpy(canal_nome,"canal_corrente");
		status = DAQmxCreateTask("",&entradas_analogicas_handler); 	
		status = DAQmxCreateAIVoltageChan(entradas_analogicas_handler,canal_AD_DAQ_string, canal_nome, DAQmx_Val_Diff,-5,5,DAQmx_Val_Volts,NULL);

	return status;
}


//************************* Função  FechaCanaisAD ************************************************************************
int FechaCanaisAD (void)
{
int status=0 ;	
//char canal_nome[100];
//double valor;

		SetBreakOnLibraryErrors (0);
		DAQmxStopTask(entradas_analogicas_handler);
		DAQmxClearTask(entradas_analogicas_handler);
		SetBreakOnLibraryErrors (1);

	return status;
}
//************************* Função  LeRapido_daq6210_Corrente ************************************************************************
int LeRapido_daq6210_Corrente (double lim_min, double lim_max, double *canal)
{
double valor;
int quantos = 10; //100;

		DAQmxReadAnalogF64 (entradas_analogicas_handler, quantos, time_out, DAQmx_Val_GroupByChannel, leituras, quantos,
                &tamanho_real, NULL);
		if(tamanho_real != quantos)
		{
		  canal=0;
		  return -1;
		}
    	Mean (leituras, quantos, &valor);
	    valor = (valor * 2) - offset_daq_c0;
	    valor = valor * 1000; //mA
		*canal = valor;
	
		return 0;
}


//************************************int LerDigital6009_Bit *************************************************				
int LerDigital6009_Bit (int porta,int *bits_lidos )
{
//int	bit_escrever,tamanho = 100;
uInt8  bits_digitais[100];
int32 samples_escrita_per_channel= 1, samples_per_channel_read, bytesPerSamp;
float64 time_out_escrita = 1;//1 segundo

	status = DAQmxCreateTask("",&portas_digitais_handler); 	
	if(porta == 0)
	{
		status = DAQmxCreateDIChan(portas_digitais_handler , "Dev1/port0/line0:7", "",DAQmx_Val_ChanForAllLines);
	}
	else
	{
		status = DAQmxCreateDIChan(portas_digitais_handler , "Dev1/port1/line0:3", "",DAQmx_Val_ChanForAllLines);
	}
	status = DAQmxReadDigitalLines(portas_digitais_handler, samples_escrita_per_channel,
		time_out_escrita,DAQmx_Val_GroupByChannel,bits_digitais,tamanho ,&samples_per_channel_read,&bytesPerSamp,NULL);
	if(porta == 0)
	{
			bits_lidos[0] = bits_digitais[0];
			bits_lidos[1] = bits_digitais[1];
			bits_lidos[2] = bits_digitais[2];
			bits_lidos[3] = bits_digitais[3];
			bits_lidos[4] = bits_digitais[4];
			bits_lidos[5] = bits_digitais[5];
			bits_lidos[6] = bits_digitais[6];
			bits_lidos[7] = bits_digitais[7];
	}
	else
	{
			bits_lidos[0] = bits_digitais[0];
			bits_lidos[1] = bits_digitais[1];
			bits_lidos[2] = bits_digitais[2];
			bits_lidos[3] = -1;
			bits_lidos[4] = -1;
			bits_lidos[5] = -1;
			bits_lidos[6] = -1;
			bits_lidos[7] = -1;
	}
	DAQmxStopTask(portas_digitais_handler);
	DAQmxClearTask(portas_digitais_handler);
 return 0;
}
