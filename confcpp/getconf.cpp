#include "getconf.h"

const char *token = "=";

Config::Config()
{
    last_seek = 0;
    fp = NULL;  
    last_section = "";  
}
Config::~Config()
{
    cout << "destroy Config" << endl;
    fclose(fp);
}
void Config::Split(char *data)
{
    char *sp, *ep; 
    memset(key, 0x00, 80);
    memset(value, 0x00, 256);
    sp = strstr(data, "=");
    ep = strstr(data, "\n");    //마지막줄에 \n이 없으면 세그멘테이션오류 발생
    if(ep == NULL)  //세그먼테이션 오류 해결
    {
        ep = strstr(data, "=");
        while(*ep != null)
        {
            ep++;
        }
    }
    //key추출
    memcpy(key, data, sp-data);
    key[sp-data+1] = null;
    // memcpy(value, sp+1, (ep-sp));
    // key[sp-data+1] = null;

    //value추출
    memcpy(value, sp+1, (ep-sp));
    value[ep-sp-1] = null;

    // cout << "[key]:" << key << "|" << endl;
    // cout << "[value]:" << value << "|" << endl;
}
int Config::findSection(string Section)
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

char* Config::nextItem()
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
        if(buf[0] == NEXTSECTION)
        return NULL;
    Split(buf);
    return key;
}
char* Config::getValue(string Section, const char *Name)
{
    rewind(fp); //fp포인터를 파일의 시작점으로 옮김.
    char buf[256];
    int NameLen = strlen(Name);
    string SectionName;
    SectionName = "["+Section+"]";
    section_find = 0;
    value[0] = null;
    while(fgets(buf, 255, fp))
    {
        if (buf[0] == '#')
            continue;

        if (section_find == 1)
        {
            //다른 섹션으로 넘어가면 NULL Return
            if(buf[0] == NEXTSECTION)
                return NULL;
            if(strncmp(buf, Name, NameLen) == 0)
            {
                //printf("%s", buf);
                Split(buf);
                //*Name을 포함하는 것이 아닌 *Name과 일치하는 key값을 찾음.
                //strlen(key) == strlen(Name) 도 됨.
                if(strncmp(key, Name, strlen(key)) == 0)
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


int get_valueNUM(string fileName, string sectionName, string keyName)
{
    Config *agentCfg = new Config();
    if (agentCfg->openCfg(fileName.c_str()) == -1)
    {
        perror("Error");    //error출력

        delete agentCfg;
        return FAIL;
    }
    
    char *threadvalue = agentCfg->getValue(sectionName, keyName.c_str());
    int tsize = EMPTY;

    if(threadvalue == NULL)
    {
        cout << "does not key" << endl;;
        
        delete agentCfg;
        return FAIL;
    }
    else
    {
        try
        {
            string stoistr = threadvalue;
            tsize = stoi(stoistr);
            cout << "value" << tsize << endl;
            
            delete agentCfg;
            return tsize;
        }
        catch(const exception& e)
        {
            if(!strncmp(e.what(), "stoi", strlen(e.what())))
                cout << "converting error integer!" << endl;
            else
                cout << "error :" << e.what() << "|" << endl;
            
            delete agentCfg;
            return FAIL;
        }
    }
}

/*
int main()
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
*/