#include "Pages/SceneView.h"
#include "Core/Engine.h"
#include "Core/WindowManager.h"
#include "FramebufferWidget.h"

namespace Editor
{
    SceneView::SceneView(EditorApp* editor, QWidget* parent)
        : BasePage(editor, parent)
    {
    /*
        If you do not set an inner widget, the SubWindow itself IS
        a widget, so SDL renders over it... AND its controls. Not
        quite the desired behavior, unless you want to hide the
        controls until mouse over. But once the mouse is over, SDL
        writes over the controls again. Keep this in mind.
    */

        _engineSurface = new FramebufferWidget(_editor, this);

        _timer = new QTimer(this);
        connect(_timer, &QTimer::timeout, this, &SceneView::Render);
        _timer->start(1000 / 60);

        setContent(_engineSurface);
    }

    SceneView::~SceneView()
    {

    }

    void SceneView::Render()
    {
        _engineSurface->OnRender(_gameTime);
    }
}