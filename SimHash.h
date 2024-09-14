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
        //uint64_t hashValue[2];             // ����Ĺ�ϣֵ
        uint64_t hashValue;             // ����Ĺ�ϣֵ
        std::vector<int> weightedHash;  // ��Ȩ��Ĺ�ϣֵ����

        // Term���캯������������ʼ��ϣֵ�������Ȩ��ϣֵ
        Term(std::string w, int weight) : word(w), weight(weight) {
            /*MurmurHash3_64(w, hashValue);*/
            hashValue = getWordHash(w);
            weightedHash.resize(64, 0);
            /*int choice = 0;*/
            for (int i = 0; i < 64; ++i) {
                /*if (i > 63) {
                    choice = 1;
                }
                if (hashValue[choice] & (1ULL << i)) {
                    weightedHash[i] = 1 * weight;
                }
                else {
                    weightedHash[i] = (- 1) * weight;
                }*/
                if (hashValue & (1ULL << i)) {
                    weightedHash[i] = 1 * weight;
                }
                else {
                    weightedHash[i] = (-1) * weight;
                }
            }
        }
    };
    
    std::vector<Term> terms;                                                                                // �洢��������������
    std::unordered_map<std::string, int> termFrequency;                                                     // �洢��Ƶ�Ĺ�ϣ��
    std::unordered_set<std::string> stopWords = { "��", "��", "��", "��", "��", "��" ,"��","��" };          // �洢ͣ�ôʵļ���
     

    static uint64_t getWordHash(const std::string& word) {
        // ���峣�������ڹ�ϣ����
        const uint64_t BIGINT_1000003 = 1000003;
        const uint64_t BIGINT_2E64M1 = 0xFFFFFFFFFFFFFFFFull;

        if (word.empty()) {
            return 0;
        }

        uint64_t hash = (static_cast<uint64_t>(word[0]) << 12); // ��ʼ��ϣֵ
        for (char ch : word) {
            uint64_t chInt = static_cast<uint64_t>(ch);
            hash = (hash * BIGINT_1000003) ^ chInt;
            hash &= BIGINT_2E64M1; // ���ֹ�ϣֵ��64λ��Χ��
        }
        hash = hash ^ static_cast<uint64_t>(word.length());
        return hash;
    }

public:

    // ���캯��������һ���ı���������зִʣ�������ÿ���ʵĴ�Ƶ
    SimHash(const std::string& text) {
        cppjieba::Jieba jieba("dict/jieba.dict.utf8", // ָ���ʵ�·��
            "dict/hmm_model.utf8", // ָ��HMMģ��·��
            "dict/user.dict.utf8", // ָ���û��ʵ�·��
            "dict/idf.utf8", // ָ��idf·��
            "dict/stop_words.utf8"); // ָ��ͣ�ô�·��

        std::vector<std::string> words;
        jieba.Cut(text, words, true); // ʹ��cppjieba���зִ�

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
    static double CalculateSimilarity(const std::string& hash_a, const std::string& hash_b) {
        int distance = 0;
        for (size_t i = 0; i < hash_a.size(); ++i) {
            // ��������SimHashֵ֮��ĺ�������
            if (hash_a[i] != hash_b[i]) {
                ++distance;
            }
        }
        // �������ƶ�
        double similarity = 0;
        similarity = 1.0 - static_cast<double>(distance) / hash_a.size();
        return similarity;
    }
};


#endif // _SIMHASH_H_
