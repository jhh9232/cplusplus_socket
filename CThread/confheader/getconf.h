#ifndef GETCONF_H
#define GETCONF_H

#include "common.h"
#include <sys/types.h>

#define NEXTSECTION '['
#define NOTHREAD -1

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

    // 설정파일을 open 한다. 
    int openCfg(const char *fname)
    {
        //설정파일이 있으면 true, 없으면 false 반환
        fp = fopen(fname, "r");
        if (fp == NULL)
            return false;
        return true;
    }

    // 주어진 문자열을 Key와 Value로 나눈다. 
    void Split(char *);

    // section이 존재하는지 검사한다. 
    int findSection(string Section);

    // fineSection으로 검색된 색션의 아이템 목록을 얻어온다. 
    // 얻어온 아이템에서 Key,Value를 얻어온다. 
    char* nextItem();

    // section과, key를 이용해서 value를 얻어온다. 
    char* getValue(string Section, const char *Name);

    // Value를 얻어온다. 
    char* getValue()
    {
        return value;
    }

    // Key를 얻어온다. 
    char* getKey()
    {
        return key;
    }
};

//윗 클래스들을 사용하여 sectionName의 keyName의 value를 숫자로 변환하여 리턴
int get_valueNUM(string fileName, string sectionName, string keyName);

#endif