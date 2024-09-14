#include "plagiarism_checker.h"

using std::cout;
using std::cerr;
using std::endl;
using std::fixed;
using std::put_time;
using std::setprecision;
using std::tm;
using std::ios;
using std::locale;
using std::string;
using std::ifstream;
using std::ofstream;
using std::exception;
using std::runtime_error;
using std::stringstream;


PlagiarismChecker::PlagiarismChecker(const string& _orig_path, const string& _copy_path, const string& _output_path)
    : _orig_path(_orig_path), _copy_path(_copy_path), _output_path(_output_path) {};

void PlagiarismChecker::checkPlagiarism() {
    try {
        // ��ȡԭ�ĺͳ�Ϯ�ı�����
        string original_text = readFile(_orig_path);// ��ǰ������һ��������
        string copied_text = readFile(_copy_path);

        Simhasher simhash1(original_text, false);
        Simhasher simhash2(copied_text, false);

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
    catch (const exception& e) {
        // ���񲢴�ӡ�쳣��Ϣ
        cerr << "������Ϣ: " << e.what() << endl;
    }
}

string PlagiarismChecker::testReadFile(const string& path) {
    return readFile(path);
}

void PlagiarismChecker::testWriteResult(double similarity) {
    writeResult(similarity);
}

string PlagiarismChecker::readFile(const string& path) {
    try {
        // ���ļ�
        ifstream file(path);
        if (!file.is_open()) {
            throw runtime_error("�޷�������·���������ı��ļ�: " + path);
        }

        // ��ȡ�ļ����ݵ��ַ�����
        stringstream buffer;
        buffer << file.rdbuf();
        string content = buffer.str();

        // �ر��ļ�
        file.close();

        
        // ����ļ������Ƿ�Ϊ��
        if (buffer.str().empty()) {
            throw runtime_error("�����ļ�����Ϊ��");
        }
        return content;
    }
    catch (const exception& e) {
        // ���񲢴�ӡ�쳣��Ϣ
        cerr << "��ȡ�ļ�ʱ��������: " << e.what() << endl;
        throw;
    }
}

void PlagiarismChecker::writeResult(double similarity) {
    try {
        // ������ļ�
        ofstream outFile(_output_path, ios::app);
        outFile.imbue(locale("en_US.UTF-8"));
        // ����ļ��޷��򿪣��׳��쳣
        if (!outFile.is_open()) {
            throw runtime_error("�޷�������·����������ļ�: " + _output_path);
        }

        // ��ȡ��ǰʱ��
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        stringstream ss;

        // ���ת�����ʱ��
        tm tm = {};
        localtime_s(&tm, &now_c);
        ss << put_time(&tm, "%Y-%m-%d %X");

        // ���������ʽ�������𰸵���λС��
        outFile << endl;
        outFile << ss.str() << endl;
        outFile << " �����ĵ��ظ��ʣ�" << fixed << setprecision(2) << similarity << endl;
        outFile << "ԭ���ļ��ľ���·����" << _orig_path << " " << "��Ϯ���ļ��ľ���·����" << _copy_path << endl;

        // �ر��ļ�
        outFile.close();
    }
    catch (const exception& e) {
        cerr << "�������ʱ�Ĵ�����Ϣ: " << e.what() << endl;
        throw;
    }
}
