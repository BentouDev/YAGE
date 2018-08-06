#ifndef PROJECT_MANAGER_IMAGE_WINDOW_H
#define PROJECT_MANAGER_IMAGE_WINDOW_H

#include <QQuickImageProvider>
#include <QApplication>
#include <QStyle>
#include <string>

class StandardIconProvider : public QQuickImageProvider
{
    QMetaEnum meta;

public:
    StandardIconProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
    {
        meta = QMetaEnum::fromType<QStyle::StandardPixmap>();
    }

    inline QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        QStyle::StandardPixmap pixmap = (QStyle::StandardPixmap) meta.keyToValue(id.toStdString().c_str());
        auto icon = QApplication::style()->standardIcon(pixmap);

        if (requestedSize.width() == -1 || requestedSize.height() == 1)
            return icon.pixmap(QSize(32, 32));

        return icon.pixmap(requestedSize);
    }
};

#endif // PROJECT_MANAGER_IMAGE_WINDOW_H