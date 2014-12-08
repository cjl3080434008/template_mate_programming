#ifndef ADAPTER_H_INCLUDED
#define ADAPTER_H_INCLUDED
template<typename T>
    class AdapterPlu
    {
        T& Reference;
    public:
        operator T& () {return Reference;}
        AdapterPlu(T& U):Reference(U){}
    };

template<typename T>
    class AdapterPlu_Const
    {
        const T& Reference;
    public:
        operator const T& () const {return Reference;}
        AdapterPlu_Const(const T& U):Reference(U){}
    };


template<typename T,typename ...TO>
    class AdapterConnect:public AdapterPlu<T>,public AdapterConnect<TO...>
    {
    public:
        template<typename Unk>
            AdapterConnect(Unk& U):AdapterPlu<T>(U),AdapterConnect<TO...>(U){}
    };

template<typename T,typename ...TO>
    class AdapterConnect_Const:public AdapterPlu_Const<T>,public AdapterConnect_Const<TO...>
    {
    public:
        template<typename Unk>
            AdapterConnect_Const(Unk& U):AdapterPlu_Const<T>(U),AdapterConnect_Const<TO...>(U){}
    };

template<typename T>
    class AdapterConnect<T>:public AdapterPlu<T>
    {
    public:
        template<typename Unk>
            AdapterConnect(Unk& U):AdapterPlu<T>(U){}
    };

template<typename T>
    class AdapterConnect_Const<T>:public AdapterPlu_Const<T>
    {
    public:
        template<typename Unk>
            AdapterConnect_Const(Unk& U):AdapterPlu_Const<T>(U){}
    };

template<typename ...TO>
    class Adapter_Const:public AdapterConnect_Const<TO...>
    {
        template<typename Pre,typename Pos>
            struct AdapterGlue_Const:public Pre,public Pos
            {
                template<typename T1,typename T2>
                    AdapterGlue_Const(T1& t,T2& t2):Pre(t),Pos(t2){}
            };
    public:
        template<typename T> Adapter_Const<TO...,T> Connect( T& L,... ) const {Adapter_Const<T> Ui(L);return Connect(  Ui );}
        template<typename ...T>
            Adapter_Const<TO...,T...> Connect_Const( Adapter_Const<T...>& L )
            {
                AdapterGlue_Const<Adapter_Const<TO...>,Adapter_Const<T...>> Tem(*this,L);
                Adapter_Const<TO...,T...> Tr(Tem);
                return Tr;
            }
        template<typename T>
            const T& Get() { return static_cast<T&>(*this); }
        template<typename Un>
            Adapter_Const(Un& U):AdapterConnect_Const<TO...>(U){}
    };

template<typename ...TO>
    class Adapter:public AdapterConnect<TO...>
    {
        template<typename Pre,typename Pos>
            struct AdapterGlue:public Pre,public Pos
            {
                template<typename T1,typename T2>
                    AdapterGlue(T1& t,T2& t2):Pre(t),Pos(t2){}
            };
    public:
        operator Adapter_Const<TO...> () const {Adapter_Const<TO...> TC(*this);return TC;}
        template<typename T> Adapter<TO...,T> Connect( T& L,... ){Adapter<T> Ui(L);return Connect(  Ui );}
        template<typename ...T>
            Adapter<TO...,T...> Connect( Adapter<T...>& L )
            {
                AdapterGlue<Adapter<TO...>,Adapter<T...>> Tem(*this,L);
                Adapter<TO...,T...> Tr(Tem);
                return Tr;
            }
        template<typename T>
            T& Get() { return static_cast<T&>(*this); }
        template<typename Un>
            Adapter(Un& U):AdapterConnect<TO...>(U){}
    };
#endif // ADAPTER_H_INCLUDED
