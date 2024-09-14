#include "SimHash.h"

int main() {
    std::string text1 = "�����������죬�����磬����������Ҫȥ����Ӱ";
    std::string text2 = "���������죬�������ʣ�������Ҫȥ����Ӱ";

    SimHash simhash1(text1);
    SimHash simhash2(text2);

    std::string hash1 = simhash1.CalculateSimHash();
    std::string hash2 = simhash2.CalculateSimHash();

    std::cout << "SimHash1: " << hash1 << std::endl;
    std::cout << "SimHash2: " << hash2 << std::endl;

    long similarity = SimHash::CalculateSimilarity(hash1, hash2);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Similarity: " << similarity << std::endl;

    return 0;
}