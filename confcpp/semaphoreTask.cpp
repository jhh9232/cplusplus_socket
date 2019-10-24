#include "common.h"
#include "semaphoreTask.h"
//Json 파싱 : RapidJson


#define BUF_LEN 128
const int QSIZE = 3;

sem_t semaphore;
queue<JsonDatas> sockDatas;
int seque = 0;
int CREATE_EXIT = false;

struct sockaddr_in server_addr;
int SockStat;

pthread_mutex_t remutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t recond = PTHREAD_COND_INITIALIZER;

int ConnSocket()
{
    char buf[BUF_LEN + 1];

    if((SockStat = sock(PF_INET, SOCK_STREAM, 0)) <= FAIL)
    {
        cout << "can't create socket" << endl;
        return false;
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(haddr);
    server_addr.sin_port = htons(3304);
    if(connect(SockStat, (struct sockaddr *)&server_addr, sizeof(server_addr)) <= FAIL)
    {
        cout << "can't connect." << endl;
        return false;
    }
    cout << "SERVER CONNECT!!" << endl;
    return true;
}

void print_queue(queue<JsonDatas>& que)
{
    int cnt = 1;
    while(!que.empty())
    {
        cout << cnt << " : " << que.front().randomstr << endl;
        que.pop();
        cnt++;
    }
}

int num = 2;

void* THREAD_createstr(void* arg)
{   
    int n = *(int *)arg;
    unsigned int *mystate = (unsigned int *)arg;
    cout << "n : " << n << endl;
    cout << getpid() << endl;
    cout << pthread_self() << endl;
    *mystate = time(NULL) ^ getpid() ^ pthread_self();
    int totaltime = 0;
    for(int i = 0; i < 3; i++)  //지금은 3번반복이지만 나중에는 무한반복 예정.
    {
        int rantime = rand_r(mystate)%5 + 1;
        cout << "Thread " << n << " sleep time : " << rantime << endl;
        WAITTIME(rantime*1000); //rantime sec만큼 대기
        sem_wait(&semaphore);
        WAITTIME(200);    //200ms 대기
        JsonDatas tmpd = {
            .id = ++seque,
            .threadID = pthread_self(),
            .randomstr = createString(mystate),
            .timestamp = get_curtime()
        };
        sockDatas.push(tmpd);
        cout << "Thread " << n << " create string, size = " << sockDatas.size() << endl;
        sem_post(&semaphore);
        totaltime += rantime;
    }
    cout << "Thread " << n << " total time : " << totaltime << endl;
    return NULL;
}

void* THREAD_alarm(void* arg)
{
    int itcnt = 0;
    while(true)
    {
        cout << itcnt << "count iterate" << endl;
        chrono::system_clock::time_point start = chrono::system_clock::now();

        //thread3이 1sleep 후 2sleep을 하고
        //thread2가 3sleep 하면 +1개가 나오는 일이 발생.
        //4개가되면 create thread를 전부 wait를 주고 queue를 비운 뒤 다시 broadcast하고 싶으나 시간관계상...
        while(chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - start).count() <= 10000 && 
        sockDatas.size() <= QSIZE)
        {
            if(CREATE_EXIT)
            {
                cout << "CREATE THREAD ALL ENDED!!!" << endl;
                print_queue(sockDatas);
                pthread_cond_signal(&recond);
                pthread_mutex_destroy(&remutex);
                pthread_cond_destroy(&recond);
                return NULL;
            }
        }
        sem_wait(&semaphore);
        cout << "while start" << endl;
        pthread_cond_signal(&recond);
        while(!sockDatas.empty()){} //sockDatas가 빌떄까지 대기
        cout << "while end" << endl;
        sem_post(&semaphore);
        start = chrono::system_clock::now();
        itcnt++;
    }

    return NULL;
}

void* THREAD_recvdata(void* arg)
{
    if(true)
    {
        cout << "EXIT" << endl;
        exit(0);
    }
    int tcnt = 0;
    while(true)
    {
        if(tcnt >= 3)
        {
            cout << "========= EXIT THREAD =========" << endl;
            pthread_exit(NULL);
        }
        cout << "========= !!WAIT START!! =========" << endl;;
        pthread_cond_wait(&recond, &remutex);
        cout << "=========== RECEIVE!!! ===========" << endl;
        string Jstr = toJSON();
        cout << Jstr << endl;
        
        tcnt++;
    }
    return NULL;
}

string createString(unsigned int* randstate)
{
    int stringlen = rand_r(randstate)%20+1;
    string res = "";
    for(int i = 0; i < stringlen; i++)
    {
        char numstr = null;
        int bigsmall = rand_r(randstate)%2;
        if(bigsmall == 0)   //대문자
        {
            numstr = rand_r(randstate)%26+65;
        }
        else    //소문자
        {
            numstr = rand_r(randstate)%26+97;
        }
        string createstr(1, numstr);
        res.append(createstr);
    }
    return res;
}

string get_curtime()    //<string>, <ctime> 헤더파일 필요
{
    time_t curr_time;
    struct tm* curr_tm;

    curr_time = time(NULL);
    curr_tm = localtime(&curr_time);\

    string curday = to_string(curr_tm->tm_year + 1900) + "-" + to_string(curr_tm->tm_mon + 1) + "-" + to_string(curr_tm->tm_mday) + " ";
    string curtime = to_string(curr_tm->tm_hour) + ":" + to_string(curr_tm->tm_min) + ":" + to_string(curr_tm->tm_sec);
    curday.append(curtime);
    //insert into test(testtime) values(to_timestamp('2009-01-03 12:33:33');
    
    return curday;
}

string toJSON()
{
    string JsonStr = "[";
    const int append = 0;
    const int start = 1;
    const int end = 2;
    while(true)
    {
        string idname = "seqid";
        string tmp = strtoJson(idname, to_string(sockDatas.front().id), true, start);
        JsonStr.append(tmp);
        tmp = strtoJson("threadID", to_string(sockDatas.front().threadID), true, append);
        JsonStr.append(tmp);
        tmp = strtoJson("randomstr", sockDatas.front().randomstr, false, append);
        JsonStr.append(tmp);
        tmp = strtoJson("timestamp", sockDatas.front().timestamp, false, end);
        JsonStr.append(tmp);
        sockDatas.pop();
        if(sockDatas.empty())
            break;
        else
            JsonStr.append(", ");
    }
    JsonStr.append("]");
    return JsonStr;
}

//기존 JsonStr, key, value, 정수?문자?, 시작{, 추가, 끝}
string strtoJson(string key, string value, int isint,  int status)
{
    string JsonStr = "";
    //append
    if(status == 0)
    {
        JsonStr.append(appJson(key, value, isint)); //..., "key": "value"
        JsonStr.append(", ");   //..., "key": "value", 
    }
    //start
    else if(status == 1)
    {
        JsonStr.append("{");  //{
        JsonStr.append(appJson(key, value, isint)); //{"key": "value"
        JsonStr.append(", ");   //{"key": "value", 
    }
    //end
    else if(status == 2)
    {
        JsonStr.append(appJson(key, value, isint)); //..., "key": "value"
        JsonStr.append("}");    //..., "key": "value"}
    }
    return JsonStr;
}
string appJson(string key, string value, int isint)
{
    string JsonStr = "\"";  //"
    JsonStr.append(key);    //"seqid
    if(isint == 1)
    {
        JsonStr.append("\": "); //"seqid": 
        JsonStr.append(value);  //"seqid": 0
    }
    else
    {
        JsonStr.append("\": \"");   //"seqid": "
        JsonStr.append(value);  //"seqid": "a
        JsonStr.append("\""); //"seqid": "a"
    }
    return JsonStr;
}