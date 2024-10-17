#include "..\Common.h"

char *SERVERIP = (char *)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char *argv[])
{
    int retval;

    // 서버 IP 주소를 입력받은 경우 설정
    if (argc > 1) SERVERIP = argv[1];

    // WSA 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // 서버 주소 구조체 초기화
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    // serveraddr.sin_addr.s_addr = inet_addr(SERVERIP); // inet_addr 함수 deprecated 되어 inet_pton 함수로 대체
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // 서버에 연결 시도
    retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    // 버퍼 초기화
    char buf[BUFSIZE + 1];
    int len;

    // 클라이언트에서 데이터 송수신
    while (1) {
        // 사용자 입력 받기
        printf("\n[입력 내용] ");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
            break;

        // '\n' 제거
        len = (int)strlen(buf);
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        if (strlen(buf) == 0)
            break;

        // 데이터 송신
        retval = send(sock, buf, (int)strlen(buf), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("[TCP 소켓] %d바이트를 전송하였습니다.\n", retval);

        // 데이터 수신
        // retval = recvn(sock, buf, retval, 0); // recv 함수의 MSG_WAITALL 플래그를 사용
        retval = recv(sock, buf, retval, MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 수신 데이터 출력
        buf[retval] = '\0';
        printf("[TCP 소켓] %d바이트를 수신하였습니다.\n", retval);
        printf("[수신 내용] %s\n", buf);
    }

    // 소켓 닫기
    closesocket(sock);

    // WSA 종료
    WSACleanup();
    return 0;
}