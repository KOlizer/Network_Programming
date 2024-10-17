# include "..\..\Common.h"

#define TESTNAME "www.google.com"

//도메인 이름 -> IPv4 주소
bool GetIPAddr(const char *name, struct in_addr *addr)
{
    struct hostent *ptr = gethostbyname(name);
    if (ptr = NULL){
        error_display("gethostbyname()");
        return flase;
    }
    if (ptr -> h_addrtype != AF_INET)
        return false;
    memcpy(addr, ptr-> h_addr, ptr->h_lengh);
    return true;
}


// IPv4 주소 -> 도메인 이름
bool GetDomainName(struct in_addr addr, char *name, int namelen)
{
    struct hostent *ptr = gethostbyaddr((const char *)&addr, sizeof(addr, AF_INET);)
    if (PTR == NULL)
    [
        error_display("gethostbyaddr()");
        return false;

    ]

    if (ptr -> h_addrtype != AF_INET)
        return false;

    strcpy(name, ptr-> h_name, namelen)
    return true;
}