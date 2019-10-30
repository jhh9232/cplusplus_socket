#include <pthread.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

/*
 * https://happytear.tistory.com/entry/%EC%93%B0%EB%A0%88%EB%93%9C%EC%99%80-%EC%8B%9C%EA%B7%B8%EB%84%90
 * �ñ׳� �ڵ鷯
 * �ڵ鷯�� ȣ��� �������� ID�� �ñ׳� ��ȣ�� ����Ѵ�.
 */

void sig_handler(int signo)
{
    printf("SIGNAL RECV TH ID %d : %d\n", pthread_self(), signo);
}

void* threadfunc2(void* arg);
void* threadfunc(void* arg);
void* s_signal(void* arg);

// ������ ID�� �����Ѵ�.  
int sigid[2];

int main()
{
    int n, i, j;
    pthread_t threadid;

    // ���ϴ� ������� �ñ׳��� �����ϴ��� Ȯ���ϱ� ���ؼ�
    // ������ ID�� Ȯ���Ѵ�.

    if ((n = pthread_create(&threadid, NULL, threadfunc2, NULL)) != 0 )
    {
        perror("Thread create error ");
        exit(0);
    }

    sigid[0] = threadid;
    printf("thread2 id %d\n", threadid);

    if ((n = pthread_create(&threadid, NULL, threadfunc, NULL)) != 0 )
    {
        perror("Thread create error ");
        exit(0);
    }

    sigid[1] = threadid;
    printf("thread id %d\n", threadid);

    if ((n = pthread_create(&threadid, NULL, s_signal, NULL)) != 0 )
    {
        perror("Thread create error ");
        exit(0);
    }
    pthread_join(threadid, NULL);
}

void* threadfunc(void *arg)
{
    int i=0, j;
    struct sigaction act;
    sigset_t newmask;

    // ����� Ȯ���� ���ؼ� ������ ID�� ����Ѵ�.
    printf("SIGINT Thread Start %d\n", pthread_self());

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    act.sa_handler = sig_handler;
    sigaction(SIGINT, &act, NULL);

    while(1)
    {
        printf("%d\n", i);
        i++;
        sleep(1);
    }
}

void* threadfunc2(void *arg)
{
    struct sigaction act;

    sigset_t newmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);
    act.sa_handler = sig_handler;
    sigaction(SIGINT, &act, NULL);

    while(1)
    {
        sleep(1);
    }
}

/*
 * SIGINT�� �ΰ��� ������� ���δٸ� �ð��������� 
 * �����Ѵ�. 
 */

void* s_signal(void *arg)
{
    int i = 1;

    while(1)
    {
        sleep(1);
        i++;
        if((i % 7) == 0)
        {
            printf("Send SIGINT %d\n", sigid[0]);
            pthread_kill(sigid[0], SIGINT);
        }

        if((i % 11) == 0)
        {
            printf("Send SIGINT %d\n", sigid[1]);
            pthread_kill(sigid[1], SIGINT);
        }
    }
}