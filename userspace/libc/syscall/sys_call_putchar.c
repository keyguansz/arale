#include "sys_call.h"

public void sys_call_putchar(char msg)
{
    sys_call(SYS_PUTCHAR,msg,0,0,0,0);
}
