#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED
#include <math.h>
#include <memory>

//Those following two structures was designed to check the final type of expression,
//such as <a1*a2+a1+a2+a1*a3> which special is that the type of <a1*a2> is not equal to <a1*a2+a1*a2>.
//If a Matrix of <int,2,3> * <double,3,4> , the final type is decltype( MatrixTypeCheckStruct<3>(int(0),double(0)) )


template <unsigned int TC>
    struct MatrixTypeCheckStruct
    {
        template<typename T1,typename T2>
        static auto CheckType(const T1& R1,const T2& R2) ->decltype(  MatrixTypeCheckStruct<TC-1>::Type(R1*R2,R1,R2) ){return nullptr;}
        template<typename T1,typename T2,typename T3>
        static auto Type(const T1& R1,const T2& R2,const T3& R3) ->decltype(  MatrixTypeCheckStruct<TC-1>::Type(R1+R2*R3,R2,R3) ) {return nullptr;}
    };

template <>
    struct MatrixTypeCheckStruct<0>
    {
        template<typename T1,typename T2,typename T3>
        static T1 Type (const T1& R1,const T2& R2,const T3& R3)  {return nullptr;}
    };

//If a Type wants to use this function, the type you used must provide operator () and the return of this operator
//have to provide operator * and operator +.
//If you Run Multicount<P>::Count(I,O), you will get I(0)*O(0)+I(1)*O(1)...+I(R)*O(R).
template<unsigned int I>
    struct Multicount
    {
        template<typename T1,typename T2>
            static auto Count( const T1& K , const T2& P )
                -> decltype(    Multicount<I-1>::Count( K,P )  +  K(I)*P(I)   )
            {
                return Multicount<I-1>::Count( K,P ) + K(I)*P(I);
            }
    };

template<>
    struct Multicount<0>
    {
        template<typename T1,typename T2>
            static auto Count( const T1& K , const T2& P )
                -> decltype(    K(0)*P(0)   )
            {
                return  K(0)*P(0);
            }
    };


template<typename StoreType,unsigned int Line,unsigned int Row>
struct MatrixData
{
    StoreType StoreData[Line*Row];
    MatrixData(const MatrixData& K)
    {
        for(int i=0;i<Line*Row;++i)
            StoreData[i]=K.StoreData[i];
    }
    MatrixData(const StoreType& Type)
    {
        for(int i=0;i<Line*Row;++i)
            StoreData[i]=Type;
    }
    MatrixData( const StoreType* K )
    {
        for(int i=0; i<Line*Row;++i )
            StoreData[i]=K[i];
    }
    MatrixData(const StoreType** K)
    {
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                StoreData[i*Row+k]=K[i][k];
    }
    MatrixData()
    {
        for(int i=0; i<Line*Row;++i )
            StoreData[i]=StoreType();
    }
    const StoreType* Data()  const {return StoreData;}
};

//Two strange matrix is unable to be instant.
template<typename Store,unsigned int P>struct MatrixData<Store,0,P>{private:MatrixData(){}virtual void UnableToCreat()=0;};
template<typename Store,unsigned int P>struct MatrixData<Store,P,0>{private:MatrixData(){}virtual void UnableToCreat()=0;};

template<class StoreType,unsigned int Line,unsigned int Row>
    class Matrix_Row
    {
        std::shared_ptr<MatrixData<StoreType,Line,Row>> Data;
        const StoreType* StartData;
    public:
        const StoreType& operator() ( unsigned int S ) const {  return *(StartData + S * Row); }
        Matrix_Row(const std::shared_ptr<MatrixData<StoreType,Line,Row>>& Po , const StoreType* I):Data(Po),StartData(I){}
    };

template<class StoreType,unsigned int Line,unsigned int Row>
    class Matrix_Line
    {
        std::shared_ptr<MatrixData<StoreType,Line,Row>> Data;
        const StoreType* StartData;
    public:
        const StoreType& operator() ( unsigned int S ) const {  return *(StartData+S); }
        Matrix_Line(const std::shared_ptr<MatrixData<StoreType,Line,Row>>& Po ,const StoreType* I):Data(Po),StartData(I){}
    };


template<typename T1,unsigned L1,unsigned int Row,typename T2,unsigned R2>
    auto operator* ( const Matrix_Line<T1,L1,Row>& ML , const Matrix_Row<  T2,Row,R2 >& MR )
        -> decltype (  Multicount<Row-1>::Count(ML,MR)  )
        {
            return  Multicount<Row-1>::Count(ML,MR);
        }

template<class StoreType,unsigned int Line,unsigned int Row>
class Matrix
{
    std::shared_ptr<MatrixData<StoreType,Line,Row>> Data;
public:
    virtual ~Matrix(){}
    Matrix():Data(new MatrixData<StoreType,Line,Row>()){}
    Matrix(const Matrix& M):Data(M.Data){}
    Matrix( const StoreType* P){Data=new MatrixData<StoreType,Line,Row>(P);}
    Matrix( const StoreType** P){Data=new MatrixData<StoreType,Line,Row>(P);}
    Matrix& operator= (const Matrix& M){Data=M.Data;return *this;}
    const Matrix_Row<StoreType,Line,Row> GetRow(unsigned int K) const
    {
        Matrix_Row<StoreType,Line,Row> Tem(Data,Data->StoreData+K);
        return Tem;
    }
    void Reset()
    {
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                Data->StoreData[i*Row+k]=StoreType();
    }
    const Matrix_Line<StoreType,Line,Row> GetLine(unsigned int K) const
    {
        Matrix_Line<StoreType,Line,Row> Tem(Data,Data->StoreData+K*Row);
        return Tem;
    }
    const StoreType& operator() (unsigned int L,unsigned R) const {return Data->StoreData[L*Row+R];}
    StoreType& operator()(unsigned int L,unsigned R)
    {
        if(!Data.unique())
        {
            std::shared_ptr<MatrixData<StoreType,Line,Row>> Temc(new MatrixData<StoreType,Line,Row>(*Data));
            Data=Temc;
        }
        return Data->StoreData[L*Row+R];
    }

    template<typename T2>
    Matrix< decltype( StoreType(0)*T2(0) ),Line,Row > operator* (const T2& T) const
    {
        Matrix< decltype( StoreType(0)*T2(0) ),Line,Row > Tem;
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                Tem(i,k)=this->operator()(i,k)*T;
        return Tem;
    }

    template<class ST2,unsigned int R2>
    Matrix<decltype(  MatrixTypeCheckStruct<Row>::CheckType(StoreType(0),ST2(0)) ),Line,R2> operator*(const Matrix<ST2,Row,R2>& M) const
    {
        Matrix< decltype( MatrixTypeCheckStruct<Row>::CheckType( StoreType(0),ST2(0) ) ),Line,R2 > Tem;
        for(int i=0;i<Line;++i)
        {
            for(int k=0;k<R2;++k)
            {
                Tem(i,k)=  GetLine(i) * M.GetRow(k) ;
            }
        }
        return Tem;
    }



    template<class ST2>
    Matrix<decltype(  StoreType(0)+ST2(0) ),Line,Row> operator+ (const Matrix<ST2,Line,Row>& M) const
    {
        Matrix<decltype(  StoreType(0)+ST2(0) ),Line,Row> Tem;
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                Tem(i,k)=this->operator()(i,k)+M(i,k);
        return Tem;
    }

    template<class ST2>
    Matrix< StoreType,Line,Row> operator+= (const Matrix<ST2,Line,Row>& M)
    {
        if(Data.use_count()>1)
        {
            std::shared_ptr<MatrixData<StoreType,Line,Row>> Tem(new MatrixData<StoreType,Line,Row>(*Data));
            Data=Tem;
        }
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                this->operator()(i,k)+=M(i,k);
        return *this;
    }

    template<class ST2>
    Matrix< StoreType,Line,Row> operator-= (const Matrix<ST2,Line,Row>& M)
    {
        if(Data.use_count()>1)
        {
            std::shared_ptr<MatrixData<StoreType,Line,Row>> Tem=new MatrixData<StoreType,Line,Row>(*Data);
            Data=Tem;
        }
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                this->operator()(i,k)-=M(i,k);
        return *this;
    }

    template<class ST2>
    Matrix<decltype(  StoreType(0)-ST2(0) ),Line,Row> operator- (const Matrix<ST2,Line,Row>& M) const
    {
        Matrix<decltype(  StoreType(0)-ST2(0) ),Line,Row> Tem;
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                Tem(i,k)=this->operator()(i,k)-M(i,k);
        return Tem;
    }
};



template<typename T2,typename StoreType,unsigned int Line,unsigned int Row>
    Matrix< decltype( T2(0)*StoreType(0) ),Line,Row > operator* (const T2& T,const Matrix<StoreType,Line,Row>& M)
    {
        Matrix< decltype( T2(0)*StoreType(0) ),Line,Row > Tem;
        for(int i=0;i<Line;++i)
            for(int k=0;k<Row;++k)
                Tem(i,k)=T*M(i,k);
        return Tem;
    }




/*template<typename StoreType,unsigned Line,unsigned int Row>
std::ostream& operator << (std::ostream& U,const Matrix<StoreType,Line,Row>& K)
{
    for(int i=0;i<Line;++i)
    {
        for(int k=0;k<Row;++k)
            U<<K(i,k)<<"  ,  ";
        U<<std::endl;
    }
    return U;
}*/


#endif // MATRIX_HPP_INCLUDED
