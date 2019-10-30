// Standard C++ Library 
#include <iostream>
#include <string>

// Standard C Library 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//semaphore
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <vector>
#include <queue>

using namespace std;

#define null '\0'
#define EMPTY 0
#define FAIL -1

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
        for(int i=0;i<5000000;i++){
            srand((int)time(NULL));
            j = rand() % 10;
        }
        cout << "Thread " << n << " has finished using the toilet." << endl;
        num++;
        cout << "num : " << num << endl;
        sem_post(&semaphore);
    }
    else if(num == 0)
    {
        cout << "The bathroom is full and Thread " << n << " is waiting." << endl;
        sem_wait(&semaphore);
        cout << "Thread " << n << " starts using the toilet." << endl;
        int j = 0;
        num--;
        for(int i=0;i<5000000;i++){
            srand((int)time(NULL));
            j = rand() % 10;
        }
        cout << "Thread " << n << " has finished using the toilet." << endl;
        num++;
        cout << "num : " << num << endl;
        sem_post(&semaphore);
    }
    return NULL;
}