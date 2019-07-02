#include "FramebufferWidget.h"
#include "Editor.h"
#include <Core/Engine.h>

namespace Editor
{
    FramebufferWidget::FramebufferWidget(EditorApp* editor, QWidget* parent)
        : QWidget(parent), _editor(editor)
    {
        //auto surface = engine().CreateOffscreenSurface();
    }

    FramebufferWidget::~FramebufferWidget()
    {
        //surface.destroy();
    }

    void FramebufferWidget::paintEvent(QPaintEvent* data)
    {
        
    }

    void FramebufferWidget::OnRender(Core::GameTime& time)
    {
        //engine().Draw(_framebuffer, time);
        //update();
    }

    const Core::Engine& FramebufferWidget::engine() const
    {
        return (*_editor->getEngine());
    }

    Core::Engine& FramebufferWidget::engine()
    {
        return (*_editor->getEngine());
    }
}