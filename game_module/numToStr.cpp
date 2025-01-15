//
// Created by Administrator on 2025/1/10.
//
#include <iostream>
#include <string>

std::string numToStr(long long number) {
    if (number == 0) {
        return "0";
    }
    std::string result;
    while (number > 0) {
        int chunk = number % 1000;
        number /= 1000;
        auto strCuk = std::to_string(chunk);
        if (number && strCuk.size() < 3) {
            if (result.empty()) {
                result = std::string(3 - strCuk.size(), '0') + strCuk;
            } else {
                result = std::string(3 - strCuk.size(), '0') + strCuk + "," + result;
            }
        } else {
            result = result.empty() ? strCuk : (strCuk + "," + result);
        }
    }
    return result;
}

int main() {
    long long number = 1230067890;
    std::cout << "Formatted number: " << numToStr(number) << std::endl;
    return 0;
}
