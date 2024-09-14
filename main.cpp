#include "process.h"

using std::endl;
using std::cout;
using std::cerr;
using std::string;
using std::exception;

int process(int argc, char* argv[]) {
    // ��������в�������
    if (argc != 4) {
        cerr << "�����ʽ������ȷ�ĸ�ʽΪ��ÿ�������Կո��������" << endl;
        cerr << argv[0] << " <ԭ�����ĵ��ļ�����·��> <��Ϯ�����ĵ��ļ�����·��> <������ļ��ľ���·��>" << endl;
        cerr << "���磺" << endl;
        cerr << argv[0] << " C:\\test\\org.txt C:\\test\\org_2.txt C:\\test\\ans.txt" << endl;
        return 1;
    }

    try {
        // ����PlagiarismChecker���󲢽������Ĳ���
        PlagiarismChecker checker(argv[1], argv[2], argv[3]);
        checker.checkPlagiarism();
    }
    catch (const exception& e) {
        // ���񲢴�ӡ�쳣��Ϣ
        cerr << "������Ϣ: " << e.what() << endl;
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    return process(argc, argv);
};
