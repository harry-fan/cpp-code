//
// Created by Administrator on 2024/6/7.
//

#include <iostream>

using namespace std;
struct __attribute__ ((__packed__)) sdshdr64 {
    uint64_t len; /* used */
    uint64_t alloc; /* excluding the header and null terminator */
    unsigned char flags; /* 3 lsb of type, 5 unused bits */
    char buf[];
};

typedef struct dictEntry {
    void *key;  // 占用8
    union {
        void *val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct dictEntry *next; // 占用8
} dictEntry;

int main() {
    std::cout << sizeof(long long) << std::endl;
    std::cout << sizeof(struct sdshdr64) << std::endl;
    std::cout << sizeof(struct dictEntry) << std::endl;
}