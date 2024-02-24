

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Mon Jan 18 22:14:07 2038
 */
/* Compiler settings for pic2movie.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __pic2movie_i_h__
#define __pic2movie_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IP2MMakeMovie_FWD_DEFINED__
#define __IP2MMakeMovie_FWD_DEFINED__
typedef interface IP2MMakeMovie IP2MMakeMovie;

#endif 	/* __IP2MMakeMovie_FWD_DEFINED__ */


#ifndef __P2MMakeMovie_FWD_DEFINED__
#define __P2MMakeMovie_FWD_DEFINED__

#ifdef __cplusplus
typedef class P2MMakeMovie P2MMakeMovie;
#else
typedef struct P2MMakeMovie P2MMakeMovie;
#endif /* __cplusplus */

#endif 	/* __P2MMakeMovie_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IP2MMakeMovie_INTERFACE_DEFINED__
#define __IP2MMakeMovie_INTERFACE_DEFINED__

/* interface IP2MMakeMovie */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IP2MMakeMovie;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("052a6c95-f7ed-4d87-8e53-c31c94bd319b")
    IP2MMakeMovie : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE StartMovie( 
            /* [in] */ BSTR outputPath) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddFrames( 
            /* [in] */ BSTR paths) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE FinishMovie( void) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FramesPerSecond( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FramesPerSecond( 
            /* [in] */ ULONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LogFile( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LogFile( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LogLevel( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LogLevel( 
            /* [in] */ ULONG newVal) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IP2MMakeMovieVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IP2MMakeMovie * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IP2MMakeMovie * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IP2MMakeMovie * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IP2MMakeMovie * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IP2MMakeMovie * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IP2MMakeMovie * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IP2MMakeMovie * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *StartMovie )( 
            IP2MMakeMovie * This,
            /* [in] */ BSTR outputPath);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AddFrames )( 
            IP2MMakeMovie * This,
            /* [in] */ BSTR paths);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *FinishMovie )( 
            IP2MMakeMovie * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FramesPerSecond )( 
            IP2MMakeMovie * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FramesPerSecond )( 
            IP2MMakeMovie * This,
            /* [in] */ ULONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LogFile )( 
            IP2MMakeMovie * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LogFile )( 
            IP2MMakeMovie * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LogLevel )( 
            IP2MMakeMovie * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LogLevel )( 
            IP2MMakeMovie * This,
            /* [in] */ ULONG newVal);
        
        END_INTERFACE
    } IP2MMakeMovieVtbl;

    interface IP2MMakeMovie
    {
        CONST_VTBL struct IP2MMakeMovieVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IP2MMakeMovie_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IP2MMakeMovie_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IP2MMakeMovie_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IP2MMakeMovie_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IP2MMakeMovie_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IP2MMakeMovie_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IP2MMakeMovie_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IP2MMakeMovie_StartMovie(This,outputPath)	\
    ( (This)->lpVtbl -> StartMovie(This,outputPath) ) 

#define IP2MMakeMovie_AddFrames(This,paths)	\
    ( (This)->lpVtbl -> AddFrames(This,paths) ) 

#define IP2MMakeMovie_FinishMovie(This)	\
    ( (This)->lpVtbl -> FinishMovie(This) ) 

#define IP2MMakeMovie_get_FramesPerSecond(This,pVal)	\
    ( (This)->lpVtbl -> get_FramesPerSecond(This,pVal) ) 

#define IP2MMakeMovie_put_FramesPerSecond(This,newVal)	\
    ( (This)->lpVtbl -> put_FramesPerSecond(This,newVal) ) 

#define IP2MMakeMovie_get_LogFile(This,pVal)	\
    ( (This)->lpVtbl -> get_LogFile(This,pVal) ) 

#define IP2MMakeMovie_put_LogFile(This,newVal)	\
    ( (This)->lpVtbl -> put_LogFile(This,newVal) ) 

#define IP2MMakeMovie_get_LogLevel(This,pVal)	\
    ( (This)->lpVtbl -> get_LogLevel(This,pVal) ) 

#define IP2MMakeMovie_put_LogLevel(This,newVal)	\
    ( (This)->lpVtbl -> put_LogLevel(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IP2MMakeMovie_INTERFACE_DEFINED__ */



#ifndef __pic2movieLib_LIBRARY_DEFINED__
#define __pic2movieLib_LIBRARY_DEFINED__

/* library pic2movieLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_pic2movieLib;

EXTERN_C const CLSID CLSID_P2MMakeMovie;

#ifdef __cplusplus

class DECLSPEC_UUID("f5fd5fa3-049f-4f80-8189-9626669aae1e")
P2MMakeMovie;
#endif
#endif /* __pic2movieLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


