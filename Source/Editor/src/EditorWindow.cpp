//
// Created by bentoo on 10/24/16.
//
#include <Utils/SafeDelete.h>
#include <QMouseEvent>
#include <QDockWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTabBar>
#include <QLabel>
#include <QToolButton>

#include "EditorWindow.h"
#include "EditorTabWidget.h"
#include "BasePage.h"

#include "ui_EditorWindow.h"

#ifdef Q_OS_WIN
#include <QtWin>
#endif

// #define ADS_IMPORT
#include <ads/API.h>
#include <ads/ContainerWidget.h>
#include <ads/SectionContent.h>
#include <ads/SectionWidget.h>
#include <ads/DropOverlay.h>

#include "icontitlewidget.h"

namespace Editor
{
	static int CONTENT_COUNT = 0;

	static ADS_NS::SectionContent::RefPtr createLongTextLabelSC(ADS_NS::ContainerWidget* container)
	{
		QWidget* w = new QWidget();
		QBoxLayout* bl = new QBoxLayout(QBoxLayout::TopToBottom);
		w->setLayout(bl);

		QLabel* l = new QLabel();
		l->setWordWrap(true);
		l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
		l->setText(QString("Lorem Ipsum ist ein einfacher Demo-Text für die Print- und Schriftindustrie. Lorem Ipsum ist in der Industrie bereits der Standard Demo-Text seit 1500, als ein unbekannter Schriftsteller eine Hand voll Wörter nahm und diese durcheinander warf um ein Musterbuch zu erstellen. Es hat nicht nur 5 Jahrhunderte überlebt, sondern auch in Spruch in die elektronische Schriftbearbeitung geschafft (bemerke, nahezu unverändert). Bekannt wurde es 1960, mit dem erscheinen von Letrase, welches Passagen von Lorem Ipsum enhielt, so wie Desktop Software wie Aldus PageMaker - ebenfalls mit Lorem Ipsum."));
		bl->addWidget(l);

		const int index = ++CONTENT_COUNT;
		ADS_NS::SectionContent::RefPtr sc = ADS_NS::SectionContent::newSectionContent(QString("uname-%1").arg(index), container, new IconTitleWidget(QIcon(), QString("Label %1").arg(index)), w);
		sc->setTitle("Ein Label " + QString::number(index));
		return sc;
	}

	static ADS_NS::SectionContent::RefPtr createBasePage(ADS_NS::ContainerWidget* container)
	{
		QWidget* w = new QWidget();
		QBoxLayout* bl = new QBoxLayout(QBoxLayout::TopToBottom);
		w->setLayout(bl);

		BasePage* p = new BasePage();
		bl->addWidget(p);

		const int index = ++CONTENT_COUNT;
		ADS_NS::SectionContent::RefPtr sc = ADS_NS::SectionContent::newSectionContent(QString("uname-%1").arg(index), container, new IconTitleWidget(QIcon(), QString("Label %1").arg(index)), w);
		sc->setTitle("Ein Label " + QString::number(index));
		return sc;
	}

	EditorWindow::EditorWindow(QWidget *parent) :
			QMainWindow(parent),
			_ui(new Ui::EditorWindow)
	{
// #ifdef Q_OS_WIN
// 		// Transparent background for windows
// 		this->setAttribute(Qt::WA_TranslucentBackground);
// 	    QtWin::enableBlurBehindWindow(this);
//     	QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
// #endif
		_ui->setupUi(this);

		/*QLayout* layout = _ui->centralWidget->layout();

		_tabWidget = new EditorTabWidget(_ui->centralWidget);

		layout->addWidget(_tabWidget);
		layout->setMargin(0);
		layout->setSpacing(0);

		addTabPage();

		connect(_tabWidget, SIGNAL(plusButtonClicked()), this, SLOT(onAddButtonClick()));
		connect(_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTabButtonClick(int)));*/
		
		ADS_NS::ContainerWidget* _container = new ADS_NS::ContainerWidget();
		setCentralWidget(_container);
	
		connect(_container, &ADS_NS::ContainerWidget::activeTabChanged, [this](const ADS_NS::SectionContent::RefPtr& sc, bool active){
			Q_UNUSED(active);
			IconTitleWidget* itw = dynamic_cast<IconTitleWidget*>(sc->titleWidget());
			if (itw)
			{
				itw->polishUpdate();
			}
		});
		connect(_container, &ADS_NS::ContainerWidget::sectionContentVisibilityChanged, [](const ADS_NS::SectionContent::RefPtr& sc, bool visible){
			qDebug() << Q_FUNC_INFO << sc->uniqueName() << visible;
		});

		ADS_NS::ContainerWidget* cw = _container;
		ADS_NS::SectionWidget* sw = NULL;

		sw = _container->addSectionContent(createLongTextLabelSC(cw), sw, ADS_NS::CenterDropArea);
		sw = _container->addSectionContent(createLongTextLabelSC(cw), sw, ADS_NS::CenterDropArea);
		sw = _container->addSectionContent(createLongTextLabelSC(cw), sw, ADS_NS::CenterDropArea);
		sw = _container->addSectionContent(createBasePage(cw), sw, ADS_NS::CenterDropArea);
		sw = _container->addSectionContent(createBasePage(cw), sw, ADS_NS::CenterDropArea);
	}

	void EditorWindow::onAddButtonClick()
	{
		addTabPage();
	}

	void EditorWindow::onCloseTabButtonClick(int index)
	{
		_tabWidget->removeTab(index);

		if(_tabWidget->count() == 0)
		{
			close();
		}
	}

	void EditorWindow::addTabPage(const char* name)
	{
		BasePage* page = new BasePage(this);
		_tabWidget->addTab(page, name);
	}

	EditorWindow::~EditorWindow()
	{
		Memory::SafeDelete(_ui);
	}

	void EditorWindow::on_actionExit_triggered()
	{
		close();
	}
}
