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

#define  AUTO                             1
#define  AUTO_TXT_TESTE                   2       /* control type: textBox, callback function: (none) */
#define  AUTO_TXT_MAQUINA                 3       /* control type: textBox, callback function: LeNSerie */
#define  AUTO_TXT_SERIE                   4       /* control type: textBox, callback function: LeNSerie */
#define  AUTO_TXT_MENSAGEM                5       /* control type: textBox, callback function: (none) */
#define  AUTO_PICTURE                     6       /* control type: picture, callback function: CallBack_TEXT */
#define  AUTO_TXT_NOME                    7       /* control type: textBox, callback function: (none) */
#define  AUTO_TXT_UNIDADE                 8       /* control type: textBox, callback function: (none) */
#define  AUTO_TXT_STATUS                  9       /* control type: textBox, callback function: (none) */
#define  AUTO_TXT_TEMPO                   10      /* control type: textBox, callback function: (none) */
#define  AUTO_LIM_INF                     11      /* control type: textBox, callback function: (none) */
#define  AUTO_NUMERIC_ZOOM                12      /* control type: numeric, callback function: FatorZoom */
#define  AUTO_LIM_SUP                     13      /* control type: textBox, callback function: (none) */
#define  AUTO_TXT_RESULTADO               14      /* control type: textBox, callback function: (none) */
#define  AUTO_TEXTMSG_28                  15      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_29                  16      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_30                  17      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_31                  18      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_32                  19      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_33                  20      /* control type: textMsg, callback function: (none) */
#define  AUTO_BOTAOZERA                   21      /* control type: command, callback function: ZeraProducao */
#define  AUTO_TEXTMSG_4                   22      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_10                  23      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_11                  24      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_12                  25      /* control type: textMsg, callback function: (none) */
#define  AUTO_DISPLAY                     26      /* control type: textMsg, callback function: (none) */
#define  AUTO_PICTURE_2                   27      /* control type: picture, callback function: CallBack_TEXT */
#define  AUTO_SERIAL_ENDITEM              28      /* control type: textMsg, callback function: (none) */
#define  AUTO_DECORATION_6                29      /* control type: deco, callback function: (none) */
#define  AUTO_DECORATION_5                30      /* control type: deco, callback function: (none) */
#define  AUTO_TEXTMSG                     31      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEMPO_TOTAL                 32      /* control type: numeric, callback function: (none) */
#define  AUTO_TEXTMSG_34                  33      /* control type: textMsg, callback function: (none) */
#define  AUTO_PN_VISTEON                  34      /* control type: textMsg, callback function: (none) */
#define  AUTO_DESCRICAO_MODELO            35      /* control type: textMsg, callback function: (none) */
#define  AUTO_TABELA                      36      /* control type: textMsg, callback function: (none) */
#define  AUTO_TXT_TESTADOS                37      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_35                  38      /* control type: textMsg, callback function: (none) */
#define  AUTO_AMOSTRA                     39      /* control type: textMsg, callback function: (none) */
#define  AUTO_TXT_REJEITADOS              40      /* control type: textMsg, callback function: (none) */
#define  AUTO_TXT_FTT                     41      /* control type: textMsg, callback function: (none) */
#define  AUTO_SERIAL_APLIQUE              42      /* control type: textMsg, callback function: (none) */
#define  AUTO_TXT_APROVADOS               43      /* control type: textMsg, callback function: (none) */
#define  AUTO_MSG_NOME                    44      /* control type: textMsg, callback function: (none) */
#define  AUTO_MODELOS                     45      /* control type: ring, callback function: ModelosCallback */
#define  AUTO_LED_CIM                     46      /* control type: LED, callback function: (none) */
#define  AUTO_MENS_FALHA                  47      /* control type: string, callback function: (none) */
#define  AUTO_PRESENCA                    48      /* control type: LED, callback function: (none) */
#define  AUTO_PIC_INSTRUCAO               49      /* control type: picture, callback function: CallBack_TEXT */
#define  AUTO_COMMANDBUTTON               50      /* control type: command, callback function: RejeitaMontagem */
#define  AUTO_MOSTRAR_VISION              51      /* control type: radioButton, callback function: MostrarVision */
#define  AUTO_TEXTMSG_37                  52      /* control type: textMsg, callback function: (none) */
#define  AUTO_TEXTMSG_38                  53      /* control type: textMsg, callback function: (none) */
#define  AUTO_BIMANUAL_REMOTO             54      /* control type: command, callback function: BimanualRemoto */
#define  AUTO_TXT_VERSAO                  55      /* control type: textMsg, callback function: (none) */
#define  AUTO_LED_NEGSAMPLE               56      /* control type: LED, callback function: (none) */
#define  AUTO_CONT_REMOTO                 57      /* control type: command, callback function: ContRemoto */
#define  AUTO_REJ_REMOTO                  58      /* control type: command, callback function: RejRemoto */
#define  AUTO_LED_MSAMPLE                 59      /* control type: LED, callback function: (none) */
#define  AUTO_VERSAO_TABS                 60      /* control type: textMsg, callback function: (none) */

#define  LOGO_BAR                         2
#define  LOGO_BAR_PICTURE                 2       /* control type: picture, callback function: CallBack_TEXT */
#define  LOGO_BAR_VERSAO_2                3       /* control type: textMsg, callback function: (none) */
#define  LOGO_BAR_VERSAO                  4       /* control type: textMsg, callback function: (none) */
#define  LOGO_BAR_TEXTMSG_5               5       /* control type: textMsg, callback function: (none) */
#define  LOGO_BAR_TEXTMSG_3               6       /* control type: textMsg, callback function: (none) */
#define  LOGO_BAR_COMMANDBUTTON           7       /* control type: command, callback function: ESC_BAR */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU1                    2
#define  MENUBAR_MENU1_MANUAL             3       /* callback function: Manual_Bar */
#define  MENUBAR_MENU1_SEPARATOR          4
#define  MENUBAR_MENU1_SAIR_PROG          5       /* callback function: Quit_Bar */
#define  MENUBAR_MENU2                    6
#define  MENUBAR_MENU2_ITEM1_2            7       /* callback function: ColetaDados_Bar */
#define  MENUBAR_MENU2_ITEM1              8       /* callback function: Version_Bar */


     /* Callback Prototypes: */

int  CVICALLBACK BimanualRemoto(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CallBack_TEXT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ColetaDados_Bar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ContRemoto(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ESC_BAR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FatorZoom(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LeNSerie(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Manual_Bar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ModelosCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MostrarVision(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Quit_Bar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK RejeitaMontagem(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RejRemoto(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Version_Bar(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ZeraProducao(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
