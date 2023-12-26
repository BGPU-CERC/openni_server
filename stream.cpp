#include "./header.h"

void stream()
{
#ifdef WIN32
    WSADATA wsaData;
    wsa_check(WSAStartup(MAKEWORD(2, 2), &wsaData));

    const struct sockaddr_in addr = {AF_INET, htons(8081), INADDR_ANY};
    SOCKET server_s = socket(AF_INET, SOCK_STREAM, 0);
    wsa_check(bind(server_s, (const sockaddr *)&addr, sizeof(addr)));
    wsa_check(listen(server_s, 10));

    char request[256] = {0};
    SOCKET client_s = accept(server_s, NULL, NULL);
    recv(client_s, request, sizeof(request), 0);
    send(client_s, request, sizeof(request), 0);
    cout << request << endl;

    _close(client_s);
    _close(server_s);
#endif
}
