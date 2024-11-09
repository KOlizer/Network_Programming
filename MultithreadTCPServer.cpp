#include "..\..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

// 클라이언트 정보 구조체 정의
typedef struct {
	SOCKET client_sock;
	char addr[INET_ADDRSTRLEN];
	int port;
} ClientInfo;


// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
    ClientInfo *client_info = (ClientInfo *)arg;
	SOCKET client_sock = client_info->client_sock;
	struct sockaddr_in clientaddr;
	char buf[BUFSIZE + 1];

    printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		client_info->addr, client_info->port);


	while (1) {
		// 데이터 받기
		retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
				printf("[TCP/%s:%d] %s\n", client_info->addr, client_info->port, buf);


		// 데이터 보내기
		retval = send(client_sock, buf, retval, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
	}

	// 소켓 닫기
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		client_info->addr, client_info->port);
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 클라이언트 정보 설정
		ClientInfo *client_info = (ClientInfo *)malloc(sizeof(ClientInfo));
		client_info->client_sock = client_sock;
		inet_ntop(AF_INET, &clientaddr.sin_addr, client_info->addr, sizeof(client_info->addr));
		client_info->port = ntohs(clientaddr.sin_port);


		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_info, 0, NULL);
		if (hThread == NULL) {
			closesocket(client_sock);
			free(client_info);  // 메모리 해제
		}
		else {
			CloseHandle(hThread);
		}
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}