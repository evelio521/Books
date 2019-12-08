集中精力设计API中80%功能的20%的类
Liskov原则 OCP 迪米特法则
避免过程的参数列表 应该用struct代码 structEx扩展

class Ellipse {
public:
    Ellipse();
    Ellipse(float major, float minor);
    void SetMajorRadius(float major);
    void SetMinorRadius(float minor);
    float GetMajorRadius() const;
    float GetMinorRadius() const;
private:
    float mMajor; float mMinor;
};


class Circle {
public:
    Circle();
    explicit Circle(float r);
    void SetRadius(float r);
    float GetRadius() const;
private:
    Ellipse mEllipse;
};


//There are many interface options you can control when designing a function call (Lakos, 1996). 
//First of all,for free functions you should consider the following alternatives:

1.Static versus non-static function.
2.Pass arguments by value, reference, or pointer.
3.Pass arguments as const or non-const.
4.Use of optional arguments with default values.
5.Return result by value, reference, or pointer.
6.Return result as const or non-const.
7.Operator or non-operator function.
8. Use of exception specifications.

//For member functions, you should consider all of these free function options as well as the following:
• Virtual versus non-virtual member function.
• Pure virtual versus non-pure virtual member function.
• Const versus non-const member function.
• Public, protected, or private member function.
• Use of the explicit keyword for non-default constructors.

i//In addition to these options that control the logical interface of a function, there are a couple of organizational attributes that you can specify for a function, such as
• Friend function versus non-friend function.
• Inline function versus non-inline function.
