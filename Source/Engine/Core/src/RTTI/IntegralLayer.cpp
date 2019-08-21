#include "Core/RTTI/RTTIIntegralLoader.h"
#include "RTTI/ClassResolver.h"
#include "Platform/Window.h"
#include "Gfx/Graphics/Viewport.h"

#define YAGE_DEFINE_INTEGRAL_RTTI_IMPL(clazz, name) \
        template<> \
        struct ::Meta::detail::define_rtti<clazz> { \
            define_rtti() { ::Meta::detail::GetClass<clazz>(); } \
        }; \
        static const ::Meta::detail::define_rtti<clazz> name{}

#define YAGE_DEFINE_INTEGRAL_RTTI(clazz) YAGE_DEFINE_INTEGRAL_RTTI_IMPL( clazz, YAGE_MAKE_UNIQUE( rtti_def_ ) )

namespace Meta
{
    template <>
    void ClassResolver::Declare<Core::Window>(RTTI::Register& reg, RTTI::ClassInfo& data)
    {
        data.CanonicalName = "Core::Window";
    }

    template <>
    void ClassResolver::Define<Core::Window>(RTTI::Register& reg, RTTI::ClassInfo& data)
    { }

    template <>
    void ClassResolver::Declare<Gfx::Viewport>(RTTI::Register& reg, RTTI::ClassInfo& data)
    {
        data.CanonicalName = "Gfx::Viewport";
    }

    template <>
    void ClassResolver::Define<Gfx::Viewport>(RTTI::Register& reg, RTTI::ClassInfo& data)
    { }
}

namespace yage::kernel
{
    void RegisterClasses(RTTI::IRegister& reg)
    {
        //// int
        //::RTTI::RegisterType<int>("int", reg);
        //// unsigned
        //::RTTI::RegisterType<unsigned>("unsigned", reg);
        //// float
        //::RTTI::RegisterType<float>("float", reg);
        //// double
        //::RTTI::RegisterType<double>("double", reg);
        //// char
        //::RTTI::RegisterType<char>("char", reg);
        //// bool
        //::RTTI::RegisterType<bool>("bool", reg);
        //// size_t
        //::RTTI::RegisterType<size_t>("size_t", reg);

        // Window
        ::RTTI::RegisterType<typename Core::Window>(Utils::CompileString("Core::Window"), reg);

        // Viewport
        ::RTTI::RegisterType<typename Gfx::Viewport>(Utils::CompileString("Gfx::Viewport"), reg);
    }
}

PUBLISH_RTTI_LAYER(RTTI::IntegralLayer, &yage::kernel::RegisterClasses);