#include <iostream>
#include <vector>

using namespace std;

// 快速排序的分区函数
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // 选择最后一个元素作为基准点
    int i = low - 1; // i 是小于基准点的区域边界

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]); // 将较小的元素放到左侧
        }
    }
    swap(arr[i + 1], arr[high]); // 将基准点放到正确的位置
    return i + 1; // 返回基准点的索引
}

// 快速排序算法
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // 获取基准点的索引

        // 分别对基准点左右两侧的子数组进行排序
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 打印数组元素
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

// 主函数，用于测试快速排序
int main() {
    vector<int> arr = {10, 7, 8, 9, 1, 5};
    int n = arr.size();

    cout << "原始数组：" << endl;
    printArray(arr);

    quickSort(arr, 0, n - 1);

    cout << "排序后的数组：" << endl;
    printArray(arr);

    return 0;
}
