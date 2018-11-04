#ifndef YAGE_EDITOR_H
#define YAGE_EDITOR_H

#include <Core/Engine.h>

namespace Editor
{
    class EditorWindow;

    class EditorApp
    {
    public:
         EditorApp();
        ~EditorApp();

        int Exec(int argc, char** args);

    private:
        EditorWindow* _MainWindow { nullptr };
        Core::Engine* _Engine { nullptr };
    };
}

#endif // YAGE_EDITOR_H