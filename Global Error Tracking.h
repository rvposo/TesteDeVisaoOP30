
#ifndef OFF
#define OFF	    					0
#endif /* OFF */

#ifndef ON
#define ON	    					1
#endif /* ON */

#ifndef off
#define off	    					0
#endif /* off */

#ifndef SEM_ERRO
#define SEM_ERRO	    			0
#endif 

#define CAMINHO_ERR_LOGS			"..\\Dados\\ErrLogs\\"

#define TAMANHO_MAX_DESC_ERROR	200
#define MAX_ACUMUL_CONSEC_ERR	5

#define CheckI2C_err			if(status == SEM_ERRO) i2c_sist_err_quant=0;else {i2c_sist_err_quant++;if(i2c_sist_err_quant > MAX_ACUMUL_CONSEC_ERR)global_error.i2c_sistem_err = ON;}
/*if(status == SEM_ERRO) 
	i2c_sist_err_quant=0;
else
{
	i2c_sist_err_quant++;
	if(i2c_sist_err_quant > MAX_ACUMUL_CONSEC_ERR)
		global_error.i2c_sistem_err = ON;	
}*/

typedef enum   
{
	ERR_TRACK_GRAVA,
	ERR_TRACK_WARNING,
	ERR_TRACK_NOTHING
} 
ERR_TRACK_MODE;
ERR_TRACK_MODE err_track_mode;

struct Global_Error_Descript
{
	char 
		global_error_desc[TAMANHO_MAX_DESC_ERROR+1],
		funcao_error_name[TAMANHO_MAX_DESC_ERROR+1];
	int
		status,
		i2c_sistem_err;
} global_error;

/********* variaveis ***********/
int 
	grava_err_desc;

/********* funções ***********/
int ConfigGlobal_ErrorTrack(ERR_TRACK_MODE track_mode);
int GlobalErrDetect(void);



