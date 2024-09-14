#include "SimHash.h"

// ģ��MurmurHash3�Ĺ�ϣ����
// ���룺�ַ����͵Ĳ��� key
// ������޷��ŵ�64λ���͵Ĺ�ϣֵ
uint64_t MurmurHash3_64(const std::string& key) {
    uint64_t hash = 0;
    for (char c : key) {
        // ����ǰ�ַ��� ASCII ֵת��Ϊ uint64_t ���ͣ������ϣֵ������������
        // Ȼ�����һ��λ����������������� (hash >> 43) & 63 λ��
        hash ^= static_cast<uint64_t>(c) << ((hash >> 43) & 63);
        // ����һ�λ�ϲ���:����ϣֵ����һ������������ȷ����ϣֵ�ķֲ����Ӿ��ȡ�
        hash *= 0xff51afd7ed558ccd;
    }
    // �������ջ�ϣ���ת��򣬳���һ����������ת���
    hash ^= hash >> 33;
    hash *= 0xc4ceb9fe1a85ec53;
    hash ^= hash >> 33;
    return hash;
}


// ���캯��
SimHash::SimHash() {}

// ����ı���SimHash
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

// �������յ�SimHashֵ
std::string SimHash::CalculateHash() const {
    std::string result;
    for (int i = 63; i >= 0; --i) {
        result.push_back(hashBits.test(i) ? '1' : '0');
    }
    return result;
}

// ��������SimHashֵ�ĺ�������
int SimHash::HammingDistance(const SimHash& other) const {
    int distance = 0;
    for (int i = 0; i < 64; ++i) {
        if (hashBits.test(i) != other.hashBits.test(i)) {
            ++distance;
        }
    }
    return distance;
}
