#ifndef YAGE_DEFAULT_DELETER_H
#define YAGE_DEFAULT_DELETER_H

namespace yage
{
	template <typename T>
	void DefaultDeleter(void* ptr)
	{
		T* ptr_t = reinterpret_cast<T*>(ptr);
		delete ptr_t;
	}
}

#endif //YAGE_DEFAULT_DELETER_H