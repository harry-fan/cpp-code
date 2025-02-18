#include <iostream>

typedef struct node {
    int data;
    struct node* next;
    node(int d):data(d), next(NULL) {}
} node;

void reverse(node* head) {
    if (head == NULL) {
        return;
    }
    node* pleft = NULL;
    node* pcurrent = head;
    node* pright = head->next;

    while (pright) {
        pcurrent->next = pleft;
        node *ptemp = pright->next;
        pright->next = pcurrent;
        pleft = pcurrent;
        pcurrent = pright;
        pright = ptemp;
    }
    while (pcurrent != NULL) {
        std::cout << pcurrent->data << "\t";
        pcurrent = pcurrent->next;
    }
}

int main() {
    node *head = new node(0);
    node *ptemp = head;
    for (int i = 1; i < 10; ++i) {
        node* tmp = new node(i);
        ptemp->next = tmp;
        ptemp = tmp;
    }
    reverse(head);
    while (head) {
        node* temp = head;
        head = head->next;
        delete temp;
    }
}