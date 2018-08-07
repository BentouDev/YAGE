#ifndef PROJECT_QML_FRAME_H
#define PROJECT_QML_FRAME_H

#include <QFrame>

class QQuickView;
class QQmlEngine;

class QmlFrame : public QFrame
{
public:
    explicit QmlFrame(const QString& source, QWidget* parent = nullptr);
    virtual ~QmlFrame();

    void SetSource(const QString& source);
    void Reload();
    auto GetEngine() -> QQmlEngine*;

private:

    QString     _CurrentSource;

    QQuickView* _View;
    QWidget*    _ViewWidget;
};

#endif // PROJECT_QML_FRAME_H