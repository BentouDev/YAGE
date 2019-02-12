#include "Utils/BorrowedPtr.h"

namespace Utils
{
    namespace detail
    {
        borrowed_ptr_base::borrowed_ptr_base(borrowed_ptr_base&& other)
            : _owner { other._owner }
        {
            _owner->registerBorrowerImpl(this);
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