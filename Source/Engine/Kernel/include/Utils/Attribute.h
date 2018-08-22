#ifndef YAGE_ATTRIBUTE_H
#define YAGE_ATTRIBUTE_H

#include "MacroDefines.h"

#ifdef ANNOTATION_GENERATOR
struct __Y_CLASS__
{
    __Y_CLASS__(const char* args)
    { }
};

#define YMeta(...) __attribute__((annotate(#__VA_ARGS__)))
#define YClass(...) __Y_CLASS__ YAGE_MAKE_UNIQUE(__y_class__)( #__VA_ARGS__ );
#else
#define YMeta(...)
#define YClass(...)
#endif

class Attribute
{
protected:
    Attribute() = default;
};

#endif // !YAGE_ATTRIBUTE_H
