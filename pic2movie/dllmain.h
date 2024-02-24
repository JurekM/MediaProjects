// dllmain.h : Declaration of module class.
#pragma once

class Cpic2movieModule : public ATL::CAtlDllModuleT< Cpic2movieModule >
{
public :
	DECLARE_LIBID(LIBID_pic2movieLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_PIC2MOVIE, "{30961f0c-1d71-49f2-86a6-44f67c6ed1a7}")
};

extern class Cpic2movieModule _AtlModule;
