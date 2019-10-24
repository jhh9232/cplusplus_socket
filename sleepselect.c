#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

void SelectSleep(int sleepnum)
{
	fd_set temps;
	int result;
	struct timeval timeout;
	FD_ZERO(&temps);            //0으로 초기화
	FD_SET(1, &temps);          //파일 디스크립터 0(stdin) 설정
	timeout.tv_sec = sleepnum;	//timeout 설정
	timeout.tv_usec = 0;        //usec = not millisec, microsec -> (1000000microsec -> 1sec)
	result = select(1, &temps, 0, 0, &timeout);
	if(result == -1)	        //오류
	{
		puts("select(): error");
		return;
	}
	else if(result == 0)	    //시간초과
	{
		puts("select(): time out");
	}
}

int main(int argc, char *argv[])
{
	clock_t start, end;
	start = clock();
	SelectSleep(3);
	end = clock();
	double duration = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%.3f sec\n", duration);
	return 0;
}