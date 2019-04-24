#include "Pages/SceneView.h"
#include "Core/Engine.h"

namespace Editor
{
	SceneView::SceneView(EditorApp* editor, QWidget* parent)
		: BasePage(editor, parent)
	{
		_window = engine()->CreateWindowFromSurface(this->winId());
	}

	SceneView::~SceneView()
	{

	}
}