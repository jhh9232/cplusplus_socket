#include <pthread.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

/*
 * 시그널 핸들러
 * 핸들러가 호출된 쓰레드의 ID와 시그널 번호를 출력한다.   
 */

void sig_handler(int signo)
{
    printf("SIGNAL thid %d : %d\n", pthread_self(),signo);
}

void *threadfunc2(void *arg);
void *threadfunc(void *arg);

int main()
{
    int n, i, j;
    pthread_t threadid;

    // SIGINT와 SIGUSR2 시그널을 
    // 시그널 마스크에 등록한다. 
    // 시그널 마스크는 모든 쓰레드에서 공유된다. 
    // 고로 다른 쓰레드에서도 
    // SIGINT, SIGUSR2시그널에 대해서 마스크 된다. 
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigaddset(&newmask, SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &newmask, NULL);

    // 원하는 쓰레드로 시그널이 전달하는지 확인하기 위해서 
    // 쓰레드 ID를 확인한다. 
    if ((n = pthread_create(&threadid, NULL, threadfunc2, NULL)) != 0 )
    {
        perror("Thread create error ");
        exit(0);
    }
    printf("thread2 id %d\n", threadid);

    if ((n = pthread_create(&threadid, NULL, threadfunc, NULL)) != 0 )
    {
        perror("Thread create error ");
        exit(0);
    }
    printf("thread id %d\n", threadid);
    pthread_join(threadid, NULL);
}

void *threadfunc(void *arg)
{
    int i=0, j;
    struct sigaction act;
    sigset_t newmask;

    // 결과의 확인을 위해서 쓰레드 ID를 출력한다.  
    printf("SIGINT Thread Start %d\n", pthread_self());


    // SIGINT에 대한 시그널 핸들러를 설치하고  
    // 시그널 마스크에서 SIGINT를 제거한다.  
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    act.sa_handler = sig_handler;
    sigaction(SIGINT, &act, NULL);
    pthread_sigmask(SIG_UNBLOCK, &newmask, NULL);
    while(1)
    {
        printf("%d\n", i)
        i++;
        sleep(1);
    }
    return ;
}

void* threadfunc2(void *arg)
{
    struct sigaction act;

    // SIGUSR2에 대한 시그널 핸들러를 설치하고 
    // 시그널 마스크에서 SIGUSR2를 제거한다.   

    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR2);
    act.sa_handler = sig_handler;
    sigaction(SIGUSR2, &act, NULL);
    pthread_sigmask(SIG_UNBLOCK, &newmask, NULL);

    while(1)
    {
        sleep(1);
    }
}