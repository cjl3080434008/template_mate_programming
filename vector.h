#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#include "matrix.h"
template<unsigned int I>
struct SingleMatrixInitStruct
{
    template<typename Store ,unsigned int P,typename T,typename ...TP>
        static void Init( Matrix<Store,P,1>& M, const T& T1, const TP&... TO ) {M(P-I,0)=T1;SingleMatrixInitStruct<I-1>::Init(M,TO...);}
};

template<>
struct SingleMatrixInitStruct<0>
{
    template<typename Store ,unsigned int P>
        static void Init( Matrix<Store,P,1>& M ){}
};

template<typename Store ,unsigned int P>
class Vector:public Matrix<Store,P,1>
{
    /*template<unsigned int K>
        auto CountResoult(const T1& R1,const T2& R2, const T3& R3) ->decltype ( CountResoult<K-1>();  )*/

public:

    Vector(){}
    Vector(const Matrix<Store,P,1>& K):Matrix<Store,P,1>(K){}
    Vector(const Store* K):Matrix<Store,P,1>(K){}
    Vector(const Store& K):Matrix<Store,P,1>(K){}
    Vector(Store* K):Matrix<Store,P,1>(K){}
    Vector(Store& K):Matrix<Store,P,1>(K){}
    template<typename ...TP>   Vector( const TP&... PL){ SingleMatrixInitStruct<P>::Init( *this , PL...); }
    template<typename ...TP>   void Set(const TP&... TO ){SingleMatrixInitStruct<P>::Init(*this,TO...);}

    const Store& operator () (unsigned int S) const {return Matrix<Store,P,1>::operator()(S,0);}
    Store& operator() (unsigned int S) {return Matrix<Store,P,1>::operator()(S,0);}
    template<typename ST>
        Vector< decltype( Store(0)*ST(0) ) , P > operator* (const ST& C) const
        {
            Vector< decltype( Store(0)*ST(0) ) , P > Tem;
            for(int i=0;i<P;++i)
                Tem(i)=this->operator()(i)*C;
            return Tem;
        }
};

template<typename Store >
class Vector<Store,3>:public Matrix<Store,3,1>
{
public:
    Vector(const Matrix<Store,3,1>& K):Matrix<Store,3,1>(K){}
    Vector(const Store* K):Matrix<Store,3,1>(K){}
    Vector(const Store& K):Matrix<Store,3,1>(K){}
    Vector(Store* K):Matrix<Store,3,1>(K){}
    Vector(Store& K):Matrix<Store,3,1>(K){}
    Vector(){}
    template<typename ...TP>   Vector( const TP&... PL){ SingleMatrixInitStruct<3>::Init( *this , PL...); }
    template<typename ...TP>   void Set(const TP&... TO ){SingleMatrixInitStruct<3>::Init(*this,TO...);}
    const Store& operator () (unsigned int S) const {return Matrix<Store,3,1>::operator()(S,0);}
    Store& operator() (unsigned int S) {return Matrix<Store,3,1>::operator()(S,0);}
    template<typename ST>
        decltype( MatrixTypeCheckStruct<3>::CheckType(Store(0),ST(0) )  ) operator* (const Vector<ST,3>& K) const
        {
            decltype( Store(0)*ST(0)+ Store(0)*ST(0)  ) Tem( Multicount<2>::Count(*this,K) );
            return Tem;
        }
    template<typename ST>
         Vector< decltype(MatrixTypeCheckStruct<3>::CheckType(Store(0),ST(0))),3> operator% ( const Vector<ST,3>& L) const
        {
            Vector< decltype(MatrixTypeCheckStruct<3>::CheckType(Store(0),ST(0))),3> Tem;
            Tem(0) = this->operator()(1) * L(2) - this->operator()(2) * L(1);
            Tem(1) = this->operator()(2) * L(0) - this->operator()(0) * L(2);
            Tem(2) = this->operator()(0) * L(1) - this->operator()(1) * L(0);
            return Tem;
        }
};


template<typename StoreType,unsigned Line>
std::ostream& operator << (std::ostream& U,const Vector<StoreType,Line>& K)
{
    for(int i=0;i<Line;++i)
        U<<K(i)<<"  ,  ";
    return U;
}


#endif // VECTOR_HPP_INCLUDED
