#include <stdion.h>
#include <stdlib.h> // exit(), ...
#include <string.h> // strerror(), ...
#include <errno.h> // errno

typedef int SCOKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

void error_quit(const char *msg)
{
    LPVOID lpMsgBufl
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        WSAGetLstError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char *)&lpMsgBuf, 0, NULL);
    MessageBoxA(NULL, (const char *)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

void err_display(const char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (char *)&lpMsgBuf, 0, NULL);
        printf("[%s] %s\n", msg, (char *)lpMsgBuf);
        LocalFree(lpMsgBuf);
}

void err_display(int errorcode)
{
    LPVOID lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG, DEFAULT),
        (char)&lpMsgBuf, 0, NULL);
    printf 
}



