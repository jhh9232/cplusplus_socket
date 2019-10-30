/*
무한 반복 스레드 실행!
5초가 초과되었기 때문에 exec_thread 종료
Quit Success!!!
*/


#include "common.h"
#include <pthread.h>

#define THREAD_TIMEOUT_SEC 5

static void *exec_loop(void *pArg);

int status = 0;

int main(int argc, char ** argv)
{
	pthread_t stExecThread;
	printf("n초 대기 스레드 실행! \n");

	//1. 스레드 생성 및 백그라운드 실행
	pthread_create(&stExecThread, NULL, exec_loop, NULL);
	//pthread_detach(stExecThread);

	//2. 5초 대기
	WAITTIME(5000);
	
	//3. 스레드 취소.
	if(!status)
	{
		printf("%d초가 초과되었기 때문에 exec_thread 종료\n", THREAD_TIMEOUT_SEC);
		pthread_cancel(stExecThread);
	}
	else
	{
		printf("스레드가 이미 종료되었습니다!\n");
	}
	
	pthread_join(stExecThread, NULL);
	printf("Quit Success!!!\n");
	return 0;
}
static void *exec_loop(void *arg)
{
	WAITTIME(3000);
	printf("FINISH\n");	//취소로 없어지는거라 실행이 안됨.
	status = 1;
	return NULL;
}