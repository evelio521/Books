#include<new> 
class EquipmentPiece {
		public:
			EquipmentPiece(int IDNumber){}
		
		};
		
	
		
		int main(){
			
	 // EquipmentPiece bestPieces[10]; // 错误！没有正确调用
                                   // EquipmentPiece 构造函数
   // EquipmentPiece *bestPieces =new EquipmentPiece[10];
                                  // 错误！与上面的问题一样
    
    //解决方法
    int ID1=0, ID2=1, ID3=2,ID4=3,ID5=4,ID6=5,ID7=6,ID8=7,ID9=8,ID10=9; // 存储设备ID号的
                                 // 变量
	int ID[10]={0,1,2,3,4,5,6,7,8,9};	
		EquipmentPiece bestPieces[] = { // 正确, 提供了构造
		EquipmentPiece(ID1), // 函数的参数
		EquipmentPiece(ID2),
		EquipmentPiece(ID3),
		EquipmentPiece(ID4),
		EquipmentPiece(ID5),
		EquipmentPiece(ID6),
		EquipmentPiece(ID7),
		EquipmentPiece(ID8),
		EquipmentPiece(ID9),
		EquipmentPiece(ID10)};
			
		//一个更通用的解决方法是利用指针数组来代替一个对象数组
		typedef EquipmentPiece* PEP; // PEP 指针指向
                                 //一个EquipmentPiece对象
		PEP bestPieces1[10]; // 正确, 没有调用构造函数
		PEP *bestPieces2 = new PEP[10]; // 也正确
		//在指针数组里的每一个指针被重新赋值，以指向一个不同的EquipmentPiece对象：
		//不过这中方法有两个缺点，第一你必须删除数组里每个指针所指向的对象。
		//如果你忘了，就会发生内存泄漏。第二增加了内存分配量，因为正如你需
		//要空间来容纳EquipmentPiece对象一样，你也需要空间来容纳指针。
		for (int i = 0; i < 10; ++i)
			bestPieces2[i] = new EquipmentPiece(ID[i]);
		
		delete [] bestPieces2;
		
		
		//如果你为数组分配raw memory，你就可以避免浪费内存。
		// 为大小为10的数组 分配足够的内存
		// EquipmentPiece 对象; 详细情况请参见条款M8
		// operator new[] 函数
		void *rawMemory =operator new[](10*sizeof(EquipmentPiece));
		// make bestPieces point to it so it can be treated as an
		// EquipmentPiece array
		EquipmentPiece *bestPieces3 =
		static_cast<EquipmentPiece*>(rawMemory);
		// construct the EquipmentPiece objects in the memory
		// 使用"placement new" (参见条款M8)
		for (int i = 0; i < 10; ++i)
					new (&bestPieces3[i]) EquipmentPiece( ID[i] );

						
	  //使用placement new的缺点除了是大多数程序员对它不熟悉外
	  //（能使用它就更难了），还有就是当你不想让它继续存在使用时，
	  //必须手动调用数组对象的析构函数，然后调用操作符delete[]来释放raw memory
	  
	  // 以与构造bestPieces对象相反的顺序解构它。
			for (int i = 9; i >= 0; --i)
				bestPieces3[i].~EquipmentPiece();
		// deallocate the raw memory
			operator delete[](rawMemory);
		//如果你忘记了这个要求而使用了普通的数组删除方法，
		//那么你程序的运行将是不可预测的。这是因为：直接删除
		//一个不是用new操作符来分配的内存指针，其结果没有被定义。
		//delete [] bestPieces; // 没有定义! bestPieces不是用new操作符分配的。
}
		/*
			
		//对于类里没有定义缺省构造函数所造成的第二个问题是它们无法
		//在许多基于模板（template-based）的容器类里使用。因为实例
		//化一个模板时，模板的类型参数应该提供一个缺省构造函数，这是
		//一个常见的要求。这个要求总是来自于模板内部，被建立的模板参
		//数类型数组里。例如一个数组模板类：
		
		template<class T>
		class Array {
		public:
			Array(int size);
			...
		private:
			T *data;
		};
		template<class T>
		Array<T>::Array(int size)
		{
			data = new T[size]; // 为每个数组元素
			... //依次调用 T::T()
		}
		//不提供缺省构造函数的虚基类，很难与其进行合作。
		//因为几乎所有的派生类在实例化时都必须给虚基类
		//构造函数提供参数。这就要求所有由没有缺省构造
		//函数的虚基类继承下来的派生类(无论有多远)都必
		//须知道并理解提供给虚基类构造函数的参数的含义。
		//派生类的作者是不会企盼和喜欢这种规定的。
		
		

		//这样的修改使得其他成员函数变得复杂，因为不再能
		//确保EquipmentPiece对象进行了有意义的初始化。假
		//设它建立一个因没有ID而没有意义的EquipmentPiece对象，
		//那么大多数成员函数必须检测ID是否存在。如果不存在ID，
		//它们将必须指出怎么犯的错误。不过通常不明确应该怎么去做，
		//很多代码的实现什么也没有提供：只是抛出一个异常或调用一个
		//函数终止程序。当这种情形发生时，很难说提供缺省构造函数
		//而放弃了一种保证机制的做法是否能提高软件的总体质量。
		class EquipmentPiece {
			public:
				EquipmentPiece( int IDNumber = UNSPECIFIED);
				...
			private:
				static const int UNSPECIFIED; // 其值代表ID值不确定。
			};
			//这允许这样建立EquipmentPiece对象
			EquipmentPiece e; //这样合法
			*/