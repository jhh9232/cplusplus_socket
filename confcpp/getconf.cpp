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
    fclose(fp);
}
void Config::Split(char *data)
{
    char *sp, *ep; 
    memset(key, 0x00, 80);
    memset(value, 0x00, BUF_LEN);
    sp = strstr(data, "=");
    ep = strstr(data, "\n");    //�������ٿ� \n�� ������ ���׸����̼ǿ��� �߻�
    if(ep == NULL)              //���׸����̼� ���� �ذ�
    {
        ep = strstr(data, "=");
        while(*ep != null)
        {
            ep++;
        }
    }
    //key����
    memcpy(key, data, sp-data);
    key[sp-data+1] = null;
    // memcpy(value, sp+1, (ep-sp));
    // key[sp-data+1] = null;

    //value����
    memcpy(value, sp+1, (ep-sp));
    value[ep-sp-1] = null;

    // cout << "[key]:" << key << "|" << endl;
    // cout << "[value]:" << value << "|" << endl;
}
int Config::findSection(string Section)
{
    rewind(fp); //fp�����͸� ������ ���������� �ű�.
    string SectionName;
    SectionName = "["+Section+"]";
    char buf[BUF_LEN];
    int rtv = 0;
    while(fgets(buf, BUF_LEN-1, fp))
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

char* Config::nextItem()
{
    //fp�� �˻��� section�� ��ġ���� ����
    char buf[BUF_LEN];
    while(1)
    {
        //�����Ͱ� ������ NULL ��ȯ
        if(fgets(buf, BUF_LEN-1, fp) == NULL)
            return NULL;
        //conf������ �ּ��� �ƴϸ�
        if(buf[0] != '#')
        {
            //buf�� = �̸� �ݺ��� ����
            if (strstr(buf, "=")) break;
        }
        }
        //���� �����Ͱ� �ٸ� �����̸�...
        if(buf[0] == NEXTSECTION)
        return NULL;
    Split(buf);
    return key;
}
char* Config::getValue(string Section, const char *KeyName)
{
    rewind(fp); //fp�����͸� ������ ���������� �ű�.
    char buf[BUF_LEN];
    int NameLen = strlen(KeyName);
    string SectionName;
    SectionName = "["+Section+"]";
    value[0] = null;
    section_find = false;
    while(fgets(buf, BUF_LEN-1, fp))
    {
        if (buf[0] == '#')
            continue;

        if (!strncmp(SectionName.c_str(), buf, Section.length()+2))
        {
            //printf("Find\n");
            last_seek = ftell(fp);
            //printf("Last_seek %d\n", last_seek);
            section_find = true;
        }
        else if(!strncmp(buf, KeyName, NameLen) && buf[NameLen] == '=')
        {
            //printf("%s", buf);
            Split(buf);
            return value;
        }
        else if(section_find && buf[0] == '[')
        {
            return NULL;
        }
    }
    return NULL;
}


int get_valueNUM(string fileName, string sectionName, string keyName)
{
    Config *agentCfg = new Config();
    if (agentCfg->openCfg(fileName.c_str()) == false)
    {
        perror("Error");    //error���

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
*/