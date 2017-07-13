#include <unabto_udp_debug.h>

#include <tcpip/tcpip.h>
#include <tcpip/udp.h>
#include <tcpip/tcpip_helpers.h>

static bool initialized = false;

struct udp_debug_state {
    UDP_SOCKET socket;
    IP_MULTI_ADDRESS address;
    uint16_t port;
};

static struct udp_debug_state state;

bool unabto_udp_debug_init(const char* remoteAddress, uint16_t remotePort)
{
    if (!TCPIP_Helper_StringToIPAddress(remoteAddress, &state.address.v4Add)) {
        return false;
    }

    state.port = remotePort;
        
    uint16_t localPort = 0;
    state.socket = TCPIP_UDP_ServerOpen(IP_ADDRESS_TYPE_ANY, 4242, 0);

    if (state.socket == INVALID_UDP_SOCKET) {
        return false;
    }
    initialized = true;
    return true;
}

void unabto_udp_debug(const char* format, ...) {
    if (!initialized) {
        return;
    }
    va_list argList;
    char buffer[1500] = { 0 };
    va_start(argList, format);
    int printed = vsnprintf(buffer, 1500, format, argList);
    va_end(argList);
    
    // rebind the udp socket if the local address has changed.
    TCPIP_NET_HANDLE netif = TCPIP_STACK_NetDefaultGet();
    IP_MULTI_ADDRESS a;
    a.v4Add.Val = TCPIP_STACK_NetAddress(netif);

    TCPIP_UDP_Bind(state.socket, IP_ADDRESS_TYPE_IPV4, 4242, &a);

    TCPIP_UDP_PutIsReady(state.socket);
    TCPIP_UDP_DestinationIPAddressSet(state.socket, IP_ADDRESS_TYPE_IPV4, &state.address);
    TCPIP_UDP_DestinationPortSet(state.socket, state.port);
    TCPIP_UDP_ArrayPut(state.socket, buffer, printed);
    TCPIP_UDP_Put(state.socket, '\n');
    TCPIP_UDP_Flush(state.socket);
}
