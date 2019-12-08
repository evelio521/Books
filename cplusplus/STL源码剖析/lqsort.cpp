#include<cstdlib>
#include<iostream>

int fcmp(const void * eleml,const void* elem2);

void mian(){
	
	int ia[10]={32,92,67,58,10,4,25,52,59,54};
	
	for(int i=0,i<10;i++)
				cout<<ia[i]<<" ";
				
	qsort(ia,size_of(ia)/size_of(int),size_of(int),fcmp);
	
  for(int i=0,i<10;i++){
				cout<<ia[i]<<" ";
				
}
int fcmp(const void * eleml,const void* elem2){
	
	const int* i1=(const int*)elem1;
	const int* i2=(const int*)elem2;
	
	if(*i1<*i2)
		return -1;
	else if(*i1==*i2)
		return 0;
	else if(*i1>*i2)
		return 1;
}