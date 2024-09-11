#include "simhash.h"
#include "plagiarism_check.h"

using std::endl;
using std::cerr;
using std::exception;

int main(int argc, char* argv[]) {
    
    // 检查命令行参数数量
    if (argc != 4) {
        cerr << "输入格式有误。正确的格式为（每个参数以空格隔开）：" << endl;
        cerr << argv[0] << " <原版论文的文件绝对路径> <抄袭版论文的文件绝对路径> <输出答案文件的绝对路径>" << endl;
        cerr << "例如：" << endl;
        cerr << argv[0] << " C:\\tests\\org.txt C:\\tests\\org_2.txt C:\\tests\\ans.txt" << endl;
        return 0;
    }

    // 用于非命令行参数输入测试
    // 定义字符串变量来存储文件路径
    //string _orig_path = "E:\\2024_project\\plagiarism_check\\text\\orig.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_add.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_del.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_1.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_10.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_15.txt";
    //string _output_path = "E:\\2024_project\\plagiarism_check\\text\\answer.txt";

    /*PlagiarismChecker checker(_orig_path, _copy_path, _output_path);*/

    try {
        // 创建PlagiarismChecker对象并进行论文查重
        PlagiarismChecker checker(argv[1], argv[2], argv[3]);
        checker.checkPlagiarism();
    }
    catch (const exception& e) {
        // 捕获并打印异常信息
        cerr << "错误信息: " << e.what() << endl;
        return 0;
    }
    return 0;
};
