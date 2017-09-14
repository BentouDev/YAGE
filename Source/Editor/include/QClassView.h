//
// Created by Bentoo on 2017-04-01.
//

#ifndef VIDYAGAEM_QCLASSVIEW_H
#define VIDYAGAEM_QCLASSVIEW_H

#include <QtWidgets/QFrame>
#include "QProperty.h"

namespace Editor
{
	template <typename T>
	class QClassView : public QFrame
	{
		T* _instance;

		QList<Editor::QProperty*> _properties;

	public:
		explicit QClassView(QWidget* parent = nullptr)
			: QFrame(parent)
		{

		}

		void setInstance(T* instance)
		{
			_instance = instance;
			for(auto* prop : _properties)
			{
				prop->setInstance(_instance);
			}
		}
	};
}

#endif //VIDYAGAEM_QCLASSVIEW_H
