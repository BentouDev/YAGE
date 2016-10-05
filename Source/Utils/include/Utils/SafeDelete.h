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
				unsigned char* current, result = 0;
				T* start = &pVal[i];
				T* end	 = start + 1;

				for(current  = reinterpret_cast<unsigned char*>(start);
					current != reinterpret_cast<unsigned char*>(end);
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
