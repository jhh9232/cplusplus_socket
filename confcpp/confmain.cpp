#include "getconf.h"
#include "semaphoreTask.h"
int main()
{
    sockDatas.push({0, 0, "tmp", ""});
    pthread_t sendth, recvth;
    int rchk = pthread_create(&recvth, NULL, THREAD_recvdata, NULL);
    int gchk = pthread_create(&sendth, NULL, THREAD_alarm, NULL);
    if(rchk < 0)
    {
        perror("recv thread error\n");
    }
    if(gchk < 0)
    {
        perror("send thread error\n");
        return 0;
    }
    string fileName = "./config.conf";
    string sectionName = "thread count";
    string keyName = "count";

    int tsize = FAIL;
    if ((tsize = get_valueNUM(fileName, sectionName, keyName)) == FAIL)
        return 0;
    
    cout << "===================================================" << endl;

    vector<pthread_t> threads;
    vector<unsigned int> facs;
    for(int tn = 1; tn < tsize+1; tn++)
    {
        threads.push_back(EMPTY);
        facs.push_back((unsigned int)tn);
    }

    sem_init(&semaphore, 0, 1); //return :: 0 -> success, others -> fail

    cout << "Semaphore test Start!" << endl;
    //스레드 생성
    for(int i=0; i < tsize; i++)
        pthread_create(&threads[i], NULL, THREAD_createstr, (void*)&facs[i]);

    //스레드 조인
    for(int i=0; i < tsize; i++)
        pthread_join(threads[i], NULL);
    
    CREATE_EXIT = true;

    cout << "All Thread ended" << endl;

    sem_destroy(&semaphore);

    pthread_join(sendth, NULL);

    return 0;
}