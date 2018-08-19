#ifndef YAGE_GAME_LIB_API_H
#define YAGE_GAME_LIB_API_H

namespace Core
{
    class IGameLib;
}

#if _WIN32
#ifdef GAME_LIB_EXPORTS
#define GAME_LIB_API __declspec(dllexport)
#else
#define GAME_LIB_API __declspec(dllimport)
#endif
#else
#define GAME_LIB_API
#endif

GAME_LIB_API Core::IGameLib* CreateGameLib();
GAME_LIB_API bool DestroyGameLib(Core::IGameLib*& ptr);

#endif // YAGE_GAME_LIB_API_H