/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MANUAL                           1
#define  MANUAL_BUT_FIM                   2       /* control type: command, callback function: Manual_Fim */
#define  MANUAL_CONTINUA                  3       /* control type: LED, callback function: (none) */
#define  MANUAL_REJEITA                   4       /* control type: LED, callback function: (none) */
#define  MANUAL_BUT_RESETAREMERGENCIA     5       /* control type: command, callback function: Manual_ResetarEmergencia */
#define  MANUAL_TRAVA_AVANCADO_1          6       /* control type: LED, callback function: (none) */
#define  MANUAL_TRAVA_AVANCADO_2          7       /* control type: LED, callback function: (none) */
#define  MANUAL_NUMERIC_ZOOM              8       /* control type: numeric, callback function: FatorZoom */
#define  MANUAL_SEQ_VISION_MANUAL         9       /* control type: command, callback function: SeqVisionManual */
#define  MANUAL_TXT_SEGURANCA_BOTOES      10      /* control type: textMsg, callback function: (none) */
#define  MANUAL_PIC_LOGO                  11      /* control type: picture, callback function: (none) */
#define  MANUAL_RESETAEMERGENCIA          12      /* control type: radioButton, callback function: Manual_ResetaEmergencia */
#define  MANUAL_TXT_ATUADORESSENSORES     13      /* control type: textMsg, callback function: (none) */
#define  MANUAL_TXT_RECURSOS              14      /* control type: textMsg, callback function: (none) */
#define  MANUAL_RIS_THREAD_SISTEMA        15      /* control type: slide, callback function: Manual_ThreadSistema */
#define  MANUAL_HABITAEMERGENCIA          16      /* control type: radioButton, callback function: Manual_HabitaEmergencia */
#define  MANUAL_DEC_TRAVACLAMP_2          17      /* control type: deco, callback function: (none) */
#define  MANUAL_TXT_GERAL                 18      /* control type: textMsg, callback function: (none) */
#define  MANUAL_DEC_THREAD_SISTEMA        19      /* control type: deco, callback function: (none) */
#define  MANUAL_DEC_SECURANCA_BOTOES      20      /* control type: deco, callback function: (none) */
#define  MANUAL_DEC_TRAVACLAMP_1          21      /* control type: deco, callback function: (none) */
#define  MANUAL_DEC_RECURSOS              22      /* control type: deco, callback function: (none) */
#define  MANUAL_EMERGENCIA                23      /* control type: LED, callback function: (none) */
#define  MANUAL_LIBERA_CAMERAS            24      /* control type: radioButton, callback function: LiberarCameras */
#define  MANUAL_TREINAR_VISION            25      /* control type: radioButton, callback function: TreinarVision */
#define  MANUAL_MONITORAR_VISION          26      /* control type: radioButton, callback function: MonitorarVision */
#define  MANUAL_RING_TESTE                27      /* control type: ring, callback function: SeqVisionManual */
#define  MANUAL_MODELO_VISION             28      /* control type: slide, callback function: (none) */
#define  MANUAL_GRAVAR                    29      /* control type: slide, callback function: GravarImagem */
#define  MANUAL_MOSTRAR_VISION            30      /* control type: radioButton, callback function: MostrarVision */
#define  MANUAL_DESCARTE_LINER            31      /* control type: LED, callback function: (none) */
#define  MANUAL_CRIAR_NSAMPLE             32      /* control type: binary, callback function: CriarNegatSample */
#define  MANUAL_COMMANDBUTTON_17          33      /* control type: command, callback function: RedefinirPeriodicidade */
#define  MANUAL_COMMANDBUTTON_27          34      /* control type: command, callback function: CopiarTabelasRede */
#define  MANUAL_LIGA_IONIZADOR            35      /* control type: radioButton, callback function: Manual_Liga_Ionizador */
#define  MANUAL_BUZZER                    36      /* control type: radioButton, callback function: Manual_buzzer */
#define  MANUAL_SINAL_VERMELHO            37      /* control type: radioButton, callback function: Manual_sinalizador_vermelho */
#define  MANUAL_SINAL_VERDE               38      /* control type: radioButton, callback function: Manual_sinalizador_verde */
#define  MANUAL_ILUMINACAO_VISION_SUP     39      /* control type: radioButton, callback function: Manual_Iluminacao_Vision_Superior */
#define  MANUAL_TRAVACLAMP                40      /* control type: radioButton, callback function: Manual_TravaClamp */
#define  MANUAL_SOL_DESCARTE_LINER        41      /* control type: radioButton, callback function: Manual_Sol_Descarte_Liner */
#define  MANUAL_TEXTMSG_44                42      /* control type: textMsg, callback function: (none) */
#define  MANUAL_DECORATION_148            43      /* control type: deco, callback function: (none) */
#define  MANUAL_DECORATION_149            44      /* control type: deco, callback function: (none) */
#define  MANUAL_TEXTMSG_42                45      /* control type: textMsg, callback function: (none) */
#define  MANUAL_TEXTMSG_47                46      /* control type: textMsg, callback function: (none) */
#define  MANUAL_DECORATION_170            47      /* control type: deco, callback function: (none) */
#define  MANUAL_REPARAMETRIZAR            48      /* control type: ring, callback function: Reparam_ROIs */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CopiarTabelasRede(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CriarNegatSample(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FatorZoom(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GravarImagem(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LiberarCameras(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_buzzer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_Fim(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_HabitaEmergencia(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_Iluminacao_Vision_Superior(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_Liga_Ionizador(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_ResetaEmergencia(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_ResetarEmergencia(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_sinalizador_verde(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_sinalizador_vermelho(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_Sol_Descarte_Liner(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_ThreadSistema(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Manual_TravaClamp(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MonitorarVision(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MostrarVision(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RedefinirPeriodicidade(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Reparam_ROIs(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SeqVisionManual(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TreinarVision(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
