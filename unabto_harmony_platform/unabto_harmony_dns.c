#include <unabto_platform_types.h>
#include <unabto/unabto_external_environment.h>
#include <tcpip/udp.h>

void nabto_dns_resolve(const char* id)
{
    if (TCPIP_DNS_Resolve(id, TCPIP_DNS_TYPE_A) != TCPIP_DNS_RES_OK) {
        NABTO_LOG_ERROR(("resolve maybe faoled"));
    }
}

nabto_dns_status_t nabto_dns_is_resolved(const char* id, uint32_t* v4addr)
{
    IP_MULTI_ADDRESS hostIp;
    TCPIP_DNS_RESULT result = TCPIP_DNS_IsResolved(id, &hostIp, IP_ADDRESS_TYPE_IPV4);
    if (result == TCPIP_DNS_RES_OK) {
        *v4addr = TCPIP_Helper_ntohl(hostIp.v4Add.Val);
        return NABTO_DNS_OK;
    }
    if (result == TCPIP_DNS_RES_PENDING) {
        return NABTO_DNS_NOT_FINISHED;
    }

    return NABTO_DNS_ERROR;
}
