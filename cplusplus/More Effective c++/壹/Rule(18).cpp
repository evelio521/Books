����:18
	/*����̯��Ԥ�ڵļ���ɱ�*/
	template<class NumericalType>
	class DataCollection {
		public:
			NumericalType min() const;
			NumericalType max() const;
			NumericalType avg() const;
			...
	};
	//����min,max��avg�����ֱ𷵻�����������ϵ���Сֵ�����ֵ��ƽ��ֵ��
	//�����ַ���ʵ�������ֺ�����ʹ��eager evaluation(������㷨)����
	//min,max��avg����������ʱ�����Ǽ�⼯�������е���ֵ��Ȼ�󷵻�һ��
	//���ʵ�ֵ��ʹ��lazy evaluation��������㷨��,ֻ��ȷʵ��Ҫ�����ķ�
	//��ֵʱ���ǲ�Ҫ��������������ȷ��׼ȷ��ֵ�����ݽṹ��ʹ�� 
	//over-eager evaluation������������㷨����������ʱ����Ŀǰ���ϵ�
	//��Сֵ�����ֵ��ƽ��ֵ��������min,max��avg������ʱ�����ǿ��Բ���
	//��������̷�����ȷ����ֵ�����Ƶ������min,max��avg�����ǰѸ��ټ���
	//��Сֵ�����ֵ��ƽ��ֵ�Ŀ�����̯��������Щ�����ĵ����ϣ�ÿ�κ�����
	//������̯�Ŀ�����eager evaluation��lazy evaluationҪС��
	
	
	//over-eager evaluation�ķ���
	
	  //1.����over-eager��򵥵ķ�������caching(����)��Щ�Ѿ����������
	  //  ���Ժ��п�����Ҫ��ֵ��
	  
	  //�������д��һ�����������ṩ�йع�Ա����Ϣ����Щ��Ϣ�еľ�������
	  //Ҫ�Ĳ����ǹ�Ա�İ칫������롣�������Ա��Ϣ�洢�����ݿ�����Ƕ�
	  //�ڴ����Ӧ�ó�����˵����Ա����Ŷ��ǲ���صģ��������ݿⲻ�Բ鳭��
	  //�ǽ����Ż���Ϊ�˱�����ĳ�������ݿ���ɳ��صĸ��������Ա�дһ����
	  //��findCubicleNumber������������ҵ������ݡ��Ժ���Ҫ�Ѿ�����ȡ�ĸ�
	  //���ʱ��������cache���ҵ��������������ݿ��ѯ��
	  	int findCubicleNumber(const string& employeeName)
			{
					// ���徲̬map���洢 (employee name, cubicle number)
					// pairs. ��� map ��local cache��
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
							the result of looking up employeeName��s cubicle
							number in the database;
					cubes[employeeName] = cubicle; // add the pair
					// (employeeName, cubicle)
					// to the cache
					return cubicle;
			}
			
		//2.Prefetching(Ԥ��ȡ)����һ�ַ���
		
		
		template<class T> // dynamic����
		class DynArray { ... }; // ģ��
		DynArray<double> a; // ����ʱ, ֻ�� a[0]
		// �ǺϷ�������Ԫ��
		a[22] = 3.5; // a �Զ���չ
		//: ��������0��22
		// �ǺϷ���
		a[32] = 0; // ��������չ;
		// ���� a[0]-a[32]�ǺϷ���
		//һ��DynArray�����������Ҫʱ������չ�أ�
		//һ��ֱ�ӵķ����Ƿ�������Ķ�����ڴ档����������
		template<class T>
		T& DynArray<T>::operator[](int index)
		{
		if (index < 0) {
		throw an exception; // ���������Բ��Ϸ�
		}
		if (index >��ǰ��������ֵ) {
			
		  //����new�����㹻�Ķ����ڴ棬��ʹ�������Ϸ�;
		}
		   // ����indexλ���ϵ�����Ԫ��;
		}
		
		//ʹ��Over-eager evaluation��������ԭ���������ڱ�����������ĳߴ�
		//����������i����ô����λ�������ԭ�����ǿ��ܻ�����������ߴ�����
		//δ�����ɱ�i�������������Ϊ�˱���Ϊ��չ�����еڶ��Σ�Ԥ���еģ�
		//�ڴ���䣬������������DynArray�ĳߴ����ʹi �Ϸ��ĳߴ�Ҫ����
		//��ϣ��δ������չ���������������ṩ�ķ�Χ�ڡ��������ǿ���������д
		//DynArray::operator[]��
		template<class T>
		T& DynArray<T>::operator[](int index)
		{
				if (index < 0) throw an exception;
				if (index > ��ǰ��������ֵ) {
				int diff = index �C ��ǰ��������ֵ;
				//����new�����㹻�Ķ����ڴ棬ʹ��index+diff�Ϸ�;
				}
				//����indexλ���ϵ�����Ԫ��;
		}
		//�������ÿ�η�����ڴ���������չ�����ڴ������
		
		DynArray<double> a; // ����a[0]�ǺϷ���
		a[22] = 3.5; // ����new��չ
		// a�Ĵ洢�ռ䵽����44
		// a���߼��ߴ�
		// ��Ϊ23
		a[32] = 0; // a���߼��ߴ�
		// ���ı䣬����ʹ��a[32],
		// ����û�е���new