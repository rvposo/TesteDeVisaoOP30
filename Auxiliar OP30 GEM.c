#include <windows.h>
#include <ansi_c.h>
#include <utility.h>
#include <formatio.h>
#include <mmsystem.h>
#include "Mensagens.h"
#include "Main OP30 GEM.h"
#include "Auto OP30 GEM.h"
#include "Vision Scripts OP30 Cluster GEM.h"
#include "Master_e_Negative_Samples.h"

//
// Função: LeConfiguracoes()
//
int LeConfiguracoes(void)
{
	char
		resposta[100],
		configuracao[50],
		tmp[90];
	int
		file,
		i,
		tamanho;

	cortina_luz_presente 	= OFF;
	succao_presente			= OFF;
	prensa_shield_presente	= OFF;
	/////////////// Le arquivo de configuração ///////////////
	file = OpenFile( ARQ_CONFIG_MAQ, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII );
	if (file < 0)
	{
		MessagePopup ( " Erro Configuração", "Falha ao abrir arquivo de configuração." );
		return -1;
	}
	for(i=0; i < 100; i++) //le até 100 linhas
	{
		if(ReadLine (file, tmp, 80)<0)//fim do arquivo
			break;
		strcpy (resposta, tmp);
		tamanho = strlen(resposta);
		if (!tamanho) continue; //continua no loop se string for vazia
		StringUpperCase (resposta);
		if(strcmp(resposta, "FIM") == 0)
			break;
		else if(strstr (resposta, "EM_DEBUG=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, 9, 3);
			Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "ON") == 0)
				em_debug = ON;
			else
				em_debug = OFF;
		}
		else if(strstr (resposta, "MONITOR=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, 23, 3);
			Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "A") == 0)
				tipo_monitor = MONITOR_A;
			else
				tipo_monitor = MONITOR_B;
		}
		else if (strstr (resposta, "COMM_SIST=") != NULL)
		{
			CopyString (configuracao, 0, resposta, 10, 2);
			comm_sistema = atoi(configuracao);
		}
		else if (strstr (resposta, "COMM_SCANNER=") != NULL)
		{
			CopyString (configuracao, 0, resposta, 13, 2);
			comm_scanner = atoi(configuracao);
		}
			else if(strstr (resposta, "FREQ_SOLIC_SAMPLE=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, 18, -1);
 	   		Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "DIARIA") == 0)
				freq_solicit_samples = DIARIA;
			else if(strcmp(configuracao, "TURNO_RESET") == 0)
				freq_solicit_samples = TURNO_RESET;
			else
				freq_solicit_samples = TURNO;
		}

		else if(strstr (resposta, "SAMPLES=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, 8, -1);
 	   		Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "OPCIONAL") == 0)
				modo_samples = OPCIONAL;
			else
				modo_samples = SEMPRE;
		}
		else if(strstr (resposta, "CORTINA_DE_LUZ=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, strlen("CORTINA_DE_LUZ="), -1);
 	   		Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "ON") == 0)
				cortina_luz_presente = ON;
			else
				cortina_luz_presente = OFF;
		}
		else if(strstr (resposta, "SUCCAO_PRESENTE=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, strlen("SUCCAO_PRESENTE="), -1);
 	   		Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "ON") == 0)
				succao_presente = ON;
			else
				succao_presente = OFF;
		}
		else if(strstr (resposta, "PRENSA_SHIELD_PRESENTE=")!= NULL)
		{
			CopyString (configuracao, 0, resposta, strlen("PRENSA_SHIELD_PRESENTE="), -1);
 	   		Scan(configuracao, "%s>%s", configuracao);//elimina tab ou espaco apos a string
			if(strcmp(configuracao, "ON") == 0)
				prensa_shield_presente = ON;
			else
				prensa_shield_presente = OFF;
		}
	}
	CloseFile (file);
	return 0;
}


//
// Função: long val(char *str, int radix)
//
// Descrição: Converte uma string em inteira longa
// Requerimento: char *str ( string a ser convertida )
//			int radix ( define se o valor da string e base 10 ou base 16 )
//
// Retorno:	Retorna o valor da string convertida.
// Ex:		char str[20];
//			long	valor;
//			valor = val( "2367", 10 );
//			após função :    valor = (2367)
//
long val(char *str, int radix)
{
	char 
		*aux;
	return ( strtol( str, &aux, radix ));
}

//
// Função: CINT
//
int CVICALLBACK CINT ( double V )
{
	int 
		r;

		r =  (int)(V*10) - ((int)V)*10;
	if ( r >= 5 )
	{						 
		return( (int)V + 1 );
	}
	else
	{
		if ( r <= -5 )
		{
			return( (int)V -1 );
		}
	}
	return( (int)V );
}

//
// Função: MOD()
//
int MOD (double a, double b)
{
	double 
		c,
		d,
		e,
		fracao,inteiro;
	int
		ho;

	c = a / b;
	fracao = modf (c, &inteiro);
	if (fracao < 0)
	{
		d = 1 + fracao;
		e = c - d;
	}
	else
	{
		e = inteiro;
	}
	ho = a - b * e;
	return ho;
}

//
// Função: itostr ( int number, unsigned int casas )
//
// Descrição: Converte um número em uma string formatada
// Requerimento:int number ( número a ser convertido )
//  			unsigned int casas ( número de casas da formatação )
// Retorno: 	string no formato  000xxx  onde xxx é um número de 3 caracteres
// Ex:			char *string, str[20];
//  			string = itostr( 12601, 7 );
//  			strcpy( str, itostr( 3, 2 ) );
//  			após função:  string = ("0012601")  str = ("03")
//
char *itostr( int number, unsigned int casas )
{
	int
		n,
		i = 0;
	char
		aux[20]="";

		if( casas > 20 ) casas = 20;
	n = (int)log10(abs(number)) + 1;
	if( n >= casas )
	{
		sprintf( resp_itostr, "%d", number );
		strcpy( resp_itostr, aux );
		i = strlen(resp_itostr);
		*(resp_itostr+i) = '\0';
		return (resp_itostr);
	}
	sprintf( aux, "%d", abs(number) );
	if( number < 0 )
		strcpy( resp_itostr, "-0" );
	else
		strcpy( resp_itostr, "0" );
	for(i=1; i < (int)(casas-n); i++ )
	{
		strcat( resp_itostr, "0" );
	}
	strcat( resp_itostr, aux );
	i = strlen(resp_itostr);
	*(resp_itostr+i) = '\0';
	return( resp_itostr );
}

//
// Função: instr()
//
int CVICALLBACK instr( unsigned int count, char *str, char ch)
{
	unsigned int
		i,
		lenght;

	lenght = strlen(str);
	if( count > lenght)
	{
		return(0);
	}
	for(i=count; i<=lenght; i++)
	{
		if( *(str+i) == ch)
			return(++i);
	}
	return(0);
}

//
// Função: Get_Pos()
//
int Get_Pos(char *Source , int ini , char car)
{
	char
		*ptr;
	int
		pos;

		Source = Source + ini - 1;
	ptr = strchr(Source , car);
	pos = ptr - Source + ini;
	return (pos);
}

//
// Função:	char *StringMid(char *str, unsigned int inicio, char ch, unsigned int *count)
// Descrição: 		Copia os caracteres de uma string da esquerda até o caracter 'ch'
// Requerimento:  	char *str (string a ser analisada )
//  				unsigned int inicio (posição inicial na string a ser copiada)
//  				char ch (caractere de posição de parada )
//	 				unsigned int *count (contém a posição do caracter ch na string)
// Retorno : 		Retorna os caracteres da string entre inicio e quantos
// Ex :				char str[20] = "abcde,fgh";
//  				char *string;
//  				unsigned int c;
//  				string = StringMid( str, 1, comma , &c );
//  				após função :    string = ("abcde")  c = (6)
//
char *StringMid(  char *str, unsigned int inicio, char ch, unsigned int *count)
{
	unsigned int
		i,
		j = 0,
		lenght;

	lenght = strlen(str);
	*(resp_str_mid) = '\0';
	if( lenght == 0 || lenght <= inicio )
		return (str);
	if( inicio < 1 )
		inicio = 1;
	for( i=inicio-1; i < lenght; i++ )
	{
		if( *(str+i) !=  ch )
		{
			*(resp_str_mid+j) = *(str+i);
			*(resp_str_mid+j+1) = '\0';
			j++;
		}
		else
		{
			break;
		}
	}
	*(resp_str_mid+j) = '\0';
	if( j+inicio >= lenght )
		*count = 0;
	else
		*count = j + inicio;
	return ( resp_str_mid );
}

//
// Função: Mensagem()
//
void Mensagem(char *Mens1, char *Mens2, char *Mens3,char *Mens4, int cor)
{
	switch(cor)
	{
		case MENS_VERMELHO:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,VAL_RED);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, VAL_WHITE);
			break;
		case MENS_AMARELO:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,VAL_YELLOW);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, MENS_PRETO);
			break;
		case MENS_AZUL:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,VAL_BLUE);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, VAL_WHITE);
			break;
		case MENS_PRETO:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,MENS_PRETO);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, VAL_WHITE);
			break;
		case MENS_CINZA:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,VAL_DK_GRAY);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, VAL_WHITE);
			break;
		case MENS_VERDE:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,VAL_DK_GREEN);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, VAL_WHITE);
			break;
		default:
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_COLOR,VAL_DK_GRAY);
			SetCtrlAttribute (painel_auto, AUTO_TXT_MENSAGEM, ATTR_TEXT_BGCOLOR, VAL_WHITE);
			break;
	}
	ResetTextBox (painel_auto, AUTO_TXT_MENSAGEM, "" ) ;
	InsertTextBoxLine ( painel_auto, AUTO_TXT_MENSAGEM, 0, Mens1);
	InsertTextBoxLine ( painel_auto, AUTO_TXT_MENSAGEM, 1, Mens2);
	InsertTextBoxLine ( painel_auto, AUTO_TXT_MENSAGEM, 2, Mens3);
	InsertTextBoxLine ( painel_auto, AUTO_TXT_MENSAGEM, 3, Mens4);
	ProcessDrawEvents ();
}

//
// Função: CodigoData()
//
// Descrição: Retorna o codigo de data no formato horas passadas da data inicial
//
int CodigoData(int datainicio)
{
	int y,
		d,
		m,
		hh,
		min,
		s,
		h,
		anobisexto,
		ajuste,
		status,
		correcao = 0;

	status = GetSystemDate (&m, &d, &y);
	if(status<0)
		return FALHA;
	status = GetSystemTime (&h, &min, &s);
	if(status<0)
		return FALHA;
	switch(m)
	{
		case 1:
			ajuste = 1;
			break;
		case 2:
			ajuste = -2;
			break;
		case 3:
			ajuste = 1;
			break;
		case 4:
			ajuste = 0;
			break;
		case 5:
			ajuste = 1;
			break;
		case 6:
			ajuste = 0;
			break;
		case 7:
			ajuste = 1;
			break;
		case 8:
			ajuste = 1;
			break;
		case 9:
			ajuste = 0;
			break;
		case 10:
			ajuste = 1;
			break;
		case 11:
			ajuste = 0;
			break;
		case 12:
			ajuste = 1;
			break;
		default:
			ajuste = 0;
			break;
	}
	if ((fmod(y,4) == 0) && (m > 2))
	{
		anobisexto = 1;
	}
	else
	{
		anobisexto = 0;
	}
	correcao = (y - 1900 - datainicio) * 365 * 24;
	hh = ((d + ajuste + anobisexto + 30*(m - 1))*24) + h + correcao;
	if(hh > 0xFFFF) return -1; //precisa ajustar a data de inicio
	return hh;
}

//
// Função:Decimal_to_Hex()
//
void Decimal_to_Hex( int Decimal, char *Hex )
{
	int
		resto,
		dividendo;
	char
		aux[10];

	if( Decimal < 0 )
		Decimal = Decimal + 256;
	resto = Decimal/16;
	dividendo = Decimal - (resto * 16);
	if( resto > 9 )
		resto = resto + 55;
	else
		resto = resto + 48;
	Fmt( Hex, "%c", resto );
	if( dividendo > 9 )
		dividendo = dividendo + 55;
	else
		dividendo = dividendo + 48;
	Fmt( aux, "%c", dividendo );
	strcat( Hex, aux );
}
//
// Função: IsOnlyNumeric ()
//
int IsOnlyNumeric ( char *str )
{
	unsigned int
		i,
		lenght,
		flag=0;

	lenght = strlen(str);
	for( i=0; i < lenght; i++ )
	{
		if( (*(str+i) <  '0' || *(str+i) >  '9') && (*(str+i) != '.' ) )
		{
			if( i==0 || i ==(lenght-1) )
			{
				if( *(str+i) != ' ' )
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			if( *(str+i) == '.' )
			{
				if(flag == 0)
					flag = 1;
				else
					return 1;
			}
		}
	}
	return 0;
}

//
// Função: Data_e_Hora()
//
void Data_e_Hora ( char *Data, char *Time, char *Mlabel )
{
	char
		hr[5],
		mn[5],
		sg[5];
	char
		dd[5],
		buffer[40];

	GetSystemTime ( &horas, &minutos , &segundos );
	Fmt( hr, "%d", horas);
	Fmt( mn, "%d", minutos);
	Fmt( sg, "%d", segundos);
	if( strlen(sg) == 1 )
		Fmt( sg, "0%d", segundos );
	if( strlen(hr) == 1 )
		Fmt( hr, "0%d", horas );
	if( strlen(mn) == 1 )
		Fmt( mn, "0%d", minutos );
	Fmt ( buffer, "%s:%s:%s " , hr, mn, sg );
	strcpy( Time, buffer);
	GetSystemDate ( &Mes, &Dia, &Ano);
	switch (Mes)
	{
		case 1:
			strcpy( Mlabel, "Jan");
			break;
		case 2:
			strcpy( Mlabel, "Fev");
			break;
		case 3:
			strcpy( Mlabel, "Mar");
			break;
		case 4:
			strcpy( Mlabel, "Abr");
			break;
		case 5:
			strcpy( Mlabel, "Mai");
			break;
		case 6:
			strcpy( Mlabel, "Jun");
			break;
		case 7:
			strcpy( Mlabel, "Jul");
			break;
		case 8:
			strcpy( Mlabel, "Ago");
			break;
		case 9:
			strcpy( Mlabel, "Set");
			break;
		case 10:
			strcpy( Mlabel, "Out");
			break;
		case 11:
			strcpy( Mlabel, "Nov");
			break;
		case 12:
			strcpy( Mlabel, "Dez");
			break;
		default:
			break;
	}
	Fmt( dd, "%d", Dia);
	if( strlen(dd) == 1 )
		Fmt( dd, "0%d", Dia );
	Fmt ( buffer, "%s-%s-%d " , dd, Mlabel, Ano );
	strcpy( Data, buffer);
}
/*-------------------------------------------------------------------------------
		Grava as coordenadas na tabela txt, na posição roi_nome
---------------------------------------------------------------------------------*/
void GravaCoordenadasROI(char * roi_nome, COORDENADAS_ROI *roi_selecionado, int usar_angulo, char * arq_vision)
{
int
	arq_conf,
	arq_temp,
	tamanho,
	com_index,
	i,
	status,
	hora,
	min,
	seg;
char 
	nome_arq_temp[500]={0},
	renome_arq_atual[500]={0},
	Buf[200]={0},
	temp[200]={0},
	comentarios[200]={0};


	arq_conf= OpenFile (arq_vision, VAL_READ_ONLY,VAL_OPEN_AS_IS,VAL_ASCII);
	if(arq_conf < 0)
	{   
		strcpy(Buf, "Houve um erro ao tentar abrir o arquivo de config das Coordenadas=");
		strcat(Buf, arq_vision);
		strcat(Buf, "\n Coordenada não foi gravada");
		MessagePopup ("Erro ao abrir arquivo config", Buf);
		return;
	}
	strcpy(nome_arq_temp, DIR_CONFIG_VISION);
	strcat(nome_arq_temp, "Temp_Coord.TXT");
	arq_temp= OpenFile (nome_arq_temp, VAL_WRITE_ONLY,VAL_TRUNCATE,VAL_ASCII);
	if(arq_temp < 0)
	{																							 
		MessagePopup ("Erro ao abrir arquivo config",
					  "Houve um erro ao tentar abrir o arquivo Temporario de configuração das coordenadas\n Coordenada não foi gravada");
		return;
	}
	
    for(i=0; i < NUM_MAX_LINHAS_ARQ ; i++) //le até 100 linhas
    {
    	if(ReadLine (arq_conf, Buf, 150)<0)//fim do arquivo
    		break;
    	tamanho = strlen(Buf);
    	if (!tamanho) continue; //continua no loop se string for vazia
    	StringUpperCase (Buf);
    	if(strcmp(Buf, "FIM") == 0)
    		break;
		//localiza o index dos comentarios
		com_index	= FindPattern (Buf, 0 ,-1 , "//", 0, 0);
		if(com_index < 0)
			strcpy(comentarios, "");
		else
			CopyString (comentarios, 0, Buf, com_index, -1);
		if (strstr (Buf, roi_nome) == NULL)
		{
			status = WriteLine(arq_temp , Buf , StringLength(Buf));//Escreve/copia a linha que não será modificada
		}
		else if (strstr (Buf, roi_nome) != NULL)
		{
			if(usar_angulo)
				Fmt(temp, "%s<%s%i, %i, %i, %i, %f[p3]", roi_nome, roi_selecionado->top,
					roi_selecionado->left, roi_selecionado->height, roi_selecionado->width,roi_selecionado->angle); 
			else
				Fmt(temp, "%s<%s%i, %i, %i, %i", roi_nome, roi_selecionado->top,
					roi_selecionado->left, roi_selecionado->height, roi_selecionado->width); 
			strcat(temp,"                             ");
			if(com_index>0)
				CopyString (temp, com_index,comentarios,0 , -1);
			status = WriteLine(arq_temp, temp, StringLength(temp));//Escreve linha modificada
		}
    }
	
	CloseFile (arq_conf);
	CloseFile (arq_temp);
	Delay_thread(0.05);
	GetSystemTime (&hora, &min, &seg);
	Fmt(renome_arq_atual,"%s<%sBkp-%s-%i_%i_%i_", DIR_CONFIG_VISION_BKP, DateStr(), hora, min, seg);
	CopyString (Buf, 0,arq_vision ,strlen(DIR_CONFIG_VISION), -1);
	
	strcat(renome_arq_atual, Buf);
	status = RenameFile (arq_vision,renome_arq_atual);
	Delay_thread(0.05);
	status = RenameFile (nome_arq_temp, arq_vision);
	
}
/*-----------------------------------------------------------------------------------
		Grava as coordenadas na tabela txt, na posição roi_nome, roi tipo annulus
------------------------------------------------------------------------------------*/
void GravaCoordenadasROICircular(char * roi_nome, char * roi_nome_2, COORD_ROI_PONTEIROS *roi_selecionado, char * arq_vision)
{
int
	arq_conf,
	arq_temp,
	tamanho,
	com_index,
	i,
	status,
	hora,
	min,
	seg;
char 
	nome_arq_temp[500]={0},
	renome_arq_atual[500]={0},
	Buf[200]={0},
	temp[200]={0},
	comentarios[200]={0};


	arq_conf= OpenFile (arq_vision, VAL_READ_ONLY,VAL_OPEN_AS_IS,VAL_ASCII);
	if(arq_conf < 0)
	{   
		strcpy(Buf, "Houve um erro ao tentar abrir o arquivo de config das Coordenadas=");
		strcat(Buf, arq_vision);
		strcat(Buf, "\n Coordenada não foi gravada");
		MessagePopup ("Erro ao abrir arquivo config", Buf);
		return;
	}
	strcpy(nome_arq_temp, DIR_CONFIG_VISION);
	strcat(nome_arq_temp, "Temp_Coord.TXT");
	arq_temp= OpenFile (nome_arq_temp, VAL_WRITE_ONLY,VAL_TRUNCATE,VAL_ASCII);
	if(arq_temp < 0)
	{																							 
		MessagePopup ("Erro ao abrir arquivo config",
					  "Houve um erro ao tentar abrir o arquivo Temporario de configuração das coordenadas\n Coordenada não foi gravada");
		return;
	}
	
    for(i=0; i < NUM_MAX_LINHAS_ARQ ; i++) //le até 100 linhas
    {
    	if(ReadLine (arq_conf, Buf, 150)<0)//fim do arquivo
    		break;
    	tamanho = strlen(Buf);
    	if (!tamanho) continue; //continua no loop se string for vazia
    	StringUpperCase (Buf);
    	if(strcmp(Buf, "FIM") == 0)
    		break;
		//localiza o index dos comentarios
		com_index	= FindPattern (Buf, 0 ,-1 , "//", 0, 0);
		if(com_index < 0)
			strcpy(comentarios, "");
		else
			CopyString (comentarios, 0, Buf, com_index, -1);
		if (strstr (Buf, roi_nome) == NULL && strstr (Buf, roi_nome_2) == NULL)
		{
			status = WriteLine(arq_temp , Buf , StringLength(Buf));//Escreve/copia a linha que não será modificada
		}
		else if (strstr (Buf, roi_nome) != NULL)
		{
			Fmt(temp, "%s<%s%i, %i", roi_nome, roi_selecionado->centro_x,
				roi_selecionado->centro_y); 
			strcat(temp,"                             ");
			if(com_index>0)
				CopyString (temp, com_index,comentarios,0 , -1);
			status = WriteLine(arq_temp, temp, StringLength(temp));//Escreve linha modificada
		}
		else if (strstr (Buf, roi_nome_2) != NULL)
		{
			Fmt(temp, "%s<%s%i, %i, %f[p2], %f[p2]", roi_nome_2, roi_selecionado->raio_int,
				roi_selecionado->raio_ext, roi_selecionado->ang_inic, roi_selecionado->ang_final); 
			strcat(temp,"                             ");
			if(com_index>0)
				CopyString (temp, com_index,comentarios,0 , -1);
			status = WriteLine(arq_temp, temp, StringLength(temp));//Escreve linha modificada
		}
    }
	
	CloseFile (arq_conf);
	CloseFile (arq_temp);
	Delay_thread(0.05);
	GetSystemTime (&hora, &min, &seg);
	Fmt(renome_arq_atual,"%s<%sBkp-%s-%i_%i_%i_", DIR_CONFIG_VISION_BKP, DateStr(), hora, min, seg);
	CopyString (Buf, 0,arq_vision ,strlen(DIR_CONFIG_VISION), -1);
	
	strcat(renome_arq_atual, Buf);
	status = RenameFile (arq_vision,renome_arq_atual);
	Delay_thread(0.05);
	status = RenameFile (nome_arq_temp, arq_vision);
	
}
/*-------------------------------------------------------------------------------
		Grava as coordenadas tipo line na tabela txt, na posição roi_nome
---------------------------------------------------------------------------------*/
void GravaCoordenadasLine(char * roi_nome, COORDENADAS_LINE *roi_selecionado, char * arq_vision)
{
int
	arq_conf,
	arq_temp,
	tamanho,
	com_index,
	i,
	status,
	hora,
	min,
	seg;
char 
	nome_arq_temp[500]={0},
	renome_arq_atual[500]={0},
	Buf[200]={0},
	temp[200]={0},
	comentarios[200]={0};


	arq_conf= OpenFile (arq_vision, VAL_READ_ONLY,VAL_OPEN_AS_IS,VAL_ASCII);
	if(arq_conf < 0)
	{   
		strcpy(Buf, "Houve um erro ao tentar abrir o arquivo de config das Coordenadas=");
		strcat(Buf, arq_vision);
		strcat(Buf, "\n Coordenada não foi gravada");
		MessagePopup ("Erro ao abrir arquivo config", Buf);
		return;
	}
	strcpy(nome_arq_temp, DIR_CONFIG_VISION);
	strcat(nome_arq_temp, "Temp_Coord.TXT");
	arq_temp= OpenFile (nome_arq_temp, VAL_WRITE_ONLY,VAL_TRUNCATE,VAL_ASCII);
	if(arq_temp < 0)
	{																							 
		MessagePopup ("Erro ao abrir arquivo config",
					  "Houve um erro ao tentar abrir o arquivo Temporario de configuração das coordenadas\n Coordenada não foi gravada");
		return;
	}
	
    for(i=0; i < NUM_MAX_LINHAS_ARQ ; i++) //le até 100 linhas
    {
    	if(ReadLine (arq_conf, Buf, 150)<0)//fim do arquivo
    		break;
    	tamanho = strlen(Buf);
    	if (!tamanho) continue; //continua no loop se string for vazia
    	StringUpperCase (Buf);
    	if(strcmp(Buf, "FIM") == 0)
    		break;
		//localiza o index dos comentarios
		com_index	= FindPattern (Buf, 0 ,-1 , "//", 0, 0);
		if(com_index < 0)
			strcpy(comentarios, "");
		else
			CopyString (comentarios, 0, Buf, com_index, -1);
		if (strstr (Buf, roi_nome) == NULL)
		{
			status = WriteLine(arq_temp , Buf , StringLength(Buf));//Escreve/copia a linha que não será modificada
		}
		else if (strstr (Buf, roi_nome) != NULL)
		{
			Fmt(temp, "%s<%s%i, %i, %i, %i", roi_nome, roi_selecionado->start.x, roi_selecionado->start.y, roi_selecionado->end.x, roi_selecionado->end.y); 
			strcat(temp,"                             ");
			if(com_index>0)
				CopyString (temp, com_index,comentarios,0 , -1);
			status = WriteLine(arq_temp, temp, StringLength(temp));//Escreve linha modificada
		}
    }
	
	CloseFile (arq_conf);
	CloseFile (arq_temp);
	Delay_thread(0.05);
	GetSystemTime (&hora, &min, &seg);
	Fmt(renome_arq_atual,"%s<%sBkp-%s-%i_%i_%i_", DIR_CONFIG_VISION_BKP, DateStr(), hora, min, seg);
	CopyString (Buf, 0,arq_vision ,strlen(DIR_CONFIG_VISION), -1);
	
	strcat(renome_arq_atual, Buf);
	status = RenameFile (arq_vision,renome_arq_atual);
	Delay_thread(0.05);
	status = RenameFile (nome_arq_temp, arq_vision);
	
}
/////////////////////////////////// ///////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------
//			 int ConfigVision()
//-------------------------------------------------------------------------------------------------------------------
int ConfigVision( DADOS_CFG_VISION *dados_cfg_vision, char *arq_vision)
{
int 
	arquivo, 
	i, 
	tamanho, 
	erro, 
	index;
char 
	linha_da_tabela[300]="", 
	resposta[300], 
	aux[300], 
	mensagem[200]="",
	nome_arq_temp[256]; 

	///////////////////////////////////////////////////////////
	if(strlen(arq_vision) < 5)
	{
		MessagePopup ("Erro nome do arquivo vision",
					  "Arquivo de configuração de Vision com nome muito curto ou incorreto.");
	    return -1;
	}

	strcpy(dados_cfg_vision->arquivo_locator, DIR_TEMPLATES_VISION);
	strcpy(dados_cfg_vision->arquivo_locator_inf, DIR_TEMPLATES_VISION);
	strcpy(dados_cfg_vision->imagem_padrao, DIR_TEMPLATES_VISION);
	strcpy(dados_cfg_vision->imagem_padrao_inf, DIR_TEMPLATES_VISION);
	
	arquivo = OpenFile( arq_vision, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII ) ;
	if (arquivo < 0)
	{
		MessagePopup ("Erro ao abrir arquivo",
					  "Arquivo de configuração de Vision não encontrado.");
	    return -2;
	}
	erro = 0;
	for(i=0; i < 200 ; i++) //le até 200 linhas
	{
		if(erro)
			break;
		if(ReadLine (arquivo, linha_da_tabela, 280)<0)//fim do arquivo
		   break;
		strcpy (resposta, linha_da_tabela);
		tamanho = strlen(resposta);
		if (!tamanho) 
			continue; //continua no loop se string for vazia
		StringUpperCase (resposta);
	    if((strcmp(resposta, "FIM") == 0 || strcmp(resposta, "END") == 0) && tamanho==3)
    		break;
		
		else if (strstr (resposta, "ARQ_LOCATOR=") != NULL)
		{
			CopyString (aux, 0, linha_da_tabela, 12, -1);
			index = FindPattern (aux, 0, -1, " ", 0, 0);
			CopyString (nome_arq_temp, 0, aux, 0, index);
			strcat(dados_cfg_vision->arquivo_locator,nome_arq_temp); 
			if(strlen(nome_arq_temp) < 5)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de arquivo template de localização - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "ARQ_LOCATOR_INF=") != NULL)
		{
			CopyString (aux, 0, linha_da_tabela, strlen("ARQ_LOCATOR_INF="), -1);
			index = FindPattern (aux, 0, -1, " ", 0, 0);
			CopyString (nome_arq_temp, 0, aux, 0, index);
			strcat(dados_cfg_vision->arquivo_locator_inf,nome_arq_temp); 
			if(strlen(nome_arq_temp) < 5)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de arquivo template de localização inferior- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "ARQ_PADRAO=") != NULL || strstr (resposta, "ARQ_PADRÃO=") != NULL)
		{
			CopyString (aux, 0, linha_da_tabela, 11, -1);
			index = FindPattern (aux, 0, -1, " ", 0, 0);
			CopyString (nome_arq_temp, 0, aux, 0, index);
			strcat(dados_cfg_vision->imagem_padrao,nome_arq_temp); 
			if(strlen(nome_arq_temp) < 5)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de arquivo template PADRÃO - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "ARQ_PADRAO_INF=") != NULL || strstr (resposta, "ARQ_PADRÃO_INF=") != NULL)
		{
			CopyString (aux, 0, linha_da_tabela, strlen("ARQ_PADRAO_INF="), -1);
			index = FindPattern (aux, 0, -1, " ", 0, 0);
			CopyString (nome_arq_temp, 0, aux, 0, index);
			strcat(dados_cfg_vision->imagem_padrao_inf,nome_arq_temp); 
			if(strlen(nome_arq_temp) < 5)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de arquivo template PADRÃO inferior - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "MIN_THRESH_DISP=") != NULL)
		{
			CopyString (aux, 0, resposta, 16, -1);
			dados_cfg_vision->min_threshold=atof(aux);
			if(dados_cfg_vision->min_threshold<1)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de MÍNIMO THRESHOLD do DISPLAY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "MAX_THRESH_DISP=") != NULL)
		{
			CopyString (aux, 0, resposta, 16, -1);
			dados_cfg_vision->max_threshold=atof(aux);
			if(dados_cfg_vision->max_threshold<1)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de MÁXIMO THRESHOLD do DISPLAY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_LOCATOR=") != NULL)
		{
			CopyString (aux, 0, resposta, 18, -1);
			dados_cfg_vision->coord_locator_main.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->coord_locator_main.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->coord_locator_main.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->coord_locator_main.width=atoi(aux);
			
			if(dados_cfg_vision->coord_locator_main.top<1 ||dados_cfg_vision->coord_locator_main.left<1 ||dados_cfg_vision->coord_locator_main.height<1 ||dados_cfg_vision->coord_locator_main.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI Locator - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_LOCATOR_INF=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_LOCATOR_INF="), -1);
			dados_cfg_vision->coord_locator_inf.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->coord_locator_inf.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->coord_locator_inf.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->coord_locator_inf.width=atoi(aux);
			
			if(dados_cfg_vision->coord_locator_inf.top<1 ||dados_cfg_vision->coord_locator_inf.left<1 ||dados_cfg_vision->coord_locator_inf.height<1 ||dados_cfg_vision->coord_locator_inf.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI Locator Inferior - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ORIG=") != NULL)
		{
			CopyString (aux, 0, resposta, 11, -1);
			dados_cfg_vision->origem_linha.x=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->origem_linha.y=atoi(aux);
			
			if(dados_cfg_vision->origem_linha.x<1 ||dados_cfg_vision->origem_linha.y<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de origem do ROI da linha - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_FIM=") != NULL)
		{
			CopyString (aux, 0, resposta, 10, -1);
			dados_cfg_vision->fim_linha.x=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			dados_cfg_vision->fim_linha.y=atoi(aux);
			
			if(dados_cfg_vision->fim_linha.x<1 || dados_cfg_vision->fim_linha.y<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de fim do ROI da linha - Arq = ", arq_vision);
				erro =1;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////
		else if (strstr (resposta, "CAMERA_GAIN_MONTAGEM=") != NULL)
		{
			CopyString (aux, 0, resposta, 21, -1);
			dados_cfg_vision->camera_gain_montagem=atof(aux);

			if(dados_cfg_vision->camera_gain_montagem < MIN_GAIN_CAMERA)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de GAIN da camera para MONTAGEM - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CAMERA_SHUTTER_MONTAGEM=") != NULL)
		{
			CopyString (aux, 0, resposta, 24, -1);
			dados_cfg_vision->camera_shutter_montagem=atof(aux);
			if(dados_cfg_vision->camera_shutter_montagem < MIN_SHUTTER_CAMERA)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de SHUTTER da camera para MONTAGEM ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CAMERA_SHUTTER_PINS_DISPLAY=") != NULL)
		{
			CopyString (aux, 0, resposta, 28, -1);
			dados_cfg_vision->camera_shutter_pins_display=atof(aux);
			if(dados_cfg_vision->camera_shutter_pins_display<MIN_SHUTTER_CAMERA)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de SHUTTER CAMERA_SHUTTER_PINS_DISPLAY em ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CAMERA_SHUTTER_BARCODE_LCD=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("CAMERA_SHUTTER_BARCODE_LCD="), -1);
			dados_cfg_vision->camera_shutter_barcode_lcd=atof(aux);
			if(dados_cfg_vision->camera_shutter_barcode_lcd<MIN_SHUTTER_CAMERA)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de shutter da camera do barcode lcd em ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CAMERA_GAIN_PINS_DISPLAY=") != NULL)
		{
			CopyString (aux, 0, resposta, 25, -1);
			dados_cfg_vision->camera_gain_pins_display=atof(aux);

			if(dados_cfg_vision->camera_gain_pins_display < MIN_GAIN_CAMERA)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de GAIN CAMERA_GAIN_PINS_DISPLAY da camera - Arq = ", arq_vision);
				erro =1;
			}
		}
		
		else if (strstr (resposta, "GAMMA_MONTAGEM=") != NULL)
		{
			CopyString (aux, 0, resposta, 15, -1);
			dados_cfg_vision->gamma_montagem=atof(aux);
			if(dados_cfg_vision->gamma_montagem<0)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de GAMMA de Montagem - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CONTRASTE_MONTAGEM=") != NULL)
		{
			CopyString (aux, 0, resposta, 19, -1);
			dados_cfg_vision->contraste_montagem=atof(aux);
			if(dados_cfg_vision->contraste_montagem < 1)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de CONTRASTE_MONTAGEM - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CONTRASTE_PINS_DISPLAY=") != NULL)
		{
			CopyString (aux, 0, resposta, 23, -1);
			dados_cfg_vision->contraste_pins_display=atof(aux);
			if(dados_cfg_vision->contraste_pins_display < 1)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de CONTRASTE_PINS_DISPLAY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "GAMMA_PINS_DISPLAY=") != NULL)
		{
			CopyString (aux, 0, resposta, 19, -1);
			dados_cfg_vision->gamma_pins_display=atof(aux);
			if(dados_cfg_vision->gamma_pins_display < 0)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de GAMMA_PINS_DISPLAY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "CONTRASTE_BARCODE_LCD=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("CONTRASTE_BARCODE_LCD="), -1);
			dados_cfg_vision->contraste_barcode_lcd=atof(aux);
			if(dados_cfg_vision->contraste_barcode_lcd < 1)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de contraste do barcode lcd- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "GAMMA_BARCODE_LCD=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("GAMMA_BARCODE_LCD="), -1);
			dados_cfg_vision->gamma_barcode_lcd=atof(aux);
			if(dados_cfg_vision->gamma_barcode_lcd < 0)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de gamma do barcode LCD- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "PIXEL_POR_MM=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("PIXEL_POR_MM="), 4);
			pixel_por_mm = atof(aux);
			if(pixel_por_mm<0)
			{
				Fmt(mensagem,"%s<%s%s","Falha no dado de parametrização PIXEL_POR_MM em ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_QUANT_PIN_LCD_1=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_QUANT_PIN_LCD_1="), -1);
			coord_rois_pins_lcd[1].top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[1].left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[1].height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[1].width=atoi(aux);
			if(coord_rois_pins_lcd[1].top<1 || coord_rois_pins_lcd[1].left<1 || coord_rois_pins_lcd[1].height<1 || coord_rois_pins_lcd[1].width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS QUANTIFY do pino 1 do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_QUANT_PIN_LCD_2=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_QUANT_PIN_LCD_2="), -1);
			coord_rois_pins_lcd[2].top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[2].left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[2].height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[2].width=atoi(aux);
			if(coord_rois_pins_lcd[2].top<1 || coord_rois_pins_lcd[2].left<1 || coord_rois_pins_lcd[2].height<1 || coord_rois_pins_lcd[2].width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS QUANTIFY do pino 2 do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_QUANT_PIN_LCD_3=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_QUANT_PIN_LCD_3="), -1);
			coord_rois_pins_lcd[3].top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[3].left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[3].height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[3].width=atoi(aux);
			if(coord_rois_pins_lcd[3].top<1 || coord_rois_pins_lcd[3].left<1 || coord_rois_pins_lcd[3].height<1 || coord_rois_pins_lcd[3].width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS QUANTIFY do pino 3 do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_QUANT_PIN_LCD_4=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_QUANT_PIN_LCD_4="), -1);
			coord_rois_pins_lcd[4].top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[4].left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[4].height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[4].width=atoi(aux);
			if(coord_rois_pins_lcd[4].top<1 || coord_rois_pins_lcd[4].left<1 || coord_rois_pins_lcd[4].height<1 || coord_rois_pins_lcd[4].width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS QUANTIFY do pino 4 do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_QUANT_PIN_LCD_5=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_QUANT_PIN_LCD_5="), -1);
			coord_rois_pins_lcd[5].top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[5].left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[5].height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[5].width=atoi(aux);
			if(coord_rois_pins_lcd[5].top<1 || coord_rois_pins_lcd[5].left<1 || coord_rois_pins_lcd[5].height<1 || coord_rois_pins_lcd[5].width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS QUANTIFY do pino 5 do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_QUANT_PIN_LCD_6=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_QUANT_PIN_LCD_6="), -1);
			coord_rois_pins_lcd[6].top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[6].left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[6].height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_rois_pins_lcd[6].width=atoi(aux);
			if(coord_rois_pins_lcd[6].top<1 || coord_rois_pins_lcd[6].left<1 || coord_rois_pins_lcd[6].height<1 || coord_rois_pins_lcd[6].width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS QUANTIFY do pino 6 do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		
		
		else if (strstr (resposta, "COORD_ROI_APLIQUE=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_APLIQUE="), -1);
			coord_roi_aplique.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_aplique.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_aplique.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_aplique.width=atoi(aux);
			if(coord_roi_aplique.top<1 || coord_roi_aplique.left<1 || coord_roi_aplique.height<1 || coord_roi_aplique.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do Template do aplique - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_TRAVA_APLIQUE_ESQ=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_TRAVA_APLIQUE_ESQ="), -1);
			coord_trava_aplique_esq.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_trava_aplique_esq.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_trava_aplique_esq.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_trava_aplique_esq.width=atoi(aux);
			if(coord_trava_aplique_esq.top<1 || coord_trava_aplique_esq.left<1 || coord_trava_aplique_esq.height<1 || coord_trava_aplique_esq.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS da trava do aplique esquerda - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_TRAVA_APLIQUE_DIR=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_TRAVA_APLIQUE_DIR="), -1);
			coord_trava_aplique_dir.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_trava_aplique_dir.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_trava_aplique_dir.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_trava_aplique_dir.width=atoi(aux);
			if(coord_trava_aplique_dir.top<1 || coord_trava_aplique_dir.left<1 || coord_trava_aplique_dir.height<1 || coord_trava_aplique_dir.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS da trava do aplique esquerda - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DIFUSOR=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DIFUSOR="), -1);
			coord_qtfy_difusor.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_difusor.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_difusor.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_difusor.width=atoi(aux);
			if(coord_qtfy_difusor.top<1 || coord_qtfy_difusor.left<1 || coord_qtfy_difusor.height<1 || coord_qtfy_difusor.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de presença DIFUSOR - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DISPLAY=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DISPLAY="), -1);
			coord_qtfy_display.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_display.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_display.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_display.width=atoi(aux);
			if(coord_qtfy_display.top<1 || coord_qtfy_display.left<1 || coord_qtfy_display.height<1 || coord_qtfy_display.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de presença DISPLAY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_SHIELD=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_SHIELD="), -1);
			coord_qtfy_shield.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_shield.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_shield.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_shield.width=atoi(aux);
			if(coord_qtfy_shield.top<1 || coord_qtfy_shield.left<1 || coord_qtfy_shield.height<1 || coord_qtfy_shield.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de presença SHIELD - Arq = ", arq_vision);
				erro =1;
			}
		}

		else if (strstr (resposta, "COORD_ROI_LCD=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_LCD="), -1);
			coord_qtfy_lcd.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_lcd.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_lcd.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_qtfy_lcd.width=atoi(aux);
			if(coord_qtfy_lcd.top<1 || coord_qtfy_lcd.left<1 || coord_qtfy_lcd.height<1 || coord_qtfy_lcd.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de presença LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
    	else if (strstr (resposta, "COORD_ROI_LIGHT_GUIDE_ESQ=") != NULL)
		{
			CopyString (aux, 0, resposta, 26, -1);
			coord_temp_light_esq.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_esq.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_esq.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_esq.width=atoi(aux);
			if(coord_temp_light_esq.top<1 || coord_temp_light_esq.left<1 || coord_temp_light_esq.height<1 || coord_temp_light_esq.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do Template do LIGHT GUIDE ESQ - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_LIGHT_GUIDE_DISP=") != NULL)
		{
			CopyString (aux, 0, resposta, 27, -1);
			coord_temp_light_disp.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_disp.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_disp.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_disp.width=atoi(aux);
			if(coord_temp_light_disp.top<1 || coord_temp_light_disp.left<1 || coord_temp_light_disp.height<1 || coord_temp_light_disp.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do Template do LIGHTGUIDE DO DISPLAY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_LIGHT_GUIDE_DIR=") != NULL)
		{
			CopyString (aux, 0, resposta, 26, -1);
			coord_temp_light_dir.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_dir.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_dir.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_light_dir.width=atoi(aux);
			if(coord_temp_light_dir.top<1 || coord_temp_light_dir.left<1 || coord_temp_light_dir.height<1 || coord_temp_light_dir.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do Template do DIFUSOR DIR - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_TEMPL_LINER_INF=") != NULL)
		{
			CopyString (aux, 0, resposta, 26, -1);
			coord_temp_liner_inf.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_liner_inf.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_liner_inf.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_temp_liner_inf.width=atoi(aux);
			if(coord_temp_liner_inf.top<1 || coord_temp_liner_inf.left<1 || coord_temp_liner_inf.height<1 || coord_temp_liner_inf.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do LINER INFERIOR - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_LINER_INF=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_LINER_INF="), -1);
			coord_liner_inf.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_liner_inf.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_liner_inf.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_liner_inf.width=atoi(aux);
			if(coord_liner_inf.top<1 || coord_liner_inf.left<1 || coord_liner_inf.height<1 || coord_liner_inf.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do LINER INFERIOR QTFY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_LINER_SUP=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_LINER_SUP="), -1);
			coord_liner_sup.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_liner_sup.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_liner_sup.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_liner_sup.width=atoi(aux);
			if(coord_liner_sup.top<1 || coord_liner_sup.left<1 || coord_liner_sup.height<1 || coord_liner_sup.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do LINER SUPERIOR QTFY - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_MASK_DIFUSOR=") != NULL)
		{
			CopyString (aux, 0, resposta, 23, -1);
			coord_mask_difusor.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_mask_difusor.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_mask_difusor.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_mask_difusor.width=atoi(aux);
			if(coord_mask_difusor.top<1 || coord_mask_difusor.left<1 || coord_mask_difusor.height<1 || coord_mask_difusor.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de Mask do DIFUSOR - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DISP_LINER_PT_1=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DISP_LINER_PT_1="), -1);
			coord_roi_disp_liner_pt_1.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_1.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_1.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_1.width=atoi(aux);
			if(coord_roi_disp_liner_pt_1.top<1 || coord_roi_disp_liner_pt_1.left<1 || coord_roi_disp_liner_pt_1.height<1 || coord_roi_disp_liner_pt_1.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI do Display/Liner Ponto 1- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DISP_LINER_PT_2=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DISP_LINER_PT_2="), -1);
			coord_roi_disp_liner_pt_2.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_2.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_2.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_2.width=atoi(aux);
			if(coord_roi_disp_liner_pt_2.top<1 || coord_roi_disp_liner_pt_2.left<1 || coord_roi_disp_liner_pt_2.height<1 || coord_roi_disp_liner_pt_2.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI do Display/Liner Ponto 2- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DISP_LINER_PT_3=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DISP_LINER_PT_3="), -1);
			coord_roi_disp_liner_pt_3.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_3.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_3.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_3.width=atoi(aux);
			if(coord_roi_disp_liner_pt_3.top<1 || coord_roi_disp_liner_pt_3.left<1 || coord_roi_disp_liner_pt_3.height<1 || coord_roi_disp_liner_pt_3.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI do Display/Liner Ponto 3- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DISP_LINER_PT_4=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DISP_LINER_PT_4="), -1);
			coord_roi_disp_liner_pt_4.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_4.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_4.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_disp_liner_pt_4.width=atoi(aux);
			if(coord_roi_disp_liner_pt_4.top<1 || coord_roi_disp_liner_pt_4.left<1 || coord_roi_disp_liner_pt_4.height<1 || coord_roi_disp_liner_pt_4.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI do Display/Liner Ponto 4- Arq = ", arq_vision);
				erro =1;
			}
		}
		
		else if (strstr (resposta, "COORD_ROI_DIFUSOR_PT_1=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DIFUSOR_PT_1="), -1);
			coord_roi_difusor_pt_1.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_difusor_pt_1.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_difusor_pt_1.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_difusor_pt_1.width=atoi(aux);
			if(coord_roi_difusor_pt_1.top<1 || coord_roi_difusor_pt_1.left<1 || coord_roi_difusor_pt_1.height<1 || coord_roi_difusor_pt_1.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI do DIFUSOR Ponto 1 - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_DIFUSOR_PT_2=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_DIFUSOR_PT_2="), -1);
			coord_roi_difusor_pt_2.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_difusor_pt_2.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_difusor_pt_2.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_difusor_pt_2.width=atoi(aux);
			if(coord_roi_difusor_pt_2.top<1 || coord_roi_difusor_pt_2.left<1 || coord_roi_difusor_pt_2.height<1 || coord_roi_difusor_pt_2.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS do ROI do DIFUSOR Ponto 2- Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_BARCODE_APLIQUE=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_BARCODE_APLIQUE="), -1);
			coord_roi_barcode_aplique.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_barcode_aplique.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_barcode_aplique.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_barcode_aplique.width=atoi(aux);
			if(coord_roi_barcode_aplique.top<1 || coord_roi_barcode_aplique.left<1 || coord_roi_barcode_aplique.height<1 || coord_roi_barcode_aplique.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de Barcode - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_BARCODE_DISPLAY=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_BARCODE_DISPLAY="), -1);
			coord_roi_barcode_display.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_barcode_display.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_barcode_display.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_barcode_display.width=atoi(aux);
			if(coord_roi_barcode_display.top<1 || coord_roi_barcode_display.left<1 || coord_roi_barcode_display.height<1 || coord_roi_barcode_display.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de Barcode - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_ROI_MASK_LIGHTPIPE=") != NULL)
		{
			CopyString (aux, 0, resposta, 25, -1);
			coord_mask_lightpipe.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_mask_lightpipe.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_mask_lightpipe.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_mask_lightpipe.width=atoi(aux);
			if(coord_mask_lightpipe.top<1 || coord_mask_lightpipe.left<1 || coord_mask_lightpipe.height<1 || coord_mask_lightpipe.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS de Mask do LIGHTPIPE - Arq = ", arq_vision);
				erro =1;
			}
		}
 		else if (strstr (resposta, "COORD_ROI_PINS_LCD=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_ROI_PINS_LCD="), -1);
			coord_roi_lcd_pins.top=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_lcd_pins.left=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_lcd_pins.height=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_roi_lcd_pins.width=atoi(aux);
			if(coord_roi_lcd_pins.top<1 || coord_roi_lcd_pins.left<1 || coord_roi_lcd_pins.height<1 || coord_roi_lcd_pins.width<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS dos pinos do LCD - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_LINE_TRAVAESQ_J5=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_LINE_TRAVAESQ_J5="), -1);
			coord_line_esq_j5.start.x=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_line_esq_j5.start.y=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_line_esq_j5.end.x=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_line_esq_j5.end.y=atoi(aux);
			if(coord_line_esq_j5.start.x<1 || coord_line_esq_j5.start.y<1 || coord_line_esq_j5.end.x<1 || coord_line_esq_j5.end.y<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS da Trava Esquerda do J5 - Arq = ", arq_vision);
				erro =1;
			}
		}
		else if (strstr (resposta, "COORD_LINE_TRAVADIR_J5=") != NULL)
		{
			CopyString (aux, 0, resposta, strlen("COORD_LINE_TRAVADIR_J5="), -1);
			coord_line_dir_j5.start.x=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_line_dir_j5.start.y=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_line_dir_j5.end.x=atoi(aux);
			index = FindPattern (aux, 0, -1, ",", 0, 0)+1;
			CopyString (aux, 0, aux, index, -1);
			coord_line_dir_j5.end.y=atoi(aux);
			if(coord_line_dir_j5.start.x<1 || coord_line_dir_j5.start.y<1 || coord_line_dir_j5.end.x<1 || coord_line_dir_j5.end.y<1) 
			{
				Fmt(mensagem,"%s<%s%s","Falha nos dados de COORDENADAS da Trava Direita do J5 - Arq = ", arq_vision);
				erro =1;
			}
		}
    }
	CloseFile (arquivo);
	if(erro)
	{
		MessagePopup ("Erro definição no arq. config. vision.",mensagem);
		return -1;
	}
	strcpy(arq_vision_ant, arq_vision);
	return 0;
}

//
// Função: CVI_PlaySound()
//
// Descrição: Play the specified sound file synchronously or asynchronously.
//
short CVI_PlaySound (char *fileName, short asynchronous)
{
	int
		file_present;
		ssize_t
		tamanho;

	file_present = GetFileInfo (fileName, &tamanho);
	if(file_present != 1)
	{
		MessagePopup ("Erro nome do arquivo de som",
					  "Arquivo de SOM Multimidia não encontrado.");
		return -1;
	}
	if (asynchronous)
		return (short)sndPlaySound(fileName, SND_ASYNC);
	else
		return (short)sndPlaySound(fileName, SND_SYNC);
}

//
// Função: CVI_PlaySoundEx()
//
// Descrição: Play the specified sound file synchronously or asynchronously, with some
//			  additional options.
//
short CVI_PlaySoundEx (char *fileName, short asynchronous, short loop,
					   short playDefault, short stopCurrentSound)
{
	UINT
		flags = 0;

	if (loop)
		flags |= SND_ASYNC | SND_LOOP;
	else if (asynchronous)
		flags |= SND_ASYNC;
	else
		flags |= SND_SYNC;
	if (!playDefault)
		flags |= SND_NODEFAULT;
	if (!stopCurrentSound)
		flags |= SND_NOSTOP;
	return (short)sndPlaySound (fileName, flags);
}

//
// Função: CVI_StopCurrentSound()
//
// Descrição: Stop playing the current sound.
//
void CVI_StopCurrentSound (void)
{
	sndPlaySound(NULL, 0);
}

//
// Função: MensFinal
//
int MensFinal( char *Rodape , int Mdl_Rj , double Tempo_Total , double res , double Hi_Lim ,
			   double Low_Lim , char *Nome_Teste , int teste)
{
	char
		tmp[90];

	if(sair_programa)
		return -1;
	painel_mensagens = LoadPanel( 0, "Mensagens.uir", MENSAGEM );
	Fmt(tmp , "%s<%f[p2]" , Tempo_Total);
	InsertTextBoxLine (painel_mensagens, MENSAGEM_MSG_TXT_T_TOTAL , 0, tmp);
	if (rejeitado)
	{
		InsertTextBoxLine (painel_mensagens, MENSAGEM_MSG_TXT_CT_RJ, 0, MensagemFalha1);
		strcpy(MensagemFalha1,"");
		SetCtrlAttribute  (painel_mensagens, MENSAGEM_MSG_TXT_AP_RJ, ATTR_TEXT_BGCOLOR ,VAL_RED);
		InsertTextBoxLine (painel_mensagens, MENSAGEM_MSG_TXT_AP_RJ, 0, "PRODUTO REJEITADO");
	}
	else
	{
		InsertTextBoxLine (painel_mensagens, MENSAGEM_MSG_TXT_AP_RJ, 0, "PRODUTO APROVADO");
	}
	DisplayPanel (painel_mensagens);
	return(0);
}

