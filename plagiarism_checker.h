#ifndef PLAGIARISM_CHECKER_H_
#define PLAGIARISM_CHECKER_H_

#include <ctime>
#include "simhasher.h"

// 打包两篇论文查重的全过程，主要是从文件输入跟输出到文件
class PlagiarismChecker 
{
public:
    // 构造函数，初始化类成员变量
    PlagiarismChecker(const std::string& _orig_path, const std::string& _copy_path, const std::string& _output_path);

    // 论文查重从输入到输出的全过程
    void checkPlagiarism();

    // 公共接口用于测试
    std::string testReadFile(const std::string& path);
    void testWriteResult(double similarity);

private:
    std::string _orig_path;                         //论文原文的文件绝对路径
    std::string _copy_path;                         //抄袭版论文的文件绝对路径
    std::string _output_path;                       //输出答案文件的绝对路径

    // 读取文件内容为字符串
    std::string readFile(const std::string& path);

    // 将论文重复率结果写入输出文件
    void writeResult(double similarity);
};

#endif // PLAGIARISM_CHECKER_H_
