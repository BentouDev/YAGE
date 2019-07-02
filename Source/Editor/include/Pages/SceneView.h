#ifndef YAGE_EDITOR_BASEPAGE_H
#define YAGE_EDITOR_BASEPAGE_H

#include "BasePage.h"

#include <Utils/Handle.h>
#include <Core/GameTime.h>

#include <QTimer>

namespace Core
{
	class Window;
}

namespace Editor
{
    class FramebufferWidget;

    class SceneView : public BasePage
    {
    public:
		SceneView(EditorApp* editor, QWidget* parent = nullptr);
		virtual ~SceneView();

	protected:
		void Render();

        FramebufferWidget* _engineSurface;
		Core::GameTime _gameTime;

		QTimer* _timer;
    };
}

#endif