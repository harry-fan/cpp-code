#include <iostream>

using namespace std;
class FBase final {
public:
    FBase() {}

    virtual ~FBase() {}

    void test() {
        cout << "this is test base" << endl;
    }
};

/* error : tag final no inherit */
//class FBaseA : public FBase {
//
//};

class Base {
public:
    virtual void test() {
        cout << "this is test base" << endl;
    }

    /* private Base cant new */
private:
    Base() {}

    virtual ~Base() {}
};

/* error : private Base can not create */
//class BaseA : public Base {
//public:
//    BaseA();
//    virtual ~BaseA();
//};

int main() {
    FBase a;
    a.test();
}