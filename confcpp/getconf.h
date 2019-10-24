#ifndef GETCONF_H
#define GETCONF_H

#include "common.h"
#include <sys/types.h>

#define NEXTSECTION '['
#define BUF_LEN 256

class Config
{
private:
    FILE *fp;
    off_t last_seek;
    string last_section;
    int section_find;
    char key[80];
    char value[BUF_LEN];
public: 
    Config(); 
    ~Config();

    // ���������� open �Ѵ�. 
    int openCfg(const char *fname)
    {
        //���������� ������ true, ������ false ��ȯ
        fp = fopen(fname, "r");
        if (fp == NULL)
            return false;
        return true;
    }

    // �־��� ���ڿ��� Key�� Value�� ������. 
    void Split(char *);

    // section�� �����ϴ��� �˻��Ѵ�. 
    int findSection(string Section);

    // fineSection���� �˻��� ������ ������ ����� ���´�. 
    // ���� �����ۿ��� Key,Value�� ���´�. 
    char* nextItem();

    // section��, key�� �̿��ؼ� value�� ���´�. 
    char* getValue(string Section, const char *Name);

    // Value�� ���´�. 
    char* getValue()
    {
        return value;
    }

    // Key�� ���´�. 
    char* getKey()
    {
        return key;
    }
};

//�� Ŭ�������� ����Ͽ� sectionName�� keyName�� value�� ���ڷ� ��ȯ�Ͽ� ����
int get_valueNUM(string fileName, string sectionName, string keyName);

#endif