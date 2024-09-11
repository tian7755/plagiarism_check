#include "plagiarism_check.h"

using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;
using std::put_time;
using std::string;
using std::ifstream;
using std::runtime_error;
using std::stringstream;
using std::tm;
using std::ofstream;
using std::ios;
using std::locale;

PlagiarismChecker::PlagiarismChecker(const string& _orig_path, const string& _copy_path, const string& _output_path)
    : _orig_path(_orig_path), _copy_path(_copy_path), _output_path(_output_path) {}

void PlagiarismChecker::checkPlagiarism() {
    // 读取原文和抄袭文本内容
    string original_text = readFile(_orig_path);
    string copied_text = readFile(_copy_path);

    SimHash simhash1(original_text);
    SimHash simhash2(copied_text);

    // 计算两个论文的Simhash值
    string hash1 = simhash1.calculateSimHash();
    string hash2 = simhash2.calculateSimHash();

    // 计算两个论文的重复率
    double similarity = simhash1.calculateSimilarity(hash1, hash2);
    cout << fixed << setprecision(2);
    cout << "两论文的重复率: " << similarity << endl;

    // 将论文重复率结果写入输出文件
    writeResult(similarity);
}

string PlagiarismChecker::readFile(const string& path) {
    // 打开文件
    ifstream file(path);

    // 如果文件无法打开，抛出异常
    if (!file.is_open()) {
        throw runtime_error("无法打开以下路径的输入文本文件: " + path);
    }

    // 读取文件内容到字符串流
    stringstream buffer;
    buffer << file.rdbuf();

    // 关闭文件
    file.close();
    return buffer.str();
}

void PlagiarismChecker::writeResult(double similarity) {
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    stringstream ss;

    // 存放转换后的时间
    tm tm = {};

    // 格式化时间
    localtime_s(&tm, &now_c); 
    ss << put_time(&tm, "%Y-%m-%d %X"); 

    // 打开输出文件
    ofstream outFile(_output_path, ios::app); 
    outFile.imbue(locale("en_US.UTF-8")); 

    // 如果文件无法打开，抛出异常
    if (!outFile.is_open()) {
        throw runtime_error("无法打开以下路径的输出答案文件: " + _output_path);
    }

    // 设置输出格式，保留答案的两位小数
    outFile << endl;
    outFile << ss.str() << endl;
    outFile << " 两论文的重复率：" << fixed << setprecision(2) << similarity << endl;
    outFile << "原版文件的绝对路径：" << _orig_path << " " << "抄袭版文件的绝对路径：" << _copy_path << endl;

    // 关闭文件
    outFile.close();
}
