#include <atomic>
#include <thread>
#include <stdio.h>

template<typename T>
class LockFreeQueue {
public:
    LockFreeQueue() {
        Node* dummy = new Node(T());
        head.store(dummy);
        tail.store(dummy);
    }

    ~LockFreeQueue() {
        while (Node* node = head.load()) {
            head.store(node->next);
            delete node;
        }
    }

    void enqueue(const T& value) {
        Node* newNode = new Node(value);
        Node* oldTail;
        while (true) {
            oldTail = tail.load();
            Node* next = oldTail->next.load();
            if (oldTail == tail.load()) {
                if (next == nullptr) {
                    if (oldTail->next.compare_exchange_weak(next, newNode)) {
                        break;
                    }
                } else {
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
        }
        tail.compare_exchange_weak(oldTail, newNode);
    }

    bool dequeue(T& result) {
        Node* oldHead;
        while (true) {
            oldHead = head.load();
            Node* oldTail = tail.load();
            Node* next = oldHead->next.load();
            if (oldHead == head.load()) {
                if (oldHead == oldTail) {
                    if (next == nullptr) {
                        return false;
                    }
                    tail.compare_exchange_weak(oldTail, next);
                } else {
                    result = next->data;
                    if (head.compare_exchange_weak(oldHead, next)) {
                        break;
                    }
                }
            }
        }
        delete oldHead;
        return true;
    }

private:
    struct Node {
        T data;
        std::atomic<Node*> next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
};

LockFreeQueue<int> queue;

void producer() {
    for (int i = 1; i < 10; i++) {
        queue.enqueue(i);
        printf("producer :%d\n", i);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void consumer() {
    for (int i = 1; i < 10; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        int tmp = 0;
        if (queue.dequeue(tmp)) {
            printf("consumer %d\n", i);
        } else {
            printf("consumer error! %d\n", i);
        }
    }
}

int main() {
    // CPU原语 实现多线程编程
    std::thread t1(consumer);
    std::thread t2(producer);

    t1.join();
    t2.join();
    return 0;
}
