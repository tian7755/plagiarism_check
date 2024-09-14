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
    // �洢ÿ�������Ȩ�غ͹�ϣֵ
    struct Term{
        std::string word;               // ����
        int weight;                     // �����Ȩ��
        uint64_t hashValue;             // ����Ĺ�ϣֵ
        std::vector<int> weightedHash;  // ��Ȩ��Ĺ�ϣֵ����

        // Term���캯������������ʼ��ϣֵ�������Ȩ��ϣֵ
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
    
    std::vector<Term> terms;                                                                                // �洢��������������
    std::unordered_map<std::string, int> termFrequency;                                                     // �洢��Ƶ�Ĺ�ϣ��
    std::unordered_set<std::string> stopWords = { "��", "��", "��", "��", "��", "��" ,"��","��" };          // �洢ͣ�ôʵļ���

    // �����ַ����ĳ�ʼ��ϣֵ
    static uint64_t MurmurHash3_64(const std::string& key) {
        // ����һ�����������洢128λ�Ĺ�ϣֵ���
        uint64_t result[2] = { 0 };

        // ��ȡ��ǰʱ����Ϊ�����������������
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        // ����MurmurHash3_x64_128�����������ϣֵ
        MurmurHash3_x64_128(key.data(), static_cast<int>(key.size()), seed, result);

        return result[0];
    }

public:

    // ���캯��������һ���ı���������зִʣ�������ÿ���ʵĴ�Ƶ
    SimHash(const std::string& text) {
        cppjieba::Jieba jieba("cppjieba-master/dict/jieba.dict.utf8", // ָ���ʵ�·��
            "cppjieba-master/dict/hmm_model.utf8", // ָ��HMMģ��·��
            "cppjieba-master/dict/user.dict.utf8", // ָ���û��ʵ�·��
            "cppjieba-master/dict/idf.utf8", // ָ��idf·��
            "cppjieba-master/dict/stop_words.utf8"); // ָ��ͣ�ô�·��

        std::vector<std::string> words;
        jieba.Cut(text, words); // ʹ��cppjieba���зִ�

        for (const std::string& word : words) {
            // �Ƴ�ͣ�ôʺ͵��ִ�
            if (word.length() > 1 && stopWords.find(word) == stopWords.end()) {
                termFrequency[word]++;
            }
        }

        // ����TF-IDFȨ��
        double totalTerms = termFrequency.size();
        for (auto& pair : termFrequency) {
            double tf = pair.second;
            double idf = log((1.0 + 1.0) / (1.0 + tf));
            int weight = static_cast<int>(tf * idf);
            // �������Ȩ����ӵ�����������
            terms.emplace_back(pair.first, weight);
        }
    }

    // �����ı���SimHashֵ�������ϲ��ͽ�ά����
    std::string CalculateSimHash() {
        std::vector<int> weightedHashSum(64, 0); // ��ʼ������ֵΪ0
        for (const auto& Term: terms) {
            for (size_t i = 0; i < 64; ++i) {
                weightedHashSum[i] += Term.weightedHash[i];
            }
        }

        // ��hashBitsת��Ϊ�ַ�����ʽ��SimHashֵ
        std::string result;
        for (int i = 63; i >= 0; --i) {
            result.push_back(weightedHashSum[i] > 0 ? '1' : '0');
        }
        return result;
    }

    // ��������SimHashֵ֮��ĺ������룬�������ƶ�
    static long CalculateSimilarity(const std::string& a, const std::string& b) {
        int distance = 0;
        for (size_t i = 0; i < a.size(); ++i) {
            // ��������SimHashֵ֮��ĺ�������
            if (a[i] != b[i]) {
                ++distance;
            }
        }
        // �������ƶ�
        long similarity = 0;
        similarity = 1.0 - static_cast<long>(distance) / a.size();
        return similarity;
    }
};


#endif _SIMHASH_H_
