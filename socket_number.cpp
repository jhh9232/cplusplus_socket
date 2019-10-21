#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  //�ý��� �ݿ� �ʿ��� ��� ����
#include <sys/stat.h>   //������ ���¿� ���� ������ ����
#include <sys/socket.h> //���� �ý��� �� ����
#include <arpa/inet.h>
#include <fcntl.h>      //open�� �ʿ��� flag ����

using namespace std;

//https://m.blog.naver.com/PostView.nhn?blogId=skssim&logNo=122909467&proxyReferer=https%3A%2F%2Fwww.google.com%2F
//https://luckyyowu.tistory.com/73?category=755949

int main()
{
    int fd1, fd2, sd1, sd2;
    //���� ��ũ������ �ִ� ��. �ý��۸��� �ٸ�.
    cout << "getdtablesize() = " << getdtablesize() << endl;

    // /etc/passwd ������ �б�������� ����
    fd1 = open("/etc/passwd", O_RDONLY, 0);
    cout << "/etc/passwd's fd = " << fd1 << endl;

    //��Ʈ���� ���� ����
    sd1 = socket(PF_INET, SOCK_STREAM, 0);
    cout << "stream socket descriptor = " << sd1 << endl;

    //�����ͱ׷��� ���� ����
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