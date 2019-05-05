#include <RTTI/BaseLayer.h>
#include <Utils/MemoryBlock.h>
#include <Utils/DefaultBlock.h>
#include <RTTI/EnumInfo.h>
#include <RTTI/ClassInfo.h>

namespace RTTI
{
    BaseLayer::BaseLayer() // #NewAlloc
        : Classes()//Memory::GetDefaultBlock<BaseLayer>())
        , Enums()//Memory::GetDefaultBlock<BaseLayer>())
		, _loader{ nullptr }
    { }

    BaseLayer::~BaseLayer()
    {
        Unload();
    }

	bool BaseLayer::OnLoad(RTTI::IRegister& rtti)
	{
		if (_loader)
		{
			_loader(rtti);
			return true;
		}

		return false;
	}

    bool BaseLayer::OnUnload()
    {
        Classes.clear();
        Enums.clear();

        return true;
    }

	bool BaseLayer::TryGetType(const std::type_info& info, TypeInfo*& ptr)
	{
		ClassInfo* clazz = nullptr;
		if (TryGetClass(info, clazz))
		{
			ptr = clazz;
			return true;
		}

		EnumInfo* enumz = nullptr;
		if (TryGetEnum(info, enumz))
		{
			ptr = enumz;
			return true;
		}

		return false;
	}

	bool BaseLayer::TryGetEnum(const std::type_info& info, EnumInfo*& ptr)
	{
		auto itr = Enums.find(info.hash_code());
		if (itr != Enums.end())
		{
			ptr = (*itr).second;
			return true;
		}

		ptr = nullptr;
		return false;
	}

	bool BaseLayer::TryGetClass(const std::type_info& info, ClassInfo*& ptr)
	{
		auto itr = Classes.find(info.hash_code());
		if (itr != Classes.end())
		{
			ptr = (*itr).second;
			return true;
		}

		ptr = nullptr;
		return false;
	}

    void BaseLayer::RegisterType(const std::type_info& info, ClassInfo* clazz)
    {
        if (_loaded)
            return;

        Classes[info.hash_code()] = clazz;
    }

    void BaseLayer::RegisterType(const std::type_info& info, EnumInfo* enumz)
    {
        if (_loaded)
            return;

        Enums[info.hash_code()] = enumz;
    }
}