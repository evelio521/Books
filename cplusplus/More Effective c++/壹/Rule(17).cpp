规则:17
	/*考虑使用缓式评估*/
	
	//1.引用计数
	
		class String { ... }; // 一个string 类 (the standard
		// string type may be implemented
		// as described below, but it
		// doesn't have to be)
		String s1 = "Hello";
		String s2 = s1; / 调用string拷贝构造函数
		//通常string拷贝构造函数让s2被s1初始化后，s1和s2都有自己的”Hello”拷贝。
		//这种拷贝构造函数会引起较大的开销，因为要制作s1值的拷贝，并把值赋给s2，
		//这通常需要用new操作符分配堆内存(参见条款8)，需要调用strcpy函数拷贝s1内的数据到s2。
		
		//这是一个eager evaluation（热情计算）：只因为到string拷贝构造函数，就
		//要制作s1值的拷贝并把它赋给s2。然而这时的s2并不需要这个值的拷贝，因为s2没有被使用。
		//懒惰能就是少工作。不应该赋给s2一个s1的拷贝，而是让s2与s1共享一个值。
		//我们只须做一些记录以便知道谁在共享什么，就能够省掉调用new和拷贝字符的开销。事实上
		//s1和s2共享一个数据结构，这对于client来说是透明的，对于下面的例子来说，这没有什么
		//差别，因为它们只是读数据：
			cout << s1; // 读s1的值
			cout << s1 + s2; // 读s1和s2的值
			
	  	s2.convertToUpperCase();
			//这是至关紧要的，仅仅修改s2的值，而不是连s1的值一块修改。
			//为了这样执行语句，string的convertToUpperCase函数应该制作s2值的一个拷贝，
			//在修改前把这个私有的值赋给s2。在convertToUpperCase内部，我们不能再懒惰了：
			//必须为s2（共享的）值制作拷贝以让s2自己使用。另一方面，如果不修改s2，
			//我们就不用制作它自己值的拷贝。继续保持共享值直到程序退出。如果我们很幸运，
			//s2不会被修改，这种情况下我们永远也不会为赋给它独立的值耗费精力。

       //这种共享值方法的实现细节（包括所有的代码）在条款M29中被提供，但是其蕴含的原则就是lazy
			//evaluation：除非你确实需要，不去为任何东西制作拷贝。我们应该是懒惰的，
			//只要可能就共享使用其它值。在一些应用领域，你经常可以这么做。
			
	//2.区别对待读取和写入
				String s = "Homer's Iliad"; // 假设是一个
			// reference-counted string
			...
			cout << s[3]; // 调用 operator[] 读取s[3]
			s[3] = 'x'; // 调用 operator[] 写入 s[3]
		//首先调用operator[]用来读取string的部分值，但是第二次调用该函数是为了完成写操作。
		//我们应能够区别对待读调用和写调用，因为读取reference-counted string是很容易的，
		//而写入这个string则需要在写入前对该string值制作一个新拷贝。条款30有相信介绍


 //3.Lazy Fetching（懒惰提取）
 		class LargeObject { // 大型持久对象
				public:
						LargeObject(ObjectID id); // 从磁盘中恢复对象
						const string& field1() const; // field 1的值
						int field2() const; // field 2的值
						double field3() const; // ...
						const string& field4() const;
						const string& field5() const;
						...
			};
		//现在考虑一下从磁盘中恢复LargeObject的开销：
		void restoreAndProcessObject(ObjectID id)
		{
				LargeObject object(id); // 恢复对象
				...
		}
		//因为LargeObject对象实例很大，为这样的对象获取所有的数据，
		//数据库的操作的开销将非常大，特别是如果从远程数据库中获取
		//数据和通过网络发送数据时。而在这种情况下，不需要读去所有数据。
		//例如，考虑这样一个程序：
		void restoreAndProcessObject(ObjectID id)
		{
				LargeObject object(id);
				if (object.field2() == 0) {
						cout << "Object " << id << ": null field2.\n";
				}
		}
		
		//这里仅仅需要filed2的值，所以为获取其它字段而付出的努力都是浪费。
		
		   //解决方法
		   class LargeObject {
					public:
							LargeObject(ObjectID id);
							const string& field1() const;
							int field2() const;
							double field3() const;
							const string& field4() const;
							...
					private:
							ObjectID oid;
							mutable string *field1Value; //参见下面有关
							mutable int *field2Value; // "mutable"的讨论
							mutable double *field3Value;
							mutable string *field4Value;
							...
				};
				LargeObject::LargeObject(ObjectID id)
				: oid(id), field1Value(0), field2Value(0), field3Value(0), ...
				{}
				const string& LargeObject::field1() const
				{
						if (field1Value == 0) {
						从数据库中为filed 1读取数据，使field1Value 指向这个值;
					}
					return *field1Value;
				}
				//对象中每个字段都用一个指向数据的指针来表示，LargeObject构造
				//函数把每个指针初始化为空。这些空指针表示字段还没有从数据库中
				//读取数值。每个LargeObject成员函数在访问字段指针所指向的数据
				//之前必须字段指针检查的状态。如果指针为空，在对数据进行操作之
				//前必须从数据库中读取对应的数据。
				
	//4.Lazy Expression Evaluation(懒惰表达式计算)
	  
	  template<class T>
		class Matrix { ... }; // for homogeneous matrices
		Matrix<int> m1(1000, 1000); // 一个 1000 ＊ 1000 的矩阵
		Matrix<int> m2(1000, 1000); // 同上
		...
		Matrix<int> m3 = m1 + m2; // m1＋m2
		
		//lazy evaluation方法说这样做工作太多，所以还是不要去做。
		//而是应该建立一个数据结构来表示m3的值是m1与m2的和，在用
		//一个enum表示它们间是加法操作。很明显，建立这个数据结构
		//比m1与m2相加要快许多，也能够节省大量的内存。
		
		//考虑程序后面这部分内容，在使用m3之前，代码执行如下：
		Matrix<int> m4(1000, 1000);
		... // 赋给m4一些值
		m3 = m4 * m1;
		//现在我们可以忘掉m3是m1与m2的和（因此节省了计算的开销），
		//在这里我们应该记住m3是m4与m1运算的结果。不必说，我们不
		//用进行乘法运算。因为我们是懒惰的，还记得么？
		
		cout << m3[4]; // 打印m3的第四行
		//很明显，我们不能再懒惰了，应该计算m3的第四行值。但是我
		//们也不能雄心过大，我们没有理由计算m3第四行以外的结果；
		//m3其余的部分仍旧保持未计算的状态直到确实需要它们的值。
		//很走运，我们一直不需要。
		
		//公正地讲，懒惰有时也会失败。如果这样使用m3：
		cout << m3; // 打印m3所有的值
		//一切都完了，我们必须计算m3的全部数值。同样如果修改m3所依
		//赖的任一个矩阵，我们也必须立即计算：
		m3 = m1 + m2; // 记住m3是m1与m2的和
		m1 = m4; // 现在m3是m2与m1的旧值之和！
		
		//为lazy evaluation卖命是值得的。可以换取大量的时间和空间