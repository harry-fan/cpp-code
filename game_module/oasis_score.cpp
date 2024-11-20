//
// Created by Administrator on 2024/11/15.
//

#include <iostream>
#include <cstdint>
#include <cmath>

using namespace std;

#define formula_m 1000
#define formula_k 600
#define formula_l 0.4

int64_t calcScore(int64_t score, int64_t opScore, bool isWin) {
    double winEA = 1 / (1 + pow(10, ((score - opScore) / formula_m)));
    double winEB = 1 / (1 + pow(10, ((opScore - score) / formula_m)));
    return isWin ? floor(winEA * formula_k) : floor(winEB * formula_k * formula_l);
}

int main() {
    cout << "A win add:" << calcScore(1199, 3618, true) << endl;
    cout << "A lose dec:" << calcScore(1199, 3618, false) << endl;

    cout << "B win add:" << calcScore(3618, 1199, true) << endl;
    cout << "B lose dec:" << calcScore(3618, 1199, false) << endl;
}