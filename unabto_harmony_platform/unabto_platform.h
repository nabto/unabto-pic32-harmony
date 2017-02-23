#ifndef _UNABTO_PLATFORM_H_
#define _UNABTO_PLATFORM_H_
#include <system/clk/sys_clk.h>
#include "unabto_udp_debug.h"

#define NABTO_LOG_BASIC_PRINT(severity, cmsg) do { unabto_udp_debug cmsg; } while(0)

int nabtoMsec2Stamp(int ms);

#endif
