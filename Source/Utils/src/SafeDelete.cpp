//
// Created by bentoo on 18.11.16.
//

#include "Utils/SafeDelete.h"
#include <malloc.h>

namespace Memory
{
	template <> void SafeFree(void*& pVal)
	{
		if(pVal != nullptr)
		{
			free(pVal);
			pVal = nullptr;
		}
	}
}
