// TCPClient.c

#include "..\..\Common.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512  // BUFSIZE�� �����մϴ�.

int main(int argc, char* argv[])
{
    int retval;
    SOCKET sock = INVALID_SOCKET;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // ���� ����
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // ���� �ּ� ����
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // connect()
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    // ������ ��ſ� ����� ����
    int num1, num2, sum;
    char buf[BUFSIZE];
    int len;

    while (1) {
        // ù ��° ���� �Է�
        printf("ù ��° ���ڸ� �Է��ϼ���: ");
        if (scanf("%d", &num1) != 1) break;

        // �� ��° ���� �Է�
        printf("�� ��° ���ڸ� �Է��ϼ���: ");
        if (scanf("%d", &num2) != 1) break;

        // �Է� ���� ����
        while (getchar() != '\n');

        // ��Ʈ��ũ ����Ʈ ������ ��ȯ�Ͽ� ���ۿ� ����
        int net_num1 = htonl(num1);
        int net_num2 = htonl(num2);
        memcpy(buf, &net_num1, sizeof(net_num1));
        memcpy(buf + sizeof(net_num1), &net_num2, sizeof(net_num2));

        // ������ ������ ������
        retval = send(sock, buf, sizeof(net_num1) + sizeof(net_num2), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        // �����κ��� �հ� �ޱ�
        retval = recv(sock, buf, sizeof(int), MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) {
            printf("�������� ������ ����Ǿ����ϴ�.\n");
            break;
        }

        // �հ� ���
        memcpy(&net_num1, buf, sizeof(int));
        sum = ntohl(net_num1);
        printf("�����κ��� ���� �հ�: %d\n\n", sum);
    }

    // ���� �ݱ�
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}
