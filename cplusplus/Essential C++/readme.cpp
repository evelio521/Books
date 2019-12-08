һ
template <typename T>
inline T const& max(T const& a, T const& b);
...
max(4, 7); // OK,�ɂ�T�����ƌ���int
max(4, 4.2); // �e�`:��һ�� T ���ƌ��� int,�ڶ��� T ���ƌ��� double


�����N�������Խ�Q�������}: 
1. �уɂ������D�͞���ͬ�̈́e:
max(static_cast<double>(4), 4.2); // OK 
2. ���_ָ��T���̈́e:
max<double>(4, 4.2); // OK
3. ������templateparametersʹ�ò�ͬ���̈́e(�g�]:��˼�ǲ�Ҫ�������ǘӶ�����T)��

��
template <typename T1, typename T2, typename RT>
inline RT max (T1 const& a, T2 const& b);

max<int,double,double>(4, 4.2);

template <typename RT, typename T1, typename T2>
inline RT max (T1 const& a, T2 const& b);
...
max<double>(4,4.2); // OK,�����̈́e��double

��

template <double VAT> // �e�`:���cֵ�������� template parameters double process (double v)
{
     return v * VAT;
}
template <std::string name> // �e�`:class objects �������� template parameters 
class MyClass {
    ... 
};

template <char const* name>
class MyClass {
    ... 
};
MyClass<"hello"> x; // �e�`:����ʹ���ִ�����"hello"

template <char const* name>
Class MyClass {
    ... 
};
char const* s = "hello";
MyClass<s>x; //�e�`:s�ǡ�ָ��Ȳ��Y(internallinkage)�������ָ��


����������@�N��:
template <char const* name>
Class MyClass {
    ... 
};
extern char const s[] = "hello"; MyClass<s>x; //OK

��
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

�� 
���classtemplates����base classes,���N��ȳ��F�ĳɆT���Qx�K�ǿ��ǵȃr�this->x,��ʹ x �S�^�ж���
template <typename T>
class Base {
  public:
     void exit();
};
template <typename T>
class Derived : public Base<T> {
  public:
       void foo() {
           exit(); // Ҫ���ͺ����ⲿ exit(),Ҫ���Ͱl�����g�e�`��
       } 
}; 

��
����ĳ�ʽ�a�����_��:
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
����ĳ�ʽ�a�t���e�`��:
template <typename T,
template <typename ELEM> typename CONT = std::deque > //ERROR
  class Stack {
     ...
};

���������Щ���⣬���������ȷ��
template <typename T,
  template <typename ELEM,
            typename ALLOC = std::allocator<ELEM> >
            class CONT = std::deque>
class Stack {
    private:
        CONT<T> elems;
};

��
template <typename T>
void foo()
{
    T x = T(); // ���T�ǃȽ��̈́e,�tx����ʼ����0��false 
}

template <typename T>
class MyClass {
  private:
      T x; 
  public:
      MyClass():x(){ //�@�N�����Դ_��:��ʹT��Ƚ��̈́e,xҲ�ܱ���ʼ����
      }
};


��
template <typename T, int N, int M>
T const* max (T const (&a)[N], T const (&b)[M])
{
   return a < b ? b : a;
}