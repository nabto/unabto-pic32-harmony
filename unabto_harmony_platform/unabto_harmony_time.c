#include <unabto_platform_types.h>
#include <unabto/unabto_external_environment.h>
#include <system/clk/sys_clk.h>

bool nabtoIsStampPassed(nabto_stamp_t *stamp)
{
    nabto_stamp_t now = nabtoGetStamp();
    return (((int32_t)(*stamp - now)) < 0);
}

nabto_stamp_t nabtoGetStamp(void)
{
    uint32_t current = SYS_TMR_TickCountGet();
    return current;
}


nabto_stamp_diff_t nabtoStampDiff(nabto_stamp_t * newest, nabto_stamp_t *oldest)
{
    return (*newest) - (*oldest);
}

int nabtoStampDiff2ms(nabto_stamp_diff_t diff)
{
    uint32_t freq = SYS_TMR_TickCounterFrequencyGet();
    //ticks pr second.
    int result;
    if (freq <= 1000) {
        // resolution is <= 1ms
        // diff is in the range ~ +-600000
        result = (diff*1000/freq);
    } else {
        result = (diff/freq)*1000;
    }
    return result;
}

int nabtoMsec2Stamp(int ms) {
    return ms;
    uint32_t freq = SYS_TMR_TickCounterFrequencyGet();
    int result = (1000 * ms)/freq;
    return result;
}
