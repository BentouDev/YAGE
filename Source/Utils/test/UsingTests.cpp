//
// Created by mrjaqbq on 12.03.16.
//

#include <exception>
#include <catch.hpp>
#include <trompeloeil.hpp>
#include "Utils/Handle.h"
#include "Utils/Using.h"

namespace orgMem = Memory;

extern template struct trompeloeil::reporter<trompeloeil::specialized>;

namespace UsingTests
{
	class IFooMock
	{
	public:
        MAKE_MOCK0(Die, void());

		~IFooMock()
		{
			Die();
		}

		void throwException()
		{
			throw std::exception();
		}
	};

	namespace Memory
	{
		template<typename T>
		void SafeDelete( T*& pVal )
		{
			orgMem::SafeDelete(pVal);
		}
	}

    TEST_CASE("UsingTest")
    {
        SECTION("IsSafeDeleteCalled")
        {
            IFooMock** ref;
            USING(IFooMock, cos)
            {
                ref = &cos;
            }

            REQUIRE(*ref == nullptr);
        }

        SECTION("IsSafeDeleteCalledOnThrow")
        {
            IFooMock** someRef;
            TRY_USING(IFooMock(), cos,
            {
                someRef = &cos;
                // fakeit::Verify(Method((*cos), Die)).Exactly(1);
                REQUIRE_CALL(*cos, Die()).TIMES(1);
                cos->throwException();
            })

            REQUIRE(*someRef == nullptr);
        }
    }
}