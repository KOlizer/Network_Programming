// TCPServer.c

#include "..\..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512  // BUFSIZE�� �����մϴ�.

int main(int argc, char* argv[])
{
    int retval;
    SOCKET listen_sock = INVALID_SOCKET;
    SOCKET client_sock = INVALID_SOCKET;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // ���� ����
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // SO_REUSEADDR �ɼ� ����
    int optval = 1;
    retval = setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
    if (retval == SOCKET_ERROR) {
        err_quit("setsockopt()");
    }

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // ������ ��ſ� ����� ����
    struct sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE];
    int num1, num2, sum;

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // Ŭ���̾�Ʈ ���� ���
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            addr, ntohs(clientaddr.sin_port));

        // Ŭ���̾�Ʈ�� ������ ���
        while (1) {
            // ������ �ޱ�
            retval = recv(client_sock, buf, sizeof(int) * 2, MSG_WAITALL);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0) {
                printf("Ŭ���̾�Ʈ���� ������ ����Ǿ����ϴ�.\n");
                break;
            }

            // ������ ó��
            memcpy(&num1, buf, sizeof(int));
            memcpy(&num2, buf + sizeof(int), sizeof(int));
            num1 = ntohl(num1);
            num2 = ntohl(num2);
            sum = num1 + num2;

            // ���� �� ��� ��� ���
            printf("���� ����: %d, %d\n", num1, num2);
            printf("�հ�: %d\n", sum);

            // �հ踦 ��Ʈ��ũ ����Ʈ ������ ��ȯ�Ͽ� ����
            int net_sum = htonl(sum);
            retval = send(client_sock, (char*)&net_sum, sizeof(net_sum), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // ���� �ݱ�
        closesocket(client_sock);
        printf("[TCP ����] Ŭ���̾�Ʈ ���� ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            addr, ntohs(clientaddr.sin_port));
    }

    // ���� �ݱ�
    closesocket(listen_sock);

    // ���� ����
    WSACleanup();
    return 0;
}
