#pragma once
#ifndef _SIMHASH_H_
#define _SIMHASH_H_

#include"MurmurHash3.h"
#include <iostream>
#include <random>
#include <chrono>

#include "cppjieba/Jieba.hpp"

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


class SimHash {
private:
    // 存储每个词项及其权重和哈希值
    struct Term{
        std::string word;               // 词项
        int weight;                     // 词项的权重
        uint64_t hashValue;             // 词项的哈希值
        std::vector<int> weightedHash;  // 加权后的哈希值数组

        // Term构造函数，赋予词项初始哈希值，计算加权哈希值
        Term(std::string w, int weight) : word(w), weight(weight) {
            hashValue = MurmurHash3_64(w);
            weightedHash.resize(64, 0);
            for (int i = 0; i < 64; ++i) {
                if (hashValue & (1ULL << i)) {
                    weightedHash[i] = 1 * weight;
                }
                else {
                    weightedHash[i] = (- 1) * weight;
                }
            }
        }
    };
    
    std::vector<Term> terms;                                                                                // 存储所有特征的向量
    std::unordered_map<std::string, int> termFrequency;                                                     // 存储词频的哈希表
    std::unordered_set<std::string> stopWords = { "的", "是", "在", "有", "和", "了" ,"这","那" };          // 存储停用词的集合

    // 计算字符串的初始哈希值
    static uint64_t MurmurHash3_64(const std::string& key) {
        // 创建一个缓冲区来存储128位的哈希值结果
        uint64_t result[2] = { 0 };

        // 获取当前时间作为随机数生成器的种子
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        // 调用MurmurHash3_x64_128函数来计算哈希值
        MurmurHash3_x64_128(key.data(), static_cast<int>(key.size()), seed, result);

        return result[0];
    }

public:

    // 构造函数，接受一段文本，对其进行分词，并计算每个词的词频
    SimHash(const std::string& text) {
        cppjieba::Jieba jieba("cppjieba-master/dict/jieba.dict.utf8", // 指定词典路径
            "cppjieba-master/dict/hmm_model.utf8", // 指定HMM模型路径
            "cppjieba-master/dict/user.dict.utf8", // 指定用户词典路径
            "cppjieba-master/dict/idf.utf8", // 指定idf路径
            "cppjieba-master/dict/stop_words.utf8"); // 指定停用词路径

        std::vector<std::string> words;
        jieba.Cut(text, words); // 使用cppjieba进行分词

        for (const std::string& word : words) {
            // 移除停用词和单字词
            if (word.length() > 1 && stopWords.find(word) == stopWords.end()) {
                termFrequency[word]++;
            }
        }

        // 计算TF-IDF权重
        double totalTerms = termFrequency.size();
        for (auto& pair : termFrequency) {
            double tf = pair.second;
            double idf = log((1.0 + 1.0) / (1.0 + tf));
            int weight = static_cast<int>(tf * idf);
            // 将词项及其权重添加到特征向量中
            terms.emplace_back(pair.first, weight);
        }
    }

    // 计算文本的SimHash值，即最后合并和降维过程
    std::string CalculateSimHash() {
        std::vector<int> weightedHashSum(64, 0); // 初始化所有值为0
        for (const auto& Term: terms) {
            for (size_t i = 0; i < 64; ++i) {
                weightedHashSum[i] += Term.weightedHash[i];
            }
        }

        // 将hashBits转换为字符串形式的SimHash值
        std::string result;
        for (int i = 63; i >= 0; --i) {
            result.push_back(weightedHashSum[i] > 0 ? '1' : '0');
        }
        return result;
    }

    // 根据两个SimHash值之间的汉明距离，计算相似度
    static long CalculateSimilarity(const std::string& a, const std::string& b) {
        int distance = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            // 计算两个SimHash值之间的汉明距离
            if (a[i] != b[i]) {
                ++distance;
            }
        }
        // 计算相似度
        long similarity = 0;
        similarity = 1.0 - static_cast<long>(distance) / a.size();
        return similarity;
    }
};


#endif _SIMHASH_H_
