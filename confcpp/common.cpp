#include "common.h"
int WAITTIME(int millisec)
{
    fd_set temps;
	int result;
	struct timeval timeout;
	FD_ZERO(&temps);            //0���� �ʱ�ȭ
	FD_SET(1, &temps);          //���� ��ũ���� 0(stdin) ����
    //timeout ����
	timeout.tv_sec = millisec / 1000;              //second
	timeout.tv_usec = (millisec % 1000) * 1000;    //microsecond -> millisecond
	result = select(1, &temps, 0, 0, &timeout);
	if(result == -1)	        //����
	{
		puts("select(): error");
	}
	else if(result == 0)	    //�ð��ʰ�
	{
		puts("select(): time out");
        return true;
	}
	return false;
}