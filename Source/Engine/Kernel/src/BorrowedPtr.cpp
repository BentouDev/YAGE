#include "Utils/BorrowedPtr.h"

namespace Utils
{
    namespace detail
    {
        borrowed_ptr_base::borrowed_ptr_base(borrowed_ptr_base&& other)
            : _owner { other._owner }
        {
            _owner->registerBorrowerImpl(this);
			_owner->unregisterBorrower(&other);

			other._owner = nullptr;
        }

		borrowed_ptr_base& borrowed_ptr_base::operator=(borrowed_ptr_base&& other)
		{
			if (_owner)
				_owner->unregisterBorrower(this);
			
			_owner = other._owner;

			if (_owner)
			{
				_owner->registerBorrowerImpl(this);
				_owner->unregisterBorrower(&other);

				other._owner = nullptr;
			}

			return *this;
		}

        borrowed_ptr_base::~borrowed_ptr_base()
        {
            release();
        }

        void borrowed_ptr_base::release()
        {
            if (_owner)
                _owner->unregisterBorrower(this);
        }
    }
}