#include <vector>
#define DECLARE_STACK(Prefix, T) \ 
class Prefix##Stack \
{\
	public: \
		void Push(T val); \
	    T Pop(); \
	    bool IsEmpty() const; \
	\
	private: \
		std::vector<T> mStack; \ 
};
DECLARE_STACK(Int, int);
