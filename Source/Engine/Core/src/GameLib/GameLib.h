#ifndef YAGE_GAME_LIB_H
#define YAGE_GAME_LIB_H

namespace Core
{
    class IDynamicLibrary;

    class GameLib // Ref counted
    {
    public:
        IDynamicLibrary* _library;
    };
}

#endif // !YAGE_GAME_LIB_H