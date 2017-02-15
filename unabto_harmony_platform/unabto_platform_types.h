#ifndef _UNABTO_PLATFORM_TYPES_H_
#define _UNABTO_PLATFORM_TYPES_H_

#include <app.h>
#include <tcpip/udp.h>
#include <stdint.h>
#include <stdbool.h>

#include <platforms/unabto_common_types.h>

typedef int ssize_t;
typedef UDP_SOCKET nabto_socket_t;
typedef uint32_t nabto_stamp_t;
typedef int64_t nabto_stamp_diff_t;
typedef const char* text;

#define NABTO_INVALID_SOCKET INVALID_UDP_SOCKET

#endif
