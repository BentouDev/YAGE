//
// Created by mrjaqbq on 05.03.16.
//

#ifndef VOLKHVY_DEBUG_H
#define VOLKHVY_DEBUG_H

#include "BorrowedPtr.h"

namespace Memory
{
	template <class T> void SafeDelete(Utils::borrowed_ptr<T>& pVal)
	{
		if(pVal)
		{
			delete pVal.release();
		}
	}

	template< class T > void SafeDelete( T*& pVal )
	{
		if(pVal != nullptr)
		{
			delete pVal;
			pVal = nullptr;
		}
	}

	template< class T > void SafeDeleteArray( T*& pVal )
	{
		if(pVal != nullptr)
		{
			delete[] pVal;
			pVal = nullptr;
		}
	}

	template< class T > void SafeDeleteArray( T*& pVal, uint32_t count)
	{
		if(pVal != nullptr)
		{
			for(uint32_t i = 0; i < count; i++)
			{
				SafeDelete(pVal[i]);
			}

			delete[] pVal;
			pVal = nullptr;
		}
	}

	template< class T > void SafeFree( T*& pVal )
	{
		if(pVal != nullptr)
		{
			pVal->~T();
			free( pVal );
			pVal = nullptr;
		}
	}

	template< class T > void SafeFreeArray( T*& pVal, uint32_t count )
	{
		if(pVal != nullptr)
		{
			for(uint32_t i = 0; i < count; i++)
			{
				unsigned long long *current, result = 0;
				T* mm = &pVal[i];

				for(current = reinterpret_cast<unsigned long long *>(mm);
					current != reinterpret_cast<unsigned long long *>(mm+1);
					current++)
				{
					result |= *current;
				}

				bool isZeroed = !result;
				if(!isZeroed)
				{
					pVal[i].~T();
				}
			}

			free( pVal );
			pVal = nullptr;
		}
	}
}

#endif //VOLKHVY_DEBUG_H
