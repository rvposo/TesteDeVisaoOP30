#ifndef IMAGEPROCESSING_TASK_INCLUDE
#define IMAGEPROCESSING_TASK_INCLUDE
#include <nivision.h>
#include <nimachinevision.h>

#ifdef __cplusplus
	extern "C" {
#endif
		
#define IVA_MAX_BUFFERS 10
#define IVA_STORE_RESULT_NAMES

#define MAX_BUFFERS 10
#define INIT_POINTS_ARRAY_ELEMENTS 100

#define VisionErrChk(Function) {if (!(Function)) {success = 0; goto Error;}}
		
#define FERR_EDGE  1
#define FERR_QUANT 2
		
#define	FERRAM_VER_PINS_DISP FERR_QUANT 
		
#define	MIN_QUANTIFY_PIN_DISP  200//Pixels		
#define	MAX_QUANTIFY_PIN_DISP  255//Pixels		

 typedef enum IVA_ResultType_Enum {IVA_NUMERIC, IVA_BOOLEAN, IVA_STRING} IVA_ResultType;

typedef union IVA_ResultValue_Struct    // A result in Vision Assistant can be of type double, BOOL or string.
{
    double numVal;
    BOOL   boolVal;
    char*  strVal;
} IVA_ResultValue;

typedef struct IVA_Result_Struct
{
#if defined (IVA_STORE_RESULT_NAMES)
    char resultName[256];           // Result name
#endif
    IVA_ResultType  type;           // Result type
    IVA_ResultValue resultVal;      // Result value
} IVA_Result;

typedef struct IVA_StepResultsStruct
{
#if defined (IVA_STORE_RESULT_NAMES)
    char stepName[256];             // Step name
#endif
    int         numResults;         // number of results created by the step
    IVA_Result* results;            // array of results
} IVA_StepResults;


typedef struct IVA_Data_Struct
{
    Image* buffers[IVA_MAX_BUFFERS];            // Vision Assistant Image Buffers
    IVA_StepResults* stepResults;              	// Array of step results
    int numSteps;                               // Number of steps allocated in the stepResults array
    CoordinateSystem *baseCoordinateSystems;    // Base Coordinate Systems
    CoordinateSystem *MeasurementSystems;       // Measurement Coordinate Systems
    int numCoordSys;                            // Number of coordinate systems
} IVA_Data;
typedef struct IVA_Data_Struct2
{
    Image* buffers[MAX_BUFFERS];    	// Vision Assistant Image Buffers
    PointFloat* pointsResults;      	// Array of points used for Caliper Measurements
    int numPoints;                 	 	// Number of points allocated in the pointsResults array
    int pointIndex;                 	// Insertion Point
} IVA_Data2;

 
#define STR_CAMERA						"CAM0"
#define MIN_GAIN_CAMERA					 0
#define MIN_SHUTTER_CAMERA				 1000

#define ILUM_SUP_OFF					0
#define ILUM_SUP_ON_MAX					1
#define ILUM_SUP_ON_MED					2
#define ILUM_SUP_ON_MIN					3
#define ILUM_INF_ON_MAX					4
#define ILUM_INF_ON_MIN					5
#define ILUM_INF_OFF					6

#define NUM_MAX_EDGES					15
#define VALOR_QUANTITY_MINIMO			45
#define ERRO_ILL_MEDIA_APLIQUE			-777
#define MEDIO_TEMPLATE_DESLIGADO		5//em pixels
#define MAX_TEMPLATE_DESLIGADO			100//em pixels
#define MAX_NUM_TEMPL_PERMIT			15//número máximo de templates permitidos no HD

#define EXTRACT_VALUE					1//
#define EXTRACT_GREEN					2//

#define FERRAM_EDGE						1
#define FERRAM_CENTR_ID					2

#define CONTRASTE_PADRAO				55.000
#define LIMITE_COR_CHECK_MIN			0  
#define LIMITE_COR_CHECK_PADRAO			500 // 50% 
#define LIMITE_COR_CHECK_MAX			1000 
#define MIN_TOL_TEMPLATES				750
#define TOLERANCIA_TEMPLATES			800

#define ZOOM_FATOR_TREINAR_VISION		0.90
#define ZOOM_FATOR_PADRAO				0.30
#define CRIAR_ROI						10

#define TIPO_PATTERN_2					2
#define TIPO_PATTERN_4					4

//////////////////////////////////////////////////////////////////////////////////
#define DIR_CONFIG_VISION			"..\\Config\\VIS\\"
#define DIR_CONFIG_VISION_BKP		"..\\Config\\VIS\\BKP\\"
#define DIR_CONFIG_EIXOS			"..\\Config\\EIXOS\\"
#define	DIR_TEMPLATES_VISION		"..\\VisionNational\\Templates\\"
//////////////////////////////////////////////////////////////////////////////////

#define ARQ_VISION_HI_PADRAO			"VISION_OP30.txt"
#define ARQ_VISION_LO_PADRAO			"VISION_OP30.txt"
////////////////////////////////////
#define TEMPLATE_LIGHT_PIPE				"LightPipe_" 
#define TEMPLATE_KNOB					"Knob_" 
#define TEMPLATE_LGUIDE_TACO 			DIR_TEMPLATES_VISION"LightGuide_Taco_" 
#define TEMPLATE_LGUIDE_DISP			DIR_TEMPLATES_VISION"LightGuide_Disp_" 
#define TEMPLATE_LGUIDE_VELO 			DIR_TEMPLATES_VISION"LightGuide_Velo_"
#define TEMPLATE_LINER_INF				"Liner_Inf_" 

////////////////////////////////////
#define ARQUIVO_OCR_VELO				"VELO.ABC"
#define ARQUIVO_OCR_ODOM				"ODOMETRO.ABC"
#define ARQUIVO_OCR_CLOCK				"CLOCK.ABC"
/***************** Enumeracao dos modos de gravação de imagens ********************/
typedef enum 
{ 
	NAO_GRAVA,
	GRAVAR_IMAGEM_PARAMETRIZ,
	GRAVA_IMAG_COLOR,
} MODO_GRAVA_IMAGEM;

MODO_GRAVA_IMAGEM gravar_imagem;
//*******************************************************************
typedef struct COORDENADAS
{
	int top,
		left,
		height,
		width;
	double angle;
}COORDENADAS_ROI;
//********************************************************************
typedef struct
{
	Point start,
		  end;
	
}COORDENADAS_LINE;
//*******************************************************************
enum USAR_LOCATOR
{
	NENHUM_LOCATOR = 0,
	LOCATOR_PAINEL,
	LOCATOR_DISPLAY
}LOCATOR_A_USAR;

/************************ coordenadas da tabela config ********************************/
#define	COORD_LOCATOR_PAINEL_STR				"COORD_ROI_LOCATOR="
#define	COORD_ROI_DIFUSOR_PT_1_STR				"COORD_ROI_DIFUSOR_PT_1="
#define	COORD_ROI_DIFUSOR_PT_2_STR				"COORD_ROI_DIFUSOR_PT_2="
#define	COORD_ROI_DISP_LINER_PT_1_STR			"COORD_ROI_DISP_LINER_PT_1="
#define	COORD_ROI_DISP_LINER_PT_2_STR			"COORD_ROI_DISP_LINER_PT_2="
#define	COORD_ROI_DISP_LINER_PT_3_STR			"COORD_ROI_DISP_LINER_PT_3="
#define	COORD_ROI_DISP_LINER_PT_4_STR			"COORD_ROI_DISP_LINER_PT_4="
#define	COORD_ROI_LIGHT_GUIDE_DIR_STR			"COORD_ROI_LIGHT_GUIDE_DIR="
#define	COORD_ROI_LIGHT_GUIDE_ESQ_STR			"COORD_ROI_LIGHT_GUIDE_ESQ="
#define	COORD_ROI_LIGHT_GUIDE_DISP_STR			"COORD_ROI_LIGHT_GUIDE_DISP="
#define	COORD_ROI_DIFUSOR_STR					"COORD_ROI_DIFUSOR="
#define	COORD_ROI_DISPLAY_STR					"COORD_ROI_DISPLAY="
#define	COORD_ROI_SHIELD_STR					"COORD_ROI_SHIELD="
#define COORD_ROI_GUIA_MONT						"COORD_ROI_GUIA_MONTAGEM="
//*******************************************************************
enum REPARAM_ROIS
{
	NAO_REPARAMET = 0,
	REPARAM_ALL,
	REPARAM_ROIS,
	REPARAM_BARCODE_ROIs,
	REPARAM_DISPLAY_ROIs,
	REPARAM_DIFUSOR_ROIs,
	REPARAM_SHIELD_ROIs,
	REPARAM_LINER_ROIs,
	REPARAM_PAINEL_LOC,
	REPARAM_DISPLAY_LOC
}reparam_rois;
//********************************************************************
typedef struct CFG_VISION
{
	COORDENADAS_ROI 	
		coord_locator_main,
		coord_locator_inf,
		coord_locator_lcd;
	
	double 	
 		contraste_montagem,
		contraste_ponteiros,
		contraste_barcode_lcd,
		contraste_pins_display,
		gamma_montagem,
		gamma_barcode_lcd,
		gamma_ponteiros,
		gamma_pins_display;
	
	float	
		camera_gain_barcode_lcd,
		camera_gain_pins_display,
		camera_gain_montagem,
		camera_gain_ponteiros,
		
		camera_shutter_montagem,
		camera_shutter_ponteiros,
		camera_shutter_barcode_lcd,
		camera_shutter_pins_display;
	
	char 
		arquivo_locator[256],
		arquivo_locator_inf[256],
		imagem_padrao[256],
		imagem_padrao_inf[256];
	
	float	
		min_threshold,
		max_threshold;
	Point
		origem_linha,
		fim_linha;

}DADOS_CFG_VISION;

DADOS_CFG_VISION dados_cfg_vision;
//******************************************************************************************
COORDENADAS_ROI
	
	coord_rois_pins_lcd[10],
	coord_trava_aplique_esq,
	coord_trava_aplique_dir,
	coord_roi_aplique,
	coord_temp_light_esq,
	coord_temp_difusor_dir,
	coord_temp_light_dir,
	coord_temp_light_disp,
	coord_temp_liner_inf,
	coord_liner_lcd,
	coord_qtfy_shield,
	coord_qtfy_display,
	coord_qtfy_lcd,
	coord_qtfy_difusor,
	coord_qtfy_guia_mont,
	coord_liner_inf,
	coord_liner_sup,
	coord_mask_lightpipe,
	coord_mask_difusor,
	coord_roi_barcode_aplique,
	coord_roi_barcode_display,
	coord_roi_lcd_pins,
	coord_roi_difusor_pt_1,
	coord_roi_difusor_pt_2,
	coord_roi_disp_liner_pt_1,
	coord_roi_disp_liner_pt_2,
	coord_roi_disp_liner_pt_3,
	coord_roi_disp_liner_pt_4;
//******************************************************************************************
COORDENADAS_LINE
		coord_line_esq_j5,
		coord_line_dir_j5;
//******************************************************************************************
typedef struct COORD_PONTEIROS
{
	double 	
		ang_inic,
		ang_final;
	int centro_x,
		centro_y,
		raio_int,
		raio_ext;
	
}COORD_ROI_PONTEIROS;

COORD_ROI_PONTEIROS 
	coord_roi_pont_comb, 
	coord_roi_pont_taco,
	coord_roi_pont_velo;
//******************************************************************************************
Point  
	CoordCentrMass[200];    
Image 
	*imagem_trabalho,
 	*imagem_camera;

CoordinateTransform2 
	transform_circ,
	transform_main,
	transform_lcd,
	transform_display;
AxisReport   
	axis_report_painel,
	axis_report_lcd,
	axis_report_display;

unsigned int 
	sessao_id_cam_sup,
	sessao_id_cam_inf,
	sessao_id_anterior;
int
	quant_edges,
	numero_particulas,
	medida_distancia,
	edges_sup ,
	edges_meio,
	edges_inf ;
char
	barcode_lido[20],
	//barcode_aplique[20],
	//barcode_display[20],
	ocr_lido[10],
	cor[15],
	dir_templates[256],
	arquivo_template[256],
	//arq_vision[256],
	arq_vision_ant[256];
	
float  
	gain_anterior, 
	shutter_anterior;
	
int
	mostrar_vision,
	treinar_vision,
	monitorar_vision;

double 
	dif_res_ill_aplique,
	res_ill_aplique,
	valor_medio_quantity,
	valor_max_quantity,
	valor_min_quantity,
	vision_area_particula,
	template_encontrado,
	zoom_fator,
	coord_edge_y,
	coord_edge_x,
	variacao_padrao;

PointFloat 	
	pontos_edge[15],
	centro_id,
	reta_inic_float, 
	reta_fim_float;

int IVA_MaskFromROI(Image* image, ROI* roi, int invertMask,int extractRegion);
int IVA_Particle(Image* image, int connectivity, int pPixelMeasurements[], int numPixelMeasurements,
                                 int pCalibratedMeasurements[], int numCalibratedMeasurements,IVA_Data2* ivaData);
void IVA_SetPixelValue(PixelValue*  pixelValue,ImageType  imageType,float grayscaleConstant,
                              unsigned int redConstant,unsigned int greenConstant,unsigned int blueConstant);
int IVA_AddClosedContour(ROI* roi, int xCoordinates[], int yCoordinates[], int numPoints);
int IVA_NthOrder(Image* image, int filterSize);
int IVA_CLRExtractBlue(Image* image);

IVA_Data* IVA_InitData(int numSteps, int numCoordSys);
IVA_Data2* IVA_InitData2(void);
int IVA_DisposeData2(IVA_Data2* ivaData);
int IVA_EdgeTool2(Image* image,   ROI* roi,  int pProcessType,  int pPolarity,   int pKernelSize,  int pWidth,
                                  float pMinThreshold, int pInterpolationType, int pColumnProcessingMode, IVA_Data* ivaData,
                                  int stepIndex); 
int IVA_MatchGeometricPattern2(Image* image,
                                               IVA_Data* ivaData, char* templatePath, int extraction, int curveThreshold,
                                               int edgeFilterSize, int curveMinLength, int curveRowStepSize, int curveColumnStepSize, int curveMaxEndPointGap,  int matchMode,
                                               int subpixel,  float rangeMin[],  float rangeMax[], int matchesRequested,
                                               float score, int contrastReversal, int searchStrategy,ROI* roi, int stepIndex);
int OverlayGeometricPatternMatch2( Image* image, const GeometricPatternMatch3* match, const RGBValue* color );
int VisionGeometricTemplat_PB(Image *image, char *arquivo_templt, int tolerancia, COORDENADAS_ROI coordenadas, int treinar_template_rej, int usar_locator);
int IVA_ColorLocation(Image* image, char* templatePath,
                             int colorSensitivity,
                             int searchStrategy,  int matchesRequested, float score,
                             int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth,
                             IVA_Data2* ivaData);
int IVA_ParticleFilter(Image* image, int pParameter[], float plower[], float pUpper[],
                              int pCalibrated[], int pExclude[], int criteriaCount,
                              int rejectMatches,
                              int connectivity);
int IVA_ColorPM(Image* image, char* templatePath,int mode,
                       int subpixel, float angleRangeMin[], float angleRangeMax[], double colorScoreWeight,
                       int colorSensitivity, int searchStrategy, int matchesRequested, float score,
                       int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth,
                       IVA_Data2* ivaData);
PointFloat IVA_GetPoint(IVA_Data* ivaData, int stepIndex, int pointIndex);
int    IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex);
int    IVA_DisposeData(IVA_Data* ivaData);
int    IVA_PushBuffer(IVA_Data* ivaData, Image* image, int bufferNumber);
Image* IVA_GetBuffer(IVA_Data* ivaData, int bufferNumber);
int    IVA_AddPoint(IVA_Data2* ivaData, PointFloat point);
int    IVA_MatchPattern(Image* image, IVA_Data2* ivaData, char* templatePath, int matchMode, int subpixel,
                            float angleRangeMin[], float angleRangeMax[], int matchesRequested, float score,
                            int searchRectTop, int searchRectLeft, int searchRectHeight, int searchRectWidth);
int    IVA_MatchPattern_2(Image* image, IVA_Data* ivaData,
                                     char* templatePath, int algorithm, float angleRangeMin[], float angleRangeMax[], int advOptionsItems[], float advOptionsValues[], unsigned int numAdvOptions,int matchesRequested,
                                     float score, ROI* roi,int stepIndex);
int    IVA_FindCircularEdge(Image* image, int centerX, int centerY, int pInnerRadius, int pOuterRadius,double pStartAngle, double pEndAngle, int pThreshold, int pFilterWidth,
                                int pSteepness, double pSubsamplingRatio, int pDirection, IVA_Data2* ivaData);
int    IVA_Quantify(Image* image, ROI* roi);
int    IVA_CentroidROI(Image* image, ROI* roi, IVA_Data2* ivaData);
int    IVA_CLRExtractValue(Image* image);
int    IVA_CLRExtractLuminance(Image* image);
int    IVA_CLRExtractGreen(Image* image);
int    IVA_CLRExtractRed(Image* image);
int    IVA_CLRExtractIntensity(Image* image);
int    IVA_EdgeTool(Image* image, ROI* roi, int pProcess,unsigned int pThreshold, unsigned int pFilterWidth, unsigned int pSteepness, unsigned int pSubpixelType,unsigned int pSubpixelDivisions, IVA_Data2* ivaData);
int    imaqOverlayPatternMatch_2( Image* image, const PatternMatchReport* match, const RGBValue* color, void* reserved ) ;
int    IVA_SimpleEdge(Image* image, ROI* roi, int pType,  int pThreshold, int pHysteresys, int pProcess,int pSubpixel, IVA_Data* ivaData, int stepIndex);
int    IVA_ReadBarcode(Image* image,
                                    unsigned int minBarWidth,
                                    unsigned int minStrength,
                                    unsigned int minNumBars,
                                    int skipLocation,
                                    int barcodeTypes[],
                                    unsigned int numTypes,
                                    unsigned int numBarcodes,
                                    ROI* roi,
                                    int validate,
                                    int gradeBarcode,
                                    unsigned int scanWidth);
int IVA_GetDistance(Image* image,
                                   IVA_Data* ivaData,
                                   int stepIndex1,
                                   int resultIndex1,
                                   int stepIndex2,
                                   int resultIndex2,
                                   IVA_Result** caliperResults);

int IVA_InitCaliperResults(Image* image,
                                   IVA_Data* ivaData,
                                   int stepIndex,
                                   int numResults,
                                   int numCalibratedResults,
                                   IVA_Result** caliperResults);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Vision_Template_Forma_e_Cor(DADOS_CFG_VISION cfg_flex, int *erro_locator, int referenciar, char *arquivo_templt, int tolerancia, COORDENADAS_ROI coordenadas, int ilumicacao, int ver_cor_apenas, int treinar_template_rej);
int    Cria_Locator(Image *image, COORDENADAS_ROI roi_localizador, char *arq_imagem, int reparametrizando, int algoritimo, int berco);
int    VisionTemplatesOFF(Image *image,  COORDENADAS_ROI coordenadas, int imagem_PB);
int    VisionPatt_Template_PB(char *arquivo_templt, int tolerancia, COORDENADAS_ROI coordenadas, int treinar_template_rej, int referenciar, double shutter, double gain);
int    ConfigVision( DADOS_CFG_VISION *dados_cfg_vision, char * arq_vision);
int    MostraVision(Image *image, int som);
int    MostraVision2(Image *image, COORDENADAS_ROI coordenada, double contraste, double gamma);
double CapturaImagem_Referencia(int sessao_id , int iluminacao, float valor_shutter, float valor_gain, float contraste, float gamma);
double LightPipe_Knob_MPlate_Assy( char * mens_rejeito, int teste_manual);
int    InicializaCameras(void);
void   PararCameras(void) ;
int    VisionQuantify(COORDENADAS_ROI coordenadas, int usar_locator, int referenciar, float shutter, float gain, char * nome_roi);
int    Vision_2_Edge( COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain);
int    VisionBarcodeReader(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain, float contrast, float gamma);
int    ImagemFlexBuffer(int usar_locator);
int    IVA_LineProfile_Gray(Image* image, ROI* roi);
int    IVA_ReadText(Image* image, char* fileName, ROI* roi);
int ConfigVision( DADOS_CFG_VISION *dados_cfg_vision, char *arq_vision);
double Vision_Locator_Main(int *erro_locator, int referenciar);
int	ReparametrizaROI(Image *image,COORDENADAS_ROI *roi_selecionado, char * roi_nome, char *arq_vision, ROI *roi_param, int utiliz_roi);
void GravaCoordenadasROI(char * roi_nome, COORDENADAS_ROI *roi_selecionado, int usar_angulo, char * arq_vision);
void GravaCoordenadasROICircular(char * roi_nome, char * roi_nome_2, COORD_ROI_PONTEIROS *roi_selecionado, char * arq_vision);
void GravaCoordenadasLine(char * roi_nome, COORDENADAS_LINE *roi_selecionado, char * arq_vision);
int Vision_Line_Edge(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain);
double Vision_Locator_Pins_Lcd(int *erro_locator, int referenciar);
void VisionIluminacao(int iluminar_extern);
int Vision_Quant_PinsDisp(COORDENADAS_ROI coordenadas, int referenciar, float shutter, float gain);
float Vision_Measure_Distance_Edges(Image *image, COORDENADAS_LINE coordenadas, int usar_locator, int referenciar, char * nome_roi);

#ifdef __cplusplus
	}
#endif

#endif // ifndef IMAGEPROCESSING_TASK_INCLUDE
