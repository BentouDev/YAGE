#include "EngineLayer.h"
#include "Core/RTTI/RTTIEngineLoader.h"

namespace RTTI
{
    bool EngineLayer::OnLoad()
    {
        // Call generated method
        Meta::Engine::RegisterClasses(*RTTI::GetRegister());
        return true;
    }
}