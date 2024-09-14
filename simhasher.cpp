#include "simhasher.h"

using std::min;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;
using std::string;
using std::ifstream;
using std::exception;
using std::runtime_error;
using std::unordered_map;
using std::unordered_set;

Simhasher::Simhasher(const string& text, bool test_signal) {
    if (test_signal != true) {
        vector<string> words;

        // 加载停用词
        loadStopWords("dict/stop_words.utf8");

        try {
            // 使用cppjieba分词工具进行分词
            cppjieba::Jieba jieba(
                "dict/jieba.dict.utf8",                             // 指定词典路径
                "dict/hmm_model.utf8",                              // 指定HMM模型路径
                "dict/user.dict.utf8",                              // 指定用户词典路径
                "dict/idf.utf8",                                    // 指定idf路径
                "dict/stop_words.utf8");                            // 指定停用词路径
            jieba.Cut(text, words, true);

            // 检查分词结果是否为空
            if (words.empty()) {
                throw runtime_error("分词结果为空");
            }
        }
        catch (const exception& e) {
            cerr << "使用cppjieba分词时的错误信息: " << e.what() << endl;
            throw;
        }

        // 移除停用词和单字词，并统计词频
        for (const string& word : words) {
            if (word.length() > 1 && _stop_words.find(word) == _stop_words.end()) {
                _term_frequency[word]++;
            }
        }

        // 运用TF-IDF算法计算每个词项的权重
        double total_terms = static_cast<double>(_term_frequency.size());
        for (auto& pair : _term_frequency) {
            double tf = pair.second;
            double idf = log((1.0 + 1.0) / (1.0 + tf));
            int weight = static_cast<int>(tf * idf);
            // 将词项及其权重添加到特征向量中
            _terms.emplace_back(pair.first, weight);
        }
    }
}

string Simhasher::calculateSimHash() {
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

double Simhasher::calculateSimilarity(const string& hash_orig, const string& hash_copy) {
    int distance = 0;
    size_t simhash_length = hash_orig.size();
    double similarity = 0;
    unsigned int xor_value = 0;

    if (hash_orig.size() != hash_copy.size()) {
        // 如果两个字符串长度不同，则说明情况
        size_t simhash_length = min(hash_orig.size(), hash_copy.size());
        cout << "两个文本的Simhash值的长度不一,以下汉明距离将使用前" << simhash_length << "位的simhash值计算" << endl;
    }
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

void Simhasher::testloadStopWords(const std::string& filePath) {
    loadStopWords(filePath);
};

Simhasher::_Term::_Term(string w, int weight) : word(w), weight(weight) {
    // 赋初始哈希值
    jenkins _hasher;
    hash_value = _hasher(w.c_str(), w.size(), 0);

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

void Simhasher::loadStopWords(const string& filePath) {
    try {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "无法以下路径的打开停用词文件: " << filePath << "将使用初始100字停用词集合作为停用词词典" << endl;
            _stop_words = { "的","了","是","我","不","在","人","有","这","中","大","为","上","个","也","他","而","和","你","就","来","到","说","要","着","好","能","她","们","它","对","下","去","与","等","如","年","会","从","之","把","时","道","自","或","做","给","还","此","又","何" ,
    "a","an","the","and","but","or","nor","for","yet","so","of","to","in","on","at","by","with","about","against","between","into","through","during","before","after","above","below","up","down","under","over","again","further","then","once","here","there","when","where","why","how","all","any","both","each","few","more","most","no","nor","some","such","than","too","very","s","t","can","will" };
            return ; 
        }

        string line;
        while (getline(file, line)) {
            // 手动清除字符串中的空白字符
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

        // 检查停用词词典是否为空
        if (_stop_words.empty()) {
            cout << "停用词词典为空, 将使用初始100字停用词集合作为停用词词典" << endl;
            _stop_words = { "的","了","是","我","不","在","人","有","这","中","大","为","上","个","也","他","而","和","你","就","来","到","说","要","着","好","能","她","们","它","对","下","去","与","等","如","年","会","从","之","把","时","道","自","或","做","给","还","此","又","何" ,
    "a","an","the","and","but","or","nor","for","yet","so","of","to","in","on","at","by","with","about","against","between","into","through","during","before","after","above","below","up","down","under","over","again","further","then","once","here","there","when","where","why","how","all","any","both","each","few","more","most","no","nor","some","such","than","too","very","s","t","can","will" };
            return ;
        }
    }
    catch (const exception& e) {
        // 捕获并打印异常信息
        cerr << "加载停用词词典时发生错误: " << e.what() << endl;
        throw;
    }
}