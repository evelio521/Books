/*�˽���ʽ�ӿںͱ����ڶ�̬*/

//����template���ԣ��ӿ�����ʽ�ģ�������Ч���ʽ��
//��̬����ͨ��template���ֻ��ͺ������ؽ��������ڱ�����
template<typename T>
void doProcessing(T& w)
{
	if(w.size()>0 && w!=someNastyWidget){
		{
			T temp(w);
			temp.normalize();
			temp.swap(w);
		}
}