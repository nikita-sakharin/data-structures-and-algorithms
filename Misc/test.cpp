#include <iostream>
#include <functional>
#include <vector>

typedef signed char schar;
typedef unsigned char uchar;
typedef short shrt;
typedef unsigned short ushrt;
typedef unsigned uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;
typedef float flt;
typedef double dbl;
typedef long double ldbl;

using namespace std;

template<typename T, typename... T1> class Tuple : public Tuple<T1...> {
public:
    T val;
    Tuple<T1...>* next = static_cast<Tuple<T1...>>(this);
    Tuple(T v, T1... others) : val(v), Tuple<T1...>(others...){

    }
};


template<typename T> class Tuple<T> {
public:
    T val;
    Tuple(T v) : val(v){

    }
};


int main(){
    Tuple<int, double, char> t(1, 2.3, 'a');
    cout<<t.next->val<<endl;
}
