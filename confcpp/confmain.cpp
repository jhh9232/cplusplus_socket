#include "getconf.h"
#include "semaphoreTask.h"
int main()
{
    string fileName = "./config.conf";
    string sectionName = "thread count";
    string keyName = "count";

    int tsize = FAIL;
    if ((tsize = get_valueNUM(fileName, sectionName, keyName)) == FAIL)
        return 0;
    
    cout << "===================================================" << endl;
    
	//소켓 연결 함수 - 없으면 return 0 후 종료

	//소켓 전송 쓰레드 생성
	pthread_t recvth;
    int rchk = pthread_create(&recvth, NULL, THREAD_recvdata, NULL);
    if(rchk < 0)
    {
        perror("recv thread error\n");
    }

    vector<pthread_t> threads;
    vector<unsigned int> facs;
    for(int tn = 1; tn < tsize+1; tn++)
    {
        threads.push_back(EMPTY);
        facs.push_back((unsigned int)tn);
    }

    sem_init(&semaphore, 0, 1); //return : 0 -> success, others -> fail

    cout << "Semaphore test Start!" << endl;
    //스레드 생성
    for(int i=0; i < tsize; i++)
        pthread_create(&threads[i], NULL, THREAD_createstr, (void*)&facs[i]);

    //스레드 조인
    for(int i=0; i < tsize; i++)
	    pthread_join(threads[i], NULL);

	//스레드 완전종료
	CREATE_EXIT = true;

	cout << "All Thread ended" << endl;

	sem_destroy(&semaphore);

	
	pthread_cond_t *recond = get_cond();
	pthread_cond_signal(recond);	//대기상태중인 signal 처리

	pthread_join(recvth, NULL);	//pthreadjoin의 무한대기 해결

	return 0;
}