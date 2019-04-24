#ifndef YAGE_EDITOR_BASEPAGE_H
#define YAGE_EDITOR_BASEPAGE_H

#include "BasePage.h"
#include <QSurface>
#include <Utils/Handle.h>

namespace Core
{
	class Window;
}

namespace Editor
{
    class SceneView : public BasePage
    {
    public:
		SceneView(EditorApp* editor, QWidget* parent = nullptr);
		virtual ~SceneView();

	protected:
        QSurface* _surface;
		Utils::Handle<Core::Window> _window;
    };
}

#endif