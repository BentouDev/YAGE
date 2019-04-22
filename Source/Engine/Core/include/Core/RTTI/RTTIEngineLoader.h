#ifndef YAGE_RTTI_ENGINE_LOADER_H
#define YAGE_RTTI_ENGINE_LOADER_H

#include <RTTI/RTTIRegister.h>
#include <RTTI/BaseLayer.h>
#include <RTTI/ClassResolver.h>

//namespace detail
//{
//	void _register(BaseLayer* instance)
//	{
//		static int _layer_index = 0;
//		static std::array<BaseLayer*, 255> layer_stack;
//		layer_stack[_layer_index] = instance;
//	}
//}

//template <typename TLayer>
//struct LayerTypeGuard
//{
//	static constexpr int _counter{ 0 };
//};

namespace RTTI
{
	class EngineLayer : public BaseLayer {
	public:
		virtual bool OnLoad() override {
			YAGE_ASSERT(false, "Layer not implemented!");
			return false;
		}
	};
}

using TProcSign = void(RTTI::IRegister& rtti);
template <typename TLayer>
class LayerHelper
{
	static TLayer* _instance;
	//static_assert(++LayerTypeGuard<TLayer>::_counter == 1, "Cannot have more than one instance od layer!");

public:
	constexpr LayerHelper(TProcSign* TProcPtr)
	{
		static class impl : public TLayer {
		public:
			TProcSign* _ptr;
			impl(TProcSign* ptr) : _ptr{ ptr } {}
			virtual bool OnLoad() override
			{
				_ptr(*RTTI::GetRegister());
				return true;
			}
		} static_instance{ TProcPtr };
		_instance = &static_instance;
	}

	static TLayer* getInstance()
	{
		return _instance;
	}
};

#define IMPLEMENT_RTTI_LAYER( clazz, proc_ptr ) \
	__IMPLEMENT_RTTI_LAYER_INTERNAL ( YAGE_MAKE_UNIQUE(layer_helper), clazz, proc_ptr )

#define __IMPLEMENT_RTTI_LAYER_INTERNAL( clazz_name, clazz, proc_ptr ) \
	LayerHelper<clazz> clazz_name {proc_ptr};

template <typename T>
T* LayerHelper<T>::_instance{ nullptr };

	/*template <> BaseLayer* Meta::detail::get_layer<clazz>() { \
		static class clazz_name : public clazz { \
		public:
			
		} _instance; \
		return &_instance; \
	}*/
	/*class clazz_name : public clazz { \
		constexpr clazz_name() { \
			detail::_register(this); \
		} \
	public: \
		bool OnLoad() { return proc_ptr(); } \
	};*/

//#if _WIN32
//#ifdef YAGE_API
//#define ENGINE_CTTI_API __declspec(dllimport)
//#else
//#define ENGINE_CTTI_API __declspec(dllexport)
//#endif
//#else
//#define ENGINE_CTT_API
//#endif
//
//namespace Meta
//{
//    namespace Engine
//    {
//        ENGINE_CTTI_API void RegisterClasses(RTTI::IRegister& storage);
//    }
//}

#endif // YAGE_RTTI_ENGINE_LOADER_H