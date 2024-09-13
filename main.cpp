#include "process.h"

using std::endl;
using std::cout;
using std::cerr;
using std::string;
using std::exception;

int process(int argc, char* argv[]) {
    // 检查命令行参数数量
    if (argc != 4) {
        cerr << "输入格式有误。正确的格式为（每个参数以空格隔开）：" << endl;
        cerr << argv[0] << " <原版论文的文件绝对路径> <抄袭版论文的文件绝对路径> <输出答案文件的绝对路径>" << endl;
        cerr << "例如：" << endl;
        cerr << argv[0] << " C:\\test\\org.txt C:\\test\\org_2.txt C:\\test\\ans.txt" << endl;
        return 1;
    }

    try {
        // 创建PlagiarismChecker对象并进行论文查重
        PlagiarismChecker checker(argv[1], argv[2], argv[3]);
        checker.checkPlagiarism();
    }
    catch (const exception& e) {
        // 捕获并打印异常信息
        cerr << "错误信息: " << e.what() << endl;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    return process(argc, argv);
};
