#include "common.h"
int WAITTIME(int millisec)
{
	millisec--;
    fd_set temps;
	int result;
	struct timeval timeout;
	FD_ZERO(&temps);            //0으로 초기화
	FD_SET(1, &temps);          //파일 디스크립터 0(stdin) 설정
    //timeout 설정
	timeout.tv_sec = millisec / 1000;              //second
	timeout.tv_usec = (millisec % 1000) * 1000;    //microsecond -> millisecond
	result = select(1, &temps, 0, 0, &timeout);
	if(result == -1)	        //오류
	{
		puts("select(): error");
	}
	else if(result == 0)	    //시간초과
	{
		puts("select(): time out");
        return true;
	}
	return false;
}