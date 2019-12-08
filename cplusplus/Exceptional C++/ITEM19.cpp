ITEM19:代码复杂性
	//考虑下列函数是异常安全的还是一场中立的
	String EvaluateSalaryAndReturnName( Employee e ) 
{
  if( e.Title() == "CEO" || e.Salary() > 100000 )
  {
    cout << e.First() << " " << e.Last() << " is overpaid" << endl;
  }
  return e.First() + " " + e.Last();
}

 //basic guarantee是保证可销毁且没有泄漏
 //strong guarantee是保证可销毁且没有泄漏外还保证commit-to-rollback
 
 //流可能产生不可恢复的作用，即运算符<<输出了string一部分后抛出异常，
 //被输出的那个部分是不能反输出的，当然流的错误状态也可能被设置。
 //本题我们暂时不考虑这个。
 
 
 //以上的代码满足basic guarantee
 //第一次修改
 // Attempt #1: An improvement? 
	//
	String EvaluateSalaryAndReturnName( Employee e )
	{
	  String result = e.First() + " " + e.Last();
	  if( e.Title() == "CEO" || e.Salary() > 100000 )
	  {
	    String message = result + " is overpaid\n";
	    cout << message;
	  }
	
	  return result;
	}
	
  //考虑下列代码
  String theName;
  theName=EvaluateSalaryAndReturnName(bob);
  //函数采用的by value的方式返回，因此会有copy assignment operator和
  //拷贝构造函数被唤起如果有一个失败，那么副作用就会产生
  //为了避免拷贝构造，我们做一次啊修改
  // Attempt #2: Better now? 
		//
		void EvaluateSalaryAndReturnName( Employee e,
		                                  String&  r )
		{
		  String result = e.First() + " " + e.Last();
		  if( e.Title() == "CEO" || e.Salary() > 100000 )
		  {
		    String message = result + " is overpaid\n";
		    cout << message;
		  }
		
		  r = result;
		}
 //由于r的赋值可能失败，因此做第三次尝试
 // Attempt #3: Correct (finally!). 
	//
	auto_ptr<String>
	EvaluateSalaryAndReturnName( Employee e )
	{
	  auto_ptr<String> result
	      = new String( e.First() + " " + e.Last() );
	
	  if( e.Title() == "CEO" || e.Salary() > 100000 )
	  {
	    String message = (*result) + " is overpaid\n";
	    cout << message;
	  }
	
	  return result;  // rely on transfer of ownership;
	                  // this can't throw
	}
	//只要打印结果不抛出异常 ，我们就能把函数结果安全的返回给调用者
	
   //终点
   1.要对异常安全性提供保证，经常你要放弃一部分性能为代价
   2.如果一个函数有多重副作用，那么无法成为强异常安全，解决办法就是拆分成几个函数
   3.并不是所有的函数都需要异常安全。本题中第一次尝试已经足够好用，并不需要
     第三次那样损失一部分性能。