#include "simhash.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;

SimHash::SimHash(const string& text) {
    vector<string> words;

    // 使用cppjieba分词工具进行分词
    cppjieba::Jieba jieba("dict/jieba.dict.utf8",           // 指定词典路径
        "dict/hmm_model.utf8",                              // 指定HMM模型路径
        "dict/user.dict.utf8",                              // 指定用户词典路径
        "dict/idf.utf8",                                    // 指定idf路径
        "dict/stop_words.utf8");                            // 指定停用词路径
    jieba.Cut(text, words, true); 

    // 移除停用词和单字词，并统计词频
    for (const string& word : words) {
        if (word.length() > 1 && _stop_words.find(word) == _stop_words.end()) {
            _term_frequency[word]++;
        }
    }

    // 运用TF-IDF算法计算每个词项的权重
    double total_terms = _term_frequency.size();
    for (auto& pair : _term_frequency) {
        double tf = pair.second;
        double idf = log((1.0 + 1.0) / (1.0 + tf));
        int weight = static_cast<int>(tf * idf);
        // 将词项及其权重添加到特征向量中
        _terms.emplace_back(pair.first, weight);
    }
}

string SimHash::calculateSimHash() {
    vector<int> weightedHashSum(64, 0); 
    string result;

    // 计算Simhash值的合并过程
    for (const auto& _Term : _terms) {
        for (size_t i = 0; i < 64; ++i) {
            weightedHashSum[i] += _Term.weighted_hash[i];
        }
    }

    // 计算Simhash值的降维过程
    for (int i = 63; i >= 0; --i) {
        result.push_back(weightedHashSum[i] > 0 ? '1' : '0');
    }
    return result;
}

double SimHash::calculateSimilarity(const string& hash_orig, const string& hash_copy) {
    int distance = 0;
    double similarity = 0;

    // 统计两个Simhash值的汉明距离
    for (size_t i = 0; i < hash_orig.size(); ++i) {
        if (hash_orig[i] != hash_copy[i]) {
            ++distance;
        }
    }

    // 计算论文重复率
    similarity = 1.0 - static_cast<double>(distance) / hash_orig.size();
    return similarity;
}

SimHash::_Term::_Term(string w, int weight) : word(w), weight(weight) {
    // 赋初始哈希值
    hash_value = getWordHash(w);

    // 根据权重计算加权哈希值
    weighted_hash.resize(64, 0);
    for (int i = 0; i < 64; ++i) {
        if (hash_value & (1ULL << i)) {
            weighted_hash[i] = 1 * weight;
        }
        else {
            weighted_hash[i] = (-1) * weight;
        }
    }
}

uint64_t SimHash::getWordHash(const string& word) {
    const uint64_t BIGINT_1000003 = 1000003;                        // 作为乘法因子，用于将当前哈希值与下一个字符的哈希值混合
    const uint64_t BIGINT_2E64M1 = 0xFFFFFFFFFFFFFFFFull;           // 一个掩码，用于确保哈希值不会超过64位整数的范围

    // 初始化为字符串的第一个字符的ASCII值，并将该值左移12位
    uint64_t hash = (static_cast<uint64_t>(word[0]) << 12); 

    // 转换类型，混合哈希值，防止溢出
    for (char ch : word) {
        uint64_t chInt = static_cast<uint64_t>(ch);
        hash = (hash * BIGINT_1000003) ^ chInt;
        hash &= BIGINT_2E64M1; 
    }

    // 最终混合，考虑字符串的长度，增加哈希值的唯一性
    hash = hash ^ static_cast<uint64_t>(word.length());
    return hash;
}
