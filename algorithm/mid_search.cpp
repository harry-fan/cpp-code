#include <iostream>
#include <vector>

using namespace std;

/* [left, right] search */
int search1(vector<int> &nums, int dest) {
    int left = 0;
    int right = nums.size() - 1;
    while (left <= right) {
        int mid = left + ((right - left) / 2);
        if (nums[mid] > dest) {
            right = mid - 1;
        } else if (nums[mid] < dest) {
            left = mid + 1;
        } else {
            return nums[mid];
        }
    }
    return -1;
}

/* [left, right) search */
int search2(vector<int> &nums, int dest) {
    int left = 0;
    int right = nums.size() - 1;
    while (left < right) {
        int mid = left + ((right - left) / 2);
        if (nums[mid] > dest) {
            right = mid;
        } else if (nums[mid] < dest) {
            left = mid + 1;
        } else {
            return nums[mid];
        }
    }
    return -1;
}

int main() {
    vector<int> nums = {1,3,4,5,6,7,9,10};
    cout << search1(nums, 3) << endl;
    cout << search1(nums, 5) << endl;
    cout << search1(nums, 8) << endl;
    cout << search1(nums, 10) << endl;

    cout << search2(nums, 3) << endl;
    cout << search2(nums, 5) << endl;
    cout << search2(nums, 8) << endl;
    cout << search2(nums, 10) << endl;
}
