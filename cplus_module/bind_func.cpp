#include <iostream>
#include <functional>
#include <vector>

using namespace std;

class Object {
public:
    Object() {}
    virtual ~Object() {}

public:
    void setValue(int tmp) {
        val = tmp;
    }

    void printValue() {
        std::cout << "this value:" << val << std::endl;
    }

private:
    int val = 0;
};

int main() {
    Object tmp1;
    tmp1.setValue(10);

    Object tmp2;
    tmp2.setValue(20);

    vector<std::function<void()>> infos;
    infos.push_back(std::bind(&Object::printValue, &tmp1));
    infos.push_back(std::bind(&Object::printValue, &tmp2));
    for (auto func : infos) {
        func();
    }
    return 0;
}