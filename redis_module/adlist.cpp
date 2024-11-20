#include <iostream>

using namespace std;

typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

typedef struct list {
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;

listNode ArrayListNodes[] = {
        {NULL, NULL, NULL},
};

int main() {
    struct list *list;
    cout << sizeof(*list) << endl;
}