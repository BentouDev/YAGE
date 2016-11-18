//
// Created by bentoo on 18.11.16.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Core/MemoryModule.h>
#include <Utils/MemorySizes.h>

namespace MemoryModuleTests
{
	class MemoryModuleTest : public ::testing::Test
	{
	protected:
		const std::size_t MemorySize = Memory::MB(1);

	public:
		void SetUp()
		{

		}

		void TearDown()
		{

		}
	};

	TEST_F(MemoryModuleTest, CanCreateModule)
	{
		auto* module = new Core::MemoryModule(MemorySize);

		EXPECT_NE(module, nullptr);
		EXPECT_FALSE(module->releaseAll());

		delete module;
	}

	TEST_F(MemoryModuleTest, CanCreateAndFreeMemoryBlock)
	{
		auto* module = new Core::MemoryModule(MemorySize);

		auto& block = module->requestMemoryBlock(Memory::KB(1), "CanCreateAndFreeMemoryBlock");

		char* blockPtr = reinterpret_cast<char*>(&block);
		char* allocPtr = reinterpret_cast<char*>(&block.getAllocator());

		EXPECT_FALSE(blockPtr <= allocPtr);
		EXPECT_TRUE(allocPtr + sizeof(decltype(block.getAllocator())) < blockPtr);

		module->freeMemoryBlock(&block);

		EXPECT_FALSE(module->releaseAll());

		delete module;
	}

	TEST_F(MemoryModuleTest, DoesReportWhenLeaked)
	{
		auto* module = new Core::MemoryModule(MemorySize);
		auto& block = module->requestMemoryBlock(Memory::KB(1), "Leaked Block");
		auto* leakedMem = block.allocate(12, 2, DEBUG_SOURCE_INFO);

		EXPECT_TRUE(module->releaseAll());

		delete module;
	}
}
