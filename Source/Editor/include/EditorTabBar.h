//
// Created by Bentoo on 2016-10-25.
//

#ifndef GAME_EDITORTABBAR_H
#define GAME_EDITORTABBAR_H

#include <QTabBar>
#include <QToolButton>

namespace Editor
{
	class EditorTabBar : public QTabBar
	{
		Q_OBJECT

	protected:
		void placePlusButton();

		virtual void tabLayoutChange() override;

		virtual void resizeEvent(QResizeEvent *event) override;

		QToolButton* plusButton;

	protected slots:
		void emitPlusButtonClicked();

	signals:
		void plusButtonClicked();

	public:
		explicit EditorTabBar(QWidget* parent = nullptr);

		virtual QSize minimumSizeHint() const override;

		virtual QSize sizeHint() const override;

		QToolButton* getPlusButton() const;
	};
}

#endif //GAME_EDITORTABBAR_H
