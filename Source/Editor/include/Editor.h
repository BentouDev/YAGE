#ifndef YAGE_EDITOR_H
#define YAGE_EDITOR_H

namespace Core
{
	class Engine;
}

namespace Editor
{
    class EditorWindow;

    class EditorApp
    {
    public:
         EditorApp();
        ~EditorApp();

        int Exec(int argc, char** args);

		Core::Engine* getEngine() { return _Engine; }

    private:
        EditorWindow* _MainWindow { nullptr };
		Core::Engine* _Engine { nullptr };
    };
}

#endif // YAGE_EDITOR_H