#ifndef _CIMSOCKETS_H
#define _CIMSOCKETS_H

#if defined(INCLUDE_AFTER_WINDOWS_H) && !defined(_INC_WINDOWS)
#error  This header must be included before utility.h and formatio.h
#error  because it includes cviauto.h which includes Windows SDK headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */

#include <cviauto.h>

#ifdef __cplusplus
    extern "C" {
#endif
extern const IID CIMSockets_IID__SMF;

extern const IID CIMSockets_IID__Comm;
extern const IID CIMSockets_IID__TRANSPONDER;

HRESULT CVIFUNC CIMSockets_New_SMF (const char *server,
                                    int supportMultithreading, LCID locale,
                                    int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets_Open_SMF (const char *fileName, const char *server,
                                     int supportMultithreading, LCID locale,
                                     int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets_Active_SMF (const char *server,
                                       int supportMultithreading, LCID locale,
                                       int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets__SMFGetmy_sockID (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *my_sockID);

HRESULT CVIFUNC CIMSockets__SMFSetmy_sockID (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short my_sockID);

HRESULT CVIFUNC CIMSockets__SMFGetmy_BaseTimestamp (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long *my_BaseTimestamp);

HRESULT CVIFUNC CIMSockets__SMFSetmy_BaseTimestamp (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long my_BaseTimestamp);

HRESULT CVIFUNC CIMSockets__SMFGetmy_BaseData (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               char **my_BaseData);

HRESULT CVIFUNC CIMSockets__SMFSetmy_BaseData (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *my_BaseData);

HRESULT CVIFUNC CIMSockets__SMFGetmy_Station_ID (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **my_Station_ID);

HRESULT CVIFUNC CIMSockets__SMFSetmy_Station_ID (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *my_Station_ID);

HRESULT CVIFUNC CIMSockets__SMFGetmy_Timeout (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              short *my_Timeout);

HRESULT CVIFUNC CIMSockets__SMFSetmy_Timeout (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              short my_Timeout);

HRESULT CVIFUNC CIMSockets__SMFreadini (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        const char *arquivo, const char *secao,
                                        const char *linha, char **arg4);

HRESULT CVIFUNC CIMSockets__SMFwriteini (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *arquivo, const char *secao,
                                         const char *linha,
                                         const char *parametro, short *arg5);

HRESULT CVIFUNC CIMSockets__SMFopen_sock (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, const char *host,
                                          const char *service, short timeout,
                                          short *arg4);

HRESULT CVIFUNC CIMSockets__SMFsync_sock (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, short sockID,
                                          const char *station, short timeout,
                                          long *arg4);

HRESULT CVIFUNC CIMSockets__SMFclose_sock (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, short sockID,
                                           short timeout, short *arg3);

HRESULT CVIFUNC CIMSockets__SMFerrormsg (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short result,
                                         char **arg2);

HRESULT CVIFUNC CIMSockets__SMFmoveout (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, short sockID,
                                        short timeout, const char *station,
                                        const char *barcode, const char *status,
                                        short *arg6);

HRESULT CVIFUNC CIMSockets__SMFmovein (CAObjHandle objectHandle,
                                       ERRORINFO *errorInfo, short sockID,
                                       short timeout, const char *station,
                                       const char *barcode,
                                       char **mensagem_retorno, short *arg6);

HRESULT CVIFUNC CIMSockets__SMFpackout (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, short sockID,
                                        short timeout, const char *station,
                                        const char *boxBarcode,
                                        SAFEARRAY **barcode, short quantidade,
                                        short *arg7);

HRESULT CVIFUNC CIMSockets__SMFenvtest (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, short sockID,
                                        short timeout, const char *station,
                                        const char *unitId, const char *groupId,
                                        const char *groupVersion,
                                        const char *testId,
                                        const char *testResult, double testValue,
                                        short flag, short *arg11);

HRESULT CVIFUNC CIMSockets__SMFSMFMessage (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, char **arg1);

HRESULT CVIFUNC CIMSockets__SMFBuild (CAObjHandle objectHandle,
                                      ERRORINFO *errorInfo, short sockID,
                                      short timeout, const char *station,
                                      const char *barcodePai,
                                      const char *barcodeFilho, short *arg6);

HRESULT CVIFUNC CIMSockets__SMFoperator (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short sockID,
                                         short timeout, const char *station,
                                         const char *operator_id,
                                         const char *onoff,
                                         char **mensagem_retorno, short *arg7);

HRESULT CVIFUNC CIMSockets__SMFenvtestgroup (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo, short sockID,
                                             short timeout, const char *station,
                                             const char *groupId,
                                             const char *groupVersion,
                                             short quantidade,
                                             SAFEARRAY **testId,
                                             SAFEARRAY **testDesc,
                                             SAFEARRAY **lowLimit,
                                             SAFEARRAY **highLimit,
                                             SAFEARRAY **unMed, short *arg12);

HRESULT CVIFUNC CIMSockets_New_Comm (const char *server,
                                     int supportMultithreading, LCID locale,
                                     int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets_Open_Comm (const char *fileName, const char *server,
                                      int supportMultithreading, LCID locale,
                                      int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets_Active_Comm (const char *server,
                                        int supportMultithreading, LCID locale,
                                        int reserved, CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets__CommOpenCOM (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short comPort,
                                         const char *comSettings, short *arg3);

HRESULT CVIFUNC CIMSockets__CommCloseCOM (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, short *arg1);

HRESULT CVIFUNC CIMSockets__CommReadCOM (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, char **buffer,
                                         short *arg2);

HRESULT CVIFUNC CIMSockets__CommWriteCOM (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *buffer, short *arg2);

HRESULT CVIFUNC CIMSockets_New_TRANSPONDER (const char *server,
                                            int supportMultithreading,
                                            LCID locale, int reserved,
                                            CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets_Open_TRANSPONDER (const char *fileName,
                                             const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets_Active_TRANSPONDER (const char *server,
                                               int supportMultithreading,
                                               LCID locale, int reserved,
                                               CAObjHandle *objectHandle);

HRESULT CVIFUNC CIMSockets__TRANSPONDERLeitura (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                short comPort, char **buffer,
                                                short timeout, short *arg4);

HRESULT CVIFUNC CIMSockets__TRANSPONDERInicializa (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *var_transponder,
                                                   const char *modelo,
                                                   char **numserie, short *arg4);

HRESULT CVIFUNC CIMSockets__TRANSPONDERRecupera (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *var_transponder,
                                                 char **numserie, short *arg3);

HRESULT CVIFUNC CIMSockets__TRANSPONDERLimpa (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *var_transponder,
                                              short *arg2);

HRESULT CVIFUNC CIMSockets__TRANSPONDERRegride (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *modelo, short *arg2);

HRESULT CVIFUNC CIMSockets__TRANSPONDERAssocia (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *var_transponder,
                                                const char *numserie,
                                                char **nserie, short *arg4);
#ifdef __cplusplus
    }
#endif
#endif /* _CIMSOCKETS_H */
