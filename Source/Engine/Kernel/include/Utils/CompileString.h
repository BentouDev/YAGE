#ifndef YAGE_COMPILE_STRING_H
#define YAGE_COMPILE_STRING_H

#include <cstdint>
#include <memory>

namespace Utils
{
    class CompileString
    {
        const char* data;

    public:
        template <std::size_t size>
        constexpr CompileString(const char (*str)[size])
        {
            data = (*str);
        }
        
        template <std::size_t size>
        constexpr CompileString(const char (&str)[size])
        {
            data = &(str)[0];
        }

        const char* c_str()
        {
            return data;
        }
    };
}

#endif // !YAGE_COMPILE_STRING_H