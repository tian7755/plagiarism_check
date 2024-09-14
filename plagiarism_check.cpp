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
    // ��ȡԭ�ĺͳ�Ϯ�ı�����
    string original_text = readFile(_orig_path);
    string copied_text = readFile(_copy_path);

    SimHash simhash1(original_text);
    SimHash simhash2(copied_text);

    // �����������ĵ�Simhashֵ
    string hash1 = simhash1.calculateSimHash();
    string hash2 = simhash2.calculateSimHash();

    // �����������ĵ��ظ���
    double similarity = simhash1.calculateSimilarity(hash1, hash2);
    cout << fixed << setprecision(2);
    cout << "�����ĵ��ظ���: " << similarity << endl;

    // �������ظ��ʽ��д������ļ�
    writeResult(similarity);
}

string PlagiarismChecker::readFile(const string& path) {
    // ���ļ�
    ifstream file(path);

    // ����ļ��޷��򿪣��׳��쳣
    if (!file.is_open()) {
        throw runtime_error("�޷�������·���������ı��ļ�: " + path);
    }

    // ��ȡ�ļ����ݵ��ַ�����
    stringstream buffer;
    buffer << file.rdbuf();

    // �ر��ļ�
    file.close();
    return buffer.str();
}

void PlagiarismChecker::writeResult(double similarity) {
    // ��ȡ��ǰʱ��
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    stringstream ss;

    // ���ת�����ʱ��
    tm tm = {};

    // ��ʽ��ʱ��
    localtime_s(&tm, &now_c); 
    ss << put_time(&tm, "%Y-%m-%d %X"); 

    // ������ļ�
    ofstream outFile(_output_path, ios::app); 
    outFile.imbue(locale("en_US.UTF-8")); 

    // ����ļ��޷��򿪣��׳��쳣
    if (!outFile.is_open()) {
        throw runtime_error("�޷�������·����������ļ�: " + _output_path);
    }

    // ���������ʽ�������𰸵���λС��
    outFile << endl;
    outFile << ss.str() << endl;
    outFile << " �����ĵ��ظ��ʣ�" << fixed << setprecision(2) << similarity << endl;
    outFile << "ԭ���ļ��ľ���·����" << _orig_path << " " << "��Ϯ���ļ��ľ���·����" << _copy_path << endl;

    // �ر��ļ�
    outFile.close();
}
