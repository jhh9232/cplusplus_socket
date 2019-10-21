#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  //시스템 콜에 필요한 상수 선언
#include <sys/stat.h>   //파일의 상태에 대한 데이터 선언
#include <sys/socket.h> //소켓 시스템 콜 선언
#include <arpa/inet.h>
#include <fcntl.h>      //open에 필요한 flag 선언

using namespace std;

//https://m.blog.naver.com/PostView.nhn?blogId=skssim&logNo=122909467&proxyReferer=https%3A%2F%2Fwww.google.com%2F
//https://luckyyowu.tistory.com/73?category=755949

int main()
{
    int fd1, fd2, sd1, sd2;
    //파일 디스크립터의 최대 값. 시스템마다 다름.
    cout << "getdtablesize() = " << getdtablesize() << endl;

    // /etc/passwd 파일을 읽기전용모드로 열기
    fd1 = open("/etc/passwd", O_RDONLY, 0);
    cout << "/etc/passwd's fd = " << fd1 << endl;

    //스트림형 소켓 열기
    sd1 = socket(PF_INET, SOCK_STREAM, 0);
    cout << "stream socket descriptor = " << sd1 << endl;

    //데이터그램형 소켓 열기
    sd2 = socket(PF_INET, SOCK_DGRAM, 0);
    cout << "datagram socket descriptor = " << sd2 << endl;

    fd2 = open("/etc/hosts", O_RDONLY, 0);
    cout << "/etc/hosts's fd = " << fd2 << endl;

    close(fd2);
    close(fd1);
    close(sd2);
    close(sd1);

    return 0;
}