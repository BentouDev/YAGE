#include "yage.h"
#include <cstdio>
// #include "engine_export.h"

YAGE_API int YageTest()
{
	puts("Yage!\n");
	return 0;
}

#if _WIN32
#include <windows.h>

YAGE_API BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	return TRUE;
}
#endif