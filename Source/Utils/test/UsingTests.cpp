//
// Created by mrjaqbq on 12.03.16.
//

#include <exception>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <Handle.h>
#include "Using.h"

namespace orgMem = Memory;

namespace UsingTests
{
	class FooMock
	{
	public:
		MOCK_METHOD0(Die, void());

		~FooMock()
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

	class UsingTest : public ::testing::Test
	{

	};

	TEST_F(UsingTest, IsSafeDeleteCalled)
	{
		FooMock** ref;
		USING(FooMock, cos)
		{
			ref = &cos;
		}

		EXPECT_EQ(*ref, nullptr);
	}

	TEST_F(UsingTest, IsSafeDeleteCalledOnThrow)
	{
		FooMock** someRef;
		TRY_USING(FooMock(), cos,
		{
			someRef = &cos;
			EXPECT_CALL(*cos, Die()).Times(1);
			cos->throwException();
		})

		EXPECT_EQ(*someRef, nullptr);
	}
}