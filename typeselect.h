#ifndef TYPESELECT_H_INCLUDED
#define TYPESELECT_H_INCLUDED
template<bool ,typename First,typename Second>
    struct TypeSelect{ typedef First type; };
template<typename First,typename Second>
    struct TypeSelect<false,First,Second>{ typedef Second type; };
#endif // MULITYPE_HPP_INCLUDED
