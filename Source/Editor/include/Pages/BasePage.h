//
// Created by bentoo on 10/24/16.
//

#ifndef GAME_BASEPAGE_H
#define GAME_BASEPAGE_H

#include <QFrame>
#include <QtWidgets/QHBoxLayout>

namespace Core
{
	class Engine;
}

namespace Editor
{
	class EditorApp;

	class BasePage : public QFrame
	{
		Q_OBJECT

		QFrame*  _contentFrame;
		QWidget* _contentWidget;

	protected:
		QHBoxLayout* _toolbarLayout;
		EditorApp* _editor;

		Core::Engine* engine();

		void setContent(QWidget* widget);
		QWidget* getContent();

	public:
		BasePage(EditorApp* editor, QWidget *parent = nullptr);
		virtual ~BasePage() {}
		
		void addButton(const char*, const char* = "");
	};
}

#endif //GAME_BASEPAGE_H