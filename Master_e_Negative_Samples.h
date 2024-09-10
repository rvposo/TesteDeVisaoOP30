#define HORA_TURNO_1			7
#define HORA_TURNO_2			17
#define HORA_TURNO_3			3

#define	CAMINHO_SAMPLE  		"..\\Dados\\Samples\\"     //para leitura da sample  Positiva/Negativa
#define	ARQ_N_SAMPL_REGIST 		"NegSamplesRegistradas.txt"
#define	ARQ_SEQ_NSAM_SEM    	"NegSamplSequSemanal.txt"
#define STR_MAST_SAMP_OK		"MASTER-SAMPLE=OK"
#define STR_NEG_SAMP_OK			"NEGATIV-SAMPLE=OK"
#define MAX_NUM_NEG_SAMPLES		100
#define MAX_LEN_NSAMPLE			50
#define SEMANAL					1
#define DIARIO					2
#define STR_SERIAL_NEG_SAMPLE	"NSerie="
#define STR_NEG_SAMPLE_DIARIA   "=Diaria"
#define STR_NEG_SAMPLE_SEMANA   "=Semanal"
#define STR_DIA_ATUAL			"DiaAtual="
#define STR_DIA_1				"NegativeSamples-DIA-1"
#define STR_DIA_2				"NegativeSamples-DIA-2"
#define STR_DIA_3				"NegativeSamples-DIA-3"
#define STR_DIA_4				"NegativeSamples-DIA-4"
#define STR_DIA_5				"NegativeSamples-DIA-5"
#define STR_COMENT				"----------------------"


//#define VER_REG_NSAMPLE			ON

#define NSAMPLE_SEM_REG			-1
#define NSAMPL_ERR_TABS			-2
#define NSAMPLE_ARQ_REG			-3
#define NSAMPLE_ARQ_LER			-4

/***************** Enumeracao dos modos de gravação de imagens ********************/
enum 
{ 
	TESTE,
	MASTER_SAMPLE,
	NEGATIVE_SAMPLE
	
} amostra_tipo;

/***************** Enumeracao dos modos solicitação de samples ********************/
enum 
{ 
	DIARIA,
	TURNO,
	TURNO_RESET
	
} freq_solicit_samples;
/***************** Enumeracao dos modos de teste das samples ********************/
enum 
{ 
	NUNCA,
	SEMPRE,
	OPCIONAL
	
} modo_samples;
//************************************************************************************************
struct 
{
	char serial[30],
		 nome[MAX_LEN_NSAMPLE];
}negat_samples_registr[MAX_NUM_NEG_SAMPLES];
int neg_samples_a_ser_test[MAX_NUM_NEG_SAMPLES],
	neg_samples_semanal[MAX_NUM_NEG_SAMPLES];
int 
	cond_master_sample,
	negativ_sample_ok,
	master_sample_ok,
	criar_negat_sample,
	quant_neg_samp_regist,
	periodicidade,
	neg_samp_rejeitou,
	index_neg_samp_testando,
	verificado_freq_sample;
char ident_da_sample[200];	

int Le_NegSamp_a_testar(void) ;
int RegistSeqNegSamp_Semanal(void);
int DefSeqNegSample_DiariaSemanal(void);
int LerNegSamplesRegistradas(void);
int LerDiaAtual(int carregar, int indexar);
int	Verifica_Status_das_Samples(void);
void Grava_Hist_e_Status_das_Samples(int Max_Test);
int RegistraNegatSample(void);
int AutorizaCriarNegatSample(void);
int Gerenciar_Samples(void);
int AutorizaTestarNegSample(void);
int VerConsistNegatSample(void);
int LeMasterSample(void);  
int LeMasterSampleNegativo(void);
int	VerFreqSamples(void);
int	Deleta_Status_das_Samples(void);
int GravaTipoAtual(void);
