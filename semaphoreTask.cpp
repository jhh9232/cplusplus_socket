#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <string>
#include>
//Json �Ľ� : RapidJson

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
        
        //ȭ����� �̿��ϴ� �ð��̶� ����
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
    //������ ����
    int b = 2;
    int c = 3;

    int a = 1;
    pthread_create(&threads[0], NULL, threadF, (void*)&a);
    a++;
    pthread_create(&threads[1], NULL, threadF, (void*)&a);
    a++;
    pthread_create(&threads[2], NULL, threadF, (void*)&a);

    //������ ����
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

�ڼ��� ���� �������� �ȸ´�.
��, 1 3�� ȭ����� ���ٰ� ���´µ� �� ���ڱ� 2�� ȭ����� �� ���ִٰ��ϴ°�?

�̰Ϳ� ���� �ش��� ������ ����.
{
    ... ���� ...
	printf("������ 3�� ȭ��� �̿��� ���ƽ��ϴ�.\n");
	num++;
	sem_post(&semaphore);
}
�� �κ��� ���� printf�� �ϰ� �� ���, ���ؽ�Ʈ ����Ī�� �������� �� �� �ִ�.
�׷��� num �� V(sem_post)�� �Ͼ�� �ʾ� �� á�ٰ� printf�Ȱ��̴�.
�������� ���ø� �����ϴµ� �̷��� ����� �κб��� �� ����� �־�� �Ϻ��� ������� �ɰ��̴�.

*/