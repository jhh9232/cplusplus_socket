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

//https://disclosure.tistory.com/entry/%EC%8B%AC%ED%94%8C%ED%95%9C-1-1-%EC%B1%84%ED%8C%85-%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%A8-%ED%81%B4%EB%9D%BC%EC%9D%B4%EC%96%B8%ED%8A%B8?category=516221

int SockStat;
struct sockaddr_in server_addr;
//struct sockaddr_in server_addr : ������ �����ּ� ����ü


int ConnectSock()
{
    char buf[BUF_LEN + 1];
    char* ip = "127.0.0.1";
    /* socket() : TCP ���� ���� */
    if((SockStat = socket(PF_INET, SOCK_STREAM, 0)) <= Fail)
    {
        cout << "can't create socket" <<endl;
        return false;
    }
    //������ �����ּ� ����ü server_addr�� NULL�� �ʱ�ȭ
    bzero((char*)&server_addr, sizeof(server_addr));

    //�ּ� ü�踦 AF_INET �� ����
    server_addr.sin_family = AF_INET;

    //32��Ʈ�� IP�ּҷ� ��ȯ
    server_addr.sin_addr.s_addr = inet_addr(ip);

    //daytime ���� ��Ʈ ��ȣ
    server_addr.sin_port = htons(12345);

    if(connect (SockStat, (struct sockaddr *)&server_addr, sizeof(server_addr)) <= Fail)
    {//������ �����û
        cout << "can't connect." << endl;
        return false;
    }
    cout << "SERVER CONNECT!!" << endl;
    return true;
}

int main(int argc, char* argv[])
{
    if(ConnectSock() == false)
    {
        return 0;
    }
    while(true)
    {
        /* �Է� ���� ���� ����  */
        char message[BUF_LEN];
        memset(message,0,strlen(message));

        /* ����ڷκ��� �Է� ���� */
        printf("Client : ");
        fgets(message,BUF_LEN,stdin);
        message[strlen(message)-1] = '\0';

        /* �Է� ���� ���� ���� */
        size_t echoStringLen = strlen(message);
        ssize_t numBytes = send (SockStat,message, echoStringLen, 0);
        if (numBytes == Fail)
            cout << "send error" << endl;
        
        /* ���������κ��� ���ú�  */
        char buffer[BUF_LEN];
        memset(buffer,0,sizeof(buffer));
        numBytes = recv(SockStat,buffer,BUF_LEN,0);
        if(numBytes == -1 )
            cout << "receive error" << endl;
        
        /* �Է� ���� ���ڰ� /quit �� ��� �극��ũ */
        if(!strcmp(buffer,"/q"))
            break; 


        printf("from %s server : %s\n",inet_ntoa(server_addr.sin_addr),buffer);
        memset(buffer,0,sizeof(buffer));
    }
    close(SockStat);
    printf("Close");
    return 0;
}
//g++ -std=c++17 -g -o echo_client.exe echo_client.cpp