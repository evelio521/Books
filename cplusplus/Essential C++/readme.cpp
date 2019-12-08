一
template <typename T>
inline T const& max(T const& a, T const& b);
...
max(4, 7); // OK,兩個T都被推導為int
max(4, 4.2); // 錯誤:第一個 T 被推導為 int,第二個 T 被推導為 double


有三種方法可以解決上述問題: 
1. 把兩個引數轉型為相同型別:
max(static_cast<double>(4), 4.2); // OK 
2. 明確指定T的型別:
max<double>(4, 4.2); // OK
3. 對各個templateparameters使用不同的型別(譯註:意思是不要像上面那樣都叫做T)。

二
template <typename T1, typename T2, typename RT>
inline RT max (T1 const& a, T2 const& b);

max<int,double,double>(4, 4.2);

template <typename RT, typename T1, typename T2>
inline RT max (T1 const& a, T2 const& b);
...
max<double>(4,4.2); // OK,返回型別為double

三

template <double VAT> // 錯誤:浮點值不能作為 template parameters double process (double v)
{
     return v * VAT;
}
template <std::string name> // 錯誤:class objects 不能作為 template parameters 
class MyClass {
    ... 
};

template <char const* name>
class MyClass {
    ... 
};
MyClass<"hello"> x; // 錯誤:不能使用字串常量"hello"

template <char const* name>
Class MyClass {
    ... 
};
char const* s = "hello";
MyClass<s>x; //錯誤:s是「指向內部聯結(internallinkage)物件」的指標


但是你可以這麼寫:
template <char const* name>
Class MyClass {
    ... 
};
extern char const s[] = "hello"; MyClass<s>x; //OK

四
template <typename T>
class MyClass {
    typename T::SubType * ptr;
    ... 
};
 
template<int N>
void printBitset (std::bitset<N> const& bs)
{
    std::cout << bs.template to_string<char,char_traits<char>,allocator<char> >();
}

五 
如果classtemplates擁有base classes,那麼其內出現的成員名稱x並非總是等價於this->x,即使 x 係繼承而來
template <typename T>
class Base {
  public:
     void exit();
};
template <typename T>
class Derived : public Base<T> {
  public:
       void foo() {
           exit(); // 要不就呼叫外部 exit(),要不就發生編譯錯誤。
       } 
}; 

六
下面的程式碼是正確的:
template <typename T,
template <typename ELEM> class CONT = std::deque > //OK
  class Stack {
     ...
};
template <typename T,
            template <typename> class CONT = std::deque >
class Stack {
  ...
};
下面的程式碼則是錯誤的:
template <typename T,
template <typename ELEM> typename CONT = std::deque > //ERROR
  class Stack {
     ...
};

上面编译有些问题，下面才是正确的
template <typename T,
  template <typename ELEM,
            typename ALLOC = std::allocator<ELEM> >
            class CONT = std::deque>
class Stack {
    private:
        CONT<T> elems;
};

七
template <typename T>
void foo()
{
    T x = T(); // 如果T是內建型別,則x被初始化為0或false 
}

template <typename T>
class MyClass {
  private:
      T x; 
  public:
      MyClass():x(){ //這麼做可以確保:即使T為內建型別,x也能被初始化。
      }
};


八
template <typename T, int N, int M>
T const* max (T const (&a)[N], T const (&b)[M])
{
   return a < b ? b : a;
}