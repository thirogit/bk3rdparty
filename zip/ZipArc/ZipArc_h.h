

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Oct 18 12:47:51 2009
 */
/* Compiler settings for .\ZipArc.odl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __ZipArc_h_h__
#define __ZipArc_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IZipArc_FWD_DEFINED__
#define __IZipArc_FWD_DEFINED__
typedef interface IZipArc IZipArc;
#endif 	/* __IZipArc_FWD_DEFINED__ */


#ifndef __Document_FWD_DEFINED__
#define __Document_FWD_DEFINED__

#ifdef __cplusplus
typedef class Document Document;
#else
typedef struct Document Document;
#endif /* __cplusplus */

#endif 	/* __Document_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __ZipArc_LIBRARY_DEFINED__
#define __ZipArc_LIBRARY_DEFINED__

/* library ZipArc */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ZipArc;

#ifndef __IZipArc_DISPINTERFACE_DEFINED__
#define __IZipArc_DISPINTERFACE_DEFINED__

/* dispinterface IZipArc */
/* [uuid] */ 


EXTERN_C const IID DIID_IZipArc;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CF49F80B-86E3-46ED-BF1F-B0D01B558200")
    IZipArc : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IZipArcVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZipArc * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZipArc * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZipArc * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IZipArc * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IZipArc * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IZipArc * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IZipArc * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IZipArcVtbl;

    interface IZipArc
    {
        CONST_VTBL struct IZipArcVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZipArc_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IZipArc_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IZipArc_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IZipArc_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IZipArc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IZipArc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IZipArc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IZipArc_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Document;

#ifdef __cplusplus

class DECLSPEC_UUID("737CCB50-E9A0-46FB-AC0C-409B5E943C40")
Document;
#endif
#endif /* __ZipArc_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


