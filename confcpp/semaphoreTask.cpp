#include "common.h"
#include "semaphoreTask.h"
#include "httppost.h"
//Json 파싱 : RapidJson

#define QSIZE 100
const int MAXTIME = 90000;   //90000ms -> 90sec


const int RANDTIME = 10000;   //5000ms

sem_t semaphore;
queue<JsonDatas> sockDatas;
int seque = 0;

int CREATE_EXIT = false;

int totaltime = 0;

struct sockaddr_in server_addr;
int SockStat;


pthread_mutex_t remutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t recond = PTHREAD_COND_INITIALIZER;

pthread_cond_t* get_cond()
{
	return &recond;
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
void ClearQueue()
{
    //빈 큐와 스왑함으로써 큐의 메모리 해제와 동시에 초기화
    queue<JsonDatas> emptyque;
    swap(sockDatas, emptyque);
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
    for(int i = 0; true; i++)  //지금은 3번반복이지만 나중에는 무한반복 예정.
    {
        int rantime = rand_r(mystate) % (RANDTIME - 999);	//0~9000초까지
        cout << "Thread " << n << " sleep time : " << rantime << endl;
        WAITTIME(rantime + 1000); //rantime sec만큼 대기
		sem_wait(&semaphore);
		
        //LOCK
        totaltime += rantime;
        JsonDatas tmpd = {
            .id = ++seque,
            .threadID = pthread_self(),
            .randomstr = createString(mystate),
            .timestamp = get_curtime()
        };
        sockDatas.push(tmpd);
        cout << "Thread " << n << " create string, size = " << sockDatas.size() << endl;
        if(sockDatas.size() > QSIZE || totaltime >= MAXTIME)
        {
            totaltime = 0;
            pthread_cond_signal(&recond);
        }
        else
        {
            sem_post(&semaphore);
        }
		//LOCK

		WAITTIME(200);
    }
    cout << "Thread " << n << " total time : " << totaltime << endl;
    return NULL;
}

void* THREAD_recvdata(void* arg)
{
    while(!CREATE_EXIT)
    {
        cout << "========= !!WAIT START!! =========" << endl;;
        pthread_cond_wait(&recond, &remutex);
        cout << "=========== RECEIVE!!! ===========" << endl;
        queue<JsonDatas> temp = sockDatas;
        ClearQueue();
        sem_post(&semaphore);
        string Jstr = toJSON(&temp);
        cout << Jstr << endl;
		PostData("10.0.0.95", "/postdata.php", Jstr.c_str());
    }
	pthread_mutex_destroy(&remutex);
	pthread_cond_destroy(&recond);
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
    curr_tm = localtime(&curr_time);

    string curday = to_string(curr_tm->tm_year + 1900) + "-" + to_string(curr_tm->tm_mon + 1) + "-" + to_string(curr_tm->tm_mday) + " ";
    string curtime = to_string(curr_tm->tm_hour) + ":" + to_string(curr_tm->tm_min) + ":" + to_string(curr_tm->tm_sec);
    curday.append(curtime);
    //insert into test(testtime) values(to_timestamp('2009-01-03 12:33:33');
    
    return curday;
}

string toJSON(queue<JsonDatas>* sockDatas)
{
    string JsonStr = "[";
    const int append = 0;
    const int start = 1;
    const int end = 2;
    while(!sockDatas->empty())
    {
        string idname = "seqid";
        string tmp = strtoJson(idname, to_string(sockDatas->front().id), true, start);
        JsonStr.append(tmp);
        tmp = strtoJson("threadID", to_string(sockDatas->front().threadID), true, append);
        JsonStr.append(tmp);
        tmp = strtoJson("randomstr", sockDatas->front().randomstr, false, append);
        JsonStr.append(tmp);
        tmp = strtoJson("timestamp", sockDatas->front().timestamp, false, end);
        JsonStr.append(tmp);
        JsonStr.append(", ");
        sockDatas->pop();
    }
	JsonStr.erase(JsonStr.length()-2, 2);
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