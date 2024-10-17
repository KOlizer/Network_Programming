#include "..\..\Common.h"

char *SERVERIP = (char *) "127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512

int main(int argc, char *argv[])
{
    int retval;

    // 명령행 인수가 있으면 IP 주소로 사용
    if (argc > 1) SERVERIP = argv[1];

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa != 0))
        return 1 ;

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) error_quit("socket()");

    // Connect()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    // *retval**은 소켓 함수들의 반환 값을 저장하는데 사용되며, 소켓 자체가 아니라 소켓 함수 호출 결과(성공/실패 여부)를 나타냅니다.

    retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect");

    // 데이터 통신에 사용할 변수
    char buf[BUFSIZE + 1];
    int len;

    // 서버 데이터 통신
    while (1){
        // 데이터 입력
        printf("\n[보낼 데이터] ");
        if(fgets(buf, BUFSIZE + 1, stdin) == NULL)
            break;

        // '\n'문자 제거
        len = (int)strlen(buf);
        if (buf[len-1] == '\n')
            buf[len-1] = '\0';
        if(strlen(buf) == 0)
            breakl

        //  데이터 보내기 
        retval = send(sock, buf, (int)strlen(buf), 0);
        if (retval == SOCKET_ERROR){
            err_display("send()");
            break;
        }
        printf("[TCP] 클라이언트] %d바이트를 보냈습니다. \n", retval);

        // 데이터 받기
        retval = recv(sock, buf, retval, MSG_WAITALL);
            // sock: 받을 소켓, buf: 저장할 버퍼, retval: 이전 송신에 보낸만큼 받을 데이터 크기)
        if(retval == SOCKET__ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 받은 데이터 출력
        buf[retval] = '\0';
        printf("[TCP클라이언트] %d 바이트를 받았습니다. \n", retval)
        printf("[받은 데이터] %s\n", buf);

        // 소켓 닫기
        closesocket(sock);

        // 윈속 종료
        WSACleanup();
        return 0;


    }


}