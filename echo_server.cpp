//���� ���α׷��ֿ� ���� ������� ����
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

#define BUF_LEN 128
#define Fail -1
#define null 0x00
#define true 1
#define false 0

//https://luckyyowu.tistory.com/88?category=755949

int main(int argc, char* argv[])
{
    char buffer[BUF_LEN];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    //server_fd, client_fd : �� ���� ��ȣ
    int server_fd, client_fd;
    int len, msg_size;

    if(argc != 2)
    {
        printf("useage : %s [port]\n", argv[0]);
        exit(0);
    }

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == Fail)
    {// ���� ����
        cout << "Server : Can't open stream socket\n" << endl;
        exit(0);
    }
    //server_Addr �� NULL�� �ʱ�ȭ
    memset(&server_addr, null, sizeof(server_addr));

    //server_addr ����
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {//bind() ȣ��
        cout << "Server : Can't bind local address. " << endl;
        exit(0);
    }

    if(listen(server_fd, 5) < 0)
    {//������ ���� ������ ����
        cout << "Server : Can't listening connect." << endl;
        exit(0);
    }

    cout << "Server : waiting connection request." << endl;
    len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (unsigned int*)&len);
    //client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
    if(client_fd <= Fail)
    {
        cout << "Server : accept failed." << endl;
        exit(0);
    }
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
    cout << "Server : " << temp << " client connected." << endl;

    while(true)
    {
        /* ���� ���� ���� ���� */
        char buffer[BUF_LEN]; 
        memset(buffer,0,sizeof(buffer));

        /* ������ ���� ���� */
        ssize_t numbytesrcvd = recv(client_fd,buffer,BUF_LEN,0);
        if(numbytesrcvd == -1)
            cout << " recv error" << endl;

        /* ���Ź��� ������ ���*/
        printf("from %s client : %s\n",inet_ntoa(client_addr.sin_addr),buffer);

        /* ������ �Է¹��� ���� ���� */
        char message[BUF_LEN];
        memset(message,0,sizeof(message));
        strcpy(message, buffer);
        cout << message << endl;
        memset(buffer,0,sizeof(buffer));
        /* ���� �޽��� �Է� ���� */
        // printf("Server : ");
        // fgets(message,BUF_LEN,stdin);

        /* ������ ���ڿ� �Է� ���� ���� */
        if(send(client_fd,message,strlen(message),0)== -1)
            cout << "send error " << endl;
        
        /* �켱 �۽���, �Է¹��� ���۰� quit �� ��� ����  */
        if(strcmp(message,"/q") == 0)
            break;

    }
    close(client_fd);
    cout << "Server : " << temp << " client closed." << endl;
    close(server_fd);
    return 0;
}