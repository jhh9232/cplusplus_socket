#include <pthread.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

/*
 * �ñ׳� �ڵ鷯
 * �ڵ鷯�� ȣ��� �������� ID�� �ñ׳� ��ȣ�� ����Ѵ�.   
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

    // SIGINT�� SIGUSR2 �ñ׳��� 
    // �ñ׳� ����ũ�� ����Ѵ�. 
    // �ñ׳� ����ũ�� ��� �����忡�� �����ȴ�. 
    // ��� �ٸ� �����忡���� 
    // SIGINT, SIGUSR2�ñ׳ο� ���ؼ� ����ũ �ȴ�. 
    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    sigaddset(&newmask, SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &newmask, NULL);

    // ���ϴ� ������� �ñ׳��� �����ϴ��� Ȯ���ϱ� ���ؼ� 
    // ������ ID�� Ȯ���Ѵ�. 
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

    // ����� Ȯ���� ���ؼ� ������ ID�� ����Ѵ�.  
    printf("SIGINT Thread Start %d\n", pthread_self());


    // SIGINT�� ���� �ñ׳� �ڵ鷯�� ��ġ�ϰ�  
    // �ñ׳� ����ũ���� SIGINT�� �����Ѵ�.  
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

    // SIGUSR2�� ���� �ñ׳� �ڵ鷯�� ��ġ�ϰ� 
    // �ñ׳� ����ũ���� SIGUSR2�� �����Ѵ�.   

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