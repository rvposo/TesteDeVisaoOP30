#include <windows.h> 
#include "NIIMAQdx.h"
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>
#include <tcpsupp.h>
#include <cvirte.h>		
#include <userint.h>
#include <stdio.h> 
#include "Auto OP30 GEM.h"
#include "Main OP30 GEM.h"
#include "Manual OP30 GEM.h"
#include "Vision Scripts OP30 Cluster GEM.h"

#define MAX_BUFFERS 10
#define INIT_POINTS_ARRAY_ELEMENTS 100
#define DIMINUIR 		1
#define AUMENTAR 		2
#define GAMMA_AJUSTADO 	3
#define VisionErrChk(Function) {if (!(Function)) {success = 0; goto Error;}}


int CapturaImagemVision(int sessao_id , int iluminar_extern, float valor_shutter, float valor_gain);

FindTransformPatternOptions TPOption = {IMAQ_MATCH_SHIFT_INVARIANT, 600.00, TRUE, NULL, 0,
			TRUE, TRUE, TRUE};



/******************************** InicializaCameras ************************************************************************/
int InicializaCameras(void)
{
Image *imagem_teste=NULL;
int i;

	gain_anterior 		=-1;
	shutter_anterior	=-1;
	sessao_id_anterior	=-1;
	gravar_imagem		=NAO_GRAVA;
	
	Mensagem( "Aguarde...","Configurando câmera para testes com Sistema de Visão", "", "", MENS_AZUL) ;
	
	DisplayPanel (painel_auto);
	imaqMoveWindow (0, imaqMakePoint(0 , 0) );
	Delay_thread(0.01) ;
	imagem_teste = imaqCreateImage (IMAQ_IMAGE_RGB, 2);
	Delay_thread(0.1) ;
	SetBreakOnLibraryErrors (0);
	for(i=0;i<=2;i++)
	{
		IMAQdxUnconfigureAcquisition(i);
		IMAQdxCloseCamera (i);
		IMAQdxConfigureGrab (i);
	}  
	SetBreakOnLibraryErrors (1);
	Delay_thread(0.2) ;
	
	if(IMAQdxOpenCamera (STR_CAMERA, IMAQdxCameraControlModeController, &sessao_id_cam_sup) != IMAQdxErrorSuccess)
	{
		MessagePopup ( " ERRO NA COMUNICAÇÃO C/ CÂMERA SUPERIOR" , "Falha na comunicação com a Câmera ") ;
		return -1 ;   
	}
	IMAQdxSnap (sessao_id_cam_sup,imagem_teste );
	Delay_thread(0.01) ;
	
	if(mostrar_vision)
	{
		SetCtrlAttribute (painel_manual, MANUAL_TREINAR_VISION, ATTR_DIMMED, 0);
		SetCtrlAttribute (painel_manual, MANUAL_MONITORAR_VISION, ATTR_DIMMED, 0);
	}
	imaqDispose (imagem_teste); 
	
	return 0;
}

/******************************** PararCameras ************************************************************************/
void PararCameras(void)
{
int i;

	SetBreakOnLibraryErrors (0);
	for(i=0;i<=2;i++)
	{
		sessao_id_anterior=-1;
		gain_anterior =-1;shutter_anterior=-1;
		IMAQdxStopAcquisition (i);
		IMAQdxUnconfigureAcquisition(i);
	}
	SetBreakOnLibraryErrors (1);
}
//---------------------  int ReparametrizaROI -----------------------------------------
int	ReparametrizaROI(Image *image,COORDENADAS_ROI *roi_selecionado, char * roi_nome, char *arq_vision, ROI *roi_param, int utiliz_roi)
{
Image  
	*imagem_sem_overlay=NULL;
char
	chpt_ajustar[150],
	linha[110]={0},
	arq_rep_nome[256];
ContourID 
	contourID;
ContourInfo2 
	*contourInfo = NULL;
int	
	status = BOT_OFF,
	vez = 0;
static COORDENADAS_ROI 
	roi_anterior;
COORDENADAS_ROI 
	roi_loc_corrig,
	roi_reajust;
ROI 
	*roi=NULL;

		imaqSetToolContextSensitivity (TRUE);
		imaqShowToolWindow (TRUE);
		
		roi_loc_corrig.top=0;
		roi_loc_corrig.left=0;
		roi_loc_corrig.height=0;
		roi_loc_corrig.width=0;
		roi_loc_corrig.angle=0;
		if(utiliz_roi == OFF)
		{
			roi_anterior.top 	=  roi_selecionado->top;
			roi_anterior.left 	=  roi_selecionado->left;
			roi_anterior.height =  roi_selecionado->height;
			roi_anterior.width 	=  roi_selecionado->width;
			roi_anterior.angle 	=  roi_selecionado->angle;
			return SEM_ERRO;
		}
		else
		{
			imagem_sem_overlay = imaqCreateImage (IMAQ_IMAGE_RGB, 2);
			imaqDuplicate (imagem_sem_overlay, image);
			MostraVision(imagem_sem_overlay, OFF);
			roi = imaqCreateROI (); //cria roi baseado em coordenadas recebidas
			if(utiliz_roi == CRIAR_ROI)
			{
				roi_anterior.top 	=  roi_selecionado->top;
				roi_anterior.left 	=  roi_selecionado->left;
				roi_anterior.height =  roi_selecionado->height;
				roi_anterior.width 	=  roi_selecionado->width;
				roi_anterior.angle 	=  roi_selecionado->angle;
			    imaqAddRectContour(roi, imaqMakeRect(roi_selecionado->top, roi_selecionado->left, 
							roi_selecionado->height, roi_selecionado->width));
			}
			else
			{
				roi = roi_param;
			}
			imaqSetWindowROI (0, roi);
			roi = imaqGetWindowROI(0);
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
			roi_loc_corrig.top=contourInfo->structure.rotatedRect->top;
			roi_loc_corrig.left=contourInfo->structure.rotatedRect->left;
			roi_loc_corrig.height=contourInfo->structure.rotatedRect->height;
			roi_loc_corrig.width=contourInfo->structure.rotatedRect->width;
			roi_loc_corrig.angle=contourInfo->structure.rotatedRect->angle;
			imaqSetROIColor (roi, &IMAQ_RGB_GREEN);
			MostraVision(imagem_sem_overlay, ON);
		}
			
		Fmt(chpt_ajustar,"%s<%s%s", "Ajuste o ROI - ", roi_nome);
		chpt_ajustar[strlen(chpt_ajustar)-1] = 0;
		Delay_thread(0.1);
		parar_mensagens_auto = ON;
		COR_MENSAGENS = MENS_VERMELHO;
		Mensagem("Parada p/ reparametrização de ROI...","Ajuste o ROI VERDE observando a região desejada", 
				chpt_ajustar,
				"Após finalizar aperte continue.", COR_MENSAGENS);
		
		rej_remoto = OFF;
		cont_remoto = OFF;
		while (status != CONT  && status != REJ && status_emergencia)//forca leitura de rej. ou cont 
		{
			esperando_ler_botoeira = ON;
			status = LeBotoeira();
			if(status == CONT && vez == 0) //primeiro CONT limpa demais overlay
			{
				imaqClearOverlay (imagem_sem_overlay, NULL); 
				imaqSetROIColor (roi, &IMAQ_RGB_RED);
				MostraVision(imagem_sem_overlay, ON);
				while(STATUS.Continua)
					Delay_thread(0.010);
				Delay_thread(0.300);
				vez = 1;
				status = BOT_OFF;
			}
		}
		COR_MENSAGENS = MENS_AZUL;
		parar_mensagens_auto = OFF;
		if(status == REJ)
		{
    		imaqDispose(roi);
			return -1;
		}
		roi = imaqGetWindowROI(0);
		contourID = imaqGetContour (roi, 0);
		contourInfo = imaqGetContourInfo2 (roi, contourID);
		
		roi_reajust.top		= contourInfo->structure.rotatedRect->top;
		roi_reajust.left	= contourInfo->structure.rotatedRect->left;
		roi_reajust.height	= contourInfo->structure.rotatedRect->height;
		roi_reajust.width	= contourInfo->structure.rotatedRect->width;
		roi_reajust.angle	= contourInfo->structure.rotatedRect->angle;
		
		roi_selecionado->top	= roi_anterior.top 	  + (roi_reajust.top 	- roi_loc_corrig.top);
		roi_selecionado->left	= roi_anterior.left   + (roi_reajust.left 	- roi_loc_corrig.left);
		roi_selecionado->height	= roi_anterior.height + (roi_reajust.height - roi_loc_corrig.height);
		roi_selecionado->width	= roi_anterior.width  + (roi_reajust.width 	- roi_loc_corrig.width);
		roi_selecionado->angle	= roi_anterior.angle  + (roi_reajust.angle 	- roi_loc_corrig.angle);
		GravaCoordenadasROI(roi_nome, roi_selecionado, OFF, arq_vision);
		MostraVision(image, OFF);
		imaqShowToolWindow (FALSE);
    	imaqDispose(roi);
    	imaqDispose(imagem_sem_overlay);
return 0;
}
//---------------------  int ReparametrizaLine -----------------------------------------
int	ReparametrizaLine(Image *image, COORDENADAS_LINE *roi_selecionado, char * roi_nome, DADOS_CFG_VISION cfg_vision, char *arq_vision, ROI *roi_param, int utiliz_roi)
{
Image  
	*imagem_sem_overlay=NULL;
char
	chpt_ajustar[150];
ContourID 
	contourID;
ContourInfo2 
	*contourInfo = NULL;
int	
	status = BOT_OFF,
	vez = 0;
static COORDENADAS_LINE 
	roi_anterior;
COORDENADAS_LINE 
	roi_loc_corrig,
	roi_reajust;
ROI 
	*roi=NULL;

		roi_loc_corrig.start.x=0;
		roi_loc_corrig.end.x=0;
		roi_loc_corrig.start.y=0;
		roi_loc_corrig.end.y=0;

		if(utiliz_roi == OFF)
		{
			roi_anterior.start.x =  roi_selecionado->start.x;
			roi_anterior.end.x 	 =  roi_selecionado->end.x;
			roi_anterior.start.y =  roi_selecionado->start.y;
			roi_anterior.end.y 	 =  roi_selecionado->end.y;
			return SEM_ERRO;
		}
		else
		{
			imagem_sem_overlay = imaqCreateImage (IMAQ_IMAGE_RGB, 2);
			imaqDuplicate (imagem_sem_overlay, image);
			MostraVision(imagem_sem_overlay, OFF);
			roi = imaqCreateROI (); //cria roi baseado em coordenadas recebidas
			if(utiliz_roi == CRIAR_ROI)
			{
				roi_anterior.start.x =  roi_selecionado->start.x;
				roi_anterior.start.y =  roi_selecionado->start.y;
				roi_anterior.end.x 	 =  roi_selecionado->end.x;
				roi_anterior.end.y 	 =  roi_selecionado->end.y;
			    imaqAddLineContour(roi, imaqMakePoint(roi_selecionado->start.x, roi_selecionado->start.y), 
										imaqMakePoint(roi_selecionado->end.x, roi_selecionado->end.y));
			}
			else
			{
				roi = roi_param;
			}
			imaqSetWindowROI (0, roi);
			roi = imaqGetWindowROI(0);
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
			roi_loc_corrig.start.x = contourInfo->structure.line->start.x;
			roi_loc_corrig.start.y = contourInfo->structure.line->start.y;
			roi_loc_corrig.end.x   = contourInfo->structure.line->end.x;
			roi_loc_corrig.end.y   = contourInfo->structure.line->end.y;
			imaqSetROIColor (roi, &IMAQ_RGB_GREEN);
			MostraVision(imagem_sem_overlay, ON);
		}
			
		Fmt(chpt_ajustar,"%s<%s%s", "Ajuste o ROI - ", roi_nome);
		chpt_ajustar[strlen(chpt_ajustar)-1] = 0;
		Delay_thread(0.1);
		parar_mensagens_auto = ON;
		Mensagem("Parada p/ reparametrização de ROI...","Ajuste o ROI VERDE observando a região desejada", 
				chpt_ajustar,
				"Após finalizar aperte continue.", COR_MENSAGENS);
		
		rej_remoto = OFF;
		cont_remoto = OFF;
		while (status != CONT  && status != REJ && status_emergencia)//forca leitura de rej. ou cont 
		{
			esperando_ler_botoeira = ON;
			status = LeBotoeira();
			if(status == CONT && vez == 0) //primeiro CONT limpa demais overlay
			{
				imaqClearOverlay (imagem_sem_overlay, NULL); 
				imaqSetROIColor (roi, &IMAQ_RGB_RED);
				MostraVision(imagem_sem_overlay, ON);
				while(STATUS.Continua)
					Delay_thread(0.010);
				Delay_thread(0.300);
				vez = 1;
				status = BOT_OFF;
			}
		}
		COR_MENSAGENS = MENS_AZUL;
		parar_mensagens_auto = OFF;
		if(status == REJ)
		{
    		imaqDispose(roi);
			return -1;
		}
		roi = imaqGetWindowROI(0);
		contourID = imaqGetContour (roi, 0);
		contourInfo = imaqGetContourInfo2 (roi, contourID);
		
		roi_reajust.start.x   = contourInfo->structure.line->start.x;
		roi_reajust.start.y   = contourInfo->structure.line->start.y;
		roi_reajust.end.x	  = contourInfo->structure.line->end.x;
		roi_reajust.end.y	  = contourInfo->structure.line->end.y;
		
		roi_selecionado->start.x = roi_anterior.start.x + (roi_reajust.start.x	- roi_loc_corrig.start.x);
		roi_selecionado->end.x	 = roi_anterior.end.x   + (roi_reajust.end.x 	- roi_loc_corrig.end.x);
		roi_selecionado->start.y = roi_anterior.start.y + (roi_reajust.start.y	- roi_loc_corrig.start.y);
		roi_selecionado->end.y	 = roi_anterior.end.y   + (roi_reajust.end.y 	- roi_loc_corrig.end.y);
		GravaCoordenadasLine(roi_nome, roi_selecionado, arq_vision);
		MostraVision(image, OFF);
    	imaqDispose(roi);
    	imaqDispose(imagem_sem_overlay);
return 0;
}
/*-----------------------------------------------------------------------------------------------------
                    Seleciona a iluminação para os testes de vision
------------------------------------------------------------------------------------------------------*/
void VisionIluminacao(int iluminar_extern)
{
double 
	delay_ill_on = 0.4;
		// iluminação maxima
		if(iluminar_extern == ILUM_SUP_ON_MAX && !SAIDA.iluminacao_superior_max)
		{
			SAIDA.iluminacao_superior_max = ON;
			Delay_thread(delay_ill_on);
		}
		else if(!iluminar_extern == ILUM_SUP_ON_MAX && SAIDA.iluminacao_superior_max) //desliga iluminação
		{
			SAIDA.iluminacao_superior_max = OFF;
			Delay_thread(delay_ill_on);
		}
		// iluminação media
		else if(iluminar_extern == ILUM_SUP_ON_MED && !SAIDA.iluminacao_superior_med)
		{
			SAIDA.iluminacao_superior_med = ON;
			Delay_thread(delay_ill_on);
		}
		else if(!iluminar_extern == ILUM_SUP_ON_MED && SAIDA.iluminacao_superior_med) //desliga iluminação
		{
			SAIDA.iluminacao_superior_med = OFF;
			Delay_thread(delay_ill_on);
		}
		// iluminação minima
		else if(iluminar_extern == ILUM_SUP_ON_MIN && !SAIDA.iluminacao_superior_min)
		{
			SAIDA.iluminacao_superior_min = ON;
			Delay_thread(delay_ill_on);
		}
		else if(!iluminar_extern == ILUM_SUP_ON_MIN && SAIDA.iluminacao_superior_min) //desliga iluminação
		{
			SAIDA.iluminacao_superior_min = OFF;
			Delay_thread(delay_ill_on);
		}
		
		else if(iluminar_extern == ILUM_SUP_OFF)
		{
			SAIDA.iluminacao_superior_max = OFF;
			SAIDA.iluminacao_superior_med = OFF;
			SAIDA.iluminacao_superior_min = OFF;
			Delay_thread(0.01);
		}
}
/******************************** CapturaImagemVision ************************************************************************/
int CapturaImagemVision(int sessao_id , int iluminar_extern, float valor_shutter, float valor_gain)
{
int
    setup_camera = OFF,
	error_code,
	status = 0;
Image 
	*imagem=NULL;
ImageType
	imageType;
PixelValue
	fillValue;

	//////////////////////////////////////////
	if(valor_gain < MIN_GAIN_CAMERA || valor_shutter < MIN_SHUTTER_CAMERA)
	{
		MessagePopup ("Erro setup câmera",
              "Valores de shutter ou gain inválidos para a camera");
		return -1;
	}
	imagem = imaqCreateImage (IMAQ_IMAGE_U8, 2);
	
	VisionIluminacao(iluminar_extern);
	
	if(sessao_id != sessao_id_anterior ||  gain_anterior !=valor_gain || shutter_anterior !=valor_shutter)
	{
		SetBreakOnLibraryErrors (0);
		IMAQdxStopAcquisition (sessao_id);// != IMAQdxErrorSuccess)
		Delay(0.50);
		IMAQdxUnconfigureAcquisition(sessao_id);
		Delay(0.50);
		status = IMAQdxConfigureAcquisition (sessao_id, 1, 1);// != IMAQdxErrorSuccess)
		Delay(0.050);
		status = IMAQdxStartAcquisition (sessao_id);// != IMAQdxErrorSuccess)
		Delay(0.050);
		SetBreakOnLibraryErrors (1);
		Delay(0.010);
		sessao_id_anterior = sessao_id;
		gain_anterior		= -1;
		shutter_anterior 	= -1;
	} 
	if(gain_anterior != valor_gain)
	{
		Delay(0.010);
		IMAQdxSetAttribute (sessao_id, "CameraAttributes::AnalogControl::Gain", IMAQdxValueTypeF64, valor_gain);//item 22
		Delay(0.010);
		gain_anterior 	= valor_gain;
		setup_camera = ON;
	}
	if(shutter_anterior !=valor_shutter)
	{
		Delay(0.010);
		IMAQdxSetAttribute (sessao_id, "CameraAttributes::AcquisitionControl::ExposureTime", IMAQdxValueTypeF64, valor_shutter);//item 26
		Delay(0.010);
		shutter_anterior 	= valor_shutter;
		setup_camera = ON;
	}
	if(setup_camera)
	{
		Delay(0.200);
		IMAQdxGetImage (sessao_id, imagem_camera, IMAQdxBufferNumberModeNext, 0, 0);
		Delay(0.100);
	}
	else	
		Delay(0.100);
	if(IMAQdxGetImage (sessao_id, imagem_camera, IMAQdxBufferNumberModeNext, 0, 0) != IMAQdxErrorSuccess)
		return -3;
    //-------------------------------------------------------------------//
    //                         Geometry: Rotation                        //
    //-------------------------------------------------------------------//
    // Gets the type of the source image.
    imaqGetImageType(imagem_camera, &imageType);
    // Sets the fill value to black.
    IVA_SetPixelValue(&fillValue, imageType, 0, 0, 0, 0);
    // Rotates the image.
    status = imaqRotate2(imagem_camera, imagem_camera, 180, fillValue, IMAQ_BILINEAR, TRUE);
	if(status <= 0)
	{
		error_code = imaqGetLastError ();
		return -4;
	}
	
	
	
	return SEM_ERRO;
}

/******************************** CapturaImagem_Referencia ************************************************************************/
double CapturaImagem_Referencia(int sessao_id , int iluminacao, float valor_shutter, float valor_gain, float contraste, float gamma)
{
double 
	res=1;
int 
	status;

BCGOptions 
	bcgOptions;
	
 
		//////////////////////////////////////////////
		//  captura uma imagem
		//////////////////////////////////////////////
		status = CapturaImagemVision(sessao_id, iluminacao, valor_shutter, valor_gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera");
			return -1;					
		}
		imaqDuplicate (imagem_trabalho, imagem_camera);
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}

		IVA_CLRExtractValue(imagem_trabalho);
		if(monitorar_vision || mostrar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
		//-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//

	    bcgOptions.brightness 	= 128;
	    bcgOptions.contrast 	= contraste;
	    bcgOptions.gamma 		= gamma;
	    // Applies brightness, contrast, and gamma correction to the image
	    imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL);

		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
	    
return res;
}


/******************************** Vision_Template_Forma_e_Cor ************************************************************************/
double Vision_Template_Forma_e_Cor(DADOS_CFG_VISION cfg_flex, int *erro_locator, int referenciar, char *arquivo_templt, int tolerancia, COORDENADAS_ROI coordenadas, int ilumicacao, int ver_cor_apenas, int treinar_template_rej)
{	   
char 
	arquivo[256],
	path[200];
ContourID 
	contourID;
ContourInfo2 
	*contourInfo = NULL;
Image  
	*template_destino = NULL ,
	*image_copia  	  = NULL;
LearnColorPatternOptions 
	options;
long 
	tamanho;
int success = 1,
	status,
	treinado,
	i,
	arquivo_presente,
	confirmacao;
IVA_Data *ivaData;

BCGOptions 
	redBCGOptions,
 	greenBCGOptions,
 	blueBCGOptions;
ROI 
	*roi = NULL,
	*roi_procura = NULL,
	*roi_temp;
float 
    advOptionsValues[9] = {10,2,300,3,5,2,1,20,0},
	angleRangeMin[2] = {-20,0},
	angleRangeMax[2] = {20,0};
int advOptionsItems[9] = {0,2,5,6,7,8,1,3,4};
	
COORDENADAS_ROI 
	roi_selecionado;
	
	if(tolerancia < MIN_TOL_TEMPLATES)
	{
		MessagePopup ("Erro de setup",
					  "Tolerancia do template abaixo do minimo permitido");
		return -1;
	}
    *erro_locator=OFF;
    // Initializes internal data (buffers and array of points for caliper measurements)
	VisionErrChk(ivaData = IVA_InitData(8, 0)); 
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_sup, ilumicacao, cfg_flex.camera_shutter_montagem, cfg_flex.camera_gain_montagem);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		IVA_CLRExtractIntensity(imagem_camera);
		imaqDuplicate (imagem_trabalho, imagem_camera);
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
	}
	if(imagem_trabalho == NULL)
	{ 
		MessagePopup ("ERRO ", "Falha na imagem p/ visão dos Light Guide");
		return -1;					
	}
	
	if(monitorar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
    template_encontrado = 0;
    //VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
    treinado = OFF;
	roi_temp = imaqCreateROI();
	
retesta:    
    i=0;
	//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
	if(mostrar_vision || treinar_vision)
	{
		imaqOverlayRect(imagem_trabalho, imaqMakeRect(coordenadas.top, coordenadas.left, coordenadas.height, coordenadas.width),
			&IMAQ_RGB_YELLOW, IMAQ_DRAW_VALUE, NULL);
		MostraVision(imagem_trabalho, OFF);
    }
	do
    {
    	i++;
    	Fmt(arquivo, "%s<%s%d%s",arquivo_templt,i,".png");
    	arquivo_presente = GetFileInfo (arquivo, &tamanho);
		if(arquivo_presente == 1)
			
			{

			VisionErrChk( IVA_MatchPattern_2(imagem_trabalho,
                                     ivaData, arquivo,
									 IMAQ_MATCH_LOW_DISCREPANCY, 	  
                                     angleRangeMin, angleRangeMax,
									 advOptionsItems, advOptionsValues,
									 9, 1, tolerancia,
                                     roi_procura, 3));
			Delay_thread(0.005);
		}
	}while(!template_encontrado && arquivo_presente == 1);

		
	//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
	imaqOverlayRect(imagem_trabalho, imaqMakeRect(coordenadas.top, coordenadas.left, coordenadas.height, coordenadas.width),
		&IMAQ_RGB_YELLOW, IMAQ_DRAW_VALUE, NULL);
	if(mostrar_vision)
	{
		MostraVision(imagem_trabalho, OFF);
		Delay_thread(0.01);
	}
	
	
	
	if(!template_encontrado && treinar_vision && !treinado && treinar_template_rej)
	{
			if(i > MAX_NUM_TEMPL_PERMIT)
			{
				MessagePopup ("Não pode criar template...",
							  "Excedeu o número máximo permitido de templates!!");
				goto Error;
			}
			//********** Verifica qual ferramenta aplicar para construção/manipulação do ROI **********************************
			roi = imaqGetWindowROI(0);
			if (roi && imaqGetContourCount(roi)) 
			{
				contourID = imaqGetContour (roi, 0);
				contourInfo = imaqGetContourInfo2 (roi, contourID);
			}
			if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
			{
				imaqSetCurrentTool (IMAQ_RECTANGLE_TOOL);
			}
			else
			{
				imaqSetCurrentTool (IMAQ_SELECTION_TOOL);
			}
		
		
	erro_roi:
	    	if(mostrar_vision)
	    		MostraVision(imagem_trabalho, OFF);
		
			//************* Espera criação do Template ***********************************************************************
			
			parar_mensagens_auto = ON;
			strcpy(path,CAMINHO_SONS);
			strcat(path,"bongo06.wav");	 
			CVI_PlaySound (path, ON );
			Delay_thread(1);
			CVI_StopCurrentSound();
			Mensagem("Parada p/ criação de template...",arquivo, 
				"Especifique a região de interesse dentro da região de procura(amarelo)",
				"Após finalizar aperte continue.", MENS_AZUL);
			
			//modo_thread_sistema = ESCRITA_E_LEIT_NORMAL;
			while(STATUS.Continua == ON || STATUS.Rejeita == ON)
			{
				Delay_thread(0.1);
			}
			while(STATUS.Continua == OFF && STATUS.Rejeita == OFF)
			{
				Delay_thread(0.1);
			}
			parar_mensagens_auto = OFF;
			if(STATUS.Rejeita == ON)
				goto Error;
		
			//********** Verifica se houve um ROI selecionado **********************************
			roi = imaqGetWindowROI(0);
			if (roi && imaqGetContourCount(roi)) 
			{
				contourID = imaqGetContour (roi, 0);
				contourInfo = imaqGetContourInfo2 (roi, contourID);
			}
			if (contourInfo==NULL) 
			{
				MessagePopup ("Criando novo template",
							  "Selecione uma Região de Interesse(ROI)");
				goto erro_roi;
			}
			image_copia = imaqCreateImage (IMAQ_IMAGE_RGB, 7);
			imaqDuplicate (image_copia, imagem_trabalho);
			roi_selecionado.top=contourInfo->structure.rect->top;
			roi_selecionado.left=contourInfo->structure.rect->left;
			roi_selecionado.height=contourInfo->structure.rect->height;
			roi_selecionado.width=contourInfo->structure.rect->width;
			roi_selecionado.angle=0;					 
			if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
			{
				MessagePopup ("Criando novo template",
							  "Selecione uma Região de Interesse(ROI)");
				goto erro_roi;
			}
			else if(roi_selecionado.top <= coordenadas.top || roi_selecionado.left <= coordenadas.left
				|| roi_selecionado.height>= coordenadas.height || roi_selecionado.width>= coordenadas.width)
			{
				MessagePopup ("Criando novo template",
							  "A Região de Interesse(ROI) deve estar dentro da região de procura.");
				goto erro_roi;
			}
		
			else if(VisionTemplatesOFF(image_copia, roi_selecionado, OFF)!=1)
			{
				MessagePopup ("Criando novo template",
							  "Quantidade insuficiente de pixels para treinamento do led.");
				goto erro_roi;
			}
			else
			{ 
				confirmacao = ConfirmPopup ("Confirma novo template",
	              	"Confirme a criação de novo template.");
				
		   if(confirmacao)
            {
				Mensagem("Aguarde...","Criando novo template.", arquivo,"",MENS_AZUL);
				template_destino = imaqCreateImage (IMAQ_IMAGE_U8, 2);
				
				/* extract image to learn */
				imaqScale (template_destino, imagem_trabalho, 1, 1, IMAQ_SCALE_LARGER, *(contourInfo->structure.rect));
				//options.featureMode= IMAQ_COLOR_AND_SHAPE_FEATURES;
				options.featureMode= IMAQ_SHAPE_FEATURES;
				options.learnMode = IMAQ_LEARN_ALL;
				options.threshold = 80;
				options.ignoreMode = IMAQ_IGNORE_NONE;
				options.colorsToIgnore = NULL;
				options.numColorsToIgnore = 0;
				
				imaqLearnPattern2 (template_destino, IMAQ_LEARN_ALL, NULL);
	           	imaqWriteVisionFile (template_destino, arquivo, NULL); 	
	    		imaqDispose(template_destino);
				treinado = ON;
				goto retesta;
			}
			else 
				template_encontrado=0;	
        }

	           		imaqDispose(image_copia);
	}
    // Releases the memory allocated in the IVA_Data structure.
	
Error:
    //IVA_DisposeData(ivaData);
    IVA_DisposeData2(ivaData);
	
	return template_encontrado;
} 


/******************************** VisionGeometricTemplat_PB ************************************************************************/
int VisionGeometricTemplat_PB(Image *image, char *arquivo_templt, int tolerancia, COORDENADAS_ROI coordenadas, int treinar_template_rej, int usar_locator)
{
	Image  
		*template_destino, 
		*image_copia;
	ContourID 
		contourID;
	ContourInfo2 
		*contourInfo = NULL;
	int 
		success = 1,
    	i;
	IVA_Data *ivaData;
    ROI 
		*roi,
        *roi1 = NULL;
ImageType 
		imageType;
    int long tamanho;
    int 
		arquivo_presente, 
		treinado, 
		confirmacao;
    float rangeMin[4] = {0,0,50,0};
    float rangeMax[4] = {0,0,200,50};
char 
		arquivo[256],
		path[200];
    COORDENADAS_ROI 
		roi_selecionado;
Rect
	coordenada_tranformada;

	 //CurveOptions
	LearnGeometricPatternAdvancedOptions2 advancedLearnOptions;
	 
    template_encontrado = 0;
    //VisionErrChk(ivaData = IVA_InitData());
    VisionErrChk(ivaData = IVA_InitData(4, 0));
	VisionErrChk(imaqGetImageType(image, &imageType));
	
    treinado = OFF;
	if(tolerancia < MIN_TOL_TEMPLATES)
	{
		MessagePopup ("Erro de setup",
					  "Tolerancia do template abaixo do minimo permitido");
		return -1;
	}
   VisionErrChk(roi1 = imaqCreateROI());
    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    VisionErrChk(imaqAddRectContour(roi1, imaqMakeRect(coordenadas.top, coordenadas.left, coordenadas.height, coordenadas.width)));
retesta:    
    i=0;
    do
    {
    	i++;
    	Fmt(arquivo, "%s<%s%d%s",arquivo_templt,i,".png");
    	arquivo_presente = GetFileInfo (arquivo, &tamanho);
    	if(arquivo_presente == 1)
    	{
			
				
			VisionErrChk(IVA_MatchGeometricPattern2(image, ivaData, 
				arquivo, IMAQ_NORMAL_IMAGE, 75, IMAQ_NORMAL, 15, 5, 5, 10, 0, FALSE, 
				rangeMin, rangeMax, 1, tolerancia, IMAQ_ORIGINAL_CONTRAST, 
				IMAQ_GEOMETRIC_MATCHING_BALANCED, roi1, 3));
		    // Cleans up resources associated with the object
			Delay_thread(0.005);


		}
	}while(!template_encontrado && arquivo_presente == 1);
	
	//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
	imaqSetROIColor (roi1, &IMAQ_RGB_YELLOW);
	imaqOverlayROI (image, roi1 , IMAQ_POINT_AS_CROSS, NULL, NULL);
		if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(image, OFF);
	}
	if(!template_encontrado && treinar_vision && !treinado && treinar_template_rej)
	{
		if(i > MAX_NUM_TEMPL_PERMIT)
		{
			MessagePopup ("Não pode criar template...",
						  "Excedeu o número máximo permitido de templates!!");
			goto Error;
		}
		//********** Verifica qual ferramenta aplicar para construção/manipulação do ROI **********************************
		roi = imaqGetWindowROI(0);
		if (roi && imaqGetContourCount(roi)) 
		{
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
		}
		if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
		{
			imaqSetCurrentTool (IMAQ_RECTANGLE_TOOL);
		}
		else
		{
			imaqSetCurrentTool (IMAQ_SELECTION_TOOL);
		}
		
		//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
		imaqClearOverlay (image, NULL);
		//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
		imaqSetROIColor (roi1, &IMAQ_RGB_YELLOW);
		imaqOverlayROI (image, roi1 , IMAQ_POINT_AS_CROSS, NULL, NULL);
		
erro_roi:
		MostraVision(image, OFF);
		
		//************* Espera criação do Template ***********************************************************************
		parar_mensagens_auto = ON;
		strcpy(path,CAMINHO_SONS);
		strcat(path,"bongo06.wav");	 
		CVI_PlaySound (path, ON );
		Delay_thread(1);
		CVI_StopCurrentSound();
		Mensagem("Parada p/ criação de template...",arquivo, 
				"Especifique a região de interesse dentro da região de procura(amarelo)",
				"Após finalizar aperte continue.", MENS_AZUL);
		while(STATUS.Continua == ON || STATUS.Rejeita == ON)
		{
			Delay_thread(0.1);
		}
		while(STATUS.Continua == OFF && STATUS.Rejeita == OFF)
		{
			Delay_thread(0.1);
		}
		parar_mensagens_auto = OFF;
		if(STATUS.Rejeita == ON)
			goto Error;
		
		//********** Verifica se houve um ROI selecionado **********************************
		roi = imaqGetWindowROI(0);
		if (roi && imaqGetContourCount(roi)) 
		{
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
		}
		if (contourInfo==NULL) 
		{
			MessagePopup ("Criando novo template",
						  "Selecione uma Região de Interesse(ROI)");
			goto Error;
			
		}
		image_copia = imaqCreateImage (IMAQ_IMAGE_U8, 2);
		imaqDuplicate (image_copia, image);
		roi_selecionado.top=contourInfo->structure.rect->top;
		roi_selecionado.left=contourInfo->structure.rect->left;
		roi_selecionado.height=contourInfo->structure.rect->height;
		roi_selecionado.width=contourInfo->structure.rect->width;
		roi_selecionado.angle=0;					 
		imaqGetROIBoundingBox (roi1, &coordenada_tranformada);
		coordenadas.height=coordenada_tranformada.height;
		coordenadas.left=coordenada_tranformada.left;
		coordenadas.width=coordenada_tranformada.width;
		coordenadas.top=coordenada_tranformada.top;
		if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
		{
			MessagePopup ("Criando novo template",
						  "Selecione uma Região de Interesse(ROI)");
			goto Error;
		}
		else if(roi_selecionado.top <= coordenadas.top || roi_selecionado.left <= coordenadas.left
			|| roi_selecionado.height>= coordenadas.height || roi_selecionado.width>= coordenadas.width)
		{
			MessagePopup ("Criando novo template",
						  "A Região de Interesse(ROI) deve estar dentro da região de procura.");
			goto Error;
		}
		
		else if(VisionTemplatesOFF(image_copia, roi_selecionado, ON)!=1)
		{
			MessagePopup ("Criando novo template",
						  "Quantidade insuficiente de pixels para treinamento do led.");
			goto Error;
		}
		else
		{ 
			confirmacao = ConfirmPopup ("Confirma novo template",
              	"Confirme a criação de novo template.");
            if(confirmacao)
            {
				Mensagem("Aguarde...","Criando novo template.", arquivo,"", MENS_AZUL);
				template_destino = imaqCreateImage (IMAQ_IMAGE_U8, 2);
				
				/* extract image to learn */
				imaqScale (template_destino, image, 1, 1, IMAQ_SCALE_LARGER, *(contourInfo->structure.rect));
				
			    // Setup the learn options
			    advancedLearnOptions.minScaleFactor = 60;
			    advancedLearnOptions.maxScaleFactor = 125;
			    advancedLearnOptions.minRotationAngleValue = 0;
			    advancedLearnOptions.maxRotationAngleValue = 360;
			    advancedLearnOptions.imageSamplingFactor = 0;
    
				
   				if(imaqLearnGeometricPattern2 (template_destino, IMAQ_NO_OFFSET, 0, NULL, &advancedLearnOptions, NULL) != 0)
				{
					imaqWriteVisionFile (template_destino, arquivo, NULL); 	
					treinado = ON;
	    		}
				else
				{
					MessagePopup ("Falha ao tentar treinar template", "Houve uma falha ao treinar o template selecionado");
				}
				imaqDispose(template_destino);
				goto retesta;
			}
			else 
				template_encontrado=0;	
        }
		//****************************************************************************************************************
	}
    // Releases the memory allocated in the IVA_Data structure.
    IVA_DisposeData(ivaData);
Error:
	imaqDispose(roi1);
	return template_encontrado;
}

/******************************** VisionPatt_Template_PB ************************************************************************/
int VisionPatt_Template_PB(char *arquivo_templt, int tolerancia, COORDENADAS_ROI coordenadas, int treinar_template_rej, int referenciar, double shutter, double gain)
{
Image  
	*template_destino, 
	*image_copia;
ContourID 
	contourID;
ContourInfo2 
	*contourInfo = NULL;
int 
	success = 1,
	i,
	status,
	arquivo_presente, 
	treinado, 
	confirmacao;
IVA_Data *ivaData;
ROI 
	*roi = NULL,
	*roi_procura = NULL;
 BCGOptions bcgOptions;
 ImageType 
	imageType;
PixelValue fillValue;
int long tamanho;
float angleRangeMin[2] = {0,0};
float angleRangeMax[2] = {0,0};
int advOptionsItems[9] = {0,2,5,6,7,8,1,3,4};
float advOptionsValues[9] = {10,2,300,3,5,2,1,20,0};
char 
	arquivo[256],
	path[200];
COORDENADAS_ROI 
		roi_selecionado;
Rect
	coordenada_tranformada;
   
    ////////////////////////////////////////
	if(tolerancia < MIN_TOL_TEMPLATES)
	{
		MessagePopup ("Erro de setup",
					  "Tolerancia do template abaixo do minimo permitido");
		return -1;
	}
    template_encontrado = 0;
	
    VisionErrChk(ivaData = IVA_InitData(8, 0));
    treinado = OFF;
	VisionErrChk(roi_procura = imaqCreateROI());
    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    VisionErrChk(imaqAddRectContour(roi_procura, imaqMakeRect(coordenadas.top, coordenadas.left, 
				coordenadas.height, coordenadas.width)));
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_sup, ON, shutter, gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
	    //-------------------------------------------------------------------//
	    //                         Geometry: Rotation                        //
	    //-------------------------------------------------------------------//
	    // Gets the type of the source image.
	    VisionErrChk(imaqGetImageType(imagem_camera, &imageType));
	    // Sets the fill value to black.
	    IVA_SetPixelValue(&fillValue, imageType, 0, 0, 0, 0);
	    // Rotates the image.
	    VisionErrChk(imaqRotate2(imagem_camera, imagem_camera, 180, fillValue, IMAQ_BILINEAR, TRUE));
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
		imaqDuplicate (imagem_trabalho, imagem_camera);
		VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//
	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = dados_cfg_vision.contraste_montagem;
	    bcgOptions.gamma = dados_cfg_vision.gamma_montagem;

	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	}
	else
	{
		VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
	}
	if(imagem_trabalho == NULL)
	{ 
		MessagePopup ("ERRO ", "Falha na imagem p/ visão das teclas");
		return -1;					
	}
	
	//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
	imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
	imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
	if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
	
retesta:    
    i=0;
	// verifica todos os arquivos template
    do{
    	i++;
    	Fmt(arquivo, "%s<%s%d%s",arquivo_templt,i,".png");
    	arquivo_presente = GetFileInfo (arquivo, &tamanho);
    	if(arquivo_presente == 1)
    	{

			VisionErrChk( IVA_MatchPattern_2(imagem_trabalho,
                                     ivaData, arquivo,
									 IMAQ_MATCH_LOW_DISCREPANCY, 	   //IMAQ_MATCH_ROTATION_INVARIANT,
                                     angleRangeMin, angleRangeMax,
									 advOptionsItems, advOptionsValues,
									 9, 1, tolerancia,
                                     roi_procura, 3));
			Delay_thread(0.005);
		}
	}while(!template_encontrado && arquivo_presente == 1);
	
	if(!template_encontrado && treinar_vision && !treinado && treinar_template_rej)
	{
		if(i > MAX_NUM_TEMPL_PERMIT)
		{
			MessagePopup ("Não pode criar template...",
						  "Excedeu o número máximo permitido de templates!!");
			goto Error;
		}
		//********** Verifica qual ferramenta aplicar para construção/manipulação do ROI **********************************
		roi = imaqGetWindowROI(0);
		if (roi && imaqGetContourCount(roi)) 
		{
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
		}
		if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
		{
			imaqSetCurrentTool (IMAQ_RECTANGLE_TOOL);
		}
		else
		{
			imaqSetCurrentTool (IMAQ_SELECTION_TOOL);
		}
		
		imaqClearOverlay (imagem_trabalho, NULL);
		//////////// Faz a marcação do região de procura /////////////////////////////////////////////////////
		imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
		imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
		
erro_roi:
		MostraVision(imagem_trabalho, OFF);
		
		//************* Espera criação do Template ***********************************************************************
		parar_mensagens_auto = ON;
		strcpy(path,CAMINHO_SONS);
		strcat(path,"bongo06.wav");	 
		CVI_PlaySound (path, ON );
		Delay_thread(1);
		CVI_StopCurrentSound();
		Mensagem("Parada p/ criação de template...",arquivo, 
				"Especifique a região de interesse dentro da região de procura(AZUL)",
				"Após finalizar aperte continue.", MENS_AZUL);
		while(STATUS.Continua == ON || STATUS.Rejeita == ON)
		{
			Delay_thread(0.1);
		}
		while(STATUS.Continua == OFF && STATUS.Rejeita == OFF)
		{
			Delay_thread(0.1);
		}
		parar_mensagens_auto = OFF;
		if(STATUS.Rejeita == ON)
			goto Error;
		
		//********** Verifica se houve um ROI selecionado **********************************
		roi = imaqGetWindowROI(0);
		if (roi && imaqGetContourCount(roi)) 
		{
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
		}
		if (contourInfo==NULL) 
		{
			MessagePopup ("Criando novo template",
						  "Selecione uma Região de Interesse(ROI)");
			goto Error;
			
		}
		image_copia = imaqCreateImage (IMAQ_IMAGE_U8, 2);
		imaqDuplicate (image_copia, imagem_trabalho);
		roi_selecionado.top=contourInfo->structure.rect->top;
		roi_selecionado.left=contourInfo->structure.rect->left;
		roi_selecionado.height=contourInfo->structure.rect->height;
		roi_selecionado.width=contourInfo->structure.rect->width;
		roi_selecionado.angle=0;
		// atualiza as coordenadas do ROI mostrado na imagem
		imaqGetROIBoundingBox (roi_procura, &coordenada_tranformada);
		coordenadas.height=coordenada_tranformada.height;
		coordenadas.left=coordenada_tranformada.left;
		coordenadas.width=coordenada_tranformada.width;
		coordenadas.top=coordenada_tranformada.top;
		
		if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
		{
			MessagePopup ("Criando novo template",
						  "Selecione uma Região de Interesse(ROI)");
			//goto erro_roi;
			goto Error;
		}
		else if(roi_selecionado.top <= coordenadas.top || roi_selecionado.left <= coordenadas.left
			|| roi_selecionado.height>= coordenadas.height || roi_selecionado.width>= coordenadas.width)
		{
			MessagePopup ("Criando novo template",
						  "A Região de Interesse(ROI) deve estar dentro da região de procura.");
			goto Error;
		}
		
		else if(VisionTemplatesOFF(image_copia, roi_selecionado, ON)!=1)
		{
			MessagePopup ("Criando novo template",
						  "Quantidade insuficiente de pixels para treinamento do led.");
			goto Error;
		}
		else
		{ 
			confirmacao = ConfirmPopup ("Confirma novo template",
              	"Confirme a criação de novo template.");
            if(confirmacao)
            {
				Mensagem("Aguarde...","Criando novo template.", arquivo,"", MENS_AZUL);
				//template_destino = imaqCreateImage (IMAQ_IMAGE_RGB, 2);
				template_destino = imaqCreateImage (IMAQ_IMAGE_U8, 2);
				
				/* extract image to learn */
				imaqScale (template_destino, imagem_trabalho, 1, 1, IMAQ_SCALE_LARGER, *(contourInfo->structure.rect));
			  	
				if(imaqLearnPattern2 (template_destino, IMAQ_LEARN_ALL, NULL) != 0)
				{
	           		imaqWriteVisionFile (template_destino, arquivo, NULL); 	
					treinado = ON;
	    		}
				else
				{
					MessagePopup ("Falha ao tentar treinar template", "Houve uma falha ao treinar o template selecionado");
				}
	    		imaqDispose(template_destino);
				goto retesta;
			}
			else 
				template_encontrado=0;	
        }
		//****************************************************************************************************************
	}
    // Releases the memory allocated in the IVA_Data structure.
    IVA_DisposeData(ivaData);
Error:
	imaqDispose(roi_procura);
	return template_encontrado;
}

/******************************** ImagemFlexBuffer ************************************************************************/
int ImagemFlexBuffer(int usar_locator)
{
	
BCGOptions 
	bcgOptions;
	
		imaqDuplicate (imagem_trabalho, imagem_camera);
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
		
		IVA_CLRExtractValue(imagem_trabalho);
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
		//-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//

	    bcgOptions.brightness = 128;

    // Applies brightness, contrast, and gamma correction to the image
    imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL);
	if(monitorar_vision)
	{
		MostraVision(imagem_trabalho, OFF);
	}
return 0;
}
/******************************** Vision_2_Edge ************************************************************************/
int Vision_2_Edge(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain)
{
	int 
		success = 1,
		status,
		res = -1;
	IVA_Data *ivaData;
    ROI 
    	*roi_procura = NULL;
    ImageType 
		imageType;
 	BCGOptions
		bcgOptions;
    	   
	
    VisionErrChk(ivaData = IVA_InitData(8, 0));
	VisionErrChk(roi_procura = imaqCreateROI());
    VisionErrChk(imaqAddRectContour(roi_procura, imaqMakeRect(coordenadas.top, coordenadas.left, 
				coordenadas.height, coordenadas.width)));
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_sup, ILUM_SUP_ON_MAX, shutter, gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
		imaqTransformROI2 (roi_procura, &transform_main.referenceSystem, &transform_main.measurementSystem);
		imaqDuplicate (imagem_trabalho, imagem_camera);
		VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//
	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = dados_cfg_vision.contraste_montagem;
	    bcgOptions.gamma = dados_cfg_vision.gamma_montagem;
		  
	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	}
	else
	{
		VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
	}
	//////////// Faz a marcação da região de medição do Quantify /////////////////////////////////////////////////////
	imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
	imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
	if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
	
	IVA_EdgeTool2(imagem_trabalho, roi_procura, IMAQ_ALL, 
		IMAQ_SEARCH_FOR_ALL_EDGES, 
		17, //kernel size
		13, //width
		120, //strenght
		IMAQ_BILINEAR_FIXED, 
		IMAQ_AVERAGE_COLUMNS, ivaData, 5);
	res = 1;
	if(quant_edges != 2)
		res = -1;
	if(mostrar_vision)
	{
		MostraVision(imagem_trabalho, OFF);
	}
	
    IVA_DisposeData(ivaData);
Error:
	imaqDispose(roi_procura);
	if(coord_edge_y == 0 || coord_edge_x == 0)
		return -2;
	return res;
}
/*---------------------------------------------------------------------------------------------
			  Detecta os edges com em uma linha
-----------------------------------------------------------------------------------------------*/
int Vision_Line_Edge(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain)
{
	int 
		success = 1,
		status,
		res = 0;
	IVA_Data *ivaData;
    ROI 
    	*roi_procura = NULL;
    ImageType 
		imageType;
 	BCGOptions
		bcgOptions;
    	   
	
    VisionErrChk(ivaData = IVA_InitData(8, 0));
	VisionErrChk(roi_procura = imaqCreateROI());
    VisionErrChk(imaqAddLineContour(roi_procura, imaqMakePoint(coordenadas.top, coordenadas.left),  //1528,592
                                         imaqMakePoint(coordenadas.height, coordenadas.width))); //1526, 1194
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_inf, ILUM_INF_ON_MAX, shutter, gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
		imaqTransformROI2 (roi_procura, &transform_lcd.referenceSystem, &transform_lcd.measurementSystem);
		imaqDuplicate (imagem_trabalho, imagem_camera);
		VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//
	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = dados_cfg_vision.contraste_pins_display;
	    bcgOptions.gamma = dados_cfg_vision.gamma_pins_display;
		  
	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	}
	else
	{
		VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
	}
	
	//////////// Faz a marcação da região de medição do Quantify /////////////////////////////////////////////////////
	imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
	imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
	if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
	
	IVA_EdgeTool2(imagem_trabalho, roi_procura, IMAQ_ALL, 
		IMAQ_SEARCH_FOR_FALLING_EDGES,//IMAQ_SEARCH_FOR_ALL_EDGES, 
		5,  //kernel size
		9,  //width
		80, //strenght
		IMAQ_BILINEAR_FIXED, 
		IMAQ_AVERAGE_COLUMNS, ivaData, 5);
	res = quant_edges;
	if(mostrar_vision)
	{
		MostraVision(imagem_trabalho, OFF);
	}
	
    IVA_DisposeData(ivaData);
Error:
	imaqDispose(roi_procura);
	if(coord_edge_y == 0 || coord_edge_x == 0)
		res = -2;
	return res;
}

/*---------------------------------------------------------------------------------------------
			  Detecta os pinos do display por quantify
-----------------------------------------------------------------------------------------------*/
int Vision_Quant_PinsDisp(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain)
{
int 
	success = 1,
	status,
	res = 0,
	i;
IVA_Data *ivaData;
ROI 
	*roi_procura = NULL;
ImageType 
	imageType;
BCGOptions
	bcgOptions;
int pKernel[25] = {0,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,
    1,0};
StructuringElement structElem;

	VisionErrChk(ivaData = IVA_InitData(8, 0));
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_inf, ILUM_INF_ON_MAX, shutter, gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
		imaqDuplicate (imagem_trabalho, imagem_camera);
		VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//
	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = dados_cfg_vision.contraste_pins_display;
	    bcgOptions.gamma = dados_cfg_vision.gamma_pins_display;
		  
	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    //-------------------------------------------------------------------//
	    //                          Gray Morphology                          //
	    //-------------------------------------------------------------------//

	    // Sets the structuring element.
	    structElem.matrixCols = 5;
	    structElem.matrixRows = 5;
	    structElem.hexa = 0;
	    structElem.kernel = pKernel;

	    // Applies morphological transformations
	    VisionErrChk(imaqGrayMorphology2(imagem_trabalho, imagem_trabalho, IMAQ_DILATE, &structElem, 3));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	}
	else
	{
		VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
	}
	
	if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
	res = 1;
	for(i=1; i<= NUM_PINOS_DISPL; i++)
	{
		VisionErrChk(roi_procura = imaqCreateROI());
	    VisionErrChk(imaqAddRectContour(roi_procura, imaqMakeRect(coord_rois_pins_lcd[i].top, coord_rois_pins_lcd[i].left, 
					coord_rois_pins_lcd[i].height, coord_rois_pins_lcd[i].width)));
		imaqTransformROI2 (roi_procura, &transform_lcd.referenceSystem, &transform_lcd.measurementSystem);
		//////////// Faz a marcação da região de medição do Quantify /////////////////////////////////////////////////////
		imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
		imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
		if(mostrar_vision)
			MostraVision(imagem_trabalho, OFF);
		IVA_Quantify(imagem_trabalho, roi_procura);
		if(valor_medio_quantity < MIN_QUANTIFY_PIN_DISP || valor_medio_quantity > MAX_QUANTIFY_PIN_DISP)
		{
			Fmt(MensagemFalha1, "%s<%s%i%s%f[p1]%s","Falha na verificação do pino ",i, " do Display - leu=",valor_medio_quantity, " Pixels");
			res = -1;
			break;
		}
		imaqDispose(roi_procura);
	}
	
	
	
    IVA_DisposeData(ivaData);
Error:
	return res;
}

/******************************** VisionQuantify ************************************************************************/
int VisionQuantify(COORDENADAS_ROI coordenadas, int usar_locator, int referenciar, float shutter, float gain, char * nome_roi)
{
int 
	success = 1,
	status,
	reparametrizado 	= OFF,
	reparam         	= OFF;
IVA_Data *ivaData;
ROI 
	*roi_procura = NULL;
ImageType 
	imageType;
BCGOptions
	bcgOptions;
    	   
	switch(usar_locator)
	{
		case DIFUS_PONTO_1:
		case DIFUS_PONTO_2:
			if(reparam_rois == REPARAM_DIFUSOR_ROIs)
				reparam = ON;
			break;
		case DISPLAY:
			if(reparam_rois == REPARAM_DISPLAY_ROIs)
				reparam = ON;
			break;
		case SHIELD:
			if(reparam_rois == REPARAM_SHIELD_ROIs)
				reparam = ON;
			break;
		case DISP_LINER_PONTO_1:
		case DISP_LINER_PONTO_2:
		case DISP_LINER_PONTO_3:
		case DISP_LINER_PONTO_4:
			if(reparam_rois == REPARAM_LINER_ROIs)
				reparam = ON;
			break;
	}
	if(reparam_rois == REPARAM_ALL || reparam_rois == REPARAM_ROIS)
			reparam = ON;
	
    VisionErrChk(ivaData = IVA_InitData(8, 0));
reparametrizou:
	VisionErrChk(roi_procura = imaqCreateROI());
    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    VisionErrChk(imaqAddRectContour(roi_procura, imaqMakeRect(coordenadas.top, coordenadas.left, 
				coordenadas.height, coordenadas.width)));
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_sup, ON, shutter, gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
		imaqDuplicate (imagem_trabalho, imagem_camera);
		VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//
	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = dados_cfg_vision.contraste_montagem;
	    bcgOptions.gamma = dados_cfg_vision.gamma_montagem;

	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	}
	else
	{
		VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
	}
	if(reparam && !reparametrizado)
		ReparametrizaROI(imagem_trabalho, &coordenadas, nome_roi, id_arq_vision[modelo_atual_index], roi_procura, OFF);  
	switch(usar_locator)
	{
		case NENHUM_LOCATOR:  
			break;
		case LOCATOR_PAINEL:
			imaqTransformROI2 (roi_procura, &transform_main.referenceSystem, &transform_main.measurementSystem);
			break;
		default:
			imaqTransformROI2 (roi_procura, &transform_main.referenceSystem, &transform_main.measurementSystem);
			break;
	}
	if(reparam && !reparametrizado)
	{
		ReparametrizaROI(imagem_trabalho, &coordenadas, nome_roi, id_arq_vision[modelo_atual_index], roi_procura, ON);
		reparametrizado = ON;
		imaqDispose(roi_procura);
		Delay_thread(0.010);
		goto reparametrizou;
	}
	//////////// Faz a marcação da região de medição do Quantify /////////////////////////////////////////////////////
	imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
	imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
	if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
	IVA_Quantify(imagem_trabalho, roi_procura);
    IVA_DisposeData(ivaData);
Error:
	imaqDispose(roi_procura);
return valor_medio_quantity;
}
/******************************** VisionBarcodeReader ************************************************************************/
int VisionBarcodeReader(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain, float contrast, float gamma)
{
	int 
		success = 1,
		status;
	IVA_Data *ivaData;
    ROI 
    	*roi_procura = NULL;
    ImageType 
		imageType;
 	BCGOptions
		bcgOptions;
    float kernel[81] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,100,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1};
    int 
		barcodeTypes[2] = {8, 2}; //code 128 e code 39
    	   
	
    // Initializes internal data (buffers and array of points for caliper measurements)
    VisionErrChk(ivaData = IVA_InitData(9, 0));

	VisionErrChk(roi_procura = imaqCreateROI());
    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    VisionErrChk(imaqAddRectContour(roi_procura, imaqMakeRect(coordenadas.top, coordenadas.left, 
				coordenadas.height, coordenadas.width)));
	if(referenciar)
	{
		status = CapturaImagemVision(sessao_id_cam_sup, ILUM_SUP_ON_MAX, shutter, gain);//digitaliza a imagem
		if(status != IMAQdxErrorSuccess)
		{ 
			MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
			return -1;					
		}
		if(monitorar_vision)
		{
			MostraVision(imagem_camera, ON);
		}
		imaqDuplicate (imagem_trabalho, imagem_camera);
		VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	    // Sets the image border size
	    VisionErrChk(imaqSetBorderSize(imagem_trabalho, 4));
 
	    //-------------------------------------------------------------------//
	    //                        Filters: Convolution                       //
	    //-------------------------------------------------------------------//

	    // Applies a linear filter to an image by convolving the image with a filtering kernel.
	    VisionErrChk(imaqConvolve(imagem_trabalho, imagem_trabalho, kernel, 9, 9, 0, NULL));
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//
	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = contrast;
	    bcgOptions.gamma = gamma;

	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));
		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, ON);
		}
	}
	else
	{
		VisionErrChk(imaqGetImageType(imagem_trabalho, &imageType));
	}
	//////////// Faz a marcação da região de medição do Quantify /////////////////////////////////////////////////////
	imaqSetROIColor (roi_procura, &IMAQ_RGB_BLUE);
	imaqOverlayROI (imagem_trabalho, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
	if(mostrar_vision)
	{
		imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
		MostraVision(imagem_trabalho, OFF);
	}
	VisionErrChk(IVA_ReadBarcode(imagem_trabalho, 3, 75, 10, 0, barcodeTypes, 2, 1, 
		roi_procura, FALSE, 0, 1));
    IVA_DisposeData(ivaData);
Error:
	imaqDispose(roi_procura);
return success;
}
/*---------------------------------------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------------------------------------*/
float Vision_Measure_Distance_Edges(Image *image, COORDENADAS_LINE coordenadas, int usar_locator, int referenciar, char * nome_roi)
{
int
	success = 1,
	reparam = OFF,
	reparametrizado = OFF;
float 
	contraste=0,
	gamma=0,
	medida = 0;
		
    IVA_Data *ivaData;
    BCGOptions bcgOptions;
    ROI
		*roi = NULL,
		*roi_procura = NULL;
		
    IVA_Result *caliperResults;

    // Initializes internal data (buffers and array of points for caliper measurements)
    VisionErrChk(ivaData = IVA_InitData(5, 0));

	switch(usar_locator)
	{
		case LOCATOR_PAINEL:
			if(reparam_rois == REPARAM_DIFUSOR_ROIs || reparam_rois == REPARAM_SHIELD_ROIs)
				reparam = ON;
			break;
		case LOCATOR_DISPLAY:
			if(reparam_rois == REPARAM_DISPLAY_ROIs)
				reparam = ON;
			break;
	}
	if(reparam_rois == REPARAM_ALL || reparam_rois == REPARAM_ROIS)
			reparam = ON;
	
reparametrizou:
    // Creates a new, empty region of interest.
    VisionErrChk(roi_procura = imaqCreateROI());

    // Creates a new line ROI contour and adds the line to the provided ROI.
    VisionErrChk(imaqAddLineContour(roi_procura, imaqMakePoint(coordenadas.start.x, coordenadas.start.y),		  
                                                 imaqMakePoint(coordenadas.end.x, coordenadas.end.y)));  
	if(reparam && !reparametrizado)
		ReparametrizaLine(image, &coordenadas, nome_roi , dados_cfg_vision, id_arq_vision[modelo_atual_index], roi_procura, OFF);  
	switch(usar_locator)
	{
		case NENHUM_LOCATOR:  
			break;
		case LOCATOR_PAINEL:
			if (referenciar)
			{
				CapturaImagem_Referencia(sessao_id_cam_sup , ON, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, dados_cfg_vision.contraste_montagem, dados_cfg_vision.gamma_montagem);
				contraste = dados_cfg_vision.contraste_montagem;
				gamma	  = dados_cfg_vision.gamma_montagem;
			}
			imaqTransformROI2 (roi_procura, &transform_main.referenceSystem, &transform_main.measurementSystem);
			break;
		case LOCATOR_DISPLAY:
			if (referenciar)
			{
				CapturaImagem_Referencia(sessao_id_cam_sup , ON, dados_cfg_vision.camera_shutter_pins_display, dados_cfg_vision.camera_gain_pins_display, dados_cfg_vision.contraste_pins_display, dados_cfg_vision.gamma_pins_display);
				contraste = dados_cfg_vision.contraste_pins_display;
				gamma	  = dados_cfg_vision.gamma_pins_display;
			}
			imaqTransformROI2 (roi_procura, &transform_lcd.referenceSystem, &transform_lcd.measurementSystem);
			break;
		default:
			break;
	}
	if(referenciar != OFF)
	{
		//VisionErrChk(IVA_CLRExtractValue(imagem_trabalho));
	    //-------------------------------------------------------------------//
	    //                             Brightness                            //
	    //-------------------------------------------------------------------//

	    bcgOptions.brightness = 128;
	    bcgOptions.contrast = contraste;
	    bcgOptions.gamma = gamma;

	    // Applies brightness, contrast, and gamma correction to the image
	    VisionErrChk(imaqBCGTransform(imagem_trabalho, imagem_trabalho, &bcgOptions, NULL));

		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
		imaqDuplicate (image, imagem_trabalho);
	}
	else
	{
		imaqDuplicate (imagem_trabalho, image);
	}

	if(reparam && !reparametrizado)
	{
		ReparametrizaLine(image, &coordenadas, nome_roi , dados_cfg_vision, id_arq_vision[modelo_atual_index], roi_procura, ON);
		reparametrizado = ON;
		imaqDispose(roi_procura);
		Delay_thread(0.010);
		goto reparametrizou;
	}

	VisionErrChk(IVA_EdgeTool2(image, roi_procura, IMAQ_ALL, 
		IMAQ_SEARCH_FOR_ALL_EDGES, 7, 7, 50, IMAQ_BILINEAR_FIXED, 
		IMAQ_AVERAGE_COLUMNS, ivaData, 3));

	imaqOverlayROI (image, roi_procura , IMAQ_POINT_AS_CROSS, NULL, NULL);
	if(mostrar_vision)
	{
		MostraVision(image, OFF);
	}
    // Cleans up resources associated with the object
    imaqDispose(roi_procura);

	VisionErrChk(IVA_InitCaliperResults(image, ivaData, 4, 1, 2, &caliperResults));

    // Computes the distance between two points.
    VisionErrChk(IVA_GetDistance(image, ivaData, 3, 1, 3, 7, &caliperResults));
	
    // Releases the memory allocated in the IVA_Data structure.
    IVA_DisposeData(ivaData);
	if(pixel_por_mm>0)
		medida = medida_distancia/pixel_por_mm;
	else
		medida = -2.00;
		
	return medida;
Error:
	return -1.00;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   VISÃO DO TEMPLATE - Desligados - verifica se há pixels sufuciente 
/******************************** VisionTemplatesOFF ************************************************************************/
int VisionTemplatesOFF(Image *image,  COORDENADAS_ROI coordenadas, int imagem_PB)
{
int success = 1, res = 0;
ROI *roi;
	
   
    VisionErrChk(roi = imaqCreateROI());
    VisionErrChk(imaqAddRectContour(roi, 
    		imaqMakeRect(coordenadas.top, coordenadas.left, 
    					coordenadas.height,coordenadas.width)));
	VisionErrChk(IVA_Quantify(image, roi));
	if(mostrar_vision)
	{
		imaqOverlayROI (image, roi , IMAQ_POINT_AS_CROSS, NULL, NULL);
		MostraVision(image, OFF);
	}
	if(valor_medio_quantity>MEDIO_TEMPLATE_DESLIGADO || valor_max_quantity>MAX_TEMPLATE_DESLIGADO)
		res=1;
	else
		res=0;
    imaqDispose(roi);
Error:
	
	return res;
}

/******************************** Cria_Locator ************************************************************************/
int Cria_Locator(Image *image, COORDENADAS_ROI roi_localizador, char *arq_imagem, int reparametrizando, int algoritimo, int berco)
{

int confirm = 1,
	status = 0,
	i;

ROI *roi;
ContourInfo2 *contourInfo = NULL;
ContourID contourID;
COORDENADAS_ROI roi_selecionado;
Image  *template_destino, *image_copia;
RotationAngleRange angleRanges_patt_learn={0, 360};
PMLearnAdvancedSetupDataOption  *advancedOptions; 
TemplateReport temp_report;
unsigned int 
	numAdvOptions = 6;
int advOptionsItems[6]    = {102,  //IMAQ_PYLO_MatchOffsetXPosition
						     103,  //IMAQ_PYLO_MatchOffsetYPosition
						     104,  //IMAQ_PYLO_MatchOffsetAngle
						     0,	 //IMAQ_LDLO_SearchStrategy
						     105,  //IMAQ_PYLO_MaxPyramidLevelToStoreData
						     100}; //IMAQ_PYLO_SubpixelSampleSize
float advOptionsValues[6] = {0,
						     6,
							 0, 
							 20,
							 10,
							 20};
///////////////////////////////////////////////////////////

		
	if(!reparametrizando)
		MessagePopup ("Erro vision.", "Template de localização faltando ou criação solicitada...");
	if(!treinar_vision && !reparametrizando)
		return -1;
	advancedOptions = (PMLearnAdvancedSetupDataOption*)malloc(numAdvOptions * sizeof(PMLearnAdvancedSetupDataOption));
    for (i = 0 ; i < numAdvOptions ; i++)
    {
        advancedOptions[i].learnSetupOption = advOptionsItems[i];
        advancedOptions[i].value = advOptionsValues[i];
    }
		
    MostraVision(image, OFF);
	if(!reparametrizando)
		   confirm = ConfirmPopup ("Confirmar criação de novo template","Deseja criar um novo template?");
    if(confirm == 1)
    {
		roi = imaqGetWindowROI (berco);
		if (roi && imaqGetContourCount(roi)) 
		{
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
		}
		if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
			imaqSetCurrentTool (IMAQ_RECTANGLE_TOOL);
		else
			imaqSetCurrentTool (IMAQ_SELECTION_TOOL);
		//////////// Faz a marcação do região de procura /////////////////////////////////////////////
		imaqOverlayRect(image, imaqMakeRect(roi_localizador.top, roi_localizador.left, roi_localizador.height, roi_localizador.width),
						&IMAQ_RGB_YELLOW, IMAQ_DRAW_VALUE, NULL);
		MostraVision(image, OFF);
		//************* Espera criação do Template *********************************************************
		parar_mensagens_auto = ON;
		Delay_thread(0.1);
		Mensagem("Parada p/ criação de template...",arq_imagem, 
				"Especifique a região de interesse dentro da região de procura(amarelo)",
				"Após finalizar aperte continue.", MENS_AZUL);
volta:
		STATUS.Continua = OFF;
		modo_thread = ESCRITA_E_LEITURA;
		while(STATUS.Continua == OFF)
		{
			Delay_thread(0.1);
		}
		parar_mensagens_auto = OFF;
		//********** Verifica se houve um ROI selecionado **********************************
		roi = imaqGetWindowROI (berco);
		if (roi && imaqGetContourCount(roi)) 
		{
			contourID = imaqGetContour (roi, 0);
			contourInfo = imaqGetContourInfo2 (roi, contourID);
		}
		image_copia = imaqCreateImage (IMAQ_IMAGE_U8, 2);
		imaqDuplicate (image_copia, image);
		roi_selecionado.top=contourInfo->structure.rect->top;
		roi_selecionado.left=contourInfo->structure.rect->left;
		roi_selecionado.height=contourInfo->structure.rect->height;
		roi_selecionado.width=contourInfo->structure.rect->width;
		roi_selecionado.angle=0;					 
		if (!roi || !contourInfo || contourInfo->type != IMAQ_RECT) 
		{
			MessagePopup ("Criando novo template",
					  "Selecione uma Região de Interesse(ROI)");
			goto volta;
		}
		else if(roi_selecionado.top <= roi_localizador.top || roi_selecionado.left <= roi_localizador.left
			|| roi_selecionado.height>= roi_localizador.height || roi_selecionado.width>= roi_localizador.width)
		{
			MessagePopup ("Criando novo template",
			  "A Região de Interesse(ROI) deve estar dentro da região de procura.");
			goto volta;
		}
		else
		{ 
			Mensagem("Aguarde...","Criando novo template.", arq_imagem,"", MENS_AZUL);
			template_destino = imaqCreateImage (IMAQ_IMAGE_U8, 2);
			if(algoritimo == TIPO_PATTERN_2)
			{
				/* extract image to learn */
				imaqScale (template_destino, image, 1, 1, IMAQ_SCALE_LARGER, *(contourInfo->structure.rect));
				imaqLearnPattern2 (template_destino, IMAQ_LEARN_ALL, NULL);
	        }
			else
			{
				imaqSetImageSize(template_destino, roi_selecionado.width, roi_selecionado.height);
				imaqCopyRect (template_destino, image,  *(contourInfo->structure.rect),
							  imaqMakePoint(0,0));
				status = 0; 
				status = imaqLearnPattern4 (template_destino, NULL, IMAQ_MATCH_GRAYVALUE_PYRAMID, &angleRanges_patt_learn, NULL, 0, &temp_report);
				if(status < 0)
				{
					MessagePopup ("Falha ao tentar criar template", "Houve uma falha ao tentar criar um novo Template ou Localizador");
				}
			}
	        imaqWriteVisionFile (template_destino, arq_imagem, NULL); 	
	    	imaqDispose(template_destino);
        }
    	imaqDispose(roi);
    }
    else
    	return -1;	

imaqDispose(image_copia);
return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// Encontra o localizador MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Vision_Locator_Main(int *erro_locator, int referenciar)
{
long tamanho;
char 
	arquivo[256];
COORDENADAS_ROI roi_selecionado;
Image
	*locator_imagem,
	*image_copia  	  = NULL,
	*image_padrao;
double 
	res=1;
int 
	confirmar = 1,
	reparam_locator = OFF,
	status;
ROI *roi = NULL;
IVA_Data 
	*ivaData;

	
	    *erro_locator=OFF;
 		if(reparam_rois == REPARAM_ALL || reparam_rois == REPARAM_PAINEL_LOC)
			reparam_locator = ON;

		// Initializes internal data (buffers and array of points for caliper measurements)
	    ivaData = IVA_InitData(5, 0);
		if(referenciar)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////
			//  captura uma imagem
			////////////////////////////////////////////////////////////////////////////////////////////////
			status = CapturaImagem_Referencia(sessao_id_cam_sup , ON, dados_cfg_vision.camera_shutter_montagem, dados_cfg_vision.camera_gain_montagem, dados_cfg_vision.contraste_montagem, dados_cfg_vision.gamma_montagem);
			if(status < 0)
			{ 
				MessagePopup ("ERRO ", "Falha na comunicação com a camera...");
				return -1;					
			}
		}

		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
		roi_selecionado.top=dados_cfg_vision.coord_locator_main.top;
		roi_selecionado.left=dados_cfg_vision.coord_locator_main.left;
		roi_selecionado.height=dados_cfg_vision.coord_locator_main.height;
		roi_selecionado.width=dados_cfg_vision.coord_locator_main.width;
		roi_selecionado.angle=0;					 
		if(reparam_locator)
		{
			ReparametrizaROI(imagem_trabalho, &roi_selecionado, COORD_LOCATOR_PAINEL_STR, id_arq_vision[modelo_atual_index], roi, CRIAR_ROI);  
			dados_cfg_vision.coord_locator_main.top=roi_selecionado.top;
			dados_cfg_vision.coord_locator_main.left=roi_selecionado.left;
			dados_cfg_vision.coord_locator_main.height=roi_selecionado.height;
			dados_cfg_vision.coord_locator_main.width=roi_selecionado.width;
	    }
	    //-------------------------------------------------------------------//
	    //						LE O LOCALIZADOR P&B
	    //-------------------------------------------------------------------//
		strcpy(arquivo, dados_cfg_vision.arquivo_locator);
		locator_imagem = imaqCreateImage (IMAQ_IMAGE_U8, 2);
reparam1:
		if(GetFileInfo (arquivo, &tamanho)!=1)
		{
			image_copia = imaqCreateImage (IMAQ_IMAGE_U8, 2);
			imaqDuplicate (image_copia,imagem_trabalho);
			if(Cria_Locator(image_copia,roi_selecionado,arquivo, reparam_locator, TIPO_PATTERN_2, 0) !=0)
	      		*erro_locator=1;
		}	
		else if(reparam_locator)
		{
			status = ConfirmPopup ("Novo Locator para o painel", "Deseja substituir o locator antigo criando novo locator para painel?");
			if(status == 1)
			{
				DeleteFile (arquivo);
				goto reparam1;
			}
		}
		if(!*erro_locator)
			imaqReadFile(locator_imagem, arquivo, NULL, NULL);		  
		if(monitorar_vision) 
		{
			MostraVision(imagem_trabalho, OFF);
		}
		
		if(!*erro_locator)
		{
		    //						LE A IMAGEM PADRAO P&B
		    //-------------------------------------------------------------------//
			strcpy(arquivo, dados_cfg_vision.imagem_padrao);
		
			image_padrao = imaqCreateImage (IMAQ_IMAGE_U8, 2);
		    imaqCast(image_padrao, image_padrao, IMAQ_IMAGE_U8, NULL, 0);
reparam2:
			if(GetFileInfo (arquivo, &tamanho)!=1)
			{
				if(treinar_vision || reparam_locator)
				{
					if(!reparam_locator)
					{
						MessagePopup ("Erro vision.",
	          			"	Imagem padrão de localização do teste faltando...");
					}
	        		MostraVision(image_padrao, ON);
					if(!reparam_locator)
					{
	        			confirmar = ConfirmPopup ("Confirmar criação de imagem padrão do LCD",
	                 								"Deseja criar a imagem padrão?");
					}
					if(confirmar == 1)
					{
						imaqScale (imagem_trabalho, imagem_trabalho, 1, 1, IMAQ_SCALE_LARGER, IMAQ_NO_RECT);
						imaqWriteBMPFile (imagem_trabalho, arquivo, FALSE, NULL);
                    }
	                else
	          			*erro_locator=1;
				}
				else
				{
					MessagePopup ("Erro vision.",
	          			"Imagem padrão de localização do faltando...");
	          		*erro_locator=1;
				}
			}
			else if(reparam_locator)
			{
				status = ConfirmPopup ("Nova Imagem Padrão", "Deseja substituir a antiga Imagem Padrão criando uma nova para painel?");
				if(status == 1)
				{
					DeleteFile (arquivo);
					goto reparam2;
				}
			}
	    
		    imaqReadFile(image_padrao, arquivo, NULL, NULL);
		    //-------------------------------------------------------------------//
		    //						LOCALIZA E ARMAZENA COORDENADAS NO PADRAO
		    //-------------------------------------------------------------------//
			status = imaqFindTransformPattern (image_padrao,locator_imagem , &transform_main,
		                                   imaqMakeRotatedRect(dados_cfg_vision.coord_locator_main.top, dados_cfg_vision.coord_locator_main.left, 
	                                   dados_cfg_vision.coord_locator_main.height, dados_cfg_vision.coord_locator_main.width,0),
	                                   IMAQ_FIND_REFERENCE, &TPOption, &axis_report_painel);
			if(monitorar_vision)
			{
				imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);  
				MostraVision(image_padrao, OFF);
			}
	        axis_report_painel.origin.x = -1;
	        axis_report_painel.origin.y = -1;
		    //-------------------------------------------------------------------//
		    //						ATUALIZA COORDENADAS
		    //-------------------------------------------------------------------//
			status = imaqFindTransformPattern (imagem_trabalho,locator_imagem , &transform_main,
		                                   imaqMakeRotatedRect(dados_cfg_vision.coord_locator_main.top, dados_cfg_vision.coord_locator_main.left, 
	                                   dados_cfg_vision.coord_locator_main.height, dados_cfg_vision.coord_locator_main.width,0),
	                                   IMAQ_UPDATE_TRANSFORM, &TPOption, &axis_report_painel);

			if(axis_report_painel.origin.x <= 0 && axis_report_painel.origin.y <= 0)
	          *erro_locator=1;
			imaqDispose(image_padrao);
			imaqDispose(locator_imagem);
			if(monitorar_vision)
				MostraVision(imagem_trabalho, OFF);
		}
return res;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// Encontra o localizador do lcd - Camera Inferior
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Vision_Locator_Pins_Lcd(int *erro_locator, int referenciar)
{
long tamanho;
char 
	arquivo[256];
COORDENADAS_ROI roi_selecionado;
Image
	*locator_imagem,
	*image_copia  	  = NULL,
	*image_padrao;
double 
	res=1;
int 
	confirmar = 1,
	reparam_locator = OFF,
	status;

	    *erro_locator=OFF;
 
		if(referenciar)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////
			//  captura uma imagem
			////////////////////////////////////////////////////////////////////////////////////////////////
			status = CapturaImagem_Referencia(sessao_id_cam_inf , ILUM_INF_ON_MAX, dados_cfg_vision.camera_shutter_pins_display, dados_cfg_vision.camera_gain_pins_display, dados_cfg_vision.contraste_pins_display, dados_cfg_vision.gamma_pins_display);
			if(status < 0)
			{ 
				MessagePopup ("ERRO ", "Falha na comunicação com a camera. Teste pinos LCD - Camera Inferior...");
				return -1;					
			}
		}

		if(monitorar_vision)
		{
			MostraVision(imagem_trabalho, OFF);
		}
	    //-------------------------------------------------------------------//
	    //						LE O LOCALIZADOR P&B
	    //-------------------------------------------------------------------//
		strcpy(arquivo, dados_cfg_vision.arquivo_locator_inf);
		locator_imagem = imaqCreateImage (IMAQ_IMAGE_U8, 2);
		if(GetFileInfo (arquivo, &tamanho)!=1)
		{
			roi_selecionado.top=dados_cfg_vision.coord_locator_inf.top;
			roi_selecionado.left=dados_cfg_vision.coord_locator_inf.left;
			roi_selecionado.height=dados_cfg_vision.coord_locator_inf.height;
			roi_selecionado.width=dados_cfg_vision.coord_locator_inf.width;
			roi_selecionado.angle=0;					 
			image_copia = imaqCreateImage (IMAQ_IMAGE_U8, 2);
			imaqDuplicate (image_copia,imagem_trabalho);
			if(Cria_Locator(image_copia,roi_selecionado,arquivo, reparam_locator, TIPO_PATTERN_2, 0) !=0)
	      		*erro_locator=1;
		}	
		if(!*erro_locator)
			imaqReadFile(locator_imagem, arquivo, NULL, NULL);
		if(monitorar_vision) 
		{
			MostraVision(imagem_trabalho, OFF);
		}
		
		if(!*erro_locator)
		{
		    //						LE A IMAGEM PADRAO P&B
		    //-------------------------------------------------------------------//
			strcpy(arquivo, dados_cfg_vision.imagem_padrao_inf);
		
			image_padrao = imaqCreateImage (IMAQ_IMAGE_U8, 2);
		    imaqCast(image_padrao, image_padrao, IMAQ_IMAGE_U8, NULL, 0);
	    
			if(GetFileInfo (arquivo, &tamanho)!=1)
			{
				if(treinar_vision)
				{
					MessagePopup ("Erro vision.",
	          			"Imagem padrão de localização do teste de pinos LCD faltando...");
	        		MostraVision(image_padrao, ON);
	        		if(ConfirmPopup ("Confirmar criação de imagem padrão do LCD",
	                 "Deseja criar a imagem padrão?")==1)
	                 {
						imaqScale (imagem_trabalho, imagem_trabalho, 1, 1, IMAQ_SCALE_LARGER, IMAQ_NO_RECT);
						imaqWriteBMPFile (imagem_trabalho, arquivo, FALSE, NULL);
	                 }
	                else
	          			*erro_locator=1;
				}
				else
				{
					MessagePopup ("Erro vision.",
	          			"Imagem padrão de localização do LCD faltando...");
	          		*erro_locator=1;
				}
			}
	    
		    imaqReadFile(image_padrao, arquivo, NULL, NULL);
		    //-------------------------------------------------------------------//
		    //						LOCALIZA E ARMAZENA COORDENADAS NO PADRAO
		    //-------------------------------------------------------------------//
			status = imaqFindTransformPattern (image_padrao,locator_imagem , &transform_lcd,
		                                   imaqMakeRotatedRect(dados_cfg_vision.coord_locator_inf.top, dados_cfg_vision.coord_locator_inf.left, 
	                                   dados_cfg_vision.coord_locator_inf.height, dados_cfg_vision.coord_locator_inf.width,0),
	                                   IMAQ_FIND_REFERENCE, &TPOption, &axis_report_lcd);
			if(monitorar_vision)
			{
				imaqSetWindowPalette(0,IMAQ_PALETTE_GRAY, NULL, 0);
				MostraVision(image_padrao, OFF);
			}
	        axis_report_lcd.origin.x = -1;
	        axis_report_lcd.origin.y = -1;
		    //-------------------------------------------------------------------//
		    //						ATUALIZA COORDENADAS
		    //-------------------------------------------------------------------//
			status = imaqFindTransformPattern (imagem_trabalho,locator_imagem , &transform_lcd,
		                                   imaqMakeRotatedRect(dados_cfg_vision.coord_locator_inf.top, dados_cfg_vision.coord_locator_inf.left, 
	                                   dados_cfg_vision.coord_locator_inf.height, dados_cfg_vision.coord_locator_inf.width,0),
	                                   IMAQ_UPDATE_TRANSFORM, &TPOption, &axis_report_lcd);

			if(axis_report_lcd.origin.x <= 0 && axis_report_lcd.origin.y <= 0)
	          *erro_locator=1;
			imaqDispose(image_padrao);
			imaqDispose(locator_imagem);
			if(monitorar_vision)
				MostraVision(imagem_trabalho, OFF);
		}
return res;
}

/******************************** MostraVision ************************************************************************/
int MostraVision(Image *image, int som)
{
char path[200];

	imaqShowWindow (0, FALSE);
	Delay_thread(0.1);
	strcpy(path,CAMINHO_SONS);
	if(monitorar_vision)
	{
		strcat(path,"campainha_1.wav");	 
		STATUS.Continua = OFF;
		STATUS.Rejeita = OFF;
		modo_thread = ESCRITA_E_LEITURA;
		imaqShowScrollbars (0, TRUE);
		imaqZoomWindow2 (0, zoom_fator, zoom_fator, IMAQ_NO_POINT);
		imaqShowWindow (0, TRUE);
		imaqDisplayImage (image, 0, TRUE);
		imaqMoveWindow (0, imaqMakePoint(0 , 0) );
		CVI_PlaySound (path, ON );
		Delay_thread(0.9);
		CVI_StopCurrentSound();
		do
		{
			Delay_thread(0.01);
		}while(STATUS.Continua != ON && STATUS.Rejeita != ON);
	}
	else
	{
		if(som)
		{
			strcat(path,"pratos.wav");	
			CVI_PlaySound (path, ON );
			Delay_thread(1);
			CVI_StopCurrentSound();
		}
		imaqShowScrollbars (0, TRUE);
		imaqZoomWindow2 (0, zoom_fator, zoom_fator, IMAQ_NO_POINT);
		imaqDisplayImage (image, 0, TRUE);
		imaqMoveWindow (0, imaqMakePoint(0 , 0) );
	}
return 0;
}


//---------------------  int MostraVision2(Image *image) -----------------------------------------
int MostraVision2(Image *image, COORDENADAS_ROI coordenada, double contraste, double gamma)
{
ROI *roi;
Image  
	*imagem_brightness = NULL;
BCGOptions 
	redBCGOptions,
 	greenBCGOptions,
 	blueBCGOptions;
   
	imagem_brightness = imaqCreateImage (IMAQ_IMAGE_RGB, 7);
	redBCGOptions.brightness = 128;
    redBCGOptions.contrast = contraste;
    redBCGOptions.gamma = gamma;

    greenBCGOptions.brightness = 128;
    greenBCGOptions.contrast = contraste;
    greenBCGOptions.gamma = gamma;

    blueBCGOptions.brightness = 128;
    blueBCGOptions.contrast = contraste;
    blueBCGOptions.gamma = gamma;
    // Applies brightness, contrast, and gamma correction to the image
    imaqColorBCGTransform(imagem_brightness, image, &redBCGOptions, &greenBCGOptions, &blueBCGOptions, NULL);
	
	
   // Creates a new, empty region of interest.
    roi = imaqCreateROI();

    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    imaqAddRectContour(roi, imaqMakeRect(coordenada.top, coordenada.left, coordenada.height, coordenada.width));

    //-------------------------------------------------------------------//
    //                           Mask from ROI                           //
    //-------------------------------------------------------------------//

	IVA_MaskFromROI(imagem_brightness, roi, FALSE, TRUE);

    // Cleans up resources associated with the object
    imaqDispose(roi);
	imaqShowWindow (1, FALSE);
	Delay_thread(0.1);
	imaqZoomWindow2 (1, zoom_fator, zoom_fator, IMAQ_NO_POINT);
	imaqDisplayImage (imagem_brightness, 1, TRUE);
	imaqMoveWindow (1, imaqMakePoint(150 , 0) );
return 0;
}



//*****************************************MostrarVision****************************************************************
int CVICALLBACK MostrarVision (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &mostrar_vision);
			if(mostrar_vision)
			{
				SetCtrlAttribute (painel_manual, MANUAL_TREINAR_VISION, 	ATTR_DIMMED, 0);
				SetCtrlAttribute (painel_manual, MANUAL_MONITORAR_VISION, 	ATTR_DIMMED, 0);
				SetCtrlAttribute (painel_manual, MANUAL_REPARAMETRIZAR, 	ATTR_DIMMED, 0);
			}
			else
			{
				SetCtrlAttribute (painel_manual, MANUAL_TREINAR_VISION, 	ATTR_DIMMED, 1);
				SetCtrlAttribute (painel_manual, MANUAL_MONITORAR_VISION, 	ATTR_DIMMED, 1);
				SetCtrlAttribute (painel_manual, MANUAL_REPARAMETRIZAR, 	ATTR_DIMMED, 1);
			}
			SetCtrlAttribute (painel_manual, MANUAL_MOSTRAR_VISION, ATTR_CTRL_VAL, mostrar_vision);
			SetCtrlAttribute (painel_auto, 	AUTO_MOSTRAR_VISION, 	ATTR_CTRL_VAL, mostrar_vision);
			SetActiveCtrl (painel_auto, AUTO_TXT_SERIE);
		break;
	}
	return 0;
}
//********************************************FatorZoom*************************************************************
int CVICALLBACK FatorZoom (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &zoom_fator);
			SetCtrlVal (painel_auto, 	AUTO_NUMERIC_ZOOM, 		zoom_fator);
			SetCtrlVal (painel_manual, 	MANUAL_NUMERIC_ZOOM,   zoom_fator);
				break;
		}
	return 0;
}

//******************************************TreinarVision***************************************************************
int CVICALLBACK TreinarVision(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &treinar_vision);
			if(treinar_vision)
			{
				zoom_fator = ZOOM_FATOR_TREINAR_VISION;
				SetCtrlVal (painel_auto, 	AUTO_NUMERIC_ZOOM, 	 zoom_fator);
				SetCtrlVal (painel_manual, 	MANUAL_NUMERIC_ZOOM, zoom_fator);
			}
			else
			{
				zoom_fator = ZOOM_FATOR_PADRAO;
				SetCtrlVal (painel_auto, 	AUTO_NUMERIC_ZOOM, 	 zoom_fator);
				SetCtrlVal (painel_manual, 	MANUAL_NUMERIC_ZOOM, zoom_fator);
			}
				
			break;
	}
	return 0;
}
//*********************************************************************************************************
//									MonitorarVision														  *
//*********************************************************************************************************
int CVICALLBACK MonitorarVision (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &monitorar_vision);
			if(monitorar_vision)
				SetCtrlAttribute (painel_manual, MANUAL_GRAVAR, ATTR_DIMMED, 0);
			else
				SetCtrlAttribute (painel_manual, MANUAL_GRAVAR, ATTR_DIMMED, 1);
			break;
	}
	return 0;
}

//**************************************LiberarCameras*******************************************************************
int CVICALLBACK LiberarCameras (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int liberar_cameras;

	switch (event)
		{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &liberar_cameras);
			if(liberar_cameras)
			{
				IMAQdxCloseCamera (sessao_id_cam_sup);
			}
			else
			{
         		IMAQdxOpenCamera (STR_CAMERA, IMAQdxCameraControlModeController, &sessao_id_cam_sup);
			}	
			break;
		}
	return 0;
}
//********************************************GravarImagem***********************************************************
int CVICALLBACK GravarImagem(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int gravacao;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, control, &gravacao);
			if(gravacao==0)
				gravar_imagem=NAO_GRAVA;
			else if(gravacao==1)
				gravar_imagem=GRAVAR_IMAGEM_PARAMETRIZ;
			else if(gravacao==2)
				gravar_imagem=GRAVA_IMAG_COLOR;
			break;
	}
	return 0;
}
//**********************************************SeqVisionManual***************************************************************
int CVICALLBACK SeqVisionManual (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
int 
	valor,
	model_vision;
char 
	msg[50] = {0};
double 
	res = 0;
										  
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panel, MANUAL_NUMERIC_ZOOM , 	&zoom_fator);
			GetCtrlVal (panel, MANUAL_MODELO_VISION , 	&model_vision);
			
			strcpy(id_arq_vision[modelo_atual_index], DIR_CONFIG_VISION);
				
			switch(model_vision)
			{
				case 0:
					MessagePopup ("Modelo não selecionado", 
						"Para rodar uma sequencia de vision primeiro selecione um modelo");
					return -1;
					break;
				case 1:
					strcat(id_arq_vision[modelo_atual_index], ARQ_VISION_HI_PADRAO);
					break;
				case 2:
					strcat(id_arq_vision[modelo_atual_index], ARQ_VISION_LO_PADRAO);
					break;
			}
			GetCtrlVal (panel, MANUAL_MONITORAR_VISION , &monitorar_vision);
			GetCtrlVal (panel, MANUAL_RING_TESTE , &valor);
			strcpy(MensagemFalha1, "");
			res = -1;
			
			switch (valor)
			{
				case 1:
					//res = LightPipe_Knob_MPlate_Assy(MensagemFalha1, ON);
					if(strlen(MensagemFalha1) > 0 || res < 0)
					{
						MessagePopup ("Erro Vision - Knob ou LightPipe", MensagemFalha1);	
						break;
					}
					break;
				case 2:
					//res = Difusor_MPlate_Assy( MensagemFalha1, ON);
					if(strlen(MensagemFalha1) > 0 || res < 0)
					{
						MessagePopup ("Erro Vision - Difusor", MensagemFalha1);	
						break;
					}
					break;
				case 3:
					//res = LCD_Assy( MensagemFalha1, ON);
					if(strlen(MensagemFalha1) > 0 || res < 0)
					{
						MessagePopup ("Erro Vision - LCD", MensagemFalha1);	
						break;
					}
					break;
				case 4:
					res = Medidas_Travas_Conector_J2901(MensagemFalha1);
					if(strlen(MensagemFalha1) > 0 || res < 0)
					{
						MessagePopup ("Erro Vision - LCD", MensagemFalha1);	
						break;
					}
					break;
				default:
					res = -1;
					break;
			}
			if (res>=0)
				strcpy(msg, "Aprovado");
			if (res<0)
				strcpy(msg, "Rejeitado");
			MessagePopup ("Resultado", msg);
	}
	return 0;
}



