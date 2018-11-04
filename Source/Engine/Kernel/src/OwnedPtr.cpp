#include "Utils/OwnedPtr.h"

namespace Utils
{
    namespace detail
    {
        void owned_ptr_base::registerBorrowerImpl(detail::borrowed_ptr_base* borrower)
        {
            auto itr = std::find_if(_borrowers.begin(), _borrowers.end(), 
                [=](const detail::borrowed_ptr_base* index){
                    return index == borrower;
            });

            if (itr == _borrowers.end())
                _borrowers.add(borrower);
        }

        void owned_ptr_base::unregisterBorrower(detail::borrowed_ptr_base* borrower)
        {
            for (int i = 0; i < _borrowers.size(); i++)
            {
                if (_borrowers[i] == borrower)
                {
                    _borrowers.eraseAt(i);
                    borrower->_owner = nullptr;
                    break;
                }
            }
        }
    }
}