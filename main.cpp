#include "simhash.h"
#include "plagiarism_check.h"

using std::endl;
using std::cerr;
using std::exception;

int main(int argc, char* argv[]) {
    
    // ��������в�������
    if (argc != 4) {
        cerr << "�����ʽ������ȷ�ĸ�ʽΪ��ÿ�������Կո��������" << endl;
        cerr << argv[0] << " <ԭ�����ĵ��ļ�����·��> <��Ϯ�����ĵ��ļ�����·��> <������ļ��ľ���·��>" << endl;
        cerr << "���磺" << endl;
        cerr << argv[0] << " C:\\tests\\org.txt C:\\tests\\org_2.txt C:\\tests\\ans.txt" << endl;
        return 0;
    }

    // ���ڷ������в����������
    // �����ַ����������洢�ļ�·��
    //string _orig_path = "E:\\2024_project\\plagiarism_check\\text\\orig.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_add.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_del.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_1.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_10.txt";
    //string _copy_path = "E:\\2024_project\\plagiarism_check\\text\\orig_0.8_dis_15.txt";
    //string _output_path = "E:\\2024_project\\plagiarism_check\\text\\answer.txt";

    /*PlagiarismChecker checker(_orig_path, _copy_path, _output_path);*/

    try {
        // ����PlagiarismChecker���󲢽������Ĳ���
        PlagiarismChecker checker(argv[1], argv[2], argv[3]);
        checker.checkPlagiarism();
    }
    catch (const exception& e) {
        // ���񲢴�ӡ�쳣��Ϣ
        cerr << "������Ϣ: " << e.what() << endl;
        return 0;
    }
    return 0;
};
