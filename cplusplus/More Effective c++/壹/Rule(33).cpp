����:33
	/*����β�������Ϊ������*/
	
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
		//����ֻд���˸�ֵ���㺯�������Ѿ�������æ��һ���ˡ��������Ĵ��룺
		Lizard liz1;
		Lizard liz2;
		Animal *pAnimal1 = &liz1;
		Animal *pAnimal2 = &liz2;
		...
		*pAnimal1 = *pAnimal2;
		//�������������⡣
		/*
		��һ�����һ�еĸ�ֵ������õ���Animal��ģ���Ȼ��ض����������Lizard��
		      �����ֻ��liz1��Animal���ֱ��޸ġ����ǲ��ָ�ֵ���ڸ�ֵ��liz1��
		      Animal��Ա����������liz2��ֵ������Lizard��Ա����û���ı䡣
		�ڶ�,����������г���Ա�Ѵ���д����������ָ����������ֵ�����ټ����ر�
				 ����Щ��C�зḻ�����ת�Ƶ�C++�ĳ���Ա�����ԣ�����Ӧ�ý���ֵ��Ƶ�
				 ������ġ���Item M32ָ���ģ����ǵ���Ӧ�����ױ���ȷ���ö������ױ��ô�
				 ������������������ױ��ô�
	  */
	  
	  //�����������Ľ���취�ǽ���ֵ��������Ϊ�麯�������Animal::operator=��
	  //�麯�����Ǿ丳ֵ��佫����Lizard�ĸ�ֵ������Ӧ�ñ����õİ汾��
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
			  
		//�������ƵĴ���������
		//����һ��������͸�ֵ�������һ��Lizard���ұ���һ��Chicken��������͸�ֵ��
		//C++��ͨ���������⣬��ΪC++��ǿ����ԭ���������ǷǷ���Ȼ����ͨ����Animal��
		//��ֵ������Ϊ�麯�������Ǵ��˻�����Ͳ������š�
		
		//��ʹ�����Ǵ������ѡ�����Ӧ������ͨ��ָ�����ͬ���͸�ֵ������ֹͨ��ͬ����ָ��
		//���л�����͸�ֵ�����仰˵������������������
			Animal *pAnimal1 = &liz1;
			Animal *pAnimal2 = &liz2;
			...
			*pAnimal1 = *pAnimal2; // assign a lizard to a lizard
			//�����ֹ������
			Animal *pAnimal1 = &liz;
			Animal *pAnimal2 = &chick;
			...
			*pAnimal1 = *pAnimal2; // assign a chicken to a lizard
			
			//���������޸ĺ����������Ľ���취
			//���ǿ���ʹ��dynamic_cast����Item M2����ʵ�֡���������ôʵ��Lizard�ĸ�ֵ������
			Lizard& Lizard::operator=(const Animal& rhs)
			{
			// make sure rhs is really a lizard
			const Lizard& rhs_liz = dynamic_cast<const Lizard&>(rhs);
			proceed with a normal assignment of rhs_liz to *this;
			}
			
			//�������ֻ��rhsȷʵ��Lizard����ʱ��������*this�����rhs����Lizard���ͣ�
			//�������ݳ�dynamic_castת��ʧ��ʱ�׵�bad_cast���͵��쳣��
			
			Lizard liz1, liz2;
			...
			liz1 = liz2; // no need to perform a
			// dynamic_cast: this
			// assignment must be valid
		//���ǿ��Դ�������������������Ӹ��ӶȻ򻨷�dynamic_cast��
		//ֻҪ��Lizard������һ��ͨ����ʽ�ĸ�ֵ������
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
		//ʵ���ϣ������˺����Ǹ���operator=��Ҳ�ͼ���ǰ�ߵ�ʵ�֣�
		Lizard& Lizard::operator=(const Animal& rhs)
		{
		return operator=��dynamic_cast<const Lizard>(rhs));
		}
		
		//�������������ͼ��rhsת��Ϊһ��Lizard�����ת���ɹ���ͨ���ĸ�ֵ���������ã�
		//����һ��bad_cast�쳣���׳���
		
		//��������������ǻ������ҵ���һ���ⷨ
		//�����׵ķ�������Animal�н�operator=��Ϊprivate�����ǣ�Lizard������Ը�ֵ��Lizard����Chicken������Ը�ֵ��Chicken���󣬵����ֻ������͸�ֵ����ֹ��
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
		//���ҵ��ǣ�AnimalҲ��ʵ���࣬�������ͬʱ��Animal�����ĸ�ֵ����Ϊ�Ƿ��ˣ�
		Animal animal1, animal2;
		...
		animal1 = animal2; // error! attempt to call
		// private Animal::operator=
		//���ң���Ҳʹ�ò�������ȷʵ��Lizard��Chicken��ĸ�ֵ��������Ϊ������ĸ�ֵ��
		//�����������ε��������ĸ�ֵ����������
		Lizard& Lizard::operator=(const Lizard& rhs)
		{
		if (this == &rhs) return *this;
		Animal::operator=(rhs); // error! attempt to call
		// private function. But
		// Lizard::operator= must
		// call this function to
		... // assign the Animal parts
		} // of *this!

   //����������Ȼ�������⡣��������Animal�����ĸ�ֵ����ֹLizard��Chicken����ͨ��Animal��ָ����в��ָ�ֵ��
   //������������Ȼ���ڡ��������ô�죿
   
   //���յĽ���취
          
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
			//�����Ƹ�����������Ҫ�Ķ�����ͬ���ͼ�ĸ�ֵ���������ָ�ֵ��ͬ���ͼ�ĸ�ֵ����ֹ��
			//������ĸ�ֵ�����������Ե��û���ĸ�ֵ�������������⣬�����漰Aniaml��Lizard��Chicken
			//��Ĵ��붼����Ҫ�޸ģ���Ϊ��Щ����Ȼ����������Ϊ������AbstractAnimalǰ������һ�¡�
			//�϶�����Щ������Ҫ���±��룬������Ϊ��á�ȷ���˱���ͨ���ĸ�ֵ������Ϊ����ȷ�Ķ���Ϊ
			//���ܲ���ȷ�ĸ�ֵ��䲻�ܱ���ͨ�����������ĺ�С�Ĵ��ۡ�
			
			
			//�����������ʵ����C1��C2������ϲ��C2���м̳���C1����Ӧ�ý�������ļ̳в�θ�Ϊ������ļ�
			//�в�Σ�ͨ������һ���µĳ�����A����C1��C2�������̳У�
			
			
			       C1                   A
			       |                  /   \
			       C2                C1   C2