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
        
    // �־��� ���ڿ��� Key�� Value�� ������. 
    int Split(char *data)
    {
        char *sp, *ep; 
        memset(key, 0x00, 80);
        memset(value, 0x00, 256);
        sp = strstr(data, "=");
        ep = strstr(data, "\n");    //�������ٿ� \n�� ������ ���׸����̼ǿ��� �߻� �����ٶ�
        memcpy(key, data, sp-data); 
        key[sp-data+1] = '\0';

        memcpy(value, sp+1, (ep-sp));
        key[sp-data+1] = '\0';

        memcpy(value, sp+1, (ep-sp));
        value[ep-sp-1] = '\0';
    }

    // ���������� open �Ѵ�. 
    int openCfg(const char *fname)
    {
        //���������� ������ true, ������ -1 ��ȯ
        fp = fopen(fname, "r");
        if (fp == NULL)
            return -1;
        return 1;
    }

    // section�� �����ϴ��� �˻��Ѵ�. 
    int findSection(string Section)
    {
        rewind(fp); //fp�����͸� ������ ���������� �ű�.
        string SectionName;
        SectionName = "["+Section+"]";
        char buf[256];
        int rtv = 0;
        while(fgets(buf, 255, fp))
        {
            //buf�ȿ� [SectionName]�� ���ԵǾ� ������ fp�����͸� [SectionName]�� ��ġ��Ű�� �ݺ��� ����
            if (strncmp(SectionName.c_str(), buf, SectionName.length()) == 0)
            {
                last_seek = ftell(fp);
                rtv = 1;
                break;
            }
        }
        return rtv;
    }

    // fineSection���� �˻��� ������ ������ ����� ���´�. 
    // ���� �����ۿ��� Key,Value�� ���´�. 
    char* nextItem()
    {
        //fp�� �˻��� section�� ��ġ���� ����
        char buf[256];
        while(1)
        {
            //�����Ͱ� ������ NULL ��ȯ
            if(fgets(buf, 255, fp) == NULL)
                return NULL;
            //conf������ �ּ��� �ƴϸ�
            if(buf[0] != '#')
            {
                //buf�� = �̸� �ݺ��� ����
                if (strstr(buf, "=")) break;
            }
         }
         //���� �����Ͱ� �ٸ� �����̸�...
         if(buf[0] == '[')
            return NULL;
        Split(buf);
        return key;
    }

    // section��, key�� �̿��ؼ� value�� ���´�. 
    char* getValue(string Section, const char *Name)
    {
        rewind(fp); //fp�����͸� ������ ���������� �ű�.
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
        perror("Error");    //error���
        return 0;
    }
    
    
    printf("|%s of [%s]=%s|\n", keyName.c_str(), sectionName.c_str(), agentCfg->getValue(sectionName, keyName.c_str()));
    rtv = agentCfg->findSection("thread count");    //section �˻�
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