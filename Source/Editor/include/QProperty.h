//
// Created by Bentoo on 2017-04-01.
//

#ifndef VIDYAGAEM_QPROPERTY_H
#define VIDYAGAEM_QPROPERTY_H

#include <QtCore/QObject>
#include <Utils/Assert.h>

#define DECL_QPROPERTY(Type, Member) QProperty<Type, decltype(Type::Member), &Type::Member>

namespace Editor
{
	template <typename T>
	class QBaseProperty : public QObject
	{
	protected:
		T* _instance;

	public:
		explicit QBaseProperty(QObject* parent)
			: QObject(parent)
		{ }

		inline void setInstance(T* instance)
		{
			_instance = instance;
		}

		inline T* getInstance() const
		{
			return _instance;
		}
	};

	template <typename T, typename V, V T::*memberptr_t>
	class QProperty : public QBaseProperty<T>
	{
		Q_PROPERTY(V Value WRITE setValue READ getValue)

	public:
		explicit QProperty(T* instance = nullptr, QObject* parent = nullptr)
			: QBaseProperty(parent), _instance(instance)
		{ }

		inline void setValue(V value)
		{
			YAGE_ASSERT(_instance != nullptr, "Cannot set QProperty when instance is null!");
			(_instance->*memberptr_t) = value;
		}

		inline V getValue() const
		{
			YAGE_ASSERT(_instance != nullptr, "Cannot get from QProperty when instance is null!");
			return (_instance->*memberptr_t);
		}
	};
}

#endif //VIDYAGAEM_QPROPERTY_H
