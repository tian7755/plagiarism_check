#pragma once
#ifndef _PLAGIARISM_CHECK_
#define _PLAGIARISM_CHECK_

#include "SimHash.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>


class PlagiarismChecker {
public:
    // ���캯������ʼ�����Ա����
    PlagiarismChecker(const std::string& origPath, const std::string& copyPath, const std::string& outputPath)
        : origPath(origPath), copyPath(copyPath), outputPath(outputPath) {}

    // ��鳭Ϯ�Ĺ�������
    void checkPlagiarism() {
        // ��ȡԭ�ĺͳ�Ϯ�ļ�����
        std::string originalText = readFile(origPath);
        std::string copiedText = readFile(copyPath);

        SimHash simhash1(originalText);
        SimHash simhash2(copiedText);

        std::string hash1 = simhash1.CalculateSimHash();
        std::string hash2 = simhash2.CalculateSimHash();

        // ���������ı������ƶ�
        double similarity = simhash1.CalculateSimilarity(hash1, hash2);
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "�����ĵĲ�����: " << similarity << std::endl;

        // �����д������ļ�
        writeResult(similarity);
    }

private:
    std::string origPath;
    std::string copyPath;
    std::string outputPath;

    // ��ȡ�ļ����ݵ��ַ���
    std::string readFile(const std::string& path) {
        // ���ļ�
        std::ifstream file(path);

        // ����ļ��޷��򿪣��׳��쳣
        if (!file.is_open()) {
            throw std::runtime_error("�޷�������·���������ļ�: " + path);
        }

        // ��ȡ�ļ����ݵ��ַ�����
        std::stringstream buffer;
        buffer << file.rdbuf();
        // �ر��ļ�
        file.close();
        // ���ض�ȡ������
        return buffer.str();
    }

    // ��������д������ļ�
    void writeResult(double similarity) {
        // ��ȡ��ǰʱ��
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;

        // ���ڴ��ת�����ʱ��
        std::tm tm = {};

        // ��ʽ��ʱ��
        localtime_s(&tm, &now_c); // ��ȫ��ת��ʱ��
        ss << std::put_time(&tm, "%Y-%m-%d %X"); // ʹ�� &tm ������ֱ�ӵ��� localtime

        // ������ļ�
        std::ofstream outFile(outputPath, std::ios::app); // ʹ��appģʽ׷������
        outFile.imbue(std::locale("en_US.UTF-8")); // ���� locale Ϊ UTF-8

        // ����ļ��޷��򿪣��׳��쳣
        if (!outFile.is_open()) {
            throw std::runtime_error("�޷�������·��������ļ�: " + outputPath);
        }
        // д��һ�����з���ȷ�����ļ�ĩβ����һ��
        outFile << std::endl;
        // ���������ʽ��������λС��
        outFile <<ss.str() << std::endl;
        outFile <<" �����ĵĲ����ʣ�" << std::fixed << std::setprecision(2) << similarity << std::endl;
        outFile <<"��1�ļ��ľ���·����" << origPath << " " <<"��2�ļ��ľ���·����" << copyPath << std::endl;
        // �ر��ļ�
        outFile.close();
    }
};

#endif // _PLAGIARISM_CHECK_
