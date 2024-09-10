
/*
typedef struct
{
	long hi,lo;
}int64,uInt64;
*/





#include "NIDAQmx.h"

#define MIN_TENSAO_DA		0
#define	MAX_TENSAO_DA		5
#define MIN_TENSAO_DA4461	-10
#define	MAX_TENSAO_DA4461	10
#define MIN_TENSAO_AD		-10
#define	MAX_TENSAO_AD		+10
#define PI					3.1415926535
//#define CYCLES_PER_BUFFER	5
///#define SAMPLE_RATE_PADRAO	48000.0 
///#define SAMP_PER_CHANN_IN	48000   

#define SAMPLE_NUMBER	48000.0    
#define SAMPLE_RATE	48000 

#define SAMP_PER_CHANN_OUT	1000
#define QTDADE_HARMONICAS	10

#define  FREQUENCIA_PADRAO	   1000
#define  CICLO_POR_BUFFER	   3


#define FREQ_MIN_MONITORAR  10 //10Hz
#define FREQ_MAX_MONITORAR  22000 //22KHz


#define TERMIN_CONF_DIFERENCIAL		0
#define TERMIN_CONF_SING_ENDED		1

#define	DAQ_6009			1
#define	DAQ_6210			2
#define	METER_4065			3


char 	canal_AD_DAQ_string[256],
		canal_DA_DAQ_string[256],
	 	canal_AD_4461_0_string[256],
		canal_DA_4461_0_string[256],
	 	canal_AD_4461_1_string[256],
		canal_DA_4461_1_string[256],
		canal_dig_string[256],
		DAQ_string[256],
		_4461_string[256];


TaskHandle			entradas_analogicas_handler, 
					entrada_anal_handler_vmedio, 
	 				saidas_analogicas_handler,
					portas_digitais_handler,
					entrada_4461_handler_0,
					entrada_4461_handler_1,
					saida_4461_handler_0,   
					saida_4461_handler_1;   


int init_NI_daq (void);//inicializa o módulo USB DAQ 9000
int stop_NI_daq (void);//encerra e libera handlers 
int init_NI_4461 (void);
int stop_NI_4461 (void);//encerra e libera handlers 
void GenSineWave(int numElements, double amplitude, double frequency, double *phase, double sineWave[]);
int Saida_4461(int canal, double frequencia, double amplitude);
int SetSampleClockRate(TaskHandle taskHandle, float64 desiredFreq, int32 sampsPerBuff, float64 cyclesPerBuff, float64 *desiredSampClkRate, float64 *sampsPerCycle, float64 *resultingSampClkRate, float64 *resultingFreq);
int Ler_4461(int sincronizar, int canal, double  *level, double *freq, double *distorcao);

double Vmedio_NIdaq_1ch //le e retorna o valor da média de tensão DC presente em um canal de A/D
	(int canal, double lim_min, double lim_max, int terminal_conf);
int Vmedio_daq6009_4ch//le o valor da média de tensão DC presente nos 4 primeiros canais de A/D. Retorna o erro 
	(double lim_min, double lim_max, double *canal0, double *canal1, double *canal2,double *canal3); 

int FormaOnda_NI_daq_1ch //le e retorna a forma de onda presente em um canal de A/D
(double lim_min, double lim_max,int canal, int leit_por_seg, int quant_leit, double *leituras);

int FormaOnda_daq6009_4ch //le a forma de onda presente nos 4 primeiros canais de A/D. Retorna o erro
(double lim_min, double lim_max,int leit_por_seg, int num_leit,
	 double *canal0, double *canal1, double *canal2,double *canal3); 

int Atualiza_DA_DAQ6009 (double tensao_ch0, double tensao_ch1);
int EscDigital6009_Byte(int porta, uInt32 valor_escrever);
int EscDigital6009_Bit(int porta, int linha, uInt32 valor_escrever);
int LerDigital6009_Bit (int porta,int *bits_lidos );
int EscDigital6210_Byte(uInt32 valor_escrever);
int InitDigital6210(void);
int StopDigital6210(void);
int modelo_daq;
int FechaCanaisAD (void);
int IniciaCanaisAD_Corrente (void);
int LeRapido_daq6210_Corrente (double lim_min, double lim_max, double *canal);
float64 resultingSampClkRate;
int Inicia8canaisAD (void) ;
int LeRapido_daq6210_8ch (double lim_min, double lim_max, double *canal0, double *canal1,double *canal2,double *canal3
	,double *canal4, double *canal5, double *canal6, double *canal7) ;
