#include "simhash.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;

SimHash::SimHash(const string& text) {
    vector<string> words;

    // ʹ��cppjieba�ִʹ��߽��зִ�
    cppjieba::Jieba jieba("dict/jieba.dict.utf8",           // ָ���ʵ�·��
        "dict/hmm_model.utf8",                              // ָ��HMMģ��·��
        "dict/user.dict.utf8",                              // ָ���û��ʵ�·��
        "dict/idf.utf8",                                    // ָ��idf·��
        "dict/stop_words.utf8");                            // ָ��ͣ�ô�·��
    jieba.Cut(text, words, true); 

    // �Ƴ�ͣ�ôʺ͵��ִʣ���ͳ�ƴ�Ƶ
    for (const string& word : words) {
        if (word.length() > 1 && _stop_words.find(word) == _stop_words.end()) {
            _term_frequency[word]++;
        }
    }

    // ����TF-IDF�㷨����ÿ�������Ȩ��
    double total_terms = _term_frequency.size();
    for (auto& pair : _term_frequency) {
        double tf = pair.second;
        double idf = log((1.0 + 1.0) / (1.0 + tf));
        int weight = static_cast<int>(tf * idf);
        // �������Ȩ����ӵ�����������
        _terms.emplace_back(pair.first, weight);
    }
}

string SimHash::calculateSimHash() {
    vector<int> weightedHashSum(64, 0); 
    string result;

    // ����Simhashֵ�ĺϲ�����
    for (const auto& _Term : _terms) {
        for (size_t i = 0; i < 64; ++i) {
            weightedHashSum[i] += _Term.weighted_hash[i];
        }
    }

    // ����Simhashֵ�Ľ�ά����
    for (int i = 63; i >= 0; --i) {
        result.push_back(weightedHashSum[i] > 0 ? '1' : '0');
    }
    return result;
}

double SimHash::calculateSimilarity(const string& hash_orig, const string& hash_copy) {
    int distance = 0;
    double similarity = 0;

    // ͳ������Simhashֵ�ĺ�������
    for (size_t i = 0; i < hash_orig.size(); ++i) {
        if (hash_orig[i] != hash_copy[i]) {
            ++distance;
        }
    }

    // ���������ظ���
    similarity = 1.0 - static_cast<double>(distance) / hash_orig.size();
    return similarity;
}

SimHash::_Term::_Term(string w, int weight) : word(w), weight(weight) {
    // ����ʼ��ϣֵ
    hash_value = getWordHash(w);

    // ����Ȩ�ؼ����Ȩ��ϣֵ
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
    const uint64_t BIGINT_1000003 = 1000003;                        // ��Ϊ�˷����ӣ����ڽ���ǰ��ϣֵ����һ���ַ��Ĺ�ϣֵ���
    const uint64_t BIGINT_2E64M1 = 0xFFFFFFFFFFFFFFFFull;           // һ�����룬����ȷ����ϣֵ���ᳬ��64λ�����ķ�Χ

    // ��ʼ��Ϊ�ַ����ĵ�һ���ַ���ASCIIֵ��������ֵ����12λ
    uint64_t hash = (static_cast<uint64_t>(word[0]) << 12); 

    // ת�����ͣ���Ϲ�ϣֵ����ֹ���
    for (char ch : word) {
        uint64_t chInt = static_cast<uint64_t>(ch);
        hash = (hash * BIGINT_1000003) ^ chInt;
        hash &= BIGINT_2E64M1; 
    }

    // ���ջ�ϣ������ַ����ĳ��ȣ����ӹ�ϣֵ��Ψһ��
    hash = hash ^ static_cast<uint64_t>(word.length());
    return hash;
}
