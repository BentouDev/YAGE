#ifndef YAGE_EDITOR_FRAMEBUFFER_WIDGET_H
#define YAGE_EDITOR_FRAMEBUFFER_WIDGET_H

#include <QWidget>

namespace Core
{
    class Engine;
    struct GameTime;
}

namespace Editor
{
    class EditorApp;

    class FramebufferWidget : public QWidget
    {
        Q_OBJECT

        EditorApp* _editor;

    public:
        FramebufferWidget(EditorApp* editor, QWidget* parent = nullptr);
        ~FramebufferWidget();

        void paintEvent(QPaintEvent* data);
        void OnRender(Core::GameTime& time);

        Core::Engine& engine();
        const Core::Engine& engine() const;
    };
}

#endif//YAGE_EDITOR_FRAMEBUFFER_WIDGET_H