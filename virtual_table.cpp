#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual void func1() {}
    virtual void func2() {}
};

class Derived : public Base {
public:
    void func1() override {}
    void func2() override {}
    //virtual void func3() {}
};

int main() {
    Derived d;
    Base* b = &d;

    // 获取虚指针地址
    intptr_t* vptr = *(reinterpret_cast<intptr_t**>(b));
    
    std::cout << "Virtual Table Address: " << vptr << std::endl;

    // 打印虚函数表中的内容，直到找到NULL指针为止
    int count = 0;
    while (vptr[count] != 0) {
        std::cout << "VTable[" << count << "]: " << reinterpret_cast<void*>(vptr[count]) << std::endl;
        count++;
    }

    std::cout << "Size of Virtual Table: " << count << " entries" << std::endl;

    return 0;
}

