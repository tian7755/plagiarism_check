#include "Plagiarism_check.h"

int main(int argc, char* argv[]) {
    // 检查命令行参数数量
    if (argc != 4) {
        std::cerr << "输入格式错误。正确的格式为：" << std::endl;
        std::cerr << argv[0] << " <文1文件的绝对路径> <文2文件的绝对路径> <输出文件的绝对路径>" << std::endl;
        std::cerr << "例如：" << std::endl;
        std::cerr << argv[0] << " C:\\tests\\org.txt C:\\tests\\org_2.txt C:\\tests\\ans.txt" << std::endl;
        return 0;
    }

    // 定义字符串变量来存储文件路径
    //std::string origPath = "E:\\2024_project\\plagiarism_check\\text\\orig.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_add.txt";
    ///*std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_del.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_1.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_10.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_15.txt";*/
    //std::string outputPath = "E:\\2024_project\\plagiarism_check\\text\\answer.txt";

    // 创建PlagiarismChecker对象并检查抄袭
    /*PlagiarismChecker checker(origPath, copyPath, outputPath);*/

    try {
        PlagiarismChecker checker(argv[1], argv[2], argv[3]);
        checker.checkPlagiarism();
    }
    catch (const std::exception& e) {
        // 捕获并打印异常信息
        std::cerr << "错误信息: " << e.what() << std::endl;
        return 0;
    }

    return 0;
};
