#ifndef SEMAPHORETASK_H
#define SEMAPHORETASK_H

#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include <queue>

//socket Library
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct ThreadData JsonDatas;

struct ThreadData{
    int id;
    long unsigned int threadID;
    string randomstr;
    string timestamp;
};


extern sem_t semaphore;
extern queue<JsonDatas> sockDatas;
extern int seque;
extern int CREATE_EXIT;

extern struct sockaddr_in server_addr;
extern int SockStat;

void print_queue(queue<string>&);

void* THREAD_createstr(void*);
void* THREAD_alarm(void*);
void* THREAD_recvdata(void *);

string createString(unsigned int *);
string get_curtime();
string toJSON();
string strtoJson(string, string, int, int);
string appJson(string, string, int);


#endif