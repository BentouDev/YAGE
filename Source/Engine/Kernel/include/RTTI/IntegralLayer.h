#ifndef YAGE_RTTI_INTEGRAL_H
#define YAGE_RTTI_INTEGRAL_H

#include "RTTILayer.h"
#include <Utils/Defines.h>

namespace RTTI
{
    class YAGE_API IntegralLayer : public ILayer
    {
    protected:
        virtual bool OnLoad() override;
        virtual bool OnUnload() override;
    };
}

#endif //YAGE_RTTI_INTEGRAL_H