// TCPServer.c

#include "..\..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512  // BUFSIZE를 정의합니다.

int main(int argc, char* argv[])
{
    int retval;
    SOCKET listen_sock = INVALID_SOCKET;
    SOCKET client_sock = INVALID_SOCKET;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // SO_REUSEADDR 옵션 설정
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

    // 데이터 통신에 사용할 변수
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

        // 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));

        // 클라이언트와 데이터 통신
        while (1) {
            // 데이터 받기
            retval = recv(client_sock, buf, sizeof(int) * 2, MSG_WAITALL);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0) {
                printf("클라이언트와의 연결이 종료되었습니다.\n");
                break;
            }

            // 데이터 처리
            memcpy(&num1, buf, sizeof(int));
            memcpy(&num2, buf + sizeof(int), sizeof(int));
            num1 = ntohl(num1);
            num2 = ntohl(num2);
            sum = num1 + num2;

            // 수신 및 계산 결과 출력
            printf("받은 숫자: %d, %d\n", num1, num2);
            printf("합계: %d\n", sum);

            // 합계를 네트워크 바이트 오더로 변환하여 전송
            int net_sum = htonl(sum);
            retval = send(client_sock, (char*)&net_sum, sizeof(net_sum), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send()");
                break;
            }
        }

        // 소켓 닫기
        closesocket(client_sock);
        printf("[TCP 서버] 클라이언트 연결 종료: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));
    }

    // 소켓 닫기
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
