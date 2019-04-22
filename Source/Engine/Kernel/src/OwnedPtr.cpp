#include "Utils/OwnedPtr.h"

namespace Utils
{
    namespace detail
    {
		owned_ptr_base::~owned_ptr_base()
		{
			for (auto* ptr : _borrowers)
			{
				YAGE_ASSERT(ptr, "borrowed_ptr is null!");
				YAGE_ASSERT(ptr->_owner == this, "borrowed_ptr points to wrong parent [%p]", (void*) ptr->_owner);
				ptr->_owner = nullptr;
			}

			_borrowers.clear();
		}

        void owned_ptr_base::destroy_type_erased(void* ptr)
        {
            _deleter(ptr);
        }

        void owned_ptr_base::registerBorrowerImpl(detail::borrowed_ptr_base* borrower)
        {
            auto itr = std::find_if(_borrowers.begin(), _borrowers.end(), 
                [=](const detail::borrowed_ptr_base* index){
                    return index == borrower;
            });

            if (itr == _borrowers.end())
                _borrowers.push_back(borrower);
        }

        void owned_ptr_base::unregisterBorrower(detail::borrowed_ptr_base* borrower)
        {
            for (int i = 0; i < _borrowers.size(); i++)
            {
                if (_borrowers[i] == borrower)
                {
					_borrowers.erase(_borrowers.begin() + i);
                    borrower->_owner = nullptr;
                    break;
                }
            }
        }
    }
}