#include "common.h"
int WAITTIME(int millisec)
{
	millisec--;
	int result;
	struct timeval timeout;
    //timeout 설정
	timeout.tv_sec = millisec / 1000;              //second
	timeout.tv_usec = (millisec % 1000) * 1000;    //microsecond -> millisecond
	result = select(0, NULL, NULL, NULL, &timeout);
	if(result == -1)	        //오류
	{
		puts("select(): error");
	}
	else if(result == 0)	    //시간초과
	{
        return true;
	}
	return false;
}