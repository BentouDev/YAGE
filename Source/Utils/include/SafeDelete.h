//
// Created by mrjaqbq on 05.03.16.
//

#ifndef VOLKHVY_DEBUG_H
#define VOLKHVY_DEBUG_H

namespace Memory
{
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

	template< class T > void SafeFree( T*& pVal )
	{
		if(pVal != nullptr)
		{
			pVal->~T();
			free( pVal );
			pVal = nullptr;
		}
	}
}

#endif //VOLKHVY_DEBUG_H
