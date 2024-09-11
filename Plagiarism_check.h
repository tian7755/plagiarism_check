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
    // 构造函数，初始化类成员变量
    PlagiarismChecker(const std::string& origPath, const std::string& copyPath, const std::string& outputPath)
        : origPath(origPath), copyPath(copyPath), outputPath(outputPath) {}

    // 检查抄袭的公共方法
    void checkPlagiarism() {
        // 读取原文和抄袭文件内容
        std::string originalText = readFile(origPath);
        std::string copiedText = readFile(copyPath);

        SimHash simhash1(originalText);
        SimHash simhash2(copiedText);

        std::string hash1 = simhash1.CalculateSimHash();
        std::string hash2 = simhash2.CalculateSimHash();

        // 计算两个文本的相似度
        double similarity = simhash1.CalculateSimilarity(hash1, hash2);
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "两论文的查重率: " << similarity << std::endl;

        // 将结果写入输出文件
        writeResult(similarity);
    }

private:
    std::string origPath;
    std::string copyPath;
    std::string outputPath;

    // 读取文件内容到字符串
    std::string readFile(const std::string& path) {
        // 打开文件
        std::ifstream file(path);

        // 如果文件无法打开，抛出异常
        if (!file.is_open()) {
            throw std::runtime_error("无法打开以下路径的输入文件: " + path);
        }

        // 读取文件内容到字符串流
        std::stringstream buffer;
        buffer << file.rdbuf();
        // 关闭文件
        file.close();
        // 返回读取的内容
        return buffer.str();
    }

    // 将计算结果写入输出文件
    void writeResult(double similarity) {
        // 获取当前时间
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;

        // 用于存放转换后的时间
        std::tm tm = {};

        // 格式化时间
        localtime_s(&tm, &now_c); // 安全地转换时间
        ss << std::put_time(&tm, "%Y-%m-%d %X"); // 使用 &tm 而不是直接调用 localtime

        // 打开输出文件
        std::ofstream outFile(outputPath, std::ios::app); // 使用app模式追加内容
        outFile.imbue(std::locale("en_US.UTF-8")); // 设置 locale 为 UTF-8

        // 如果文件无法打开，抛出异常
        if (!outFile.is_open()) {
            throw std::runtime_error("无法打开以下路径的输出文件: " + outputPath);
        }
        // 写入一个换行符，确保在文件末尾另起一行
        outFile << std::endl;
        // 设置输出格式，保留两位小数
        outFile <<ss.str() << std::endl;
        outFile <<" 两论文的查重率：" << std::fixed << std::setprecision(2) << similarity << std::endl;
        outFile <<"文1文件的绝对路径：" << origPath << " " <<"文2文件的绝对路径：" << copyPath << std::endl;
        // 关闭文件
        outFile.close();
    }
};

#endif // _PLAGIARISM_CHECK_
