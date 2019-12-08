规则:33
	/*将非尾端类设计为抽象类*/
	
	      Animal
	      /    \
	  Lizard   Chicken
	  
	 class Animal {
		public:
			Animal& operator=(const Animal& rhs);
			...
		};
	 class Lizard: public Animal {
		public:
			Lizard& operator=(const Lizard& rhs);
			...
		};
	 class Chicken: public Animal {
		public:
			Chicken& operator=(const Chicken& rhs);
			...
		};
		//这里只写出了赋值运算函数，但已经够我们忙乎一阵了。看这样的代码：
		Lizard liz1;
		Lizard liz2;
		Animal *pAnimal1 = &liz1;
		Animal *pAnimal2 = &liz2;
		...
		*pAnimal1 = *pAnimal2;
		//这里有两个问题。
		/*
		第一，最后一行的赋值运算调用的是Animal类的，虽然相关对象的类型是Lizard。
		      结果，只有liz1的Animal部分被修改。这是部分赋值。在赋值后，liz1的
		      Animal成员有了来自于liz2的值，但其Lizard成员部分没被改变。
		第二,问题是真的有程序员把代码写成这样。用指针来给对象赋值并不少见，特别
				 是那些对C有丰富经验而转移到C++的程序员。所以，我们应该将赋值设计得
				 更合理的。如Item M32指出的，我们的类应该容易被正确适用而不容易被用错，
				 而上面这个类层次是容易被用错。
	  */
	  
	  //针对上述问题的解决办法是将赋值运算申明为虚函数。如果Animal::operator=是
	  //虚函数，那句赋值语句将调用Lizard的赋值操作（应该被调用的版本）
	  class Animal {
			public:
				virtual Animal& operator=(const Animal& rhs);
				...
		};
		class Lizard: public Animal {
			public:
				virtual Lizard& operator=(const Animal& rhs);
				...
		};
		class Chicken: public Animal {
			public:
				virtual Chicken& operator=(const Animal& rhs);
				...
		};
		Lizard liz;
		Chicken chick;
		Animal *pAnimal1 = &liz;
		Animal *pAnimal2 = &chick;
		...
		*pAnimal1 = *pAnimal2; // assign a chicken to
		// a lizard!
			  
		//上面的设计的带来的问题
		//这是一个混合类型赋值：左边是一个Lizard，右边是一个Chicken。混合类型赋值在
		//C++中通常不是问题，因为C++的强类型原则将评定它们非法。然而，通过将Animal的
		//赋值操作设为虚函数，我们打开了混合类型操作的门。
		
		//这使得我们处境艰难。我们应该允许通过指针进行同类型赋值，而禁止通过同样的指针
		//进行混合类型赋值。换句话说，我们想允许这样：
			Animal *pAnimal1 = &liz1;
			Animal *pAnimal2 = &liz2;
			...
			*pAnimal1 = *pAnimal2; // assign a lizard to a lizard
			//而想禁止这样：
			Animal *pAnimal1 = &liz;
			Animal *pAnimal2 = &chick;
			...
			*pAnimal1 = *pAnimal2; // assign a chicken to a lizard
			
			//对于上述修改后带来的问题的解决办法
			//我们可以使用dynamic_cast（见Item M2）来实现。下面是怎么实现Lizard的赋值操作：
			Lizard& Lizard::operator=(const Animal& rhs)
			{
			// make sure rhs is really a lizard
			const Lizard& rhs_liz = dynamic_cast<const Lizard&>(rhs);
			proceed with a normal assignment of rhs_liz to *this;
			}
			
			//这个函数只在rhs确实是Lizard类型时将它赋给*this。如果rhs不是Lizard类型，
			//函数传递出dynamic_cast转换失败时抛的bad_cast类型的异常。
			
			Lizard liz1, liz2;
			...
			liz1 = liz2; // no need to perform a
			// dynamic_cast: this
			// assignment must be valid
		//我们可以处理上述情况而无需增加复杂度或花费dynamic_cast，
		//只要在Lizard中增加一个通常形式的赋值操作：
		class Lizard: public Animal {
		public:
		virtual Lizard& operator=(const Animal& rhs);
		Lizard& operator=(const Lizard& rhs); // add this
		...
		};
		Lizard liz1, liz2;
		...
		liz1 = liz2; // calls operator= taking
		// a const Lizard&
		Animal *pAnimal1 = &liz1;
		Animal *pAnimal2 = &liz2;
		...
		*pAnimal1 = *pAnimal2; // calls operator= taking
		// a const Animal&
		//实际上，给出了后面那个的operator=，也就简化了前者的实现：
		Lizard& Lizard::operator=(const Animal& rhs)
		{
		return operator=（dynamic_cast<const Lizard>(rhs));
		}
		
		//现在这个函数试图将rhs转换为一个Lizard。如果转换成功，通常的赋值操作被调用；
		//否则，一个bad_cast异常被抛出。
		
		//解决上述问题我们还可以找到另一个解法
		//最容易的方法是在Animal中将operator=置为private。于是，Lizard对象可以赋值给Lizard对象，Chicken对象可以赋值给Chicken对象，但部分或混合类型赋值被禁止：
		class Animal {
		private:
		Animal& operator=(const Animal& rhs); // this is now
		... // private
		};
		class Lizard: public Animal {
		public:
		Lizard& operator=(const Lizard& rhs);
		...
		};
		class Chicken: public Animal {
			public:
		Chicken& operator=(const Chicken& rhs);
		...
		};
		Lizard liz1, liz2;
		...
		liz1 = liz2; // fine
		Chicken chick1, chick2;
		...
		chick1 = chick2; // also fine
		Animal *pAnimal1 = &liz1;
		Animal *pAnimal2 = &chick1;
		...
		*pAnimal1 = *pAnimal2; // error! attempt to call
		// private Animal::operator=
		//不幸的是，Animal也是实体类，这个方法同时将Animal对象间的赋值评定为非法了：
		Animal animal1, animal2;
		...
		animal1 = animal2; // error! attempt to call
		// private Animal::operator=
		//而且，它也使得不可能正确实现Lizard和Chicken类的赋值操作，因为派生类的赋值操
		//作函数有责任调用其基类的赋值操作函数：
		Lizard& Lizard::operator=(const Lizard& rhs)
		{
		if (this == &rhs) return *this;
		Animal::operator=(rhs); // error! attempt to call
		// private function. But
		// Lizard::operator= must
		// call this function to
		... // assign the Animal parts
		} // of *this!

   //上述代码依然存在问题。但“允许Animal对象间的赋值而阻止Lizard和Chicken对象通过Animal的指针进行部分赋值”
   //的两难问题仍然存在。程序该怎么办？
   
   //最终的解决办法
          
                   AbstractAnimal
              /        |          \
       Lizard       Animal          Chicken
       
       
       
       	class AbstractAnimal {
					protected:
						AbstractAnimal& operator=(const AbstractAnimal& rhs);
					public:
						virtual ~AbstractAnimal() = 0; // see below
						...
				};
				class Animal: public AbstractAnimal {
					public:
						Animal& operator=(const Animal& rhs);
						...
				};
				class Lizard: public AbstractAnimal {
					public:
						Lizard& operator=(const Lizard& rhs);
						...
				};
				class Chicken: public AbstractAnimal {
					public:
						Chicken& operator=(const Chicken& rhs);
						...
				};
			//这个设计给你所以你需要的东西。同类型间的赋值被允许，部分赋值或不同类型间的赋值被禁止；
			//派生类的赋值操作函数可以调用基类的赋值操作函数。此外，所有涉及Aniaml、Lizard或Chicken
			//类的代码都不需要修改，因为这些类仍然操作，其行为与引入AbstractAnimal前保持了一致。
			//肯定，这些代码需要重新编译，但这是为获得“确保了编译通过的赋值语句的行为是正确的而行为
			//可能不正确的赋值语句不能编译通过”所付出的很小的代价。
			
			
			//如果你有两个实体类C1和C2并且你喜欢C2公有继承自C1，你应该将两个类的继承层次改为三个类的继
			//承层次，通过创造一个新的抽象类A并将C1和C2都从它继承：
			
			
			       C1                   A
			       |                  /   \
			       C2                C1   C2