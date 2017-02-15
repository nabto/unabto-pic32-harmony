#include <system/random/sys_random.h>
#include <unabto/unabto_external_environment.h>

void nabto_random(uint8_t* buffer, size_t size)
{
    SYS_RANDOM_CryptoBlockGet(buffer, size);
}
