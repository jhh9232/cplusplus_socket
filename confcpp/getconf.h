#ifndef GETCONF_H
#define GETCONF_H

#include "common.h"
#include <sys/types.h>

#define NEXTSECTION '['

class Config
{
private:
    FILE *fp;
    off_t last_seek;
    string last_section;
    int section_find;
    char key[80];
    char value[256];
public: 
    Config(); 
    ~Config();

    // ���������� open �Ѵ�. 
    int openCfg(const char *fname)
    {
        //���������� ������ true, ������ -1 ��ȯ
        fp = fopen(fname, "r");
        if (fp == NULL)
            return -1;
        return 1;
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