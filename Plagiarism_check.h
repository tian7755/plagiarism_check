#ifndef _PLAGIARISM_CHECK_
#define _PLAGIARISM_CHECK_

#include <ctime>
#include "simhash.h"

// �����ƪ���Ĳ��ص�ȫ���̣���Ҫ�Ǵ��ļ������������ļ�
class PlagiarismChecker 
{
public:
    // ���캯������ʼ�����Ա����
    PlagiarismChecker(const std::string& _orig_path, const std::string& _copy_path, const std::string& _output_path);

    // ���Ĳ��ش����뵽�����ȫ����
    void checkPlagiarism();

private:
    std::string _orig_path;                         //����ԭ�ĵ��ļ�����·��
    std::string _copy_path;                         //��Ϯ�����ĵ��ļ�����·��
    std::string _output_path;                       //������ļ��ľ���·��

    // ��ȡ�ļ�����Ϊ�ַ���
    std::string readFile(const std::string& path);

    // �������ظ��ʽ��д������ļ�
    void writeResult(double similarity);
};

#endif // _PLAGIARISM_CHECK_
