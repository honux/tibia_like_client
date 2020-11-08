/********************************************************************//**
  @class utils::Functor
  Stores a pointer-to-function or pointer-to-member with the associated
  call arguments.

  @class utils::Function
  Encapsulates a functor object and manages its lifetime automatically.
*************************************************************************/

#pragma once

#include "types.h"
#include "traits.h"

#include <cassert>
#include <cstdarg>
#include <new>

#define _IDENTITY(T)   typename Identity<T>::Type

namespace utils
{
    #pragma warning(disable:4793) // message about /clr vararg support

    /************************************************************************
      Functor Class Declaration
    *************************************************************************/
    class Functor
    {
        template<typename Result> friend class Function;

    public:

        Functor ()
        : m_refCount(0)
        {
        }

        virtual ~Functor ()
        {
        }

        /// 
        /// Calls the functor.
        /// 
        virtual void Call () = 0;

        /// 
        /// Calls the functor and gets the value returned.
        /// @param[out] result Pointer that will receive the value.
        /// 
        virtual void CallAndGetResult (void* result)
        {
            int FunctorHasNoReturnValue = 0;
            assert(FunctorHasNoReturnValue);
        }

        /// 
        /// Changes a parameter of the functor.
        /// @param[in] param One-based index of the parameter, or zero to set the member instance.
        /// @param[in] ... Argument to be set.
        /// 
        virtual void SetParameter (uint param, ...)
        {
            int FunctorHasNoParameters = 0;
            assert(FunctorHasNoParameters);
        }

        /// 
        /// Calls the functor.
        /// 
        void operator() ()
        {
            Call();
        }

    protected:

        unsigned int m_refCount;
    };

    /************************************************************************
      Function Class Declaration
    *************************************************************************/
    template<typename Result = void>
    class Function
    {
    public:

        Function (Functor* functor)
        : m_functor(functor)
        {
            m_functor->m_refCount++;
        }

        Function (Function& copy)
        {
            m_functor = copy.m_functor;
            m_functor->m_refCount++;
        }

        ~Function ()
        {
            m_functor->m_refCount--;
            if (m_functor->m_refCount == 0)
            {
                delete m_functor;
            }
        }

        Function& operator= (Function const& function)
        {
            if (&function == this)
            {
                return (*this);
            }

            m_functor->m_refCount--;
            if (m_functor->m_refCount == 0)
            {
                delete m_functor;
            }

            m_functor = function.m_functor;
            m_functor->m_refCount++;
            return (*this);
        }

        Result operator() ()
        {
            Result result; // must have default constructor.
            m_functor->CallAndGetResult(&result);
            return result;
        }

    protected:

        Functor* m_functor;
    };

    template<>
    inline void Function<void>::operator() ()
    {
        m_functor->Call();
    }

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result>
    class CdeclFunctor0 : public Functor
    {
    public:

        typedef typename Result(__cdecl *Callback)();

        explicit CdeclFunctor0 (Callback callback)
        : m_callback(callback)
        {
        }

        virtual void Call ()
        {
            m_callback();
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback();
        }

    protected:

        Callback m_callback;
    };

    template<>
    class CdeclFunctor0<void> : public Functor
    {
    public:

        typedef void(__cdecl *Callback)();

        explicit CdeclFunctor0 (Callback callback)
            : m_callback(callback)
        {
        }

        virtual void Call ()
        {
            m_callback();
        }

    protected:

        Callback m_callback;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result>
    class StdcallFunctor0 : public Functor
    {
    public:

        typedef typename Result(__stdcall *Callback)();

        explicit StdcallFunctor0 (Callback callback)
        : m_callback(callback)
        {
        }

        virtual void Call ()
        {
            m_callback();
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback();
        }

    protected:

        Callback m_callback;
    };

    template<>
    class StdcallFunctor0<void> : public Functor
    {
    public:

        typedef void(__stdcall *Callback)();

        explicit StdcallFunctor0 (Callback callback)
        : m_callback(callback)
        {
        }

        virtual void Call ()
        {
            m_callback();
        }

    protected:

        Callback m_callback;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1>
    class CdeclFunctor1 : public Functor
    {
    public:

        typedef typename Result(__cdecl *Callback)(T1);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;

        explicit CdeclFunctor1 (Callback callback, ConstReferenceT1 t1)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1);
        }
        
        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param == 1);

            va_list args;
            va_start(args, param);

            m_param1 = va_arg(args, NonConstValueT1);
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
    };

    template<typename T1>
    class CdeclFunctor1<void, T1> : public Functor
    {
    public:

        typedef typename void(__cdecl *Callback)(T1);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;

        explicit CdeclFunctor1 (Callback callback, ConstReferenceT1 t1)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1)
        {
        }

        virtual void Call ()
        {
            m_callback((T1)(m_param1));
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param == 1);

            va_list args;
            va_start(args, param);

            m_param1 = va_arg(args, NonConstValueT1);
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1>
    class StdcallFunctor1 : public Functor
    {
    public:

        typedef typename Result(__stdcall *Callback)(T1);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;

        explicit StdcallFunctor1 (Callback callback, ConstReferenceT1 t1)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param == 1);

            va_list args;
            va_start(args, param);

            m_param1 = va_arg(args, NonConstValueT1);
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
    };

    template<typename T1>
    class StdcallFunctor1<void, T1> : public Functor
    {
    public:

        typedef typename void(__stdcall *Callback)(T1);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;

        explicit StdcallFunctor1 (Callback callback, ConstReferenceT1 t1)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param == 1);

            va_list args;
            va_start(args, param);

            m_param1 = va_arg(args, NonConstValueT1);
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2>
    class CdeclFunctor2 : public Functor
    {
    public:

        typedef typename Result(__cdecl *Callback)(T1, T2);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;

        explicit CdeclFunctor2 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2)
        : m_callback(callback), 
          m_param1((NonConstValueT1)t1), 
          m_param2((NonConstValueT2)t2)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 2);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
    };

    template<typename T1, typename T2>
    class CdeclFunctor2<void, T1, T2> : public Functor
    {
    public:

        typedef typename void(__cdecl *Callback)(T1, T2);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;

        explicit CdeclFunctor2 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2)
        : m_callback(callback), 
          m_param1((NonConstValueT1)t1), 
          m_param2((NonConstValueT2)t2)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 2);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2>
    class StdcallFunctor2 : public Functor
    {
    public:

        typedef typename Result(__stdcall *Callback)(T1, T2);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;

        explicit StdcallFunctor2 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1), 
          m_param2((NonConstValueT2)t2)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 2);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
    };

    template<typename T1, typename T2>
    class StdcallFunctor2<void, T1, T2> : public Functor
    {
    public:

        typedef typename void(__stdcall *Callback)(T1, T2);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;

        explicit StdcallFunctor2 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1), 
          m_param2((NonConstValueT2)t2)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 2);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2, typename T3>
    class CdeclFunctor3 : public Functor
    {
    public:

        typedef typename Result(__cdecl *Callback)(T1, T2, T3);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;

        explicit CdeclFunctor3 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2, m_param3);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 3);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
    };

    template<typename T1, typename T2, typename T3>
    class CdeclFunctor3<void, T1, T2, T3> : public Functor
    {
    public:

        typedef typename void(__cdecl *Callback)(T1, T2, T3);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;

        explicit CdeclFunctor3 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 3);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2, typename T3>
    class StdcallFunctor3 : public Functor
    {
    public:

        typedef typename Result(__stdcall *Callback)(T1, T2, T3);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;

        explicit StdcallFunctor3 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2, m_param3);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 3);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
    };

    template<typename T1, typename T2, typename T3>
    class StdcallFunctor3<void, T1, T2, T3> : public Functor
    {
    public:

        typedef typename void(__stdcall *Callback)(T1, T2, T3);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;

        explicit StdcallFunctor3 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 3);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2, typename T3, typename T4>
    class CdeclFunctor4 : public Functor
    {
    public:

        typedef typename Result(__cdecl *Callback)(T1, T2, T3, T4);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;

        explicit CdeclFunctor4 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 4);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
    };

    template<typename T1, typename T2, typename T3, typename T4>
    class CdeclFunctor4<void, T1, T2, T3, T4> : public Functor
    {
    public:

        typedef typename void(__cdecl *Callback)(T1, T2, T3, T4);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;

        explicit CdeclFunctor4 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 4);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2, typename T3, typename T4>
    class StdcallFunctor4 : public Functor
    {
    public:

        typedef typename Result(__stdcall *Callback)(T1, T2, T3, T4);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;

        explicit StdcallFunctor4 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 4);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
    };

    template<typename T1, typename T2, typename T3, typename T4>
    class StdcallFunctor4<void, T1, T2, T3, T4> : public Functor
    {
    public:

        typedef typename void(__stdcall *Callback)(T1, T2, T3, T4);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;

        explicit StdcallFunctor4 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 4);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2, typename T3, typename T4, typename T5>
    class CdeclFunctor5 : public Functor
    {
    public:

        typedef typename Result(__cdecl *Callback)(T1, T2, T3, T4, T5);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename ConstRef<T5>::Type ConstReferenceT5;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;
        typedef typename RemoveReference<typename NonConst<T5>::Type>::Type NonConstValueT5;

        explicit CdeclFunctor5 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 5);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else if (param == 4)
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
            else
            {
                m_param5 = va_arg(args, NonConstValueT5);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
        NonConstValueT5 m_param5;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    class CdeclFunctor5<void, T1, T2, T3, T4, T5> : public Functor
    {
    public:

        typedef typename void(__cdecl *Callback)(T1, T2, T3, T4, T5);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename ConstRef<T5>::Type ConstReferenceT5;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;
        typedef typename RemoveReference<typename NonConst<T5>::Type>::Type NonConstValueT5;

        explicit CdeclFunctor5 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 5);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else if (param == 4)
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
            else
            {
                m_param5 = va_arg(args, NonConstValueT5);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
        NonConstValueT5 m_param5;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename T1, typename T2, typename T3, typename T4, typename T5>
    class StdcallFunctor5 : public Functor
    {
    public:

        typedef typename Result(__stdcall *Callback)(T1, T2, T3, T4, T5);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename ConstRef<T5>::Type ConstReferenceT5;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;
        typedef typename RemoveReference<typename NonConst<T5>::Type>::Type NonConstValueT5;

        explicit StdcallFunctor5 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = m_callback(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 5);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else if (param == 4)
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
            else
            {
                m_param5 = va_arg(args, NonConstValueT5);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
        NonConstValueT5 m_param5;
    };

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    class StdcallFunctor5<void, T1, T2, T3, T4, T5> : public Functor
    {
    public:

        typedef typename void(__stdcall *Callback)(T1, T2, T3, T4, T5);
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename ConstRef<T5>::Type ConstReferenceT5;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;
        typedef typename RemoveReference<typename NonConst<T5>::Type>::Type NonConstValueT5;

        explicit StdcallFunctor5 (Callback callback, ConstReferenceT1 t1, ConstReferenceT2 t2, 
            ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback(callback),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        virtual void Call ()
        {
            m_callback(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 1 && param <= 5);

            va_list args;
            va_start(args, param);

            if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else if (param == 4)
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
            else
            {
                m_param5 = va_arg(args, NonConstValueT5);
            }
        }

    protected:

        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
        NonConstValueT5 m_param5;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/

    template<typename Result, typename Type>
    class MemberFunctor0 : public Functor
    {
    public:

        typedef typename Result(Type::*Callback)();
        typedef typename Result(Type::*ConstCallback)() const;

        explicit MemberFunctor0 (Callback callback, Type* instance)
        : m_callback(callback), 
          m_instance(instance)
        {
        }

        explicit MemberFunctor0 (ConstCallback callback, Type const* instance)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)();
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = (m_instance->*m_callback)();
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param == 0);

            va_list args;
            va_start(args, param);

            m_instance = va_arg(args, Type*);
        }

    protected:

        Type* m_instance;
        Callback m_callback;
    };

    template<typename Type>
    class MemberFunctor0<void, Type> : public Functor
    {
    public:

        typedef typename void(Type::*Callback)();
        typedef typename void(Type::*ConstCallback)() const;

        explicit MemberFunctor0 (Callback callback, Type* instance)
        : m_callback(callback), 
          m_instance(instance)
        {
        }

        explicit MemberFunctor0 (ConstCallback callback, Type const* instance)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)();
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param == 0);

            va_list args;
            va_start(args, param);

            m_instance = va_arg(args, Type*);
        }

    protected:

        Type* m_instance;
        Callback m_callback;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename Type, typename T1>
    class MemberFunctor1 : public Functor
    {
    public:

        typedef typename Result(Type::*Callback)(T1);
        typedef typename Result(Type::*ConstCallback)(T1) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;

        explicit MemberFunctor1 (Callback callback, Type* instance, ConstReferenceT1 t1)
        : m_callback(callback), 
          m_instance(instance),
          m_param1((NonConstValueT1)t1)
        {
        }

        explicit MemberFunctor1 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = (m_instance->*m_callback)(m_param1);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 1);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
    };

    template<typename Type, typename T1>
    class MemberFunctor1<void, Type, T1> : public Functor
    {
    public:

        typedef typename void(Type::*Callback)(T1);
        typedef typename void(Type::*ConstCallback)(T1) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;

        explicit MemberFunctor1 (Callback callback, Type* instance, ConstReferenceT1 t1)
        : m_callback(callback), 
          m_instance(instance),
          m_param1((NonConstValueT1)t1)
        {
        }

        explicit MemberFunctor1 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 1);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename Type, typename T1, typename T2>
    class MemberFunctor2 : public Functor
    {
    public:

        typedef typename Result(Type::*Callback)(T1, T2);
        typedef typename Result(Type::*ConstCallback)(T1, T2) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;

        explicit MemberFunctor2 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2)
        {
        }

        explicit MemberFunctor2 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = (m_instance->*m_callback)(m_param1, m_param2);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 2);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
    };

    template<typename Type, typename T1, typename T2>
    class MemberFunctor2<void, Type, T1, T2> : public Functor
    {
    public:

        typedef typename void(Type::*Callback)(T1, T2);
        typedef typename void(Type::*ConstCallback)(T1, T2) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;

        explicit MemberFunctor2 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2)
        {
        }

        explicit MemberFunctor2 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1, 
            ConstReferenceT2 t2)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 2);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    class MemberFunctor3 : public Functor
    {
    public:

        typedef typename Result(Type::*Callback)(T1, T2, T3);
        typedef typename Result(Type::*ConstCallback)(T1, T2, T3) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;

        explicit MemberFunctor3 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        explicit MemberFunctor3 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2, ConstReferenceT3 t3)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2, m_param3);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = (m_instance->*m_callback)(m_param1, m_param2, m_param3);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 3);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
    };

    template<typename Type, typename T1, typename T2, typename T3>
    class MemberFunctor3<void, Type, T1, T2, T3> : public Functor
    {
    public:

        typedef typename void(Type::*Callback)(T1, T2, T3);
        typedef typename void(Type::*ConstCallback)(T1, T2, T3) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;

        explicit MemberFunctor3 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        explicit MemberFunctor3 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2, ConstReferenceT3 t3)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2, m_param3);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 3);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    class MemberFunctor4 : public Functor
    {
    public:

        typedef typename Result(Type::*Callback)(T1, T2, T3, T4);
        typedef typename Result(Type::*ConstCallback)(T1, T2, T3, T4) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;

        explicit MemberFunctor4 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        explicit MemberFunctor4 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2, ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = (m_instance->*m_callback)(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 4);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
    };

    template<typename Type, typename T1, typename T2, typename T3, typename T4>
    class MemberFunctor4<void, Type, T1, T2, T3, T4> : public Functor
    {
    public:

        typedef typename void(Type::*Callback)(T1, T2, T3, T4);
        typedef typename void(Type::*ConstCallback)(T1, T2, T3, T4) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;

        explicit MemberFunctor4 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        explicit MemberFunctor4 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2, ConstReferenceT3 t3, ConstReferenceT4 t4)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2, m_param3, m_param4);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 4);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
    };

    /************************************************************************
      Template Class Declaration
    *************************************************************************/
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    class MemberFunctor5 : public Functor
    {
    public:

        typedef typename Result(Type::*Callback)(T1, T2, T3, T4, T5);
        typedef typename Result(Type::*ConstCallback)(T1, T2, T3, T4, T5) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename ConstRef<T5>::Type ConstReferenceT5;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;
        typedef typename RemoveReference<typename NonConst<T5>::Type>::Type NonConstValueT5;

        explicit MemberFunctor5 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        explicit MemberFunctor5 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2, ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void CallAndGetResult (void* result)
        {
            *(Result*)result = (m_instance->*m_callback)(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 5);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else if (param == 4)
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
            else
            {
                m_param5 = va_arg(args, NonConstValueT5);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
        NonConstValueT5 m_param5;
    };

    template<typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    class MemberFunctor5<void, Type, T1, T2, T3, T4, T5> : public Functor
    {
    public:

        typedef typename void(Type::*Callback)(T1, T2, T3, T4, T5);
        typedef typename void(Type::*ConstCallback)(T1, T2, T3, T4, T5) const;
        typedef typename ConstRef<T1>::Type ConstReferenceT1;
        typedef typename ConstRef<T2>::Type ConstReferenceT2;
        typedef typename ConstRef<T3>::Type ConstReferenceT3;
        typedef typename ConstRef<T4>::Type ConstReferenceT4;
        typedef typename ConstRef<T5>::Type ConstReferenceT5;
        typedef typename RemoveReference<typename NonConst<T1>::Type>::Type NonConstValueT1;
        typedef typename RemoveReference<typename NonConst<T2>::Type>::Type NonConstValueT2;
        typedef typename RemoveReference<typename NonConst<T3>::Type>::Type NonConstValueT3;
        typedef typename RemoveReference<typename NonConst<T4>::Type>::Type NonConstValueT4;
        typedef typename RemoveReference<typename NonConst<T5>::Type>::Type NonConstValueT5;

        explicit MemberFunctor5 (Callback callback, Type* instance, ConstReferenceT1 t1, ConstReferenceT2 t2,
            ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback(callback),
          m_instance(instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        explicit MemberFunctor5 (ConstCallback callback, Type const* instance, ConstReferenceT1 t1,
            ConstReferenceT2 t2, ConstReferenceT3 t3, ConstReferenceT4 t4, ConstReferenceT5 t5)
        : m_callback((Callback)callback), 
          m_instance((Type*)instance),
          m_param1((NonConstValueT1)t1),
          m_param2((NonConstValueT2)t2),
          m_param3((NonConstValueT3)t3),
          m_param4((NonConstValueT4)t4),
          m_param5((NonConstValueT5)t5)
        {
        }

        virtual void Call ()
        {
            (m_instance->*m_callback)(m_param1, m_param2, m_param3, m_param4, m_param5);
        }

        virtual void SetParameter (uint param, ...)
        {
            assert(param >= 0 && param <= 5);

            va_list args;
            va_start(args, param);

            if (param == 0)
            {
                m_instance = va_arg(args, Type*);
            }
            else if (param == 1)
            {
                m_param1 = va_arg(args, NonConstValueT1);
            }
            else if (param == 2)
            {
                m_param2 = va_arg(args, NonConstValueT2);
            }
            else if (param == 3)
            {
                m_param3 = va_arg(args, NonConstValueT3);
            }
            else if (param == 4)
            {
                m_param4 = va_arg(args, NonConstValueT4);
            }
            else
            {
                m_param5 = va_arg(args, NonConstValueT5);
            }
        }

    protected:

        Type* m_instance;
        Callback m_callback;
        NonConstValueT1 m_param1;
        NonConstValueT2 m_param2;
        NonConstValueT3 m_param3;
        NonConstValueT4 m_param4;
        NonConstValueT5 m_param5;
    };

    /************************************************************************
      Bind Function Definitions
    *************************************************************************/

    template<typename Result>
    inline Functor* Bind (Result(__cdecl *callback)())
    {
        return new CdeclFunctor0<Result>(callback);
    }
    template<typename Result>
    inline Functor* Bind (Result const(__cdecl *callback)())
    {
        return new CdeclFunctor0<Result>(callback);
    }

#ifndef _M_X64
    template<typename Result>
    inline Functor* Bind(Result(__stdcall *callback)())
    {
        return new StdcallFunctor0<Result>(callback);
    }
    template<typename Result>
    inline Functor* Bind(Result const(__stdcall *callback)())
    {
        return new StdcallFunctor0<Result>(callback);
    }
#endif

    template<typename Result, typename T1>
    inline Functor* Bind (Result(__cdecl *callback)(T1), _IDENTITY(T1) t1)
    {
        return new CdeclFunctor1<Result, T1>(callback, (T1)t1);
    }
    template<typename Result, typename T1>
    inline Functor* Bind (Result const(__cdecl *callback)(T1), _IDENTITY(T1) t1)
    {
        return new CdeclFunctor1<Result, T1>(callback, (T1)t1);
    }

#ifndef _M_X64
    template<typename Result, typename T1>
    inline Functor* Bind (Result(__stdcall *callback)(T1), _IDENTITY(T1) t1)
    {
        return new StdcallFunctor1<Result, T1>(callback, (T1)t1);
    }
    template<typename Result, typename T1>
    inline Functor* Bind (Result const(__stdcall *callback)(T1), _IDENTITY(T1) t1)
    {
        return new StdcallFunctor1<Result, T1>(callback, (T1)t1);
    }
#endif

    template<typename Result, typename T1, typename T2>
    inline Functor* Bind (Result(__cdecl *callback)(T1, T2), _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {
        return new CdeclFunctor2<Result, T1, T2>(callback, (T1)t1, (T2)t2);
    }
    template<typename Result, typename T1, typename T2>
    inline Functor* Bind (Result const(__cdecl *callback)(T1, T2), _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {
        return new CdeclFunctor2<Result, T1, T2>(callback, (T1)t1, (T2)t2);
    }

#ifndef _M_X64
    template<typename Result, typename T1, typename T2>
    inline Functor* Bind (Result(__stdcall *callback)(T1, T2), _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {
        return new StdcallFunctor2<Result, T1, T2>(callback, (T1)t1, (T2)t2);
    }
    template<typename Result, typename T1, typename T2>
    inline Functor* Bind (Result const(__stdcall *callback)(T1, T2), _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {
        return new StdcallFunctor2<Result, T1, T2>(callback, (T1)t1, (T2)t2);
    }
#endif

    template<typename Result, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result(__cdecl *callback)(T1, T2, T3), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {
        return new CdeclFunctor3<Result, T1, T2, T3>(callback, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result const(__cdecl *callback)(T1, T2, T3), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {
        return new CdeclFunctor3<Result, T1, T2, T3>(callback, (T1)t1, (T2)t2, (T3)t3);
    }

#ifndef _M_X64
    template<typename Result, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result(__stdcall *callback)(T1, T2, T3), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {
        return new StdcallFunctor3<Result, T1, T2, T3>(callback, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result const(__stdcall *callback)(T1, T2, T3), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {
        return new StdcallFunctor3<Result, T1, T2, T3>(callback, (T1)t1, (T2)t2, (T3)t3);
    }
#endif

    template<typename Result, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result(__cdecl *callback)(T1, T2, T3, T4), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {
        return new CdeclFunctor4<Result, T1, T2, T3, T4>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result const(__cdecl *callback)(T1, T2, T3, T4), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {
        return new CdeclFunctor4<Result, T1, T2, T3, T4>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }

#ifndef _M_X64
    template<typename Result, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result(_stdcall *callback)(T1, T2, T3, T4), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {
        return new StdcallFunctor4<Result, T1, T2, T3, T4>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result const(_stdcall *callback)(T1, T2, T3, T4), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {
        return new StdcallFunctor4<Result, T1, T2, T3, T4>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
#endif

    template<typename Result, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result(__cdecl *callback)(T1, T2, T3, T4, T5), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {
        return new CdeclFunctor5<Result, T1, T2, T3, T4, T5>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result const(__cdecl *callback)(T1, T2, T3, T4, T5), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {
        return new CdeclFunctor5<Result, T1, T2, T3, T4, T5>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }

#ifndef _M_X64
    template<typename Result, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result(__stdcall *callback)(T1, T2, T3, T4, T5), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {
        return new StdcallFunctor5<Result, T1, T2, T3, T4, T5>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result const(__stdcall *callback)(T1, T2, T3, T4, T5), _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {
        return new StdcallFunctor5<Result, T1, T2, T3, T4, T5>(callback, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
#endif

    // Functions to create member functors

    template<typename Result, typename Type>
    inline Functor* Bind (Result(Type::*callback)(), Type* instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, instance);
    }
    template<typename Result, typename Type>
    inline Functor* Bind (Result(Type::*callback)() const, Type const* instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, instance);
    }
    template<typename Result, typename Type>
    inline Functor* Bind (Result const(Type::*callback)(), Type* instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, instance);
    }
    template<typename Result, typename Type>
    inline Functor* Bind (Result const(Type::*callback)() const, Type const* instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, instance);
    }

    template<typename Result, typename Type>
    inline Functor* Bind (Result(Type::*callback)(), Type& instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, &instance);
    }
    template<typename Result, typename Type>
    inline Functor* Bind (Result(Type::*callback)() const, Type const& instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, &instance);
    }
    template<typename Result, typename Type>
    inline Functor* Bind (Result const(Type::*callback)(), Type& instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, &instance);
    }
    template<typename Result, typename Type>
    inline Functor* Bind (Result const(Type::*callback)() const, Type const& instance)
    {   // MemberFunctor0
        return new MemberFunctor0<Result, Type>(callback, &instance);
    }

    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result(Type::*callback)(T1), Type* instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, instance, (T1)t1);
    }
    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result(Type::*callback)(T1) const, Type const* instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, instance, (T1)t1);
    }
    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result const(Type::*callback)(T1), Type* instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, instance, (T1)t1);
    }
    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result const(Type::*callback)(T1) const, Type const* instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, instance, (T1)t1);
    }

    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result(Type::*callback)(T1), Type& instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, &instance, (T1)t1);
    }
    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result(Type::*callback)(T1) const, Type const& instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, &instance, (T1)t1);
    }
    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result const(Type::*callback)(T1), Type& instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, &instance, (T1)t1);
    }
    template<typename Result, typename Type, typename T1>
    inline Functor* Bind (Result const(Type::*callback)(T1) const, Type const& instance, _IDENTITY(T1) t1)
    {   // MemberFunctor1
        return new MemberFunctor1<Result, Type, T1>(callback, &instance, (T1)t1);
    }

    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result(Type::*callback)(T1, T2), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, instance, (T1)t1, (T2)t2);
    }
    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result(Type::*callback)(T1, T2) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, instance, (T1)t1, (T2)t2);
    }
    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, instance, (T1)t1, (T2)t2);
    }
    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, instance, (T1)t1, (T2)t2);
    }

    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result(Type::*callback)(T1, T2), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, &instance, (T1)t1, (T2)t2);
    }
    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result(Type::*callback)(T1, T2) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, &instance, (T1)t1, (T2)t2);
    }
    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, &instance, (T1)t1, (T2)t2);
    }
    template<typename Result, typename Type, typename T1, typename T2>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2)
    {   // MemberFunctor2
        return new MemberFunctor2<Result, Type, T1, T2>(callback, &instance, (T1)t1, (T2)t2);
    }

    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, instance, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, instance, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, instance, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, instance, (T1)t1, (T2)t2, (T3)t3);
    }

    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, &instance, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, &instance, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, &instance, (T1)t1, (T2)t2, (T3)t3);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3)
    {   // MemberFunctor3
        return new MemberFunctor3<Result, Type, T1, T2, T3>(callback, &instance, (T1)t1, (T2)t2, (T3)t3);
    }

    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }

    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4)
    {   // MemberFunctor4
        return new MemberFunctor4<Result, Type, T1, T2, T3, T4>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4);
    }

    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4, T5), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4, T5) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4, T5), Type* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4, T5) const, Type const* instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }

    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4, T5), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result(Type::*callback)(T1, T2, T3, T4, T5) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4, T5), Type& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }
    template<typename Result, typename Type, typename T1, typename T2, typename T3, typename T4, typename T5>
    inline Functor* Bind (Result const(Type::*callback)(T1, T2, T3, T4, T5) const, Type const& instance, _IDENTITY(T1) t1, _IDENTITY(T2) t2, _IDENTITY(T3) t3, _IDENTITY(T4) t4, _IDENTITY(T5) t5)
    {   // MemberFunctor5
        return new MemberFunctor5<Result, Type, T1, T2, T3, T4, T5>(callback, &instance, (T1)t1, (T2)t2, (T3)t3, (T4)t4, (T5)t5);
    }

    #pragma warning(default:4793)
}
