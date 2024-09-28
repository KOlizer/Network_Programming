// TCPClient.c

#include "..\..\Common.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512  // BUFSIZE를 정의합니다.

int main(int argc, char* argv[])
{
    int retval;
    SOCKET sock = INVALID_SOCKET;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // 서버 주소 설정
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // connect()
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    // 데이터 통신에 사용할 변수
    int num1, num2, sum;
    char buf[BUFSIZE];
    int len;

    while (1) {
        // 첫 번째 숫자 입력
        printf("첫 번째 숫자를 입력하세요: ");
        if (scanf("%d", &num1) != 1) break;

        // 두 번째 숫자 입력
        printf("두 번째 숫자를 입력하세요: ");
        if (scanf("%d", &num2) != 1) break;

        // 입력 버퍼 비우기
        while (getchar() != '\n');

        // 네트워크 바이트 오더로 변환하여 버퍼에 저장
        int net_num1 = htonl(num1);
        int net_num2 = htonl(num2);
        memcpy(buf, &net_num1, sizeof(net_num1));
        memcpy(buf + sizeof(net_num1), &net_num2, sizeof(net_num2));

        // 서버로 데이터 보내기
        retval = send(sock, buf, sizeof(net_num1) + sizeof(net_num2), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }

        // 서버로부터 합계 받기
        retval = recv(sock, buf, sizeof(int), MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) {
            printf("서버와의 연결이 종료되었습니다.\n");
            break;
        }

        // 합계 출력
        memcpy(&net_num1, buf, sizeof(int));
        sum = ntohl(net_num1);
        printf("서버로부터 받은 합계: %d\n\n", sum);
    }

    // 소켓 닫기
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
