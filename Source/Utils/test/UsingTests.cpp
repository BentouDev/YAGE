//
// Created by mrjaqbq on 12.03.16.
//

//#include <exception>
//#include <catch.hpp>
//#include <trompeloeil.hpp>
//#include "Utils/Handle.h"
//#include "Utils/Using.h"
//
//namespace orgMem = Memory;
//
//extern template struct trompeloeil::reporter<trompeloeil::specialized>;
//
//namespace UsingTests
//{
//	class IFooMock
//	{
//	public:
//		virtual ~IFooMock() { }
//
//		void throwException()
//		{
//			throw std::exception();
//		}
//	};
//
//	using FooMock = trompeloeil::deathwatched<IFooMock>;
//
//	namespace Memory
//	{
//		template<typename T>
//		void SafeDelete( T*& pVal )
//		{
//			orgMem::SafeDelete(pVal);
//		}
//	}
//
//    TEST_CASE("UsingTest")
//    {
//        SECTION("IsSafeDeleteCalled")
//        {
//            FooMock** ref;
//            USING(FooMock, cos)
//            {
//                ref = &cos;
//                REQUIRE_DESTRUCTION((*cos));
//            }
//
//            REQUIRE(*ref == nullptr);
//        }
//
//        SECTION("IsSafeDeleteCalledOnThrow")
//        {
//            FooMock** someRef;
//            TRY_USING(FooMock(), cos,
//            {
//                someRef = &cos;
//                REQUIRE_DESTRUCTION((*cos));
//                cos->throwException();
//            })
//
//            REQUIRE(*someRef == nullptr);
//        }
//    }
//}