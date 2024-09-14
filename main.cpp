#include "Plagiarism_check.h"

int main(int argc, char* argv[]) {
    // ��������в�������
    if (argc != 4) {
        std::cerr << "�����ʽ������ȷ�ĸ�ʽΪ��" << std::endl;
        std::cerr << argv[0] << " <��1�ļ��ľ���·��> <��2�ļ��ľ���·��> <����ļ��ľ���·��>" << std::endl;
        std::cerr << "���磺" << std::endl;
        std::cerr << argv[0] << " C:\\tests\\org.txt C:\\tests\\org_2.txt C:\\tests\\ans.txt" << std::endl;
        return 0;
    }

    // �����ַ����������洢�ļ�·��
    //std::string origPath = "E:\\2024_project\\plagiarism_check\\text\\orig.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_add.txt";
    ///*std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_del.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_1.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_10.txt";
    //std::string copyPath = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_15.txt";*/
    //std::string outputPath = "E:\\2024_project\\plagiarism_check\\text\\answer.txt";

    // ����PlagiarismChecker���󲢼�鳭Ϯ
    /*PlagiarismChecker checker(origPath, copyPath, outputPath);*/

    try {
        PlagiarismChecker checker(argv[1], argv[2], argv[3]);
        checker.checkPlagiarism();
    }
    catch (const std::exception& e) {
        // ���񲢴�ӡ�쳣��Ϣ
        std::cerr << "������Ϣ: " << e.what() << std::endl;
        return 0;
    }

    return 0;
};
