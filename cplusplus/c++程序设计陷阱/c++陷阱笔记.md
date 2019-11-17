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

