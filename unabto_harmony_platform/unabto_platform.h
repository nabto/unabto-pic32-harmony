#ifndef _UNABTO_PLATFORM_H_
#define _UNABTO_PLATFORM_H_
#include <system/clk/sys_clk.h>

#include <system/debug/sys_debug.h>

#define NABTO_LOG_BASIC_PRINT(severity, cmsg) do { SYS_PRINT cmsg; } while(0)

int nabtoMsec2Stamp(int ms);

#endif
