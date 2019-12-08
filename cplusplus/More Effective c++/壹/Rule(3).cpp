规则3:
	/*绝不要以多态方式处理数组*/
		class BST { ... };
		class BalancedBST: public BST { ... };

		void printBSTArray(ostream& s,const BST array[],int numElements)
		{
			for (int i = 0; i < numElements; ) {
			s << array[i]; //假设BST类
			} //重载了操作符<<
		}
		//当你传递给该函数一个含有BST对象的数组变量时，它能够正常运行：
		BST BSTArray[10];
		...
		printBSTArray(cout, BSTArray, 10); // 运行正常
		//然而，请考虑一下，当你把含有BalancedBST对象的数组变量传递给printBSTArray函数时，会产生什么样的后果：
		BalancedBST bBSTArray[10];
		...
		printBSTArray(cout, bBSTArray, 10); // 还会运行正常么？
		//你的编译器将会毫无警告地编译这个函数，但是再看一下这个函数的循环代码：
		for (int i = 0; i < numElements; ) {
		s << array[i];
		}
		/*
		 *	这里的array[I]只是一个指针算法的缩写：它所代表的是*(array)。
		 *	我们知道array是一个指向数组起始地址的指针，但是array中各元素
		 *	内存地址与数组的起始地址的间隔究竟有多大呢？它们的间隔是
		 *	i*sizeof(一个在数组里的对象)，因为在array数组[0]到[I]间有I个
		 *	对象。编译器为了建立正确遍历数组的执行代码，它必须能够确定数
		 *	组中对象的大小，这对编译器来说是很容易做到的。参数array被声明
		 *	为BST类型，所以array数组中每一个元素都是BST类型，因此每个元素
		 *	与数组起始地址的间隔是i*sizeof(BST)。
		 */
		 
		 //如果你试图删除一个含有派生类对象的数组，将会发生各种各样的问题。
		 //以下是一种你可能采用的但不正确的做法。
		 
		 //删除一个数组, 但是首先记录一个删除信息
			void deleteArray(ostream& logStream, BST array[])
			{
			logStream << "Deleting array at address"<< static_cast<void*>(array) << '\n';
			delete [] array;
			}
			BalancedBST *balTreeArray =new BalancedBST[50]; // 建立一个BalancedBST对象数组
			...
			deleteArray(cout, balTreeArray); // 记录这个删除操作
			
			//这里面也掩藏着你看不到的指针算法。当一个数组被删除时，每一个数组元素的析构函
			//数也会被调用。当编译器遇到这样的代码：
			delete [] array;
			 //它肯定象这样生成代码：
			// 以与构造顺序相反的顺序来
		 // 解构array数组里的对象
			for ( int i = 数组元素的个数 1; i >= 0;--i)
				{
					array[i].BST::~BST(); // 调用 array[i]的
					} // 析构函数
			//因为你所编写的循环语句根本不能正确运行，所以当编译成可执行代码后，
			//也不可能正常运行。语言规范中说通过一个基类指针来删除一个含有派生
			//类对象的数组，结果将是不确定的。这实际意味着执行这样的代码肯定不
			//会有什么好结果。多态和指针算法不能混合在一起来用，所以数组与多态
			//也不能用在一起。