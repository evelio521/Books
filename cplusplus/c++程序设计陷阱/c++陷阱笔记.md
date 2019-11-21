##c++陷阱

### Chapter 1

#### 陷阱1 注释泛滥

> 1. 注释必须是帮助维护人员理解代码，不能添加无用的注释，函数的参数不能过多，否则注释经常和定义不匹配。
> 2. 尽量使用有明显意义的名字。

#### 陷阱2 魔数

>1. 魔数可以用enum代替
>2. 魔数可以用初始化常量替代

#### 陷阱3 全局变量

>1. 没有任何理由使用全局变量。
>2. 全局变量会增加代码耦合性且难以维护。
>3. 全局变量要在运行时静态初始化，如果不能在编译时确定静态变量的初始值，运行时将会是灾难性的后果。
>4. 全局变量可以用单例替代

#### 陷阱4 没有区分重载和默认初始化

```c++
class C1 {
  public:
  void f1(int arg = 0) {
    
  }
};

C1 a;
a.f1(0);
a.f1();

class C2 {
  public:
   void f2() {}
   void f2(int) {}
};

C2 a;
a.f2(0);
a.f2();
```

#### 陷阱5 误解引用

> 1. c++ 能使用指针的地方都应该使用引用
> 2. 引用不是指针
> 3. 引用没有地址，不占用任何存储单元
> 4. 没有引用的引用，引用的指针
> 5. 不存在空引用，没有void引用

#### 陷阱6 误解const

> 1. const int ci = 12 无法修改ici的值
>
> 2. const int *ip3 = &i;
>
>     i =10; // OK
>
>     *ip3 = 10;// ERROR
>
> 3. extern const volatile time_t clock ,volatile表示可以修改，const不能修改，这种用法很微妙

#### 陷阱7 忽略语言的基本细节

> 1. (-2)[p] 和 *(-2 +p) 以及\*(p+-2)

#### 陷阱8 无法区分访问和可见性

> 1. IMPL可以隐藏实现细节
> 2. IMPL可以防止重新编译

####陷阱9 使用糟糕的语言

> 1. 区分c++和其他语言的的不同定义

#### 陷阱10 忽略习惯用语

> 1. 遵守每个特性原始的用法，auto_ptr放入函数可能会带来额外的开销

#### 陷阱11 不必要的小聪明

> 1. 使用运算符时，尽量清晰的表明运算顺序，不然会造成不必要的维护成本
> 2. 代码最好不要给维护造成困难，维护有时候会造成代码的逻辑错误
> 3. 选择传统的，明确的和效率稍低的方法，不要选择小聪明造成不明确难以维护的代码

#### 陷阱12 青春期行为

> 1. 严格遵守任何一个代码规范，绝对不能打破。

#### 陷阱13 混淆数据和初始值

```c++
int *pArr = new int[13];
int *pArr = new int(12); // 可以使用std::vector
```

#### 陷阱14 求值顺序不确定

> 1. 函数参数最后不要出现表达式，特别是参数之间有相关联的遍历，因为函数糗事顺序不确定，如f(i, i*=2)
> 2. 子表达式也是求值顺序不确定 a = p() + q()

#### 陷阱15 注意运算符的优先级

#### 陷阱16 for语句混乱

> 1. 循环变量应该在for语句内部定义，防止外部定义作用域过大造成维护成本增加

      ```c++
{
  for (int i = 0; i < size; ++i) {
    // other operation
  }
}
      ```

#### 陷阱17 最长符号问题

> 1. a+++++b -> a++ ++ +b 非法的
> 2. a+++ ++b -> (a++) + (++b) 合法的
> 3. 注意模板的>> 新的cpp已经修改了这个问题

#### 陷阱18 创造性的使用排列声明说明符

> 1. int const extern size = 10; // legal, but wired
> 2. int const * ptr = &size; // 合法 指向定长整数的指针
> 3. int * const ptr2 = &size; // 非法，应该是指向常量的指针
> 4. const int * pc1; // correct : pointer to const 

#### 陷阱19 函数对象不明确

> 1. String s("-------"); // explicit initilizer
> 2. String t; // default initialization
> 3. String x();// a function decalaration
> 4. String s = new String(); 等价于 String s = new String;

#### 陷阱20 类型限定符迁移

```c++
typedef int A[12];
extern const A ca; // array of 12 const ints
typdef int * AP[12][12];
volatile AP vm; // 2-D array of volatile pointers to int
volatile int *vm2[12]; // 2-D array of pointers to volatile int
// 编译器认为vm vm2是相同的类型

/*
 * 不提倡下面的写法
 */
typedef int MF() const;
MF nonmemfunc(); // error
class C {
  MF memfunc; // OK
}
```

#### 陷阱21 自我初始化

```c++
int var = 12;
{
  double var = var; // 值不确定
}

struct buf {
  char a,b,c,d;
};

void fun() {
  char* buf = new char[sizeof(buf)];// buf是四个字节
}

const int val = 12;
{
  enum {val = val};//val的枚举值是12
}
```

#### 陷阱22 static 和 Extern类型

```c++
static class Rep {
  
} rep;//static

Rep backup;// not static

// 应该采用下面的形式更加明确
class Rep {
  
};
namespace {
  static Rep rep1; // static
  static Rep rep1; // staitc
}
```

#### 陷阱23运算符重载查询异常

> 1. 尽量不要使用运算符重载，除非迫不得已
> 2. 运算符重载可能导致程序维护成本增加

#### 陷阱24 运算符->的微妙之处

```c++
class Ptr {
  public:
   Ptr(T *inti);
   T * operator->();
  private:
   T * tp_;
};

Ptr p(new T);
p->f(); // p.operator->()->f()

// 重载operator->返回的值，必须可以用于访问成员。但是这并不一定是预定义指针，
// 它可以使类对象，这个对象必须也重载了operator->

class AugPtr {
  public:
   AugPtr(T* init) :p_(init) {}
   Ptr &operator->();
  private:
  Ptr p_;
};

Ptr &AugPtr::operator->() {
  return p_;
}

AugPtr ap(new T);
ap->f();// ap.operator->().operator->()->f()
```

#### 陷阱25 #define 字面值

> 1. \#define 在进行扩展时可能会引起错误，并且可能引起内存消耗（如 #define MAX 1<<16）
> 2. 可以使用 const int max = 1<<16代替
> 3. 可以使用枚举代替，枚举不占用内存，这种方式比较好

#### 陷阱26 #define伪函数

> 1. 不要使用define定义的函数，可以用inline代替
> 2. \#define定义为函数一般和\_\_FILE\_\_等一起使用
> 3. 下面代码是缓解使用全局变量问题

```c++
class ExecBump {
  public:
    void operator()(void(*func)()) {
      func();
      ++count_;
    }
  private:
  static int count_;
};

int ExecBump::count_= 0；
  
void aFunc() {
  extern void g();
  ExecBump exec;
  int count = 0;
  while(count++ < 10) {
    exec(g);
  }
}
```

#### 陷阱27 滥用#if

> 1. 如何优化调试代码，不要滥用#if 

```c++
const bool debug = false;
void buggy() {
  if (debug) {
    //
  }
  //
   if (debug) {
    //
  }
  // 编译器会进行优化
}
```

> 2. 类函数中不要使用#if 可以使用多态代替

#### 陷阱28 断言的副作用

```c++
assert(process(arr[index])); // 不要在断言中处理逻辑
```

#### 陷阱29  通过void* 转换

> 1. 不要使用void* 转换，如果必须要转换请使用c++提供标准转换函数

```
一、static_cast转换

　　1.基本用法：static_cast<type-id> expression

　　2.使用场景：

　　a、用于类层次结构中基类和派生类之间指针或引用的转换

　　上行转换（派生类---->基类）是安全的；

　　下行转换（基类---->派生类）由于没有动态类型检查，所以是不安全的。

　　b、用于基本数据类型之间的转换，如把int转换为char，这种带来安全性问题由程序员来保证

　　c、把空指针转换成目标类型的空指针

　　d、把任何类型的表达式转为void类型

　　3.使用特点

　　a、主要执行非多态的转换操作，用于代替C中通常的转换操作

　　b、隐式转换都建议使用static_cast进行标明和替换

二、dynamic_cast转换

　　1.基本用法：dynamic_cast<type-id> expression

　　2.使用场景：只有在派生类之间转换时才使用dynamic_cast，type-id必须是类指针，类引用或者void*。

　　3.使用特点：

　　a、基类必须要有虚函数，因为dynamic_cast是运行时类型检查，需要运行时类型信息，而这个信息是存储在类的虚函数表中，只有一个类定义了虚函数，才会有虚函数表（如果一个类没有虚函数，那么一般意义上，这个类的设计者也不想它成为一个基类）。

　　b、对于下行转换，dynamic_cast是安全的（当类型不一致时，转换过来的是空指针），而static_cast是不安全的（当类型不一致时，转换过来的是错误意义的指针，可能造成踩内存，非法访问等各种问题）

　　c、dynamic_cast还可以进行交叉转换

三、const_cast转换

　　1.基本用法：const_cast<type-id>expression

　　2.使用场景：

　　a、常量指针转换为非常量指针，并且仍然指向原来的对象

　　b、常量引用被转换为非常量引用，并且仍然指向原来的对象

　　3.使用特点：

　　a、cosnt_cast是四种类型转换符中唯一可以对常量进行操作的转换符

　　b、去除常量性是一个危险的动作，尽量避免使用。一个特定的场景是：类通过const提供重载时，一般都是非常量函数调用const_cast<const T>将参数转换为常量，然后调用常量函数，然后得到结果再调用const_cast <T>去除常量性。

四、reinterpret_cast转换

　　1.基本用法:reinterpret_cast<type-id>expression

　　2.使用场景：不到万不得已，不用使用这个转换符，高危操作

　　3.使用特点：　　

　　a、reinterpret_cast是从底层对数据进行重新解释，依赖具体的平台，可移植性差

　　b、reinterpret_cast可以将整型转换为指针，也可以把指针转换为数组

　　c、reinterpret_cast可以在指针和引用里进行肆无忌惮的转换
```

#### 陷阱30 切割

> 1. 子类和父类在函数参数或者相互赋值，会产生不可预期的行为，应该避免这种切割行为
> 2. 如果是特意进行截断操作，请详细说明，减少日后维护成本

#### 陷阱31 误解指针到常量的转换

```c++
const char* pci; // pointer to const
char* const cpi;// const pointer
char const *pci2; // pointer to const 
const char* const cpci;// const pointer to const
char* ip; // pointer;

size_t strlen(const char*);

int i = strlen(cpi); // ok
pci = ip; //ok
ip = pci; //error
```

#### 陷阱32 误解指向指针的指针到常量的转换

```c++
// n == 3 signature == none none none
int * * * const cnnn = 0;
// n == 3 signature == const none none
int * * const * ncnn = 0;
// signature == none const none
int * const * * nncn = 0;
// signature == const const none
int * const * const * nccn = 0;
// signature == none  none const
const int * * * nnnc = 0;

//examples of application of rules
ncnn = cnnn; // ok
nncn = cnnn;// error
nccn = cnnn;//ok
ncnn = cnnn;// ok
nnnc = cnnn; //error


typedef int T;
const T t = 12345;
T * pt;
const T ** ppt = (const T **)&pt; // an evil cast
*ppt = &t; // put a const T * int a T *
*pt = 54321;// trash t
cout << t;

const T *pct = &t;
// ...
cout << t; // 12345
cout << *pct; // 54321

```

陷阱33 误解指向指针的指针到基类的转换

> 1. 当派生类指针是指向公共基类的指针时，指向派生类指针的指针并不是指向公共基类指针的指针。
> 2. 出现这种情况说明接口设计有问题，设计人员要负责

```c++
D1 d1;
D1 *dlp = &d1;// OK
B **ppb1 = &dlp; // error
D2 * d2p;
B **ppb2 = &d2p; // error
```

#### 陷阱34 指向多维数据的指针问题

````c++ 
int a[5];
int * const pa = a;
int * const * ppa = &pa;
const int alen = sizeof(a)/sizeof(a[0]);// alen == 5

int aa[2][3];
const int aalen = sizeof(aa)/sizeof(aa[0]);// aalen == 2

// 下面的结果令人惊讶
void f(int *, size_t);
void f(void*, size_t);

f(a, alen);
f(aa,aalen);// int (*)[3] 不能转成 int* 但是可以转换成void*

int (* const paa)[3] = aa;
int (* const *ppaa)[3] = &paa;
void f(int(*)[3], size_t);

f(aa, aalen); // ok
````

> 1. 数组名称不占用任何存储单元，所以没有任何地址
> 2. 数组名称代表了那个地址，而数组名称本身没有任何地址
> 3. 能用多维数组的地方最好使用标准容器

#### 陷阱35 未经检查向下转换类型

> 1. 基类类型最好不要转为子类的类型，如果必须要转，使用dynamic_cast(进行类型检查)

#### 陷阱36 无用转换运算符

```c++ 
// 下面的类有太多的转换运算符
class Cell {
  public:
   operator int() const;
   operator double() const;
   operator const char*() const;
};
```

> 1. 使用转换运算符过多，会增加代码的复杂程度。
> 2. 使用转换运算符过多，会使编译产生歧义
> 3. 一个最大只能出现一个转换运算符，超过1个时要重新设计该类
> 4. 声明单参数来避免转换运算符，需要加explict

#### 陷阱37 无意之中的构造函数转换

> 1. 单参数构造函数应该使用explicit，防止隐式转换

#### 陷阱38 多重继承下的转换

```c++
class A {};
class B {};
class C : public A, public B {};
C *cp = new C;
A *ap = cp; // ok
B *bp = bp; // ok 参照c++内存布局
```



####陷阱39 转换不完整的类型

> 1. 不完整的类的类型没有任何意义，但是仍然可以声明指向他们的指针和引用，以及声明使用不完整的类型作为参数，并返回不完整类型结果和函数。
> 2. 使用reinterpret_cast， 不要使用旧式转换。

####陷阱40 旧式转换

> 1. 不要使用旧式转换！！！！

#### 陷阱41 静态转换

> 1. 静态还还包括static_cast, reinterpret_cast, const_cast和旧式转换
> 2. 使用类的静态转换不是好的方法，应该重新设计类或函数。
> 3. 使用静态转换表明你和你的编译器的谈判破裂了 

####陷阱42 形参临时初始化

> 1. 使用引用传参时，千万不要用传值。
> 2. 拒绝参数临时初始化，可以通过函数重载避免参数临时初始化。

```c++
class String {
  public:
   String(cont char* = "");
   ~String();
   friend bool operator == (const String&, const String&);
   friend bool operator != (const String&, const String&);
  private:
   char* s_;
};

String s("hello");
if (s == "heelo") { // 临时初始化
  
}

//解决方案
class String {
  public:
   String(cont char* = "");
   ~String();
   friend bool operator == (const String&, const String&);
   friend bool operator != (const String&, const String&);
   friend bool operator == (const char*, const String&);
   friend bool operator != (const char*, const String&);
   friend bool operator == (const String&, const char*);
   friend bool operator != (const String&, const char*);
  private:
   char* s_;
};

```



####陷阱43 临时变量的声明周期

> 1. 注意临时变量的生命周期，debug可能不存在问题，但是release会出现不确定问题

####陷阱44 引用和临时变量

```c++
int a = 12;
int &r = a;
++a; // ++r
int *ip = &r;// &a
int &r2= 12; //error
// 指向常量引用的初始式仍然必须是左值，但是下面编译器可以从非左值创建引用
const int r3 = 12; // ok

// big/sizes.h
typedef short Int16;
typedef int Int32;

// small/sizes.h
typedef int Int16;
typedef long Int32;
```

> 1. typedef的使用要统一存放，防止不匹配类型的引用。
> 2. 不要返回声明周期不一致的引用作为函数函数结果。

####陷阱45 dynamic_cast的不确定问题

> 1. 优秀的设计结构，一般不会出现类型转换歧义，dynamic_cast转换失败就是歧义的表现。
> 2. dynamic_cast有可能不是动态的，他可能不在运行时执行检查。
> 3. 当使用dynamic_cast将派生类指针转换为它的某个公共基类时，编译器可以静态确定是否成功，不需要运行时检查。
> 4. 如果子类包含两个父类对象就会出现歧义，钻石继承。

#### 陷阱46 误解逆变性

> 1. 特定对象的数据成员的地址并不是成员指针，是一个简单指针
>
>    double * ratep = &hp->rate_; 
>
> 2. 成员指针并不是指针，也不是地址，它不引用任何特定的对象或位置
>
> 3. 成员指针引用不确定对象的确定成员，为了反引用成员指针，必须提供对象。
>
> 4. 数据成员指针一般作为偏移量实现。
>
> 5. 没有从派生类成员指针到公共基类成员指针的转换，反向是合法的。

####陷阱47 混淆赋值和初始化

> 1. 类的参数传递是通过初始化完成，一定要构造自己的构造函数
> 2. 一般来说赋值运算符要求参数是正确初始化的队形，否在在设置新值之前，赋值操作试图清除左边的参数时，可能出现问题
> 3. T d = a; // not an assignment , initialiazation

#### 陷阱48 变量作用域限制不正确

> 1. 声明语句和初始化语句不要分开，否则可能造成莫名其妙的错误，增加维护成本
> 2. 限定名称的作用域与作者使用该名称一致

#### 陷阱49 c++对赋值操作的偏执

> 1. 你自己编写副本操作（operator =），要不就是编译器为你生成，没有任何选择

#### 陷阱50 类对象的按位复制

> 1. 简单的类最好是使用编译器实现的复制操作，这样会更高效

####陷阱51 在构造函数中混淆初始化和赋值

> 1. 在构造函数中，合法要求初始化的所有类成员都被初始化
> 2. 忘记初始化会出现隐藏的错误，但是代码合法
> 3. 在成员初始化列表中执行初始化
> 4. 静态数据成员和数组不能再初始化列表中进行初始化

#### 陷阱52 初始化列表不一致问题

> 1. 初始化列表要和成员的定义顺序一致，除非他们是不想关的

#### 陷阱53 虚拟基类默认初始化

> 1. 多重继承注意构造函数的初始化问题，一般来说让直接子类进行初始化
> 2. 如果 D是B，C的直接子类，B，C是A的直接子类，最好让B，C初始化A
> 3. 虚拟继承和非虚拟继承内存布局问题

#### 陷阱54 副本构造函数基类初始化

```c++
D& D::operator=(const D& that) {
  B::operator=(that);
  m_ = that.m_;
  return *this;
}
```

> 1. 一旦编写覆盖编译器的默认函数，就要对类内这些函数整个实现

#### 陷阱55 运行时静态初始化顺序

> 1. c++所有静态数据在访问之前初始化，如果没有初始化数据将被初始化为0
> 2. 静态初始化同时发生，静态初始化不存在顺序问题
> 3. 尽量少使用外部变量，包括静态数据成员等

#### 陷阱56 直接副本初始化

> 1. 使用直接初始化 如：int i(20)， Y y(100);

#### 陷阱57 直接参数初始化

> 1. 参数传递使用直接初始化实现的
> 2. 尽量避免值传递的初始化

#### 陷阱58 忽略返回值优化

```c++ 
void operator +(String &dest, const String &lhs, const String& rhs) {
  String temp(lhs);
  temp += ths;
  dest.String::String(temp); // new (&dest) String(temp);
  temp.~String();
}
```



> 1. new (&dest) String(temp); 用函数返回值初始化类变量通常比赋值效率高。
> 2. 永远不要给未初始化的存储单元赋值

#### 陷阱59 在构造函数中初始化静态数据成员

> 1. 如果能用枚举代替静态函数，那么一定要用枚举
> 2. 不要尝试运行时静态初始化

#### 陷阱60 错误的区分标量和数组分配

> 1. new 调用operator new, delete 调用 operator delee 
> 2. new delete, malloc free, new [] delete [] 要配套使用
> 3. 可以自己实现 operator new, operator new [] ，必须要有对应的删除

#### 陷阱61 检查特定的内存错误

> 1. 任何情况下，使用new或者成功，或者引发异常，所以检查由于标准使用new产生的错误， 是徒劳的

#### 陷阱62 替换全局的new 和 delete

> 1. 尽管标准允许替换operator new, operator delete, 数组新建或者数组删除的标准版本，但是最好别这样做。
> 2. 比较安全的方法是重载全局的operator new 而不是替换他。
> 3. 虽然可以重载operator delete 但是标准表达式永远不会调用这些重载的版本，必须显示调用

```c++ 
void *operator new(size_t n, const string& pal) {
  char* p = static_cast<char*>(::operator new(n));
  const char* pattern = pat.c_str();
  if (!pattern || !patter[0]) {
    pattern = "\0";
  }
  
  const char* f = pattern;
  for(int i = 0; i < n; ++i) {
    if (!*f) {
      f = pattern;
      p[i] = **f++;
    }
  }
  return p;
}

string fill("<garbage>");
string * str1 = new string("Hello"); // standard version
string* str2 = new (fill) string("world!"); // overloaded version
delete str2;

```

#### 陷阱63 混淆成员new和delete的作用域

```c++ 
class String {
  public:
   void* operator new (size_t);
   void* operator delete (void*);
   void* operator new [](size_t);
   void* operator delete [] (void*);
   String(const char* = "");
};

void f() {
  String *sp = new String("heap"); // uses String::operator new
  int *ip = new int(12); // uses ::operator new
  delete ip;// uses :: operator delete
  delete sp;// uses String::delete
}
// 分配的作用域并不重要，所以分配的类型决定了调用的函数
```

#### 陷阱64 引发字符串异常值

> 1. 不要引发字符串字面值异常
> 2. 异常对象类型传达有关异常的必要信息，而且异常类型没有显示声明成员函数，只是提供一些描述性文本信息
> 3. 如果提供了返回描述性文本，应该通过what函数返回。
> 4. 必须为异常类提供适当的复制和析构语义

#### 陷阱65 不正确的异常机制

> 1. 捕获异常用引用，不要用值
> 2. 异常处理机制可能会增加复杂程度，但并不一定是这样。在引发和捕获异常时，要保持简单化

#### 陷阱66 滥用局部地址

> 1. 不要返回指向局部变量的指针或引用。
> 2. 函数内注意静态变量的影响，以及局部作用域的问题

#### 陷阱67 类成员初始化顺序

> 1. 注意构造函数和析构函数的初始化顺序，必须要求对称性。

####陷阱68 auto_ptr的不适当的使用

> 1. 应该避免使用auto_ptr, 新版本标准已经没有这个关键字了

#### 陷阱70 非虚拟基类析构函数

> 1. 如果基类析构函数不是虚拟的，则通过基类接口销毁派生类，会产生不确定行为
> 2. 如果一个类没有虚析构函数，那么它不应该成为基类
> 3. 单继承情况下，都没有虚拟函数，基类子对象的地址与完整对象的地址是相同的，不是标准，但是基本总是如此
> 4. 其他布局参见P208

```c++
namespace std{
  template <class Arg, class Res>
    struct unary_function{
      typedef Arg argument_type;
      typedef Res result_type;
    };
}

struct Expired : public unary_function<Deal *, bool> {
  bool operator()(const Deal* d) const {
    return d->expired();
  }
}

namespace Loki {
  // 该类只能作为基类使用
  struct OpNewCreator {
    template <class T>
    static T* Create() {return new T;}
    protected:
     ~OpNewCraetor() {}
  };
}
```



#### 陷阱71  隐藏非虚拟函数

> 1. 隐藏基类非虚拟函数会增加使用层次结构的复杂程度，但是没有提供任何可以补偿的优点
> 2. 派生类仍然可以使用被覆盖的基类虚拟函数

```c++
class Msg {
  public:
   virtual void send();
}

class XMSg : public Msg {
  public:
   void send();
}

XMsg * xmsg = new XMsg;
xmsg->send();
xmsg->Msg::send();

class MsgDecorator: public Msg {
  public:
   void send() = 0;
  private:
  Msg *decorated_;
}

inline void MsgDecorator::send() {
  decorated_->send();
}

void BeepDecorator::send() {
  MsgDecorator::send();
}
```

#### 陷阱72 Template Method过于灵活

```c++ 
class Base {
  public:
   void algorithm();
  protected:
   virtual bool hook1()  const;
   virtual bool hook2()  = 0;
};

void Base::algorithm() {
  if (hook1()) { // 调用派生类中被覆盖的受保护的虚拟函数
    hook2()
  }
}


// 下面的设计就过于随意化，模板类方法不应该声明为虚拟的
class Base {
  public:
   virtual ~Base();
   virtual bool verify() const = 0; // 子类必须实现，这个设计并不好
   virtual void doit();
   long id() const;
   void jump();
  protected:
   virtual double howHigh() const;
   virtual double howMayTimes() const = 0;
};
```

#### 陷阱73 重载虚拟函数

```c++
class Thing {
  public:
   virtual void update(int);
   virtual void update(float);
};

class MyThing: public Thing {
  public:
    void update(int);
};

MyThing *mt = new MyThing;
Thing *t = mt;
t->update(12.3);// ok base
mt->update(12.3);// oops derived!

// 对上面改进，不要重载虚拟函数
class Thing {
  public:
    void update(int);
    void update(float);
  protected:
    virtual void updateInt(int);
    virtual void updateFloat(float);
};

inline void Thing::update(int a) {
  updateInt(a);
}
inline void Thing::update(float a) {
  updateFloat(a);
}
```

#### 陷阱74 带有默认参数初始值的虚拟函数

```c++
class Thing {
  public:
   virtual void doitNTimes(int numTimes = 12);
};

class MyThing : public Thing {
  // 默认构造函数具有传递性（静态应用），这个10没有起作用，仍然是12次
  void doitNTimes(int numTimes = 10);
};

// 代码改进
class Thing {
  void doitNTimes(int numTimes = 12) {
    doitNTimesImpl(numTimes);
  }
  protected:
  virtual void doitNTimesImpl(int numTimes);
};
```

#### 陷阱75  在构造函数和析构函数中调用虚拟函数

> 1. 在构造函数和析构函数中，禁止调用虚拟函数（具体原因P235）
> 2. 不要再析构函数中抛出异常

```c++
// 下面的方法是比较好的实现方法
class B {
  public:
   B() {}
   virtual ~B() {}
};

class D : public B {
  public:
   D() {}
   ~D() {}
};

D x;
```

#### 陷阱76 虚拟赋值

```c++
// 虚拟赋值是允许的，但是找不到这么做的理由
template <typename T>
class Container {
  public:
   virtual Container &operator=(const T &) = 0;
};

template <typename T>
class List : public Container<T> {
  List &operator= (const T&);
};
template <typename T>
class Array : public Container<T> {
  Array &operator= (const T&);
};

Container<int> &c(getCurrentContainer());
c = 12; // is the meaning clear?

// 使用非歧义的非运算符函数
template <typename T>
class Container {
  public:
   virtual voidsetAll(const T &) = 0;
};

Container<int> &c(getCurrentContainer());
c = 12; //  meaning is clear


// Prototype模式
template <typename T>
class Container {
  public:
   virtual Container *clone() = 0;
};

template <typename T>
class List : public Container<T> {
  List(const List&);
  List *clone () const {return new List(*this);}
};
template <typename T>
class Array : public Container<T> {
  Array(const List&);
  Array *clone () const {return new Array(*this);}
};

Container<int> *cp = getCurrentContainer();
Container<int> *cp2 = cp->clone();
```

#### 陷阱77 没有区分重载、覆盖和隐藏

> 1. 只能在存在基类虚拟函数的情况下出现覆盖。
> 2. 覆盖和重载没有任何关系
> 3. 不能覆盖，只能隐藏非虚拟基类函数

```c++
class Processor {
  public:
   virtual ~Processor();
   bool process(Credit&);
   bool process(Acceptance&);
   bool process(OrderForm*);
};


class MyProcessor : public Processor {
  public:
   bool process(Rejection&); // 隐藏了基类的process
};

class MyProcessor : public Processor {
  public:
   using  Processor::process; // 引入基类的process
   bool process(Rejection&); // 隐藏了基类的process
};


class Doer {
  public:
   virtual ~Doer();
   bool doit(Credit&);
   virtual bool doit(Acceptance&);
   virtual bool doit(OrderForm&);
   virtual bool doit(Rejcetion&);
};

class MYDoer : public Doer {
  private:
   bool doit(Credit&);// 隐藏了基类的4个函数
   bool doit(Acceptance&); // 覆盖了对应基类的函数
   virtual bool doit(Rejcetion&) const;// 没有覆盖，新的函数
   double doit(OrderForm&);// error 和基类对应函数返回值不一致
};
```

```c++
class Vistor {
  public:
   virtual void visit(Acceptance&);
   virtual void visit(Credit&);
   virtual void visit(OrderForm&);
   virtual int numHits();
};

class ValidVisitor : public Visitor {
  void visit(Acceptance&);//覆盖
  void visit(Credit&);//覆盖
  int numHits(int);//非虚函数
};

class MyValidVisitor : public ValidVisitor {
  void visit(Acceptance&);//覆盖
  void visit(Credit&);//覆盖
  int numHits(int);//虚函数 覆盖Visitor::numHits
};
```



#### 陷阱78 错误理解虚拟函数和覆盖

> 1. 彻底理解虚函数表，特别是多继承的虚函数表。

#### 陷阱79 支配问题

> 1. 钻石继承问题，可以利用虚继承解决。
> 2. 虚继承不会产生歧义。

```c++
＃include<iostream>
using namespace std;
class A {};
class B:virtual public A {};
class C:public virtual A {};
```

#### 陷阱80 Get/Se接口

> 1. 一个类中，不应该只是Get/Set的组合，要明确给出类的实际操作
> 2. Get/Set接口设计可以使用下面的代码的方法。

```c++
class C {
 public:
  int getValue() const {
    return value_;
  }
  void setValue(int value) {
    value_ = value;
  }
 private:
  int value_;
};
```

#### 陷阱81 定常和引用数据成员

> 1. 类中几乎没有必要生成定常或者引用数据成员，定常或者引用数据成员会增加维护的难度。

```c++
// 简单的类，包含定常和应用数据成员
class C {
  public:
   c();
  private:
  int a_;
  const int b_;
  int &ra_;
};

C::C() :
     a_(12),b_(12),ra_(a_) {}

C x;
C y(x);// 这里如果使用默认实现，y的ra_指向x的a_

// 手写拷贝构造
C::C(const C& that) :
    a_(that.a_), b_(that.b_), ra_(a_) {}

x = y; // error, b_和 ra_无法赋值，引用和const无法赋值

// 下面的代码被否定，会带来很多问题
// C 是基类， 派生类赋值运算可能调用C的赋值运算，如果析构函数是虚拟的
// 则它将销毁整个对象，而不是C的部分。
// 如果析构函数不是虚拟的，则运行行为不确定
C &C::operator =(const C &that) {
  if (this != &that) {
    this->C();
    new (this) c(that);
  }
  return *this;
}
// 最后只有一个办法，避免定常和引用数据成员
```

#### 陷阱82 没有理解定常成员函数的含义

```c++
class BoundedString {
  public:
   explicit BoundedString(int len);
   size_t length() const; // 函数是定常的，返回值不是定常的，不应该修改len_的值
   void set(char c);
   void wipe() const;// 不能修改任意成员变量的值
   bool operator <(const BoundedString&);// error 尝试用定常对象的地址初始化费定常成员函数的this指针
  private:
   char* const buf_; // 指向的字符不是常量
   int len_;
   mutable int len2_; // 可以用在非静态，非定常，非引用数据成员，以指出定常成员函数可以安全的修改它
   size_t maxLen_;
};
```

> 1. 对于类X的非定常成员函数而言，this指针的类型是X* const。
> 2. 对于类X的定常成员函数而言，this指针的类型是const X*。

#### 陷阱83 没有区分聚合与"熟悉"

```c++
class Employee {
 public:
  virtual ~Employee();
  void setRole(Role* newRole);
  const Role *getRole() const;
 private:
  Role * role_;
};
Employee *e1 = getMeAnEmployee();
Employee *e2 = getMeAnEmployee();
Role * r = getMeSomethingToDo();
e1->setRole(r); 
e2->setRole(r); // bug 删除e1 e2会重复删除r对象
delete r; // e1 e2包含虚悬指针

// 下面的设计可以解决上面的问题
class Employee {
 public:
  virtual ~Employee();
  void addRole(Role* newRole);
  void shareRole(const Role* newRole);
  void copyRole(const Role* newRole);
  const Role *getRole() const;
 private:
  Role * role_;
};
```

#### 陷阱84 不正确的运算符重载

> 1. 不适用运算符重载也可以编程
> 2. 运算符重载仅仅是语法上的美化，有助于阅读和编写代码
> 3. 不要设计违背常理的运算符重载逻辑
> 4. 重载运算符必须得到统一的认识
> 5. 除非重载运算符很明显的要优于非运算符，否则就不要重载

```c++
// 可以使用下面的代替运算符重载
class Complex {
  public:
   Complex(double real = 0.0, double imag = 0.0);
   friend Complex add (const Complex&, const Complex&);// +
   friend Complex sub (const Complex&, const Complex&);// -
   friend Complex div (const Complex&, const Complex&);// /
   friend Complex mul (const Complex&, const Complex&);// *
};
```

####陷阱85 优先级和重载

> 1. 运算符的重载不能改变其优先级，如果改变优先级就应该放弃运算符重载
> 2. 确保重载运算符的优先级满足用户的期望。

```c++
// 可以使用下面的代替运算符重载
class Complex {
  public:
   Complex(double real = 0.0, double imag = 0.0);
   friend Complex operator + (const Complex&, const Complex&);// +
   friend Complex operator * (const Complex&, const Complex&);// -
   friend Complex operator ^ (const Complex&, const Complex&);// /
};

// 希望将^修改成幂数运算
a = -1 + e ^ (i * pi); 
a = -1 + (e ^ (i * pi)); // 本来的期望
a = (-1 + e) ^ (i * pi); // 实际结果
```

#### 陷阱86 友元与成员运算符

> 1. 重载运算符应该允许应用它的参数类型所支持的任何转换。

```c++
class Complex {
 public:
  Complex(double re = 0.0, double im = 0.0)
};

Complex add(const Complex&, const Complex&);
Complex c1, c2;
double d;

add(c1, c2);
add(c1, d); // add (c1, Complex(d, 0.0))
add(d, c1); // add ( Complex(d, 0.0), c1)

```

#### 陷阱87 增量和减量运算符

> 1. 前缀返回可修改的左值，使用前缀实现后缀
> 2. 尽量使用前缀

#### 陷阱88 误解模板化的复制操作

> 1. 模板成员函数永远不应该实现复制操作。
> 2. 再设计任何类时必须考虑到复制操作。

#### 陷阱89 类对象数组

```c++
void apply(Base array[], int length, void(*f)( B &)) {
  for (int i = 0; i < length; ++i) {
    f(array[i]);
  }
}

D* dp = new D[3];
apply（dp, 3, somefun);// 子类不适用，偏移大小不同，代码有严重问题

// 对上面问题的改进

void apply(Base * array[], int length, void(*f)( B *)) {
  for (int i = 0; i < length; ++i) {
    f(array[i]);
  }
}
```

#### 陷阱91 错误的理解受保护的访问

> 1. 如果希望通过派生类来表现基类接口，则可以使用公共继承
> 2. 私有继承几乎专门用于继承实现。
> 3. 派生类指针转换为基类指针，说明设计很糟糕，需要重新设计层析结构
> 4. 派生类可以使用基类受保护的成员函数。

```c++
class Inst {
  public:
   int units() const;
  private:
   int units_;
};

class Sbond : private Inst {
  //
};

void doUnits() {
  Sbind * bp = getNextBond();
  Inst * ip = (Inst *)bp;
  bp->unitss(); // error
  ip->units();// legal
}


```

```c++
class Inst {
  public:
   virtual ~Inst();
  protected:
   int units() const;
  private:
   int units_;
};
class Sbond : private Inst {
  public:
   double notional() const {
     return units() * faceval_;
   }
  private:
   double faceval_;
};

class Equity : public Inst {
  public:
   double notional() const {
     return units() * shareval_;
   }
   bool compare(Bond* bp) const {
     int bunits = bp->units(); // error! protected只能在友元或继承类访问，可以设置友元解决这个问题
     return units() < bunits;
   }
  private:
   double shareval_;
};

```

#### 陷阱92 为代码重用而是用公共继承

> 1. 如果主要是为了在派生类中重用基类的实现而使用公共继承，那么经常会产生不自然的，无法维护的设计

##### Command设计模式

```c++
// H
#ifndef _COMMAND_H_
#define _COMMAND_H_

class Command
{
public:
    virtual ~Command();
    virtual void Execute()=0;
protected:
    Command();
private:
};

class Receiver;

class ConcreteCommand : public Command
{
public:
    ConcreteCommand(Receiver* pReceiver);
    ~ConcreteCommand();
    virtual void Execute();
protected:
private:
    Receiver* _recv;
};

class Invoker
{
public:
    Invoker(Command* pCommand);
    ~Invoker();
    void Invoke();
protected:
private:
    Command* _cmd;
};

class Receiver
{
public:
    Receiver();
    ~Receiver();
    void Action();
protected:
private:
};
#endif
//CPP
#include "Command.h"
#include <iostream>

using namespace std;

Command::Command()
{}

Command::~Command()
{}

ConcreteCommand::ConcreteCommand(Receiver* pReceiver)
{
    this->_recv = pReceiver;
}

ConcreteCommand::~ConcreteCommand()
{}

void ConcreteCommand::Execute()
{
    this->_recv->Action();
}

Receiver::Receiver()
{}

Receiver::~Receiver()
{}

void Receiver::Action()
{
    cout << "Receiver::Action" << endl;
}

Invoker::Invoker(Command* pCommand)
{
    this->_cmd = pCommand;
}

Invoker::~Invoker()
{}

void Invoker::Invoke()
{
    this->_cmd->Execute();
}

//Main
#include "Command.h"

int main()
{
    //创建具体命令对象pCmd并设定它的接收者pRev
    Receiver* pRev = new Receiver();
    Command* pCmd = new ConcreteCommand(pRev);
    //请求绑定命令
    Invoker* pInv = new Invoker(pCmd);
    pInv->Invoke();

    return 0;
}
```

#####Composite设计模式

```c++
// H
#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include <vector>

using namespace std;

/*
Component抽象基类，为组合中的对象声明接口,声明了类共有接口的缺省行为(如这里的Add,Remove,GetChild函数),
声明一个接口函数可以访问Component的子组件.
*/
class Component
{
public:
    //纯虚函数，只提供接口，没有默认的实现
    virtual void Operation()=0;    

    // 虚函数,提供接口,有默认的实现就是什么都不做
    virtual void Add(Component*);
    virtual void Remove(Component*);
    virtual Component* GetChild(int index);
    virtual ~Component();
protected:
    Component();
};

//Leaf是叶子结点,也就是不含有子组件的结点类，所以不用实现Add、Remove、GetChild等方法
class Leaf:public Component
{
public:
    //只实现Operation接口
    virtual void Operation();            
    Leaf();
    ~Leaf();
};

//Composite：含有子组件的类
class Composite:public Component
{
public:
    Composite();
    ~Composite();
    //实现所有接口
    void Operation();
    void Add(Component*);
    void Remove(Component*);
    Component* GetChild(int index);
private:
    //这里采用vector来保存子组件
    vector<Component*> m_ComVec;        
};
#endif
//CPP
#include "Composite.h"
#include <iostream>

using namespace std;

Component::Component()
{}

Component::~Component()
{}

void Component::Add(Component* com)
{
    cout << "add" << endl;
}

void Component::Remove(Component* com)
{
}

void Component::Operation()
{
    cout << "Component::Operation" << endl;
}

Component* Component::GetChild(int index)
{
    return NULL;
}


Leaf::Leaf()
{}

Leaf::~Leaf()
{}

void Leaf::Operation()
{
    cout<< "Leaf::Operation" <<endl;
}

Composite::Composite()
{
}

Composite::~Composite()
{}

void Composite::Add(Component* com)
{
    this->m_ComVec.push_back(com);
}

void Composite::Remove(Component* com)
{
    this->m_ComVec.erase(&com);
}

void Composite::Operation()
{
    cout << "Composite::Operation" << endl;
    vector<Component*>::iterator iter = this->m_ComVec.begin();
    for(;iter!= this->m_ComVec.end();iter++)
    {
        (*iter)->Operation();
    }
}

Component* Composite::GetChild(int index)
{
    if(index < 0 || index > this->m_ComVec.size())
    {
        return NULL;
    }
    return this->m_ComVec[index];
}
//Main
#include "Composite.h"
#include <iostream>

using namespace std;

int main()
{
    /*
      不管是叶子Leaf还是Composite对象pRoot、pCom都实现了Operation接口，所以可以一致对待，直接调用Operation()
      体现了“使得用户对单个对象和组合对象的使用具有一致性。”
    */
    Composite* pRoot = new Composite();

    //组合对象添加叶子节点
    pRoot->Add(new Leaf());

    Leaf* pLeaf1 = new Leaf();
    Leaf* pLeaf2 = new Leaf();

    //这里的叶子再添加叶子是没有意义的。
    //由于叶子与组合对象继承了相同的接口，所以语法上是对的，实际上什么也没做(继承自基类Component的Add方法)。
    //叶子节点只实现了Operation方法，其他Add、Remove、GetChild都继承自基类，没有实际意义。
    pLeaf1->Add(pLeaf2);
    pLeaf1->Remove(pLeaf2);
    //执行叶子Operation操作
    pLeaf1->Operation();

    //组合对象实现了基类Component的所有接口，所以可以做各种操作(Add、Remove、GetChild、Operation)。
    Composite* pCom = new Composite();
    //组合对象添加叶子节点
    pCom->Add(pLeaf1);
    //组合对象添加叶子节点
    pCom->Add(pLeaf2);
    //执行组合对象Operation操作
    pCom->Operation();

    //组合对象添加组合对象
    pRoot->Add(pCom);

    //执行组合对象Operation操作
    pRoot->Operation();

    //Component* cp = pCom->GetChild(0);
    //cp->Operation();

    //pCom->Remove(pLeaf1);

    return 0;
}
```

##### Decorator模式

```c++
#include <iostream>
using namespace std;
 
class Component
{
public:
	virtual void Operation() = 0;
};
 
class concreteComponent :public Component
{
public:
	void Operation()
	{
		cout << "this is a concreteComponent, not a decorator." << endl;
	}
};
 
class Decorator :public Component
{
public:
	Decorator(Component *p) : p_Component(p) {}
	void Operation()
	{
		if (p_Component != NULL)
		{
			p_Component->Operation();
		}
	}
 
private:
	Component *p_Component;
};
 
class DecoratorA :public Decorator
{
public:
	DecoratorA(Component *p) : Decorator(p) {}
	void Operation()
	{
		add_status();
		Decorator::Operation();
	}
 
	void add_status()
	{
		cout << "I am DecoratorA. " << endl;
	}
};
 
class DecoratorB :public Decorator
{
public:
	DecoratorB(Component *p) : Decorator(p) {}
	void Operation()
	{
		add_bahavior();
		Decorator::Operation();
	}
 
	void add_bahavior()
	{
		cout << "I am DecoratorB. " << endl;
	}
};
 
int main(int argc, char*argv[])
{
	Component* object = new concreteComponent();
	Decorator *a = new DecoratorA(object);
	a->Operation();
	cout << "-----------------------------------------------------" << endl;
 
	Decorator *b = new DecoratorB(object);
	b->Operation();
	cout << "------------------------------------------------------" << endl;
 
	Decorator *ab = new DecoratorB(a);
	ab->Operation();
}
```

#### 陷阱93 具体公共基类

> 1. 公共基类不应该是具体的，应该是抽像的

#### 陷阱94 错误的使用退化层次结构

> 1. 再设计最初，要区分基类和独立类
> 2. 独立类成为基类时一种灾难

#### 陷阱95 滥用继承

> 1. 继承不能超过3层
> 2. 能用组合尽量用组合代替继承

#### 陷阱96 基于类型的控制编码

> 1. 使用多态代替类型控制编码

##### Proxy模式

```c++
//H
#ifndef _PROXY_H_
#define _PROXY_H_

// 定义了Proxy和ConcreteSubject的公有接口,
// 这样就可以在任何需要使用到ConcreteSubject的地方都使用Proxy.
class Subject
{
public:
    virtual ~Subject();
    virtual void Request()=0;
protected:
    Subject();
};

class ConcreteSubject : public Subject
{
public:
    ConcreteSubject();
    ~ConcreteSubject();
    virtual void Request();
};

//定义代理类
class Proxy : public Subject
{
public:
    Proxy();
    ~Proxy();
    void DoSomething1();
    virtual void Request();
    void DoSomething2();
private:
    Subject* _subject;
};
#endif

//CPP
#include "Proxy.h"
#include "iostream"

using namespace std;

Subject::Subject()
{}

Subject::~Subject()
{}

ConcreteSubject::ConcreteSubject()
{}

ConcreteSubject::~ConcreteSubject()
{}

void ConcreteSubject::Request()
{
    cout << "ConcreteSubject::Request" << endl;
}

Proxy::Proxy() : _subject(NULL)
{}

Proxy::~Proxy()
{}

void Proxy::DoSomething1()
{
    cout << "Proxy::DoSomething1" << endl;
}

void Proxy::DoSomething2()
{
    cout << "Proxy::DoSomething2" << endl;
}

void Proxy::Request()
{
    if(NULL == this->_subject)
    {
        this->_subject = new ConcreteSubject();
    }

    this->DoSomething1();//表示额外附加的操作

    this->_subject->Request();//代理的实体类操作

    this->DoSomething2();//表示额外附加的操作
}
// main
#include "Proxy.h"

int main()
{
    Proxy* proxy = new Proxy();
    proxy->Request();

    return 0;
}
```

#### 陷阱97 “宇宙”层次结构

> 1. 宇宙层次结构是每个类都从根类派生。
> 2. 体系结构应该尽可能的灵活，这种假设是错误的，体系结构应该是尽可能与问题领域接近，同时保持足够的灵活性，允许将来进行合理的扩展。

