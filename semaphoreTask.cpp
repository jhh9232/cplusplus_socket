#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <string>
#include>
//Json 파싱 : RapidJson

using namespace std;

#define EMPTY 0

typedef struct ThreadData{
    string id;
    int threadID;
    string randomstr;
    string timestamp;
}JsonDatas;

sem_t semaphore;

int num = 2;

void* threadF(void* arg)
{
    int i;
    int n = *(int *)arg;
    cout << "n : " << n << endl;
    if(num != 0)
    {
        num--;
        sem_wait(&semaphore);
        cout << "Thread " << n << " starts using the toilet." << endl;
        
        //화장실을 이용하는 시간이라 가정
        int j = 0;
        for(int i=0;i<1000000;i++){
            srand(time(NULL));
            j = rand() % 10;
        }
        cout << "Thread " << n << " has finished using the toilet." << endl;
        num++;
        sem_post(&semaphore);
    }
    else if(num == 0)
    {
        cout << "The bathroom is full and Thread 1 is waiting." << endl;
        while(num == 0){}
        num--;
        sem_wait(&semaphore);
        cout << "Thread " << n << " starts using the toilet." << endl;
        int j = 0;
        for(int i=0;i<1000000;i++){
            srand(time(NULL));
            j = rand() % 10;
        }
        cout << "Thread " << n << " has finished using the toilet." << endl;
        num++;
        sem_post(&semaphore);
    }
    cout << "num : " << num << endl;
    return NULL;
}

int main()
{
    vector<pthread_t> threads;
    threads.push_back(EMPTY);
    threads.push_back(EMPTY);
    threads.push_back(EMPTY);

    sem_init(&semaphore, 0, 2); //return :: 0 -> success, others -> fail

    cout << "Semaphore test Start!" << endl;
    //스레드 생성
    int b = 2;
    int c = 3;

    int a = 1;
    pthread_create(&threads[0], NULL, threadF, (void*)&a);
    a++;
    pthread_create(&threads[1], NULL, threadF, (void*)&a);
    a++;
    pthread_create(&threads[2], NULL, threadF, (void*)&a);

    //스레드 조인
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    cout << "All Thread used the toilet!" << endl;

    sem_destroy(&semaphore);

    return 0;
}

/*

Thread 3 starts using the toilet.
Thread 2 starts using the toilet.
Thread 3 has finished using the toilet.
Thread 2 has finished using the toilet.
The bathroom is full and Thread 1 is waiting.
Thread 1 starts using the toilet.
Thread 1 has finished using the toilet.

자세히 보면 말이조금 안맞다.
즉, 1 3이 화장실을 들어갔다가 끝냈는데 왜 갑자기 2가 화장실이 꽉 차있다고하는가?

이것에 대한 해답은 다음과 같다.
{
    ... 생략 ...
	printf("스레드 3이 화장실 이용을 마쳤습니다.\n");
	num++;
	sem_post(&semaphore);
}
이 부분을 보면 printf를 하고 난 즉시, 컨텍스트 스위칭을 당했음을 알 수 있다.
그래서 num 및 V(sem_post)가 일어나지 않아 꽉 찼다고 printf된것이다.
세마포어 예시를 제작하는데 이러한 사소한 부분까지 다 고려해 주어야 완벽한 세마포어가 될것이다.

*/