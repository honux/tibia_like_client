/********************************************************************//**
  @class utils::BufferForType
  Returns a buffer type that is able to store a value of the given type.

  @class utils::ConstRef
  Selects between type by value or by reference for optimal parameter passing.
  ConstRef<T>::Type is T when T is not a class type, T const& otherwise. 

  @class utils::EnableIf
  Allows a template function overload to be enabled or disabled based on a condition.

  @class utils::EnableParameterIf
  Allows an overloaded function parameter to be enabled or disabled based on a condition.

  @class utils::DisableIf
  Allows a template function overload to be enabled or disabled based on a condition.

  @class utils::DisableParameterIf
  Allows an overloaded function parameter to be enabled or disabled based on a condition.

  @class utils::IfThenElse
  Yields second or third argument depending on first argument.

  @class utils::IsClassType
  Tests whether the type is a class type.
  IsClassType<T>::Yes is true when the type is a class, struct or union.

  @class utils::IsVoidType
  Tests whether the type is void.
  IsVoidType<T>::Yes is true when the type is void.

  @class utils::NonConst
  Removes a const qualifier from the type.
*************************************************************************/

#pragma once

namespace utils
{
    /************************************************************************
      BufferForType Template Class Declaration
    *************************************************************************/

    // Primary template
    template<typename T>
    class BufferForType
    {
    public:

        typedef char Type[sizeof(T)];
    };

    // Specialization for void type
    template<>
    class BufferForType<void>
    {
    public:

        typedef char Type[1];
    };

    /************************************************************************
      EnableIf Template Class Declaration
    *************************************************************************/

    template<bool C, typename T = void>
    class EnableIf
    {
    public:

        typedef T Type;
    };

    template<typename T>
    class EnableIf<false, T>
    {
    };

    /************************************************************************
      EnableParameterIf Template Class Declaration
    *************************************************************************/

    template<bool C, typename T>
    class EnableParameterIf
    {
    public:

        typedef T Type;
    };

    template<typename T>
    class EnableParameterIf<false, T>
    {
        struct InvalidParameter 
        {
            operator T ();
            operator T () const;
        };

    public:

        typedef InvalidParameter Type;
    };

    /************************************************************************
      DisableIf Template Class Declaration
    *************************************************************************/

    template<bool C, typename T = void>
    class DisableIf
    {
    public:

        typedef T Type;
    };

    template<typename T>
    class DisableIf<true, T>
    {
    };

    /************************************************************************
      DisableParameterIf Template Class Declaration
    *************************************************************************/

    template<bool C, typename T>
    class DisableParameterIf
    {
    public:

        typedef T Type;
    };

    template<typename T>
    class DisableParameterIf<true, T>
    {
        struct InvalidParameter 
        {
            operator T ();
            operator T () const;
        };

    public:

        typedef InvalidParameter Type;
    };

    /************************************************************************
      IfThenElse Template Class Declaration
    *************************************************************************/

    // Primary template: yield second or third argument depending on first argument
    template<bool C, typename T1, typename T2>
    class IfThenElse;

    // Partial specialization: true yields second argument
    template<typename T1, typename T2>
    class IfThenElse<true, T1, T2> 
    {
    public:
        typedef T1 ResultType;
    };

    // Partial specialization: false yields third argument
    template<typename T1, typename T2>
    class IfThenElse<false, T1, T2> 
    {
    public:
        typedef T2 ResultType;
    };


    /************************************************************************
      IsClassType Template Class Declaration
    *************************************************************************/

    // SFINAE (substitution-failure-is-not-an-error) principle
    template<typename T>
    class IsClassType 
    {
    private:

        typedef char One;
        typedef short Two;
        template<typename C> static One Test (int C::*);
        template<typename C> static Two Test (...);

    public:

        enum { Yes = sizeof(Test<T>(0)) == 1 };
        enum { No = !Yes };
    };

    /************************************************************************
      IsVoidType Template Class Declaration
    *************************************************************************/

    // Primary template
    template<typename T> 
    class IsVoidType 
    { 
    public: 
        enum 
        {
            Yes = 0,
            No = 1
        }; 
    };

    // Specialization for void type
    template<>
    class IsVoidType<void>
    {
    public: 
        enum 
        {
            Yes = 1,
            No = 0
        }; 
    };

    /************************************************************************
      IsIntegralType Template Class Declaration
    *************************************************************************/

    // Primary template
    template<typename T>
    class IsIntegralType
    {
    public:
        enum
        {
            Yes = 0,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<char>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<signed char>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<unsigned char>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<wchar_t>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<signed short>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<unsigned short>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<signed int>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<unsigned int>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<signed long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<unsigned long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<signed long long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsIntegralType<unsigned long long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    /************************************************************************
      IsSignedType Template Class Declaration
    *************************************************************************/

    template<typename T>
    class IsSignedType
    {
    public:
        enum
        {
            Yes = 0,
            No = !Yes
        };
    };

    template<>
    class IsSignedType<char>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsSignedType<signed char>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsSignedType<short>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsSignedType<int>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsSignedType<long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsSignedType<long long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    /************************************************************************
      IsUnsignedType Template Class Declaration
    *************************************************************************/

    template<typename T>
    class IsUnsignedType
    {
    public:
        enum
        {
            Yes = 0,
            No = !Yes
        };
    };

    template<>
    class IsUnsignedType<unsigned char>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsUnsignedType<unsigned short>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsUnsignedType<unsigned int>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsUnsignedType<unsigned long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };

    template<>
    class IsUnsignedType<unsigned long long>
    {
    public:
        enum
        {
            Yes = 1,
            No = !Yes
        };
    };


    /************************************************************************
      ConstRef Template Class Declaration
    *************************************************************************/

    template<typename T>
    class ConstRef 
    {
    public:

        typedef typename IfThenElse< IsClassType<T>::No, T, T const& >::ResultType Type;
    };

    /************************************************************************
      Identity Template Class Declaration
    *************************************************************************/

    template<typename T>
    class Identity
    {
    public:

        typedef T Type;
    };

    // Forward value with explicit template argument
    template<typename T>
    inline T&& Forward (typename Identity<T>::Type& t)
    {
        return static_cast<T&&>(t);
    }

    /************************************************************************
      NonConst Template Class Declaration
    *************************************************************************/

    template<typename T>
    class NonConst
    {
    public:

        typedef T Type;
    };

    template<typename T>
    class NonConst<T const>
    {
    public:

        typedef T Type;
    };

    template<typename T>
    class NonConst<T const*>
    {
    public:

        typedef T* Type;
    };

    template<typename T>
    class NonConst<T const&>
    {
    public:

        typedef T& Type;
    };

    template<typename T>
    class NonConst<T const&&>
    {
    public:

        typedef T&& Type;
    };

    /************************************************************************
      RemoveReference Template Class Declaration
    *************************************************************************/
    
    // Non-reference type
    template<typename T>
    class RemoveReference
    {
    public:

        typedef T Type;
    };

    // Lvalue reference type
    template<typename T>
    class RemoveReference<T&>
    {
    public:

        typedef T Type;
    };

    // Rvalue reference type
    template<typename T>
    class RemoveReference<T&&>
    {
    public:

        typedef T Type;
    };

    // Transform any value into an rvalue
    template<typename T>
    inline typename RemoveReference<T>::Type&& Move (T&& t)
    {
        return static_cast<typename RemoveReference<T>::Type&&>(t);
    }
}
