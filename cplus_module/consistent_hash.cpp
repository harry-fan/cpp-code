#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

// 哈希函数
size_t hashFunction(const std::string& key) {
    std::hash<std::string> hasher;
    return hasher(key);
}

class ConsistentHashing {
public:
    // 构造函数，设置虚拟节点数
    ConsistentHashing(int numVirtualNodes) : numVirtualNodes(numVirtualNodes) {}

    // 添加节点
    void addNode(const std::string& node) {
        for (int i = 0; i < numVirtualNodes; ++i) {
            std::string virtualNode = node + "#" + std::to_string(i);
            size_t hash = hashFunction(virtualNode);
            ring[hash] = node;
        }
    }

    // 删除节点
    void removeNode(const std::string& node) {
        for (int i = 0; i < numVirtualNodes; ++i) {
            std::string virtualNode = node + "#" + std::to_string(i);
            size_t hash = hashFunction(virtualNode);
            ring.erase(hash);
        }
    }

    // 获取节点
    std::string getNode(const std::string& key) {
        if (ring.empty()) {
            throw std::runtime_error("No nodes in the ring.");
        }

        size_t hash = hashFunction(key);
        auto it = ring.lower_bound(hash);
        if (it == ring.end()) {
            it = ring.begin();
        }
        return it->second;
    }

    void showNodes() {
        std::cout << "show all nodes:" << std::endl;
        for (const auto &r : ring) {
            std::cout << r.first << "-" << r.second << std::endl;
        }
        std::cout << std::endl;
    }

private:
    int numVirtualNodes;
    std::map<size_t, std::string> ring;  // 哈希环
};

int main() {
    ConsistentHashing ch(3);

    // 添加节点
    ch.addNode("Node1");
    ch.addNode("Node2");
    ch.addNode("Node3");
    ch.showNodes();

    // 测试数据映射
    std::vector<std::string> keys = {"key1", "key2", "key3", "key4", "key5", "key6"};
    for (const auto& key : keys) {
        std::cout << "Key: " << key << " maps to node: " << ch.getNode(key) << std::endl;
    }

    // 删除节点
    ch.addNode("Node4");

    // 再次测试数据映射
    std::cout << "\nAfter add Node4:\n";
    for (const auto& key : keys) {
        std::cout << "Key: " << key << " maps to node: " << ch.getNode(key) << std::endl;
    }

    return 0;
}
