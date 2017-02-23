#pragma once

#include <stdint.h>
#include <stdbool.h>


bool unabto_udp_debug_init(const char* remoteAddress, uint16_t remotePort);

void unabto_udp_debug(const char* format, ...);
