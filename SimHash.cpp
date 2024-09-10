#include "SimHash.h"

// 模拟MurmurHash3的哈希函数
// 输入：字符串型的参数 key
// 输出：无符号的64位整型的哈希值
uint64_t MurmurHash3_64(const std::string& key) {
    uint64_t hash = 0;
    for (char c : key) {
        // 将当前字符的 ASCII 值转换为 uint64_t 类型，并与哈希值进行异或操作。
        // 然后进行一次位操作：将异或结果左移 (hash >> 43) & 63 位。
        hash ^= static_cast<uint64_t>(c) << ((hash >> 43) & 63);
        // 进行一次混合操作:将哈希值乘以一个常数，用于确保哈希值的分布更加均匀。
        hash *= 0xff51afd7ed558ccd;
    }
    // 进行最终混合：旋转异或，乘以一个常数，旋转异或
    hash ^= hash >> 33;
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= hash >> 33;
    return hash;
}


// 构造函数
SimHash::SimHash() {}

// 添加文本到SimHash
void SimHash::AddText(const std::string& text) {
    std::unordered_set<std::string> words = Segment(text);
    for (const std::string& word : words) {
        termFrequency[word]++;
    }
    for (const auto& pair : termFrequency) {
        uint64_t hashValue = MurmurHash3_64(pair.first);
        for (int i = 0; i < 64; ++i) {
            if (hashValue & (1ULL << i)) {
                hashBits.set(i, true);
            }
            else {
                hashBits.reset(i);
            }
        }
    }
}

// 计算最终的SimHash值
std::string SimHash::CalculateHash() const {
    std::string result;
    for (int i = 63; i >= 0; --i) {
        result.push_back(hashBits.test(i) ? '1' : '0');
    }
    return result;
}

// 计算两个SimHash值的海明距离
int SimHash::HammingDistance(const SimHash& other) const {
    int distance = 0;
    for (int i = 0; i < 64; ++i) {
        if (hashBits.test(i) != other.hashBits.test(i)) {
            ++distance;
        }
    }
    return distance;
}
