#include "../confheader/common.h"
#include "../confheader/semaphoreTask.h"
#include "../confheader/httppost.h"
#include "../confheader/get_parse_data.h"
//Json 파싱 : RapidJson

#define QSIZE 15
const int MAXTIME = 30000;   //90000ms -> 90sec
const int RANDTIME = 10000;   //10000s


sem_t semaphore;
sem_t timelock;
queue<JsonDatas> sockDatas;
int seque = 0;
int timeStatus = false;
int CREATE_EXIT = false;
vector<int> threadTime;
int ifLockStat = true;

pthread_mutex_t remutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t recond = PTHREAD_COND_INITIALIZER;
pthread_cond_t* get_cond()
{
	return &recond;
}

int AddSeque()
{
	int res = 0;
	sem_wait(&semaphore);
	res = ++seque;
	sem_post(&semaphore);
	return res;
}

int ThreadPush(JsonDatas datas)
{
	int resSize = 0;
	sem_wait(&semaphore);
	sockDatas.push(datas);
	resSize = (int)sockDatas.size();
	sem_post(&semaphore);
	return resSize;
}
int ThreadAddTime(int n_vec, int times)
{
	int resTime = 0;
	sem_wait(&timelock);
	threadTime[n_vec] += times;
	resTime = threadTime[n_vec];
	sem_post(&timelock);
	return resTime;
}
void ThreadClearTime()
{
	sem_wait(&timelock);
	for (int i = 0; i < (int)threadTime.size(); i++)
	{
		threadTime[i] = TIMENULL;
	}
	sem_post(&timelock);
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

void* THREAD_createstr(void* arg)
{
    int n = *(int *)arg;
    unsigned int *mystate = (unsigned int *)arg;
	int resTime = 0, resSize = 0;
    cout << "n : " << n << endl;
    cout << getpid() << endl;
    cout << pthread_self() << endl;
    *mystate = (unsigned int)time(NULL) ^ (unsigned int)getpid() ^ (unsigned int)pthread_self();
    for(int i = 0; i < 10; i++)  //지금은 3번반복이지만 나중에는 무한반복 예정.
    {
        int rantime = rand_r(mystate) % (RANDTIME - 999) + 1000;	//0~9000초까지
        cout << "Thread " << n << " sleep time : " << rantime << endl;
        WAITTIME(rantime); //rantime sec만큼 대기
        JsonDatas tmpd = {
            .id = AddSeque(),
            .threadID = pthread_self(),
            .randomstr = createString(mystate),
            .timestamp = get_curtime()
        };
		cout << "Sleep and Inform" << endl;
        //sockDatas.push(tmpd);
		resSize = ThreadPush(tmpd);
		cout << "resSize : " << resSize << endl;
		resTime = ThreadAddTime(n - 1,rantime);
		cout << n << "resTime : " << resTime << endl;
		if (ifLockStat && (resTime >= MAXTIME || resSize >= QSIZE))
		{
			ifLockStat = false;
			pthread_cond_signal(&recond);
			ThreadClearTime();
		}
		cout << "Thread " << n << " create string, size = " << sockDatas.size() << endl;
        
		//LOCK

		WAITTIME(200);
    }
    cout << "Thread " << n << " Finished..." << endl;
    return NULL;
}

void* THREAD_recvdata(void* arg)
{
    while(!CREATE_EXIT)
    {
        cout << "========= !!WAIT START!! =========" << endl;;
        pthread_cond_wait(&recond, &remutex);
        cout << "=========== RECEIVE!!! ===========" << endl;
		sem_wait(&semaphore);
		queue<JsonDatas> temp = sockDatas;
		if (temp.empty())
		{
			sem_post(&semaphore);
			continue;
		}
        ClearQueue();
		ifLockStat = true;
		sem_post(&semaphore);
        string Jstr = toJSON(&temp);
        cout << Jstr << endl;
		//PostData("10.0.0.95", "/postdata.php", Jstr.c_str());
    }
	pthread_mutex_destroy(&remutex);
	pthread_cond_destroy(&recond);
	printf("exit\n");
    return NULL;
}