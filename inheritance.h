#ifndef INHERITANCE_H_INCLUDE
#define INHERITANCE_H_INCLUDE
#include <tuple>
#include <memory>
#include "tupleinitclass.h"
#include "typepair.h"
/*
UnbalanceTreeInheritance, which based on template class TupleInitClass, is to be a glue for different class.
If using UnbalanceTreeInheritance<A,B,C,D> UTI, A& a(UTI) is also right.
But The constructor function of UnbalanceTree can only accept parameter list likes tuple<tuple<...>,tuple<...>...>,
which is 2 - dimension tuple. each tuple will be decode to each class's constructor function.
such as:
    A(int);B(float,int);C(char,T2);D();
and the acceptable parameter for UnbalanceTreeInheritance<A,B,C,D> is tuple<tuple<int>,tuple<float,int>,tuple<char,T2>,tuple<> >,
which will be decode like
TupleInitClass<A>( tuple<int> ),
TupleInitClass<B>( tuple<float,int> ),
TupleInitClass<C>( tuple<char,T2> ),
TupleInitClass<D>( tuple<> ).
*/

/*
UnbalanceTreeRun, which designed to access all the class in a convenience way.
To use this class, you have to provide a template class which can be specialized by one type, and his static function named "Run".
For example :
    template<typename T> class MeRun{ static void Run( T& T,int i){  T.R(  i);  }};

And then, calling UnbalanceTreeRun<MeRun>::FindPreRun(  UnbalanceTreeInheritance<A,B,C,D>(),i ) means calling
D::R(i);C::R(i);B::R(i);A::R(i);
in the meanwhile, calling UnbalanceTreeRun<MeRun>::FindPosRun(  UnbalanceTreeInheritance<A,B,C,D>(),i ) means calling
A::R(i);B::R(i);C::R(i);D::R(i);
*/




template<typename ThisClass,typename ...OtherClass>
class UnbalanceTreeInheritance:public TupleInitClass<ThisClass>,public UnbalanceTreeInheritance<OtherClass...>
{
    public:
    template<unsigned int L,unsigned int N,typename ...A1>
        UnbalanceTreeInheritance( const IndexSquence<L,N>& IS, std::tuple<A1...>& T ):TupleInitClass<ThisClass>(std::get<IndexSquence<L,N>::Value>(T)),UnbalanceTreeInheritance<OtherClass...>(IS.Next(),T){}

    template<typename ...A1>
        UnbalanceTreeInheritance( std::tuple<A1...>& T ):UnbalanceTreeInheritance( GetIndex< std::tuple_size<std::tuple<A1...>>::value >(),T ){}
    UnbalanceTreeInheritance<OtherClass...>& GetUpper(){ return *this;}
    UnbalanceTreeInheritance(){}
    typedef ThisClass type;
};

template<typename ThisClass>
class UnbalanceTreeInheritance<ThisClass>:public TupleInitClass<ThisClass>
{
    public:
    template<unsigned int L,unsigned int N,typename ...A1>
        UnbalanceTreeInheritance( const IndexSquence<L,N>& IS, std::tuple<A1...>& T ):TupleInitClass<ThisClass>(std::get<IndexSquence<L,N>::Value>(T)){}
public:
    template<typename ...A1>
        UnbalanceTreeInheritance( std::tuple<A1...>& T ):UnbalanceTreeInheritance( GetIndex< std::tuple_size<std::tuple<A1...>>::value >(),T ){}
    typedef ThisClass type;
    UnbalanceTreeInheritance(){}
};

template<typename This> struct UnbalanceTreeInheritanceType{};
template<typename This,typename ...Other>
    struct UnbalanceTreeInheritanceType<UnbalanceTreeInheritance<This,Other...>>
    {
        typedef This type;
    };

template<template<typename T2,typename ...T> class RunStruct >
    struct UnbalanceTreeRun
    {
        template<typename This,typename ...AP,typename ...AP2>
        static void FindPreRun( UnbalanceTreeInheritance< This,AP... >& UB, AP2&&... ap )
        {
            UnbalanceTreeRun<RunStruct>::FindPreRun( UB.GetUpper(),ap... );
            RunStruct<This>::Run(UB,ap...);
        }
        template<typename This,typename ...AP2>
        static void FindPreRun( UnbalanceTreeInheritance< This >& UB, AP2&&... ap )
        {
            RunStruct<This>::Run(UB,ap...);
        }
        template<typename This,typename ...AP,typename ...AP2>
        static void FindPosRun( UnbalanceTreeInheritance< This,AP... >& UB, AP2&&... ap )
        {
            RunStruct<This>::Run(UB,ap...);
            UnbalanceTreeRun<RunStruct>::FindPosRun( UB.GetUpper(),ap... );
        }
        template<typename This,typename ...AP2>
        static void FindPosRun( UnbalanceTreeInheritance< This >& UB, AP2&&... ap )
        {
            RunStruct<This>::Run(UB,ap...);
        }
    };

#endif // INHERITANCE_HPP_INCLUDE
