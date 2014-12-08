#ifndef DETECT_H_INCLUDED
#define DETECT_H_INCLUDED
/*
This is a method to detector whether a class have this function or not.
To use this, a define has to be called in any way that able to define template class and can be partial specialized.
That mean in g++, calling like template<typename T> struct L{ calling define in here  }; is not allow.
If wanting to detector function like " void (AnyType)::Go(int,int ,float ) ",the define you have to call is:
    MEMBER_FUNCTION_DETECTOR(XXX,Go,void,( a,b,X ),int a,int b,float X);
The XXX is a sample to identify overload function(Marco don't have the ability to identify overload function);

After calling this define, the follow define is able to access,
    MEMBER_FUNCTION_DETECT_RESOULT(XXX,FN,T) is a constexpr bool value,
        means whether there is a function defined in sample " XXX " , named " FN " in type " T ".
    MEMBER_FUNCTION_PROCESS(XXX,FN,T,...) is a define the run the function if MEMBER_FUNCTION_DETECT_RESOULT(XXX,FN,T) is true.
The "..." is the parameter list of the function.

For example:
struct T1 { void Run( int i ){ cout<<"Find This with "<<i<<endl;}};
struct T2 { };

MEMBER_FUNCTION_DETECTOR( DONNOTUSE,Run,(a),int a );
 if( MEMBER_FUNCTION_DETECT_RESOULT(DONNOTUSE,Run,T1) ) cout<<"Find T1"<<endl;
    else cout<<"Can not Find T1"<<endl;
 if( MEMBER_FUNCTION_DETECT_RESOULT(DONNOTUSE,Run,T2) ) cout<<"Find T2"<<endl;
    else cout<<"Can not Find T2"<<endl;

MEMBER_FUNCTION_PROCESS( DONNOTUSE,Run,T1,2 );
MEMBER_FUNCTION_PROCESS( DONNOTUSE,Run,T2,2 );
cout<<"Finish"<<endl;

output:
    Find T1
    Can not Find T2
    Find This with 2
    Finish

*/
#define MEMBER_FUNCTION_DETECTOR(Pre,FunName,ReturnType, ParaNameList  , ... )  \
    struct FUNCTION_DETECTOR_STRCUT##Pre##FunName{ \
        template<typename L , ReturnType (L::*)( __VA_ARGS__ ) > struct Plu{}; \
        template<typename SS> static char Fun_Detect(  Plu<SS,&SS:: FunName >* ); \
        template<typename SS> static long Fun_Detect(  ... ); \
        template<bool,typename __PO__GivingType> struct RunStruct{\
            static ReturnType FunName(__PO__GivingType& __PO__PreGivingTypeName,##__VA_ARGS__ ){  return __PO__PreGivingTypeName . FunName ParaNameList ;  }\
        };\
        template<typename __PO__GivingType> struct RunStruct<false,__PO__GivingType>{\
            static ReturnType FunName(__PO__GivingType& __PO__PreGivingTypeName , ##__VA_ARGS__ ) {  };\
        };\
    };

#define MEMBER_FUNCTION_DETECT_RESOULT(Pre,FunName,Type) (sizeof( FUNCTION_DETECTOR_STRCUT##Pre##FunName::Fun_Detect<Type>(nullptr)) == sizeof(char))
#define MEMBER_FUNCTION_PROCESS(Pre,FunName,Type,...) \
    FUNCTION_DETECTOR_STRCUT##Pre##FunName::RunStruct<MEMBER_FUNCTION_DETECT_RESOULT(Pre,FunName,Type),Type>::FunName(__VA_ARGS__)

#define TYPEDEF_DETECTOR(  TypeName  )  \
    struct TYPEDEF_DETECTOR_WITH_##TypeName{ \
        template<typename T> struct Plu{};\
        template<typename T> static char Detect( Plu<typename T::TypeName>* );\
        template<typename T> static long Detect( ... );\
    };\


#define TYPEDEF_DETECT_RESOULT( Type,TypeName ) (sizeof( TYPEDEF_DETECTOR_WITH_##TypeName::Detect<Type>(nullptr))==sizeof(char))

#endif // FUNCTIONDETECT_H_INCLUDED
