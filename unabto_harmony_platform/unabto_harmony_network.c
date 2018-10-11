#include <unabto_platform_types.h>
#include <unabto/unabto_external_environment.h>
#include <tcpip/udp.h>
#include <unabto/unabto_util.h>



bool nabto_socket_init(uint16_t* localPort, nabto_socket_t* socket)
{
    nabto_socket_t sock = TCPIP_UDP_ServerOpen(IP_ADDRESS_TYPE_ANY, *localPort, 0);
    if (sock != NABTO_INVALID_SOCKET) {
        *socket = sock;
        UDP_SOCKET_INFO info;
        if (TCPIP_UDP_SocketInfoGet(sock, &info)) {
            *localPort = info.localPort;
        }
        return true;
    }
    return false;
}


void nabto_socket_close(nabto_socket_t* socket)
{
    if (*socket != NABTO_INVALID_SOCKET) {
        if (!TCPIP_UDP_Close(*socket)) {
            NABTO_LOG_ERROR(("Failed to close udp socket."));
        } else {
            *socket = NABTO_INVALID_SOCKET;
        }
    }
}


ssize_t nabto_read(nabto_socket_t socket,
                   uint8_t*       buf,
                   size_t         len,
                   struct nabto_ip_address*      addr,
                   uint16_t*      port)
{
    uint16_t readyBytes = TCPIP_UDP_GetIsReady(socket);
    if (readyBytes == 0) {
        return 0;
    }
    UDP_SOCKET_INFO info;
    if (TCPIP_UDP_SocketInfoGet(socket, &info)) {
        *port = info.remotePort;
        addr->addr.ipv4 = TCPIP_Helper_ntohl(info.sourceIPaddress.v4Add.Val);
        addr->type = NABTO_IP_V4;
    }

    uint16_t readen = TCPIP_UDP_ArrayGet(socket, buf, MIN(len, readyBytes));
    return readen;
}


ssize_t nabto_write(nabto_socket_t socket,
                    const uint8_t* buf,
                    size_t         len,
                    const struct nabto_ip_address* addr,
                    uint16_t       port)
{

    IP_MULTI_ADDRESS remoteAddress;
    if (addr->type != NABTO_IP_V4) {
        return 0;
    }
    remoteAddress.v4Add.Val = TCPIP_Helper_htonl(addr->addr.ipv4);

    TCPIP_UDP_PutIsReady(socket);
    if (TCPIP_UDP_DestinationIPAddressSet(socket, IP_ADDRESS_TYPE_IPV4, &remoteAddress) &&
        TCPIP_UDP_DestinationPortSet(socket, port))
    {
        TCPIP_UDP_ArrayPut(socket, buf, len);
        return TCPIP_UDP_Flush(socket);
    }
    return 0;
}

bool nabto_get_local_ipv4(struct nabto_ip_address* ip)
{
    return false;
}

bool nabto_socket_is_equal(const nabto_socket_t* s1, const nabto_socket_t* s2)
{
    return *s1 == *s2;
}

void nabto_socket_set_invalid(nabto_socket_t* sock)
{
    *sock = NABTO_INVALID_SOCKET;
}
    
