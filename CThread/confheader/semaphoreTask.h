#ifndef SEMAPHORETASK_H
#define SEMAPHORETASK_H

#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <queue>

#define TIMENULL 0

typedef struct ThreadData JsonDatas;

struct ThreadData
{
    int id;
    long unsigned int threadID;
    string randomstr;
    string timestamp;
};


extern sem_t semaphore;
extern sem_t timelock;
extern queue<JsonDatas> sockDatas;
extern int seque;
extern int timeStatus;
extern int CREATE_EXIT;
extern vector<int> threadTime;

void print_queue(queue<string>&);
void ClearQueue();
pthread_cond_t* get_cond();

void* THREAD_createstr(void*);
void* THREAD_recvdata(void *);

#endif