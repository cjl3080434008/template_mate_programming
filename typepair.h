#ifndef TYPEPAIR_H_INCLUDED
#define TYPEPAIR_H_INCLUDED
#include <tuple>
#include <memory>
#include <type_traits>
template<typename ...Type>
    struct TypeInfo
    {
        template<typename ...T> TypeInfo<Type...,T...> operator+ ( const TypeInfo<T...>& TI);
    };

template<typename TI,typename Ik>
    struct TypePair
    {
        Ik Data;
        TypePair(Ik&& I ):Data(I){}
        const Ik& GetData()const {return Data;}
        template<typename T2,typename I2> auto operator+ (  TypePair<T2,I2>&& TP ) const
            -> TypePair<decltype(TI()+T2()),decltype(std::tuple_cat(Data,TP.Data))>
            {
                TypePair<decltype(TI()+T2()),decltype(std::tuple_cat(Data,TP.Data))> Tem(std::tuple_cat(Data,TP.Data));
                return Tem;
            }
        template<typename T2,typename I2> auto operator+ ( const TypePair<T2,I2>& TP ) const
            -> TypePair<decltype(TI()+T2()),decltype(std::tuple_cat(Data,TP.Data))>
            {
                TypePair<decltype(TI()+T2()),decltype(std::tuple_cat(Data,TP.Data))> Tem(std::tuple_cat(Data,TP.Data));
                return Tem;
            }
    };

template<unsigned int K>
struct EmptyTupleCreater
{
    static auto Create() -> decltype(
                                     std::tuple_cat(
                                                    std::tuple<std::tuple<> >() ,
                                                    EmptyTupleCreater<K-1>::Create()
                                                    )
                                     )
    {
        std::tuple<std::tuple<>> Tem;
        return std::tuple_cat( Tem , EmptyTupleCreater<K-1>::Create());
    }
};

template<>
struct EmptyTupleCreater<1>
{
    static std::tuple<std::tuple<>> Create()
    {
        std::tuple<std::tuple<>> Tem;
        return Tem;
    }
};

template<typename Name,typename ...Para>
    auto CreateTypePair( Para... Pa ) -> TypePair<TypeInfo<Name>,std::tuple<std::tuple<Para...>>>
    {
        TypePair<TypeInfo<Name> ,std::tuple<std::tuple<Para...>> >  TemPtr(std::make_tuple(std::make_tuple(Pa...)));
        return TemPtr;
    }

template<typename Type,typename ...Name>
    auto CreateTypePair(void) ->
    TypePair<TypeInfo<Type,Name...>,decltype(EmptyTupleCreater<sizeof...(Name)+1>::Create())>
    {
            TypePair<TypeInfo<Type,Name...>,decltype(EmptyTupleCreater<sizeof...(Name)+1>::Create())>
                Tem(EmptyTupleCreater<sizeof...(Name)+1>::Create());
        return Tem;
    }

/*template<typename ...T1,typename ...T2,typename ...I1,typename ...I2>
auto operator+ ( const std::shared_ptr<TypePair<TypeInfo<T1...>,std::tuple<I1...>>>& TP1,
                    const std::shared_ptr<TypePair<TypeInfo<T2...>,std::tuple<I2...>>>& TP2)
    ->decltype(  ConnectTypePair(TP1,TP2) )
    {
        return ConnectTypePair(TP1,TP2);
    }*/

#endif // TYPEPAIR_HPP_INCLUDED
