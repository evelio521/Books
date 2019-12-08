/*将与参数无关的代码抽离template*/

template<typename T,size_t n>
class SquareMatrix{
	
	public:
		void invert();
};

SquareMatrix<double,5> sm1;
...
sm1.invert();//SquareMatrix<double,5>::invert

SquareMatrix<double,10> sm2;
...
sm2.invert();//SquareMatrix<double,10>::invert

//以上代码可能会造成代码膨胀 可以用以下方法改进
   //1.
    template<typename T>
    class SquareMatrixBase{
    	
    	protected:
    		void invert(size_t matrixSize);
    	
    };
    template<typename T,size_t n>
	class SquareMatrix:private SquareMatrixBase{
		private:
			using SquareMatrixBase<T>::invert;
		public:
			void invert() { this->invert(n);};//调用base的invert
};
   //2.
   template<typename T>
    class SquareMatrixBase{
    	
    	protected:
    		SquareMatrixBase(size_t n,T* pMem):size(n),pData(pMem){}
    		void setDataPtr(T* ptr){pData=ptr;};
    	private:
    		size_t size;//矩阵大小
    		T* pData;//指向矩阵的内容
    	
    };
    template<typename T,size_t n>
	class SquareMatrix:private SquareMatrixBase{
		private:
			T data[n*n];
		public:
			SquareMatrix():SquareMatrixBase<T>(n,data);
			void invert() { this->invert(n);};//调用base的invert
};
  //3.
  template<typename T>
    class SquareMatrixBase{
    	
    	protected:
    		SquareMatrixBase(size_t n,T* pMem):size(n),pData(pMem){}
    		void setDataPtr(T* ptr){pData=ptr;};
    	private:
    		size_t size;//矩阵大小
    		T* pData;//指向矩阵的内容
    	
    };
    template<typename T,size_t n>
	class SquareMatrix:private SquareMatrixBase{
		private:
			T data[n*n];
		public:
			SquareMatrix():SquareMatrixBase<T>(n,0),pData(new T[n*n])
				{
					this->setDataPte(pData.get());
				}
		private:
			boost::scoped_array<T> pData;
			
};
  