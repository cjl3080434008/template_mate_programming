#ifndef SQUAREMATRIX_H_INCLUDED
#define SQUAREMATRIX_H_INCLUDED
#include "matrix.h"
//to do list:   I need an effective way to calculate the value of the determinate only with operator + and operator *.
template<typename ST,unsigned Each>
    decltype( ST(0)*ST(0) ) Determinate( const Matrix<ST,Each,Each>& K)
    {
        return 0;
    }


template<typename Store,unsigned int Line>
    class SquareMatrix:public Matrix<Store,Line,Line>
    {
    public:
        SquareMatrix(const Matrix<Store,Line,Line>& L):Matrix<Store,Line,Line>(L){}
        SquareMatrix(const Store& L):Matrix<Store,Line,Line>(L){}
        SquareMatrix(const Store* L):Matrix<Store,Line,Line>(L){}
        const auto Deteminate() const ->decltype(Deteminate(*this)) { return Deteminate(*this);}
    };




#endif // SQUAREMATRIX_HPP_INCLUDED
