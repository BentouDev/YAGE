#ifndef YAGE_OWNED_PTR
#define YAGE_OWNED_PTR

#include <functional>
#include "DefaultBlock.h"
#include "BorrowedPtr.h"
#include "SafeDelete.h"
#include "Assert.h"
#include "List.h"

namespace Utils
{
    template <typename T>
    class borrowed_ptr;

    namespace detail
    {
        struct TOwnedPtrMemTag
        { };

        class borrowed_ptr_base;

        class owned_ptr_base
        {
        public:
            using TDeleter = std::function<void(void*)>;

        protected:
            template <typename T>
            friend class borrowed_ptr;
            friend class borrowed_ptr_base;

            List<borrowed_ptr_base*> _borrowers;
            TDeleter                 _deleter;

            void unregisterBorrower(borrowed_ptr_base* borrower);
            void registerBorrowerImpl(borrowed_ptr_base* newBorrower);

            owned_ptr_base(TDeleter&& deleter)
                : _borrowers(Memory::GetDefaultBlock<TOwnedPtrMemTag>())
                , _deleter(std::move(deleter))
            { }

            void destroy_type_erased(void* ptr);

        public:
            virtual ~owned_ptr_base()
            { }

            virtual bool hasBorrowers() const = 0;
            virtual bool destroy() = 0;
        };
    }

    /* Owns stored pointer.
       Frees stored pointer on destruction.
       Can borrow a weak borrowed_ptr instance.
    */
    template <typename T>
    class owned_ptr : public detail::owned_ptr_base
    {
        T* _pointer;

        template <typename X>
        friend class borrowed_ptr;

        void registerBorrower(borrowed_ptr<T>* borrower)
        {
            registerBorrowerImpl(borrower);
        }

        template <typename U>
        void registerBorrower(borrowed_ptr<U>* borrower)
        {
            static_assert(std::is_base_of<U, T>::value,
                "Cannot register borrowed pointer of incompatible class!");
            registerBorrowerImpl(borrower);
        }

    public:
        owned_ptr(TDeleter deleter)
            : _pointer(nullptr)
            , owned_ptr_base(std::move(deleter))
        { }

        owned_ptr(TDeleter deleter, T* ptr)
            : _pointer(ptr)
            , owned_ptr_base(std::move(deleter))
        { }

        ~owned_ptr()
        {
            YAGE_ASSERT(!hasBorrowers(), "Attempt to destroy owner_ptr with active borrowers!");

            destroy();
        }

        // Owned ptr is not copyable
        owned_ptr(const owned_ptr& ) = delete;

        // Owned ptr is not movable (not implemented for now, doable, but tricky)
        owned_ptr(owned_ptr&&) = delete;

        operator bool() const { return get() != nullptr; }

        T* operator->() { return get(); }

        T* operator->() const { return get(); }

        explicit operator T*() { return get(); }

        explicit operator T*() const { return get(); }

        T* get() const { return _pointer; }
        T* get() { return _pointer; }


        template <typename U>
        borrowed_ptr<U> borrow()
        {
            static_assert(std::is_base_of<U, T>::value, 
                "Cannot borrow a pointer of incompatible class!");
            return borrowed_ptr<U>(this);
        }

        borrowed_ptr<T> borrow()
        {
            return borrowed_ptr<T>(this);
        }

        virtual bool hasBorrowers() const override
        {
            return !_borrowers.empty();
        }

        bool reset(T* new_value = nullptr) noexcept
        {
            if (new_value != _pointer)
            {
                destroy();

                _pointer = new_value;
            }

            return static_cast<bool>(*this);
        }

        [[nodiscard]] auto release() noexcept -> T*
        {
            auto raw_value = _pointer;
            _pointer = nullptr;
            return raw_value;
        }

        virtual bool destroy() override
        {
            if (_pointer == nullptr)
                return false;

            destroy_type_erased(_pointer);
            _pointer = nullptr;

            return true;
        }

        static auto invalid() -> owned_ptr <T>
        {
            return owned_ptr<T>();
        }
    };

    template <typename T, typename ... TArgs>
    owned_ptr<T> make_owned(Memory::IMemoryBlock& block = Memory::GetDefaultBlock<T>(), TArgs&&... args)
    {
        return owned_ptr<T>
        (
            YAGE_CREATE_NEW(block, T)(std::forward<TArgs>(args)...), [&](void* ptr){
                Memory::Delete(block, static_cast<T*>(ptr));
            }
        );
    }

    template <typename T>
    owned_ptr<T> make_owned(Memory::IMemoryBlock& block = Memory::GetDefaultBlock<T>())
    {
        return owned_ptr<T>
        (
            YAGE_CREATE_NEW(block, T)(), [&](void* ptr){
                Memory::Delete(block, static_cast<T*>(ptr));
            }
        );
    }
}

#endif //YAGE_OWNED_PTR