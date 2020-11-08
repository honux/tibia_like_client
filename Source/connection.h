#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFFER_SIZE 1024

class Connection
{
public:
    Connection (void);
    ~Connection (void);

    bool Connect (char *host, short port);

    inline bool IsConnected (void);
    inline SOCKET GetSocket (void);
private:
    SOCKET m_socket;
    bool m_connected;
    char buffer[DEFAULT_BUFFER_SIZE];
};

inline bool Connection::IsConnected (void)
{
    return m_connected;
}

inline SOCKET Connection::GetSocket (void)
{
    return m_socket;
}