//
// Created by mrjaqbq on 13.05.16.
//

#ifndef GAME_BORROWEDPTR_H
#define GAME_BORROWEDPTR_H

#include <algorithm>
#include "Defines.h"
#include "OwnedPtr.h"
#include "Assert.h"

namespace Utils
{
	template <typename T>
	class owned_ptr;

	namespace detail
    {
		class owned_ptr_base;

        class borrowed_ptr_base
        {
        protected:
            owned_ptr_base* _owner;

            borrowed_ptr_base()
                : _owner { nullptr }
            { }

            borrowed_ptr_base(owned_ptr_base* owner)
                : _owner { owner }
            { }

            borrowed_ptr_base(borrowed_ptr_base&& other);

			borrowed_ptr_base& operator=(borrowed_ptr_base&& other);

        public:
            virtual ~borrowed_ptr_base();

            void release();

            template <typename T>
            friend class owned_ptr;
            friend class owned_ptr_base;
        };
    }

    template <typename T>
    class borrowed_ptr : public detail::borrowed_ptr_base
    {
        bool isDerived = false;

        template <typename X>
        friend class owned_ptr;

        explicit borrowed_ptr(owned_ptr<T>* owner) noexcept
            : borrowed_ptr_base (owner)
        { 
            owner->registerBorrower(this);
        }

        template <typename U>
        explicit borrowed_ptr(owned_ptr<U>* owner) noexcept
            : borrowed_ptr_base (owner)
        {
            static_assert(std::is_base_of<T, U>::value, 
                "Cannot create borrowed pointer from incompatible class!");

            owner->template registerBorrower<T>(this);
            isDerived = true;
        }

	public:
        borrowed_ptr()
        { }

		// TODO: Should borrowed_ptr get owner on move?
        borrowed_ptr(borrowed_ptr && other) noexcept
			: isDerived{ other.isDerived }
			, borrowed_ptr_base{ std::move(other) }
        { }

        virtual ~borrowed_ptr()
        { }

        // Disable copy constructor
        borrowed_ptr(const borrowed_ptr& other) = delete;

		// Disable move assignment
		borrowed_ptr& operator=(borrowed_ptr&& other)
		{
			if (this != &other)
			{
				borrowed_ptr_base::operator=(std::move(other));

				isDerived = other.isDerived;
				
				/*
				YAGE_ASSERT(_owner == other._owner, "Cannot move between owners!");
				if (_owner)
				{
					borrowed_ptr::borrowed_ptr(std::move(other));
				}*/
			}
			return *this;
		}

		// Disable copy assignment
		borrowed_ptr& operator=(const borrowed_ptr & other) = delete;

        explicit operator bool() const noexcept
        {
            return _owner != nullptr && get() != nullptr;
        }

        auto operator->() noexcept -> T*
        { return get(); }

        auto operator->() const noexcept -> T*
        { return get(); }

        auto operator*() noexcept -> T*
        { return get(); }

        auto operator*() const noexcept -> T*
        { return get(); }

        auto get() const noexcept -> T*
        {
            YAGE_ASSERT(_owner, "Attempt to use unset borrowed_ptr!");
            return static_cast<owned_ptr<T>*>(_owner)->get();
        }

        owned_ptr<T>* getOwner()
        {
            YAGE_ASSERT(_owner, "Attempt to use unset borrowed_ptr!");
            if (!isDerived)
                return static_cast<owned_ptr<T>*>(_owner);
            return nullptr;
        }

        detail::owned_ptr_base* getBaseOwner()
        {
            YAGE_ASSERT(_owner, "Attempt to use unset borrowed_ptr!");
            return _owner;
        }
    };
}

#endif //GAME_BORROWEDPTR_H
