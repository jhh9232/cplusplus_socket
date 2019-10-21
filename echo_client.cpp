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

int main(int argc, char* argv[])
{
    int s, n;
    char* haddr;
    struct sockaddr_in server_addr;
    //struct sockaddr_in server_addr : 서버의 소켓주소 구조체
    char buf[BUF_LEN + 1];
    
    if(argc != 2)
    {
        printf("useage : %s ip_Address\n", argv[0]);
        exit(0);
    }
    haddr = argv[1];

    if((s = socket(PF_INET, SOCK_STREAM, 0)) <= Fail)
    {//소켓 생성과 동시에 소켓 생성 유효검사
        cout << "can't create socket" <<endl;
        exit(0);
    }
    //서버의 소켓주소 구조체 server_addr을 NULL로 초기화
    bzero((char*)&server_addr, sizeof(server_addr));

    //주소 체계를 AF_INET로 선택
    server_addr.sin_family = AF_INET;

    //32비트 IP주소로 변환
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    //daytime 서비스 포트번호
    server_addr.sin_port = htons(12345);

    if(connect (s, (struct sockaddr *)&server_addr, sizeof(server_addr)) <= Fail)
    {//서버로 연결 요청
        cout << "can't connect." << endl;
        exit(0);
    }
    cout << "SERVER CONNECT!!" << endl;
    while(true)
    {
        /* 입력 받을 버퍼 생성  */
        char message[BUF_LEN];
        memset(message,0,strlen(message));

        /* 사용자로부터 입력 받음 */
        printf("Client : ");
        fgets(message,BUF_LEN,stdin);
        message[strlen(message)-1] = '\0';

        /* 입력 받은 문자 센드 */
        size_t echoStringLen = strlen(message);
        ssize_t numBytes = send (s,message, echoStringLen, 0);
        if (numBytes == Fail)
            cout << "send error" << endl;
        
        /* 서버측으로부터 리시브  */
        char buffer[BUF_LEN];
        memset(buffer,0,sizeof(buffer));
        numBytes = recv(s,buffer,BUF_LEN,0);
        if(numBytes == -1 )
            cout << "receive error" << endl;
        
        
        if(!strcmp(buffer,"/q"))
            break; 


        printf("from %s server : %s\n",inet_ntoa(server_addr.sin_addr),buffer);
        memset(buffer,0,sizeof(buffer));
    }
    close(s);
    printf("Close");
    return 0;
}