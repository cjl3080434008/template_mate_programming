#ifndef TUPLEINITCLASS_HPP_INCLUDED
#define TUPLEINITCLASS_HPP_INCLUDED
#include <tuple>
/*
    The code in this file is designed to solve the problem which using a tuple to initialize a normal class.
    Such as:

class A
{
    A(...);
}

    To initialize this class, you have to use a special class derived form A.

class TupleInitClass:public A;

    Then while calling to TupleInitClass( make_tuple( a,b,c,d ) ) is equal to A( a,b,c,d  ).
    Otherwise, class TupleInitClass is also a template, you can use TupleInitClass<A> instant of use A directly.
    So, if wanting to initialize such class:

class B
{
    B(int,char);
}
    You can use such code:

auto Temporary=make_tuple(int(),char());
TupleInitClass<B> (Temporary);
*/

template<unsigned int K>
struct Index
{
    static const unsigned int Value;
};
template<unsigned int K>
    const unsigned int Index<K>::Value=K;

template<unsigned int Target,unsigned int Now>
    struct IndexSquence:public Index<Now>
    {
        static IndexSquence<Target,Now+1> Next() { IndexSquence<Target,Now+1> Tem;return Tem;}
    };

template<unsigned int Target>
    struct IndexSquence<Target,Target>:public Index<Target>
    {
    };

template<unsigned int T>
IndexSquence<T,0> GetIndex()
{
    IndexSquence<T,0> Tem;
    return Tem;
}


template<typename Class>
class TupleInitClass:public Class
{
    template<typename ...Type,unsigned int L,unsigned int N,typename ...AP>
    TupleInitClass( std::tuple<Type...>& K , const IndexSquence<L,N>& IS, AP&... A  ):TupleInitClass(K,IndexSquence<L,N>::Next(),A...,std::get<IndexSquence<L,N>::Value>(K)){}
    template<typename ...Type,unsigned int L,typename ...AP>
    TupleInitClass( std::tuple<Type...>& K , const IndexSquence<L,L>& IS, AP&... A  ):Class(A...){}
public:
    template< typename ...Type>
    TupleInitClass( std::tuple<Type...>& K ):TupleInitClass(K,GetIndex<sizeof...(Type)>()){}
    TupleInitClass(){}
};



#endif // TUPLEINITCLASS_HPP_INCLUDED
