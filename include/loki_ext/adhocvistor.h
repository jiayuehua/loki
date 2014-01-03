#ifndef ADHOCVISITOR_H
#define ADHOCVISITOR_H
#include <loki/typelist.h>
namespace Loki{

template <class tlist> class AdHocVisitor;
//
//class Effector
//{
//    void Visit(Horse*);
//    void Visit(Dog*);
//};
//
template <class H, class T>
class AdHocVisitor< Loki::Typelist<H, T> >: public AdHocVisitor<T>
{
public:
    using AdHocVisitor<T>::Visit;
    virtual void Visit(H*) = 0;
    template <class SomeClass>
    void StartVisit(SomeClass* p)
    {
        if (H* pFound = dynamic_cast<H*>(p))
        {
            Visit(pFound);
        }
        else
        {
            AdHocVisitor<T>::StartVisit(p);
        }
    }
};

template <class H>
class AdHocVisitor<Loki::Typelist<H, NullType> >
{
public:
    virtual void Visit(H*) = 0;
    template <class SomeClass>
    void StartVisit (SomeClass* p)
    {
        if (H* pFound = dynamic_cast<H*>(p))
        {
            Visit(pFound);
        }
        else
        {
            throw "Unknown type passed";
        }
    }
};

template <class tlist, class Effector> class FlexAdHocVisitor_Impl;
template <class H, class T, class Effector>
class FlexAdHocVisitor_Impl< Loki::Typelist<H, T> ,Effector>: public FlexAdHocVisitor_Impl<T, Effector >, virtual public Effector
{
public:
    template <class SomeClass>
    void StartVisit(SomeClass* p)
    {
        if (H* pFound = dynamic_cast<H*>(p))
        {
            typedef void (Effector::*PVistor)(H*);
            PVistor pvistor = &Effector::Visit;//check
            Effector::Visit(pFound);
        }
        else
        {
            FlexAdHocVisitor_Impl<T, Effector>::StartVisit(p);
        }
    }
};

template <class H, class Effector>
class FlexAdHocVisitor_Impl<Loki::Typelist<H, NullType>,  Effector >:virtual public Effector
{
public:
    template <class SomeClass>
    void StartVisit (SomeClass* p)
    {
        if (H* pFound = dynamic_cast<H*>(p))
        {
            typedef void (Effector::*PVistor)(H*);
            PVistor pvistor = &Effector::Visit;//check
            Effector::Visit(pFound);
        }
        else
        {
            throw "Unknown type passed";
        }
    }
};

template <class tList,class Effector>
class FlexAdHocVisitor :public FlexAdHocVisitor_Impl<typename TL::DerivedToFront<tList>::Result, Effector>
{};
template <class tlist, template <class T> class Effector> class FlexAdHocVisitorEx_Impl;
template <class H, class T,template <class T> class Effector>
class FlexAdHocVisitorEx_Impl< Loki::Typelist<H, T> ,Effector>: public FlexAdHocVisitorEx_Impl<T, Effector>, public Effector<H>
{
public:
    template <class SomeClass>
    void StartVisit(SomeClass* p)
    {
        if (H* pFound = dynamic_cast<H*>(p))
        {
            Effector<H>::Visit(pFound);
        }
        else
        {
            FlexAdHocVisitorEx_Impl<T, Effector>::StartVisit(p);
        }
    }
};

template <class H, template <class T> class Effector>
class FlexAdHocVisitorEx_Impl<Loki::Typelist<H, NullType>,  Effector >: public Effector<H>
{
public:
    template <class SomeClass>
    void StartVisit (SomeClass* p)
    {
        if (H* pFound = dynamic_cast<H*>(p))
        {
            Effector<H>::Visit(pFound);
        }
        else
        {
            throw "Unknown type passed";
        }
    }
};
template <class tlist, template <class T> class Effector>
class FlexAdHocVistorEx:public FlexAdHocVisitorEx_Impl<typename TL::DerivedToFront<tlist>::Result, Effector>
{};

}

#endif
