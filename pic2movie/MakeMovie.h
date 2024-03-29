#pragma once
#include "resource.h"
#include "pic2movie_i.h"
#include <PicsToMovie.h>

//#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
//#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
//#endif

using namespace ATL;

class ATL_NO_VTABLE CMakeMovie :
   public CComObjectRootEx<CComSingleThreadModel>,
   public CComCoClass<CMakeMovie, &CLSID_P2MMakeMovie>,
   public IDispatchImpl<IP2MMakeMovie, &IID_IP2MMakeMovie, &LIBID_pic2movieLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
   CMakeMovie()
   {
   }

   DECLARE_REGISTRY_RESOURCEID(106)

   BEGIN_COM_MAP(CMakeMovie)
         COM_INTERFACE_ENTRY(IP2MMakeMovie)
         COM_INTERFACE_ENTRY(IDispatch)
   END_COM_MAP()


   DECLARE_PROTECT_FINAL_CONSTRUCT()

   HRESULT FinalConstruct()
   {
      return S_OK;
   }

   void FinalRelease()
   {
   }

public:
   STDMETHOD(StartMovie)(BSTR outputPath);
   STDMETHOD(AddFrames)(BSTR paths);
   STDMETHOD(FinishMovie)();
   STDMETHOD(get_FramesPerSecond)(ULONG* pVal);
   STDMETHOD(put_FramesPerSecond)(ULONG newVal);
   STDMETHOD(get_LogFile)(BSTR* pVal);
   STDMETHOD(put_LogFile)(BSTR newVal);
   STDMETHOD(get_LogLevel)(ULONG* pVal);
   STDMETHOD(put_LogLevel)(ULONG newVal);

protected:
   PicsToMovie _picsToMovie{};
};

OBJECT_ENTRY_AUTO(__uuidof(P2MMakeMovie), CMakeMovie)
