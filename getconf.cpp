// Standard C++ Library 
#include <iostream>
#include <string>

// Standard C Library 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


using namespace std;

const char *token = "=";
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
    Config() 
    {
        last_seek = 0;
        fp = NULL;  
        last_section = "";  
    };  
    ~Config()
    {   
      fclose(fp);
    }
        
    // 주어진 문자열을 Key와 Value로 나눈다. 
    int Split(char *data)
    {
        char *sp, *ep; 
        memset(key, 0x00, 80);
        memset(value, 0x00, 256);
        sp = strstr(data, "=");
        ep = strstr(data, "\n");    //마지막줄에 \n이 없으면 세그멘테이션오류 발생 수정바람
        memcpy(key, data, sp-data); 
        key[sp-data+1] = '\0';

        memcpy(value, sp+1, (ep-sp));
        key[sp-data+1] = '\0';

        memcpy(value, sp+1, (ep-sp));
        value[ep-sp-1] = '\0';
    }

    // 설정파일을 open 한다. 
    int openCfg(const char *fname)
    {
        //설정파일이 있으면 true, 없으면 -1 반환
        fp = fopen(fname, "r");
        if (fp == NULL)
            return -1;
        return 1;
    }

    // section이 존재하는지 검사한다. 
    int findSection(string Section)
    {
        rewind(fp); //fp포인터를 파일의 시작점으로 옮김.
        string SectionName;
        SectionName = "["+Section+"]";
        char buf[256];
        int rtv = 0;
        while(fgets(buf, 255, fp))
        {
            //buf안에 [SectionName]이 포함되어 있으면 fp포인터를 [SectionName]에 위치시키고 반복문 종료
            if (strncmp(SectionName.c_str(), buf, SectionName.length()) == 0)
            {
                last_seek = ftell(fp);
                rtv = 1;
                break;
            }
        }
        return rtv;
    }

    // fineSection으로 검색된 색션의 아이템 목록을 얻어온다. 
    // 얻어온 아이템에서 Key,Value를 얻어온다. 
    char* nextItem()
    {
        //fp는 검색된 section의 위치부터 시작
        char buf[256];
        while(1)
        {
            //데이터가 없으면 NULL 반환
            if(fgets(buf, 255, fp) == NULL)
                return NULL;
            //conf파일이 주석이 아니면
            if(buf[0] != '#')
            {
                //buf가 = 이면 반복문 종료
                if (strstr(buf, "=")) break;
            }
         }
         //다음 데이터가 다른 섹션이면...
         if(buf[0] == '[')
            return NULL;
        Split(buf);
        return key;
    }

    // section과, key를 이용해서 value를 얻어온다. 
    char* getValue(string Section, const char *Name)
    {
        rewind(fp); //fp포인터를 파일의 시작점으로 옮김.
        char buf[256];
        int NameLen = strlen(Name);
        string SectionName;
        SectionName = "["+Section+"]";
        section_find = 0;
        value[0] = '\0';
        while(fgets(buf, 255, fp))
        {
            if (buf[0] == '#')
               continue;

            if (section_find == 1)
            {
                if(strncmp(buf, Name, NameLen) == 0)
                {
                    //printf("%s", buf);
                    Split(buf);
                    return value;
                }
            }
            else if (strncmp(SectionName.c_str(), buf, Section.length()+2) == 0)
            {
                //printf("Find\n");
                last_seek = ftell(fp);
                //printf("Last_seek %d\n", last_seek);
                section_find = 1;
            }
        }
        return NULL;
    }
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

int main(int argc, char *argv[])
{
    string sectionName = "thread count";
    string keyName = "count";
    Config *agentCfg;
    int rtv;
    agentCfg = new Config();
    rtv = agentCfg->openCfg("./config.conf");
    if (rtv == -1)
    {
        perror("Error");    //error출력
        return 0;
    }
    
    
    printf("|%s of [%s]=%s|\n", keyName.c_str(), sectionName.c_str(), agentCfg->getValue(sectionName, keyName.c_str()));
    rtv = agentCfg->findSection("thread count");    //section 검사
    if (!rtv)
    {
        return 1;
    }
    while(agentCfg->nextItem())
    {
        printf("|%s:%s|\n", agentCfg->getKey(), agentCfg->getValue());
    }
    return 0;
}