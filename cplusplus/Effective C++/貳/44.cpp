#include<iostream>
using namespace std;
/*
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

//���ϴ�����ܻ���ɴ������� ���������·����Ľ�*/
/*
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
			void invert() { this->invert(n);};//����base��invert
};
*/
//2
template<typename T>
    class SquareMatrixBase{
    	
    	protected:
    		SquareMatrixBase(size_t n,T* pMem):size(n),pData(pMem){}
    		void setDataPtr(T* ptr){pData=ptr;}
			void invert(size_t matrixSize){
				cout<<"success";
				for(int i=0;i<matrixSize;i++)
					cout<<pData[i]<<endl;
			}
    	private:
    		size_t size;//�����С
    		T* pData;//ָ����������
    	
    };
    template<typename T,size_t n>
	class SquareMatrix:private SquareMatrixBase<T>{
		private:
			using SquareMatrixBase<T>::invert;
			T data[n*n];
		public:
			SquareMatrix():SquareMatrixBase<T>(n,data){}
			void invert() { this->invert(n);}//����base��invert
};
/*
 //3.
  template<typename T>
    class SquareMatrixBase{
    	
    	protected:
    		SquareMatrixBase(size_t n,T* pMem):size(n),pData(pMem){}
    		void setDataPtr(T* ptr){pData=ptr;};
    	private:
    		size_t size;//�����С
    		T* pData;//ָ����������
    	
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
*/

int main(){
	SquareMatrix<int,5> sm;
	sm.invert();
	
	
}