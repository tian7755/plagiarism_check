#ifndef SIMHASHER_H_
#define SIMHASHER_H_

#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip> 
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <bitset>
#include <regex>
#include "cppjieba/Jieba.hpp"
#include "jenkins.h"

// 运用Simhash原理计算论文重复率
class Simhasher
{
public:
    // 构造函数，接受一段文本，对其进行分词，并根据每个词的词频计算其权重
    Simhasher(const std::string& text);

    // 完成合并和降维过程，得出文本的Simhash值
    std::string calculateSimHash();

    // 统计两个Simhash值的汉明距离，并得出论文重复率
    static double calculateSimilarity(const std::string& hash_orig, const std::string& hash_copy);

private:
    // 存储分词后的每个词项及其权重和哈希值
    struct _Term
    {
        std::string word;                           // 词项
        int weight;                                 // 词项的权重
        uint64_t hash_value;                        // 词项的哈希值
        std::vector<int> weighted_hash;             // 加权后的哈希值数组

        // _Term构造函数，赋予词项初始哈希值，并计算加权哈希值
        _Term(std::string w, int weight);
    };

    std::vector<_Term> _terms;                                                                                // 文本所有的词项
    std::unordered_map<std::string, int> _term_frequency;                                                     // 存储所有词项对应的词频的哈希表
    std::unordered_set<std::string> _stop_words;                                                              // 停用词的集合

    // 从停用词词典中加载停用词
    void loadStopWords(const std::string& filePath);
};

#endif // SIMHASHER_H_
