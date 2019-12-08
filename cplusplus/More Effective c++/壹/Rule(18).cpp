规则:18
	/*分期摊还预期的计算成本*/
	template<class NumericalType>
	class DataCollection {
		public:
			NumericalType min() const;
			NumericalType max() const;
			NumericalType avg() const;
			...
	};
	//假设min,max和avg函数分别返回现在这个集合的最小值，最大值和平均值，
	//有三种方法实现这三种函数。使用eager evaluation(热情计算法)，当
	//min,max和avg函数被调用时，我们检测集合内所有的数值，然后返回一个
	//合适的值。使用lazy evaluation（懒惰计算法）,只有确实需要函数的返
	//回值时我们才要求函数返回能用来确定准确数值的数据结构。使用 
	//over-eager evaluation（过度热情计算法），我们随时跟踪目前集合的
	//最小值，最大值和平均值，这样当min,max或avg被调用时，我们可以不用
	//计算就立刻返回正确的数值。如果频繁调用min,max和avg，我们把跟踪集合
	//最小值、最大值和平均值的开销分摊到所有这些函数的调用上，每次函数调
	//用所分摊的开销比eager evaluation或lazy evaluation要小。
	
	
	//over-eager evaluation的方法
	
	  //1.采用over-eager最简单的方法就是caching(缓存)那些已经被计算出来
	  //  而以后还有可能需要的值。
	  
	  //例如你编写了一个程序，用来提供有关雇员的信息，这些信息中的经常被需
	  //要的部分是雇员的办公隔间号码。而假设雇员信息存储在数据库里，但是对
	  //于大多数应用程序来说，雇员隔间号都是不相关的，所以数据库不对查抄它
	  //们进行优化。为了避免你的程序给数据库造成沉重的负担，可以编写一个函
	  //数findCubicleNumber，用来缓存查找到的数据。以后需要已经被获取的隔
	  //间号时，可以在cache里找到，而不用向数据库查询。
	  	int findCubicleNumber(const string& employeeName)
			{
					// 定义静态map，存储 (employee name, cubicle number)
					// pairs. 这个 map 是local cache。
					typedef map<string, int> CubicleMap;
					static CubicleMap cubes;
					// try to find an entry for employeeName in the cache;
					// the STL iterator "it" will then point to the found
					// entry, if there is one (see Item 35 for details)
					CubicleMap::iterator it = cubes.find(employeeName);
					// "it"'s value will be cubes.end() if no entry was
					// found (this is standard STL behavior). If this is
					// the case, consult the database for the cubicle
					// number, then add it to the cache
					if (it == cubes.end()) {
					int cubicle =
							the result of looking up employeeName’s cubicle
							number in the database;
					cubes[employeeName] = cubicle; // add the pair
					// (employeeName, cubicle)
					// to the cache
					return cubicle;
			}
			
		//2.Prefetching(预提取)是另一种方法
		
		
		template<class T> // dynamic数组
		class DynArray { ... }; // 模板
		DynArray<double> a; // 在这时, 只有 a[0]
		// 是合法的数组元素
		a[22] = 3.5; // a 自动扩展
		//: 现在索引0－22
		// 是合法的
		a[32] = 0; // 有自行扩展;
		// 现在 a[0]-a[32]是合法的
		//一个DynArray对象如何在需要时自行扩展呢？
		//一种直接的方法是分配所需的额外的内存。就象这样：
		template<class T>
		T& DynArray<T>::operator[](int index)
		{
		if (index < 0) {
		throw an exception; // 负数索引仍不合法
		}
		if (index >当前最大的索引值) {
			
		  //调用new分配足够的额外内存，以使得索引合法;
		}
		   // 返回index位置上的数组元素;
		}
		
		//使用Over-eager evaluation方法，其原因我们现在必须增加数组的尺寸
		//以容纳索引i，那么根据位置相关性原则我们可能还会增加数组尺寸以在
		//未来容纳比i大的其它索引。为了避免为扩展而进行第二次（预料中的）
		//内存分配，我们现在增加DynArray的尺寸比能使i 合法的尺寸要大，我
		//们希望未来的扩展将被包含在我们提供的范围内。例如我们可以这样编写
		//DynArray::operator[]：
		template<class T>
		T& DynArray<T>::operator[](int index)
		{
				if (index < 0) throw an exception;
				if (index > 当前最大的索引值) {
				int diff = index – 当前最大的索引值;
				//调用new分配足够的额外内存，使得index+diff合法;
				}
				//返回index位置上的数组元素;
		}
		//这个函数每次分配的内存是数组扩展所需内存的两倍
		
		DynArray<double> a; // 仅仅a[0]是合法的
		a[22] = 3.5; // 调用new扩展
		// a的存储空间到索引44
		// a的逻辑尺寸
		// 变为23
		a[32] = 0; // a的逻辑尺寸
		// 被改变，允许使用a[32],
		// 但是没有调用new