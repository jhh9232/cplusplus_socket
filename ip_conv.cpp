#include <iostream>
#include <arpa/inet.h> //inet_pton, inet_ntop �Լ��� ���Ե� �������

using namespace std;

int main(int argc, char* argv[])
{
    //2���� IP�ּ��� ������ ���� ����ü ���
    struct in_addr addr;
    //10���� IP�ּ��� ������ ���� �ӽ� ����
    char buf[20];

    //�Է¹��� 10���� IP �ּҸ� ���
    cout << "demical IP addr : " << argv[1] << endl;

    //�Է¹��� ���� 2���� IP�� ��ȯ�Ͽ� addr ����ü�� ����
    inet_pton(AF_INET, argv[1], &addr.s_addr);
    printf("inet_pton(%s) : 0x%x\n", argv[1], addr.s_addr);

    //addr����ü�� 2���� IP�ּ� ���� 10������ ��ȯ�Ͽ� buf�� sizeof(buf)��ŭ ����
    inet_ntop(AF_INET, &addr.s_addr, buf, sizeof(buf));
    printf("inet_ntop(0x%x) = %s\n", addr.s_addr, buf);
    return 0;
}