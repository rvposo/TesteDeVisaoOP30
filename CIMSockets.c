#include "CIMSockets.h"

typedef interface tagCIMSockets__SMF_Interface CIMSockets__SMF_Interface;

typedef struct tagCIMSockets__SMF_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( CIMSockets__SMF_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( CIMSockets__SMF_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( CIMSockets__SMF_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( CIMSockets__SMF_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( CIMSockets__SMF_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( CIMSockets__SMF_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( CIMSockets__SMF_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Getmy_sockID_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                       short *my_sockID);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Setmy_sockID_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                       short my_sockID);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Getmy_BaseTimestamp_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                              long *my_BaseTimestamp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Setmy_BaseTimestamp_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                              long my_BaseTimestamp);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Getmy_BaseData_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                         BSTR *my_BaseData);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Setmy_BaseData_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                         BSTR my_BaseData);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Getmy_Station_ID_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                           BSTR *my_Station_ID);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Setmy_Station_ID_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                           BSTR my_Station_ID);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Getmy_Timeout_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                        short *my_Timeout);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Setmy_Timeout_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                        short my_Timeout);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *readini_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                  BSTR arquivo, 
	                                                  BSTR secao, 
	                                                  BSTR linha, 
	                                                  BSTR *arg4);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *writeini_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                   BSTR arquivo, 
	                                                   BSTR secao, 
	                                                   BSTR linha, 
	                                                   BSTR parametro, 
	                                                   short *arg5);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *open_sock_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                    BSTR host, 
	                                                    BSTR service, 
	                                                    short timeout, 
	                                                    short *arg4);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *sync_sock_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                    short sockID, 
	                                                    BSTR station, 
	                                                    short timeout, 
	                                                    long *arg4);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *close_sock_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                     short sockID, 
	                                                     short timeout, 
	                                                     short *arg3);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *errormsg_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                   short result, 
	                                                   BSTR *arg2);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *moveout_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                  short sockID, 
	                                                  short timeout, 
	                                                  BSTR station, 
	                                                  BSTR barcode, 
	                                                  BSTR status, 
	                                                  short *arg6);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *movein_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                 short sockID, 
	                                                 short timeout, 
	                                                 BSTR station, 
	                                                 BSTR barcode, 
	                                                 BSTR *mensagem_retorno, 
	                                                 short *arg6);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *packout_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                  short sockID, 
	                                                  short timeout, 
	                                                  BSTR station, 
	                                                  BSTR boxBarcode, 
	                                                  SAFEARRAY **barcode, 
	                                                  short quantidade, 
	                                                  short *arg7);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *envtest_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                  short sockID, 
	                                                  short timeout, 
	                                                  BSTR station, 
	                                                  BSTR unitId, 
	                                                  BSTR groupId, 
	                                                  BSTR groupVersion, 
	                                                  BSTR testId, 
	                                                  BSTR testResult, 
	                                                  double testValue, 
	                                                  short flag, 
	                                                  short *arg11);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SMFMessage_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                     BSTR *arg1);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Build_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                short sockID, 
	                                                short timeout, 
	                                                BSTR station, 
	                                                BSTR barcodePai, 
	                                                BSTR barcodeFilho, 
	                                                short *arg6);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *operator_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                   short sockID, 
	                                                   short timeout, 
	                                                   BSTR station, 
	                                                   BSTR operator_id, 
	                                                   BSTR onoff, 
	                                                   BSTR *mensagem_retorno, 
	                                                   short *arg7);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *envtestgroup_) (CIMSockets__SMF_Interface __RPC_FAR *This, 
	                                                       short sockID, 
	                                                       short timeout, 
	                                                       BSTR station, 
	                                                       BSTR groupId, 
	                                                       BSTR groupVersion, 
	                                                       short quantidade, 
	                                                       SAFEARRAY **testId, 
	                                                       SAFEARRAY **testDesc, 
	                                                       SAFEARRAY **lowLimit, 
	                                                       SAFEARRAY **highLimit, 
	                                                       SAFEARRAY **unMed, 
	                                                       short *arg12);

} CIMSockets__SMF_VTable;

typedef interface tagCIMSockets__SMF_Interface
{
	CONST_VTBL CIMSockets__SMF_VTable __RPC_FAR *lpVtbl;
} CIMSockets__SMF_Interface;

typedef interface tagCIMSockets__Comm_Interface CIMSockets__Comm_Interface;

typedef struct tagCIMSockets__Comm_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( CIMSockets__Comm_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( CIMSockets__Comm_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( CIMSockets__Comm_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( CIMSockets__Comm_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( CIMSockets__Comm_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( CIMSockets__Comm_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( CIMSockets__Comm_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OpenCOM_) (CIMSockets__Comm_Interface __RPC_FAR *This, 
	                                                  short comPort, 
	                                                  BSTR comSettings, 
	                                                  short *arg3);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseCOM_) (CIMSockets__Comm_Interface __RPC_FAR *This, 
	                                                   short *arg1);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReadCOM_) (CIMSockets__Comm_Interface __RPC_FAR *This, 
	                                                  BSTR *buffer, 
	                                                  short *arg2);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *WriteCOM_) (CIMSockets__Comm_Interface __RPC_FAR *This, 
	                                                   BSTR buffer, 
	                                                   short *arg2);

} CIMSockets__Comm_VTable;

typedef interface tagCIMSockets__Comm_Interface
{
	CONST_VTBL CIMSockets__Comm_VTable __RPC_FAR *lpVtbl;
} CIMSockets__Comm_Interface;

typedef interface tagCIMSockets__TRANSPONDER_Interface CIMSockets__TRANSPONDER_Interface;

typedef struct tagCIMSockets__TRANSPONDER_VTable
{
	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This, 
	                                                         REFIID riid, 
	                                                         void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This);

	ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This, 
	                                                           UINT __RPC_FAR *pctinfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This, 
	                                                      UINT iTInfo, 
	                                                      LCID lcid, 
	                                                      ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This, 
	                                                        REFIID riid, 
	                                                        LPOLESTR __RPC_FAR *rgszNames, 
	                                                        UINT cNames, 
	                                                        LCID lcid, 
	                                                        DISPID __RPC_FAR *rgDispId);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( CIMSockets__TRANSPONDER_Interface __RPC_FAR * This, 
	                                                 DISPID dispIdMember, 
	                                                 REFIID riid, 
	                                                 LCID lcid, 
	                                                 WORD wFlags, 
	                                                 DISPPARAMS __RPC_FAR *pDispParams, 
	                                                 VARIANT __RPC_FAR *pVarResult, 
	                                                 EXCEPINFO __RPC_FAR *pExcepInfo, 
	                                                 UINT __RPC_FAR *puArgErr);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Leitura_) (CIMSockets__TRANSPONDER_Interface __RPC_FAR *This, 
	                                                  short comPort, 
	                                                  BSTR *buffer, 
	                                                  short timeout, 
	                                                  short *arg4);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Inicializa_) (CIMSockets__TRANSPONDER_Interface __RPC_FAR *This, 
	                                                     BSTR var_transponder, 
	                                                     BSTR modelo, 
	                                                     BSTR *numserie, 
	                                                     short *arg4);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Recupera_) (CIMSockets__TRANSPONDER_Interface __RPC_FAR *This, 
	                                                   BSTR var_transponder, 
	                                                   BSTR *numserie, 
	                                                   short *arg3);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Limpa_) (CIMSockets__TRANSPONDER_Interface __RPC_FAR *This, 
	                                                BSTR var_transponder, 
	                                                short *arg2);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Regride_) (CIMSockets__TRANSPONDER_Interface __RPC_FAR *This, 
	                                                  BSTR modelo, 
	                                                  short *arg2);

	HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Associa_) (CIMSockets__TRANSPONDER_Interface __RPC_FAR *This, 
	                                                  BSTR var_transponder, 
	                                                  BSTR numserie, 
	                                                  BSTR *nserie, 
	                                                  short *arg4);

} CIMSockets__TRANSPONDER_VTable;

typedef interface tagCIMSockets__TRANSPONDER_Interface
{
	CONST_VTBL CIMSockets__TRANSPONDER_VTable __RPC_FAR *lpVtbl;
} CIMSockets__TRANSPONDER_Interface;

const IID CIMSockets_IID__SMF =
	{
		0xC00288AE, 0x4764, 0x457B, 0xBD, 0xB2, 0x46, 0x89, 0xC, 0xC2, 0xAC, 0x76
	};

const IID CIMSockets_IID__Comm =
	{
		0xFAE80164, 0xD770, 0x4FA2, 0xBD, 0xD5, 0xCB, 0x7B, 0x21, 0xB, 0xA3, 0x6A
	};

const IID CIMSockets_IID__TRANSPONDER =
	{
		0x5C9E7510, 0x26B9, 0x4744, 0x97, 0xDA, 0x31, 0x68, 0xCE, 0xBA, 0x51, 0x67
	};

HRESULT CVIFUNC CIMSockets_New_SMF (const char *server,
                                    int supportMultithreading, LCID locale,
                                    int reserved, CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0xEC5E5A11, 0xE6D5, 0x41D7, 0xA9, 0x9B, 0x7, 0xFF, 0xC9,
	              0x28, 0x11, 0xD2};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &CIMSockets_IID__SMF,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets_Open_SMF (const char *fileName, const char *server,
                                     int supportMultithreading, LCID locale,
                                     int reserved, CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0xEC5E5A11, 0xE6D5, 0x41D7, 0xA9, 0x9B, 0x7, 0xFF, 0xC9,
	              0x28, 0x11, 0xD2};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &CIMSockets_IID__SMF,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets_Active_SMF (const char *server,
                                       int supportMultithreading, LCID locale,
                                       int reserved, CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0xEC5E5A11, 0xE6D5, 0x41D7, 0xA9, 0x9B, 0x7, 0xFF, 0xC9,
	              0x28, 0x11, 0xD2};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &CIMSockets_IID__SMF,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFGetmy_sockID (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short *my_sockID)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Getmy_sockID_ (__vtblIFacePtr,
	                                                   my_sockID));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFSetmy_sockID (CAObjHandle objectHandle,
                                             ERRORINFO *errorInfo,
                                             short my_sockID)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Setmy_sockID_ (__vtblIFacePtr,
	                                                   my_sockID));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFGetmy_BaseTimestamp (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long *my_BaseTimestamp)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Getmy_BaseTimestamp_ (__vtblIFacePtr,
	                                                          my_BaseTimestamp));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFSetmy_BaseTimestamp (CAObjHandle objectHandle,
                                                    ERRORINFO *errorInfo,
                                                    long my_BaseTimestamp)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Setmy_BaseTimestamp_ (__vtblIFacePtr,
	                                                          my_BaseTimestamp));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFGetmy_BaseData (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               char **my_BaseData)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR my_BaseData__AutoType = 0;

	*my_BaseData = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Getmy_BaseData_ (__vtblIFacePtr,
	                                                     &my_BaseData__AutoType));

	if (my_BaseData)
		__caErrChk (CA_BSTRGetCString (my_BaseData__AutoType, my_BaseData));

Error:
	CA_FreeBSTR (my_BaseData__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*my_BaseData);
		*my_BaseData = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFSetmy_BaseData (CAObjHandle objectHandle,
                                               ERRORINFO *errorInfo,
                                               const char *my_BaseData)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR my_BaseData__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (my_BaseData, &my_BaseData__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Setmy_BaseData_ (__vtblIFacePtr,
	                                                     my_BaseData__AutoType));

Error:
	CA_FreeBSTR (my_BaseData__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFGetmy_Station_ID (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 char **my_Station_ID)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR my_Station_ID__AutoType = 0;

	*my_Station_ID = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Getmy_Station_ID_ (__vtblIFacePtr,
	                                                       &my_Station_ID__AutoType));

	if (my_Station_ID)
		__caErrChk (CA_BSTRGetCString (my_Station_ID__AutoType, my_Station_ID));

Error:
	CA_FreeBSTR (my_Station_ID__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*my_Station_ID);
		*my_Station_ID = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFSetmy_Station_ID (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *my_Station_ID)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR my_Station_ID__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (my_Station_ID, &my_Station_ID__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Setmy_Station_ID_ (__vtblIFacePtr,
	                                                       my_Station_ID__AutoType));

Error:
	CA_FreeBSTR (my_Station_ID__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFGetmy_Timeout (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              short *my_Timeout)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Getmy_Timeout_ (__vtblIFacePtr,
	                                                    my_Timeout));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFSetmy_Timeout (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              short my_Timeout)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Setmy_Timeout_ (__vtblIFacePtr,
	                                                    my_Timeout));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFreadini (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo,
                                        const char *arquivo, const char *secao,
                                        const char *linha, char **arg4)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR arquivo__AutoType = 0;
	BSTR secao__AutoType = 0;
	BSTR linha__AutoType = 0;
	BSTR arg4__AutoType = 0;

	*arg4 = 0;

	__caErrChk (CA_CStringToBSTR (arquivo, &arquivo__AutoType));
	__caErrChk (CA_CStringToBSTR (secao, &secao__AutoType));
	__caErrChk (CA_CStringToBSTR (linha, &linha__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->readini_ (__vtblIFacePtr,
	                                              arquivo__AutoType,
	                                              secao__AutoType,
	                                              linha__AutoType,
	                                              &arg4__AutoType));

	if (arg4)
		__caErrChk (CA_BSTRGetCString (arg4__AutoType, arg4));

Error:
	CA_FreeBSTR (arquivo__AutoType);
	CA_FreeBSTR (secao__AutoType);
	CA_FreeBSTR (linha__AutoType);
	CA_FreeBSTR (arg4__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*arg4);
		*arg4 = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFwriteini (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo,
                                         const char *arquivo, const char *secao,
                                         const char *linha,
                                         const char *parametro, short *arg5)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR arquivo__AutoType = 0;
	BSTR secao__AutoType = 0;
	BSTR linha__AutoType = 0;
	BSTR parametro__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (arquivo, &arquivo__AutoType));
	__caErrChk (CA_CStringToBSTR (secao, &secao__AutoType));
	__caErrChk (CA_CStringToBSTR (linha, &linha__AutoType));
	__caErrChk (CA_CStringToBSTR (parametro, &parametro__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->writeini_ (__vtblIFacePtr,
	                                               arquivo__AutoType,
	                                               secao__AutoType,
	                                               linha__AutoType,
	                                               parametro__AutoType, arg5));

Error:
	CA_FreeBSTR (arquivo__AutoType);
	CA_FreeBSTR (secao__AutoType);
	CA_FreeBSTR (linha__AutoType);
	CA_FreeBSTR (parametro__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFopen_sock (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, const char *host,
                                          const char *service, short timeout,
                                          short *arg4)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR host__AutoType = 0;
	BSTR service__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (host, &host__AutoType));
	__caErrChk (CA_CStringToBSTR (service, &service__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->open_sock_ (__vtblIFacePtr,
	                                                host__AutoType,
	                                                service__AutoType, timeout,
	                                                arg4));

Error:
	CA_FreeBSTR (host__AutoType);
	CA_FreeBSTR (service__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFsync_sock (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, short sockID,
                                          const char *station, short timeout,
                                          long *arg4)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->sync_sock_ (__vtblIFacePtr, sockID,
	                                                station__AutoType, timeout,
	                                                arg4));

Error:
	CA_FreeBSTR (station__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFclose_sock (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, short sockID,
                                           short timeout, short *arg3)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->close_sock_ (__vtblIFacePtr, sockID,
	                                                 timeout, arg3));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFerrormsg (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short result,
                                         char **arg2)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR arg2__AutoType = 0;

	*arg2 = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->errormsg_ (__vtblIFacePtr, result,
	                                               &arg2__AutoType));

	if (arg2)
		__caErrChk (CA_BSTRGetCString (arg2__AutoType, arg2));

Error:
	CA_FreeBSTR (arg2__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*arg2);
		*arg2 = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFmoveout (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, short sockID,
                                        short timeout, const char *station,
                                        const char *barcode, const char *status,
                                        short *arg6)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR barcode__AutoType = 0;
	BSTR status__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (barcode, &barcode__AutoType));
	__caErrChk (CA_CStringToBSTR (status, &status__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->moveout_ (__vtblIFacePtr, sockID,
	                                              timeout, station__AutoType,
	                                              barcode__AutoType,
	                                              status__AutoType, arg6));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (barcode__AutoType);
	CA_FreeBSTR (status__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFmovein (CAObjHandle objectHandle,
                                       ERRORINFO *errorInfo, short sockID,
                                       short timeout, const char *station,
                                       const char *barcode,
                                       char **mensagem_retorno, short *arg6)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR barcode__AutoType = 0;
	BSTR mensagem_retorno__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (barcode, &barcode__AutoType));
	__caErrChk (CA_CStringToBSTR (*mensagem_retorno,
	                              &mensagem_retorno__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->movein_ (__vtblIFacePtr, sockID,
	                                             timeout, station__AutoType,
	                                             barcode__AutoType,
	                                             &mensagem_retorno__AutoType,
	                                             arg6));

	CA_FreeMemory (*mensagem_retorno);
	*mensagem_retorno = 0;
	if (mensagem_retorno)
		__caErrChk (CA_BSTRGetCString (mensagem_retorno__AutoType,
	                               mensagem_retorno));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (barcode__AutoType);
	CA_FreeBSTR (mensagem_retorno__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*mensagem_retorno);
		*mensagem_retorno = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFpackout (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, short sockID,
                                        short timeout, const char *station,
                                        const char *boxBarcode,
                                        SAFEARRAY **barcode, short quantidade,
                                        short *arg7)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR boxBarcode__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (boxBarcode, &boxBarcode__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->packout_ (__vtblIFacePtr, sockID,
	                                              timeout, station__AutoType,
	                                              boxBarcode__AutoType,
	                                              barcode, quantidade, arg7));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (boxBarcode__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_SafeArrayDestroy (*barcode);
		*barcode = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFenvtest (CAObjHandle objectHandle,
                                        ERRORINFO *errorInfo, short sockID,
                                        short timeout, const char *station,
                                        const char *unitId, const char *groupId,
                                        const char *groupVersion,
                                        const char *testId,
                                        const char *testResult, double testValue,
                                        short flag, short *arg11)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR unitId__AutoType = 0;
	BSTR groupId__AutoType = 0;
	BSTR groupVersion__AutoType = 0;
	BSTR testId__AutoType = 0;
	BSTR testResult__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (unitId, &unitId__AutoType));
	__caErrChk (CA_CStringToBSTR (groupId, &groupId__AutoType));
	__caErrChk (CA_CStringToBSTR (groupVersion, &groupVersion__AutoType));
	__caErrChk (CA_CStringToBSTR (testId, &testId__AutoType));
	__caErrChk (CA_CStringToBSTR (testResult, &testResult__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->envtest_ (__vtblIFacePtr, sockID,
	                                              timeout, station__AutoType,
	                                              unitId__AutoType,
	                                              groupId__AutoType,
	                                              groupVersion__AutoType,
	                                              testId__AutoType,
	                                              testResult__AutoType,
	                                              testValue, flag, arg11));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (unitId__AutoType);
	CA_FreeBSTR (groupId__AutoType);
	CA_FreeBSTR (groupVersion__AutoType);
	CA_FreeBSTR (testId__AutoType);
	CA_FreeBSTR (testResult__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFSMFMessage (CAObjHandle objectHandle,
                                           ERRORINFO *errorInfo, char **arg1)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR arg1__AutoType = 0;

	*arg1 = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->SMFMessage_ (__vtblIFacePtr,
	                                                 &arg1__AutoType));

	if (arg1)
		__caErrChk (CA_BSTRGetCString (arg1__AutoType, arg1));

Error:
	CA_FreeBSTR (arg1__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*arg1);
		*arg1 = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFBuild (CAObjHandle objectHandle,
                                      ERRORINFO *errorInfo, short sockID,
                                      short timeout, const char *station,
                                      const char *barcodePai,
                                      const char *barcodeFilho, short *arg6)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR barcodePai__AutoType = 0;
	BSTR barcodeFilho__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (barcodePai, &barcodePai__AutoType));
	__caErrChk (CA_CStringToBSTR (barcodeFilho, &barcodeFilho__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Build_ (__vtblIFacePtr, sockID,
	                                            timeout, station__AutoType,
	                                            barcodePai__AutoType,
	                                            barcodeFilho__AutoType, arg6));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (barcodePai__AutoType);
	CA_FreeBSTR (barcodeFilho__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__SMFoperator (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short sockID,
                                         short timeout, const char *station,
                                         const char *operator_id,
                                         const char *onoff,
                                         char **mensagem_retorno, short *arg7)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR operator_id__AutoType = 0;
	BSTR onoff__AutoType = 0;
	BSTR mensagem_retorno__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (operator_id, &operator_id__AutoType));
	__caErrChk (CA_CStringToBSTR (onoff, &onoff__AutoType));
	__caErrChk (CA_CStringToBSTR (*mensagem_retorno,
	                              &mensagem_retorno__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->operator_ (__vtblIFacePtr, sockID,
	                                               timeout, station__AutoType,
	                                               operator_id__AutoType,
	                                               onoff__AutoType,
	                                               &mensagem_retorno__AutoType,
	                                               arg7));

	CA_FreeMemory (*mensagem_retorno);
	*mensagem_retorno = 0;
	if (mensagem_retorno)
		__caErrChk (CA_BSTRGetCString (mensagem_retorno__AutoType,
	                               mensagem_retorno));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (operator_id__AutoType);
	CA_FreeBSTR (onoff__AutoType);
	CA_FreeBSTR (mensagem_retorno__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*mensagem_retorno);
		*mensagem_retorno = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

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
                                             SAFEARRAY **unMed, short *arg12)
{
	HRESULT __result;
	CIMSockets__SMF_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR station__AutoType = 0;
	BSTR groupId__AutoType = 0;
	BSTR groupVersion__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (station, &station__AutoType));
	__caErrChk (CA_CStringToBSTR (groupId, &groupId__AutoType));
	__caErrChk (CA_CStringToBSTR (groupVersion, &groupVersion__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__SMF, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->envtestgroup_ (__vtblIFacePtr, sockID,
	                                                   timeout,
	                                                   station__AutoType,
	                                                   groupId__AutoType,
	                                                   groupVersion__AutoType,
	                                                   quantidade, testId,
	                                                   testDesc, lowLimit,
	                                                   highLimit, unMed, arg12));

Error:
	CA_FreeBSTR (station__AutoType);
	CA_FreeBSTR (groupId__AutoType);
	CA_FreeBSTR (groupVersion__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_SafeArrayDestroy (*testId);
		*testId = 0;
		CA_SafeArrayDestroy (*testDesc);
		*testDesc = 0;
		CA_SafeArrayDestroy (*lowLimit);
		*lowLimit = 0;
		CA_SafeArrayDestroy (*highLimit);
		*highLimit = 0;
		CA_SafeArrayDestroy (*unMed);
		*unMed = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__SMF, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets_New_Comm (const char *server,
                                     int supportMultithreading, LCID locale,
                                     int reserved, CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0x5A5BC6F3, 0xA895, 0x4774, 0xBA, 0xE9, 0x80, 0x14, 0xCC,
	              0xDC, 0x6B, 0x90};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &CIMSockets_IID__Comm,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets_Open_Comm (const char *fileName, const char *server,
                                      int supportMultithreading, LCID locale,
                                      int reserved, CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0x5A5BC6F3, 0xA895, 0x4774, 0xBA, 0xE9, 0x80, 0x14, 0xCC,
	              0xDC, 0x6B, 0x90};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &CIMSockets_IID__Comm,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets_Active_Comm (const char *server,
                                        int supportMultithreading, LCID locale,
                                        int reserved, CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0x5A5BC6F3, 0xA895, 0x4774, 0xBA, 0xE9, 0x80, 0x14, 0xCC,
	              0xDC, 0x6B, 0x90};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &CIMSockets_IID__Comm,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets__CommOpenCOM (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, short comPort,
                                         const char *comSettings, short *arg3)
{
	HRESULT __result;
	CIMSockets__Comm_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR comSettings__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (comSettings, &comSettings__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__Comm, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->OpenCOM_ (__vtblIFacePtr, comPort,
	                                              comSettings__AutoType, arg3));

Error:
	CA_FreeBSTR (comSettings__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__Comm, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__CommCloseCOM (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo, short *arg1)
{
	HRESULT __result;
	CIMSockets__Comm_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__Comm, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->CloseCOM_ (__vtblIFacePtr, arg1));

Error:
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__Comm, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__CommReadCOM (CAObjHandle objectHandle,
                                         ERRORINFO *errorInfo, char **buffer,
                                         short *arg2)
{
	HRESULT __result;
	CIMSockets__Comm_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR buffer__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (*buffer, &buffer__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__Comm, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->ReadCOM_ (__vtblIFacePtr,
	                                              &buffer__AutoType, arg2));

	CA_FreeMemory (*buffer);
	*buffer = 0;
	if (buffer)
		__caErrChk (CA_BSTRGetCString (buffer__AutoType, buffer));

Error:
	CA_FreeBSTR (buffer__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*buffer);
		*buffer = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__Comm, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__CommWriteCOM (CAObjHandle objectHandle,
                                          ERRORINFO *errorInfo,
                                          const char *buffer, short *arg2)
{
	HRESULT __result;
	CIMSockets__Comm_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR buffer__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (buffer, &buffer__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__Comm, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->WriteCOM_ (__vtblIFacePtr,
	                                               buffer__AutoType, arg2));

Error:
	CA_FreeBSTR (buffer__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__Comm, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets_New_TRANSPONDER (const char *server,
                                            int supportMultithreading,
                                            LCID locale, int reserved,
                                            CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0xEF9CC4F9, 0x3F9B, 0x44B0, 0xB5, 0x61, 0x2B, 0x71, 0x28,
	              0x2B, 0x5F, 0xCE};

	__result = CA_CreateObjectByClassIdEx (&clsid, server,
	                                       &CIMSockets_IID__TRANSPONDER,
	                                       supportMultithreading, locale,
	                                       reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets_Open_TRANSPONDER (const char *fileName,
                                             const char *server,
                                             int supportMultithreading,
                                             LCID locale, int reserved,
                                             CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0xEF9CC4F9, 0x3F9B, 0x44B0, 0xB5, 0x61, 0x2B, 0x71, 0x28,
	              0x2B, 0x5F, 0xCE};

	__result = CA_LoadObjectFromFileByClassIdEx (fileName, &clsid, server,
	                                             &CIMSockets_IID__TRANSPONDER,
	                                             supportMultithreading, locale,
	                                             reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets_Active_TRANSPONDER (const char *server,
                                               int supportMultithreading,
                                               LCID locale, int reserved,
                                               CAObjHandle *objectHandle)
{
	HRESULT __result;
	GUID clsid = {0xEF9CC4F9, 0x3F9B, 0x44B0, 0xB5, 0x61, 0x2B, 0x71, 0x28,
	              0x2B, 0x5F, 0xCE};

	__result = CA_GetActiveObjectByClassIdEx (&clsid, server,
	                                          &CIMSockets_IID__TRANSPONDER,
	                                          supportMultithreading, locale,
	                                          reserved, objectHandle);

	return __result;
}

HRESULT CVIFUNC CIMSockets__TRANSPONDERLeitura (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                short comPort, char **buffer,
                                                short timeout, short *arg4)
{
	HRESULT __result;
	CIMSockets__TRANSPONDER_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR buffer__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (*buffer, &buffer__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__TRANSPONDER, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Leitura_ (__vtblIFacePtr, comPort,
	                                              &buffer__AutoType, timeout,
	                                              arg4));

	CA_FreeMemory (*buffer);
	*buffer = 0;
	if (buffer)
		__caErrChk (CA_BSTRGetCString (buffer__AutoType, buffer));

Error:
	CA_FreeBSTR (buffer__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*buffer);
		*buffer = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__TRANSPONDER, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__TRANSPONDERInicializa (CAObjHandle objectHandle,
                                                   ERRORINFO *errorInfo,
                                                   const char *var_transponder,
                                                   const char *modelo,
                                                   char **numserie, short *arg4)
{
	HRESULT __result;
	CIMSockets__TRANSPONDER_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR var_transponder__AutoType = 0;
	BSTR modelo__AutoType = 0;
	BSTR numserie__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (var_transponder, &var_transponder__AutoType));
	__caErrChk (CA_CStringToBSTR (modelo, &modelo__AutoType));
	__caErrChk (CA_CStringToBSTR (*numserie, &numserie__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__TRANSPONDER, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Inicializa_ (__vtblIFacePtr,
	                                                 var_transponder__AutoType,
	                                                 modelo__AutoType,
	                                                 &numserie__AutoType, arg4));

	CA_FreeMemory (*numserie);
	*numserie = 0;
	if (numserie)
		__caErrChk (CA_BSTRGetCString (numserie__AutoType, numserie));

Error:
	CA_FreeBSTR (var_transponder__AutoType);
	CA_FreeBSTR (modelo__AutoType);
	CA_FreeBSTR (numserie__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*numserie);
		*numserie = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__TRANSPONDER, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__TRANSPONDERRecupera (CAObjHandle objectHandle,
                                                 ERRORINFO *errorInfo,
                                                 const char *var_transponder,
                                                 char **numserie, short *arg3)
{
	HRESULT __result;
	CIMSockets__TRANSPONDER_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR var_transponder__AutoType = 0;
	BSTR numserie__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (var_transponder, &var_transponder__AutoType));
	__caErrChk (CA_CStringToBSTR (*numserie, &numserie__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__TRANSPONDER, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Recupera_ (__vtblIFacePtr,
	                                               var_transponder__AutoType,
	                                               &numserie__AutoType, arg3));

	CA_FreeMemory (*numserie);
	*numserie = 0;
	if (numserie)
		__caErrChk (CA_BSTRGetCString (numserie__AutoType, numserie));

Error:
	CA_FreeBSTR (var_transponder__AutoType);
	CA_FreeBSTR (numserie__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*numserie);
		*numserie = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__TRANSPONDER, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__TRANSPONDERLimpa (CAObjHandle objectHandle,
                                              ERRORINFO *errorInfo,
                                              const char *var_transponder,
                                              short *arg2)
{
	HRESULT __result;
	CIMSockets__TRANSPONDER_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR var_transponder__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (var_transponder, &var_transponder__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__TRANSPONDER, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Limpa_ (__vtblIFacePtr,
	                                            var_transponder__AutoType,
	                                            arg2));

Error:
	CA_FreeBSTR (var_transponder__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__TRANSPONDER, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__TRANSPONDERRegride (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *modelo, short *arg2)
{
	HRESULT __result;
	CIMSockets__TRANSPONDER_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR modelo__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (modelo, &modelo__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__TRANSPONDER, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Regride_ (__vtblIFacePtr,
	                                              modelo__AutoType, arg2));

Error:
	CA_FreeBSTR (modelo__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__TRANSPONDER, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}

HRESULT CVIFUNC CIMSockets__TRANSPONDERAssocia (CAObjHandle objectHandle,
                                                ERRORINFO *errorInfo,
                                                const char *var_transponder,
                                                const char *numserie,
                                                char **nserie, short *arg4)
{
	HRESULT __result;
	CIMSockets__TRANSPONDER_Interface * __vtblIFacePtr = 0;
	int __didAddRef;
	int __errorInfoPresent = 0;
	BSTR var_transponder__AutoType = 0;
	BSTR numserie__AutoType = 0;
	BSTR nserie__AutoType = 0;

	__caErrChk (CA_CStringToBSTR (var_transponder, &var_transponder__AutoType));
	__caErrChk (CA_CStringToBSTR (numserie, &numserie__AutoType));
	__caErrChk (CA_CStringToBSTR (*nserie, &nserie__AutoType));

	__caErrChk (CA_GetInterfaceFromObjHandle (objectHandle,
	                                          &CIMSockets_IID__TRANSPONDER, 0,
	                                          &__vtblIFacePtr, &__didAddRef));
	__caErrChk (__vtblIFacePtr->lpVtbl->Associa_ (__vtblIFacePtr,
	                                              var_transponder__AutoType,
	                                              numserie__AutoType,
	                                              &nserie__AutoType, arg4));

	CA_FreeMemory (*nserie);
	*nserie = 0;
	if (nserie)
		__caErrChk (CA_BSTRGetCString (nserie__AutoType, nserie));

Error:
	CA_FreeBSTR (var_transponder__AutoType);
	CA_FreeBSTR (numserie__AutoType);
	CA_FreeBSTR (nserie__AutoType);
	if (__vtblIFacePtr && __didAddRef)
		__vtblIFacePtr->lpVtbl->Release (__vtblIFacePtr);
	if (FAILED(__result))
		{
		CA_FreeMemory (*nserie);
		*nserie = 0;
		}
	CA_FillErrorInfoEx (objectHandle, &CIMSockets_IID__TRANSPONDER, __result,
	                    errorInfo, &__errorInfoPresent);
	if (__errorInfoPresent)
		__result = DISP_E_EXCEPTION;
	return __result;
}
