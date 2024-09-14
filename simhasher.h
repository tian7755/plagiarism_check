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

// ����Simhashԭ����������ظ���
class Simhasher
{
public:
    // ���캯��������һ���ı���������зִʣ�������ÿ���ʵĴ�Ƶ������Ȩ��
    Simhasher(const std::string& text);

    // ��ɺϲ��ͽ�ά���̣��ó��ı���Simhashֵ
    std::string calculateSimHash();

    // ͳ������Simhashֵ�ĺ������룬���ó������ظ���
    static double calculateSimilarity(const std::string& hash_orig, const std::string& hash_copy);

private:
    // �洢�ִʺ��ÿ�������Ȩ�غ͹�ϣֵ
    struct _Term
    {
        std::string word;                           // ����
        int weight;                                 // �����Ȩ��
        uint64_t hash_value;                        // ����Ĺ�ϣֵ
        std::vector<int> weighted_hash;             // ��Ȩ��Ĺ�ϣֵ����

        // _Term���캯������������ʼ��ϣֵ���������Ȩ��ϣֵ
        _Term(std::string w, int weight);
    };

    std::vector<_Term> _terms;                                                                                // �ı����еĴ���
    std::unordered_map<std::string, int> _term_frequency;                                                     // �洢���д����Ӧ�Ĵ�Ƶ�Ĺ�ϣ��
    std::unordered_set<std::string> _stop_words;                                                              // ͣ�ôʵļ���

    // ��ͣ�ôʴʵ��м���ͣ�ô�
    void loadStopWords(const std::string& filePath);
};

#endif // SIMHASHER_H_
