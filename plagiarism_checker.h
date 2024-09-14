#ifndef PLAGIARISM_CHECKER_H_
#define PLAGIARISM_CHECKER_H_

#include <ctime>
#include "simhasher.h"

// �����ƪ���Ĳ��ص�ȫ���̣���Ҫ�Ǵ��ļ������������ļ�
class PlagiarismChecker 
{
public:
    // ���캯������ʼ�����Ա����
    PlagiarismChecker(const std::string& _orig_path, const std::string& _copy_path, const std::string& _output_path);

    // ���Ĳ��ش����뵽�����ȫ����
    void checkPlagiarism();

    // �����ӿ����ڲ���
    std::string testReadFile(const std::string& path);
    void testWriteResult(double similarity);

private:
    std::string _orig_path;                         //����ԭ�ĵ��ļ�����·��
    std::string _copy_path;                         //��Ϯ�����ĵ��ļ�����·��
    std::string _output_path;                       //������ļ��ľ���·��

    // ��ȡ�ļ�����Ϊ�ַ���
    std::string readFile(const std::string& path);

    // �������ظ��ʽ��д������ļ�
    void writeResult(double similarity);
};

#endif // PLAGIARISM_CHECKER_H_
