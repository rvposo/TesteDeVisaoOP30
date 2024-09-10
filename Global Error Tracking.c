#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include "Global Error Tracking.h"
static int 
	arquivo_global_err = 0;


/******************** ConfigGlobal_ErrorTrack() *************************************/
int ConfigGlobal_ErrorTrack(ERR_TRACK_MODE track_mode)
{
	int
		dia,
		mes,
		ano,
		status = SEM_ERRO,
		arq_pres;
	char
		buf[100],
		mens[200];
	long 
		size;
		
	grava_err_desc = track_mode; 
	global_error.i2c_sistem_err = OFF;
	if(arquivo_global_err != 0)
	{
		SetBreakOnLibraryErrors (OFF);
		CloseFile (arquivo_global_err);
		SetBreakOnLibraryErrors (ON);
	}
	switch(grava_err_desc)
	{
		case ERR_TRACK_GRAVA:
			GetSystemDate ( &mes, &dia, &ano);// faz o nome do arquivo 
			sprintf(buf,"%sErr_%d-%d-%d.txt",CAMINHO_ERR_LOGS, dia,mes,ano);
			arq_pres = GetFileInfo(buf , &size);
			if (arq_pres == 1)
			  arquivo_global_err  = OpenFile(buf , VAL_WRITE_ONLY , VAL_APPEND , VAL_ASCII );
			else   
			  arquivo_global_err  = OpenFile(buf , VAL_WRITE_ONLY , VAL_OPEN_AS_IS , VAL_ASCII );
			if(arquivo_global_err < 0)
			{
				strcpy(mens, "Falha ao tentar abrir arquivo de logs de falhas - Arquivo=" );
				strcat(mens, buf);
				MessagePopup ("Erro ao tentar abrir arquivo de logs de falhas", mens);
				status = -1;
			}
			break;
		case ERR_TRACK_WARNING:
			break;
		case ERR_TRACK_NOTHING:
			break;
	}
	
	return status;
}

/*********************** GlobalErrDetect() ******************************************/
int GlobalErrDetect(void)
{
char 
	desc_err[(TAMANHO_MAX_DESC_ERROR *2)+50],
	*hora;

	switch(grava_err_desc)
	{
		case ERR_TRACK_GRAVA:
			hora = TimeStr ();
			Fmt (desc_err, "%s<%s; Função=%s;retornou %i; %s",hora, global_error.funcao_error_name, global_error.status, global_error.funcao_error_name);
			WriteLine (arquivo_global_err, desc_err, strlen(desc_err));
			strcpy(global_error.global_error_desc, "");	
			strcpy(global_error.funcao_error_name, "");
			break;
		case ERR_TRACK_WARNING:
			break;
		case ERR_TRACK_NOTHING:
			break;
	}
	return SEM_ERRO;
}
