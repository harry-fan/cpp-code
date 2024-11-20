#ifndef _RANDOM_UTILS_H_
#define _RANDOM_UTILS_H_

#include <set>
#include <map>
#include <vector>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>

#define PAN_RAND_POOL_SIZE 10240

using namespace std;
class RandomGenerator {
public:
    RandomGenerator() {
        fd = -1;
        update();
    }

    uint32_t gen() {
        if (m_iPos >= PAN_RAND_POOL_SIZE) {
            update();
            m_iPos = 0;
        }
        return m_aRands[m_iPos++];
    }

private:
    void update() {
        m_iPos = 0;
        if (fd < 0) {
            fd = open("/dev/urandom", O_RDONLY);
        }
        if (fd < 0) {
            return;
        }
        if (read(fd, (void *) m_aRands, sizeof(m_aRands)) < 0) {
            return;
        }
    }

private:
    int32_t fd;
    uint32_t m_iPos;
    uint32_t m_aRands[PAN_RAND_POOL_SIZE];
};

inline uint32_t random32() {
    RandomGenerator obj;
    return obj.gen();
}

inline int32_t random32(int32_t min, int32_t max) {
    if (min == max) {
        return min;
    }
    if (min > max) {
        std::swap(min, max);
    }
    return random32() % (max - min + 1) + min;
}

// 随机产生[0, max]范围内的一个数
inline uint32_t random32(uint32_t max) {
    return (uint32_t) random32(0, max);
}

// 从数组中随机一个，空数组返回0
inline int64_t random(const vector<int64_t> &items) {
    return items.empty() ? 0 : items.at(random32(items.size() - 1));
}

// n个元素中随机选取m个元素，每个元素被选取概率为m/n
// 如果n <= m，则所有元素全部被选取，并且不会重复选取
template<class T>
inline vector<T> randomBatch(const set<T> &datas, int64_t m) {
    vector<T> selection;
    if (m <= 0) {
        return selection;
    }
    int64_t count = 0;
    for (const auto &data : datas) {
        if ((int64_t) selection.size() < m) {
            selection.push_back(data);
        } else {
            int64_t idx = random32(count);
            if (idx < m) {
                selection[idx] = data;
            }
        }
        count++;
    }
    return selection;
}

// 从vector中随机选取一个元素
template<class T>
inline T randomVector(const vector<T> &items) {
    if (items.empty()) {
        return {};
    }
    return items.at(random32(items.size() - 1));
}

// 从map中随机选取一个元素
template<class _Key, class _Value>
inline pair<_Key, _Value> randomMap(const map<_Key, _Value> &items) {
    if (items.empty()) {
        return {};
    }
    auto iter = items.begin();
    std::advance(iter, random32(items.size() - 1));
    return *iter;
}

// 从set中随机选取一个元素
template<class T>
inline T randomSet(const set<T> &items) {
    if (items.empty()) {
        return {};
    }
    auto iter = items.begin();
    std::advance(iter, random32(items.size() - 1));
    return *iter;
}

// 根据权重随机选取一个元素
// items: map<weight, T>
// result: pair<ramdomWeight, T>
template<class T> inline pair<int64_t, T> randomByWeight(const map<int64_t, T> &items) {
    if (items.empty()) {
        return {};
    }
    auto totalWeight = items.rbegin()->first;
    if (totalWeight <= 0) {
        return {};
    }
    T result = {};
    auto randomWeight = random32(totalWeight - 1);
    for (const auto &item : items) {
        if (randomWeight < item.first) {
            result = item.second;
            break;
        }
    }
    return make_pair(randomWeight, result);
}

inline bool isOccur100(int32_t prob) {
    return (int32_t) random32(0, 99) < prob;
}

inline bool isOccur1000(int32_t prob) {
    return (int32_t) random32(0, 999) < prob;
}

inline bool isOccur10000(int32_t prob) {
    return (int32_t) random32(0, 9999) < prob;
}

#endif // _RANDOM_UTILS_H_
