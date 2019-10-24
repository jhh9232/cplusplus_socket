#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>

#define BUFSIZE 30

int main(int argc, char *argv[])
{
	fd_set reads, temps;
	int result;

	char message[BUFSIZE];
	int str_len;
	struct timeval timeout;

	FD_ZERO(&reads);        //0���� �ʱ�ȭ
	FD_SET(0, &reads);      //���� ��ũ���� 0(stdin) ����

	while(1)
	{
		temps = reads;      //���� ������ ���� ���纻 temp�� �̿�
		
		timeout.tv_sec = 3;	//timeout ����
		timeout.tv_usec = 0;

		result = select(1, &temps, 0, 0, &timeout);
		if(result == -1)	//����
		{
			puts("select(): error");
			exit(1);
		}
		else if(result == 0)	//�ð��ʰ�
		{
			puts("select(): time out");
		}
		else
		{
			if(FD_ISSET(0, &temps))
			{
				str_len = read(0, message, BUFSIZE);
				message[str_len] = 0;
				fputs(message, stdout);
				printf("message : %s", message);
				char * tmp = message;
				if(!strncmp(tmp, "/quit", 5))
				{
					break;
				}
				else
				{
					printf("not quit\n");
				}
				
			}
		}
	}
	return 0;
}