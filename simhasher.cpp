#include "simhasher.h"

using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::string;
using std::runtime_error;
using std::unordered_map;
using std::unordered_set;

Simhasher::Simhasher(const string& text) {
    vector<string> words;

    // ����ͣ�ô�
    loadStopWords("dict/stop_words.utf8");

    try {
        // ʹ��cppjieba�ִʹ��߽��зִ�
        cppjieba::Jieba jieba(
            "dict/jieba.dict.utf8",                             // ָ���ʵ�·��
            "dict/hmm_model.utf8",                              // ָ��HMMģ��·��
            "dict/user.dict.utf8",                              // ָ���û��ʵ�·��
            "dict/idf.utf8",                                    // ָ��idf·��
            "dict/stop_words.utf8");                            // ָ��ͣ�ô�·��
        jieba.Cut(text, words, true);

        // ���ִʽ���Ƿ�Ϊ��
        if (words.empty()) {
            throw runtime_error("�ִʽ��Ϊ��");
        }
    }
    catch (const std::exception& e) {
        cerr << "ʹ��cppjieba�ִ�ʱ�Ĵ�����Ϣ: " << e.what() << endl;
        throw;
    }

    // �Ƴ�ͣ�ôʺ͵��ִʣ���ͳ�ƴ�Ƶ
    for (const string& word : words) {
        if (word.length() > 1 && _stop_words.find(word) == _stop_words.end()) {
            _term_frequency[word]++;
        }
    }

    // ����TF-IDF�㷨����ÿ�������Ȩ��
    double total_terms = static_cast<double>(_term_frequency.size());
    for (auto& pair : _term_frequency) {
        double tf = pair.second;
        double idf = log((1.0 + 1.0) / (1.0 + tf));
        int weight = static_cast<int>(tf * idf);
        // �������Ȩ����ӵ�����������
        _terms.emplace_back(pair.first, weight);
    }
}

string Simhasher::calculateSimHash() {
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

double Simhasher::calculateSimilarity(const string& hash_orig, const string& hash_copy) {
    int distance = 0;
    double similarity = 0;
    unsigned int xor_value = 0;

    if (hash_orig.size() != hash_copy.size()) {
        // ��������ַ������Ȳ�ͬ�����޷����㺺������
        throw std::invalid_argument("�����ı���Simhashֵ�ĳ��Ȳ�һ��");
    }
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

Simhasher::_Term::_Term(string w, int weight) : word(w), weight(weight) {
    // ����ʼ��ϣֵ
    jenkins _hasher;
    hash_value = _hasher(w.c_str(), w.size(), 0);

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

void Simhasher::loadStopWords(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("�޷���ͣ�ô��ļ�: " + filePath);
        }

        std::string line;
        while (std::getline(file, line)) {
            // �ֶ�����ַ����еĿհ��ַ�
            auto it = line.begin();
            while (it != line.end()) {
                if (isspace(static_cast<unsigned char>(*it))) {
                    it = line.erase(it);
                }
                else {
                    ++it;
                }
            }
            if (!line.empty()) {
                _stop_words.insert(line);
            }
        }

        // ���ͣ�ôʴʵ��Ƿ�Ϊ��
        if (_stop_words.empty()) {
            throw runtime_error("ͣ�ôʴʵ�Ϊ��");
        }

    }
    catch (const std::exception& e) {
        // ���񲢴�ӡ�쳣��Ϣ
        cerr << "��ȡ�ļ�ʱ��������: " << e.what() << endl;
        throw;
    }
}