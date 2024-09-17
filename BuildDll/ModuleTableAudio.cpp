
#ifdef WIN32
#include "stdafx.h"
#endif

#include "Framework.h"
#include "Errors.h"
#include "ModuleLibDLL.h"

#ifdef	__cplusplus
extern "C" {
#endif

#include "TestAudio.h"


static const ModClassModule *s_module_descriptor_table[] =
{
	LISTOFCLASSOBJECTS
};

static UINT s_module_descriptor_table_size = sizeof(s_module_descriptor_table) / sizeof(s_module_descriptor_table[0]);

typedef void (*Regfun)(ClassDescriptor **, int);

/** Register the modules in this DLL. */
void FIXNAME(RegisterModules2)(Regfun pFun)
{
	if (pFun)
	{
		pFun((ClassDescriptor **)s_module_descriptor_table, s_module_descriptor_table_size);
	}
	else
	{
		awe_fwRegisterModules((ClassDescriptor **)s_module_descriptor_table, s_module_descriptor_table_size);
	}
}

void FIXNAME(RegisterModules)()
{
	awe_fwRegisterModules((ClassDescriptor **)s_module_descriptor_table, s_module_descriptor_table_size);
}


const char *FIXNAME(ModuleDepend)()
{
	return USESDLLS;
}

const UINT32 FIXNAME(ModuleVersion)()
{
	return 0;
}

#ifdef	__cplusplus
}
#endif

/**
 * @}
 * End of file.
 */
