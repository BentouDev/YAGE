#ifndef YAGE_EDITOR_H
#define YAGE_EDITOR_H

namespace Core
{
    class Engine;
}

namespace Editor
{
    class EditorWindow;

    class Editor
    {
    public:
         Editor();
        ~Editor();

    private:
        EditorWindow* _MainWindow { nullptr };
        Core::Engine* _Engine { nullptr };
    };
}

#endif // YAGE_EDITOR_H