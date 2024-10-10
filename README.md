# debug_log: A Portable Debug Logging Module with User-defined Category and Type Switches.

for personal use.

```
#include <stdio.h>

#include "debug_log.h"

int
main (int argc, char **argv)
{
  debug_log_init (argv[0]);                                //prepare progname, etc.

  DEBUG_OUTPUT_SET (STDOUT);                               //enable stdout output.
  DEBUG_OUTPUT_FILE_SET ("debug_log.log");                 //enable file output.

  DEBUG_SET (MTCP, SEQNUM);                                //enable MTCP/SEQNUM debug.
  DEBUG_UNSET (MTCP, RECV);                                //disable MTCP/RECV debug.

  debug_log ("test test, argc: %d", argc);                 //this is always printed.
  DEBUG_LOG (MTCP, RECV, "argc: %d, %s: %s",               //MTCP/RECV is disabled. (no output.)
            argc, "argv", (argc > 1 ? argv[1] : "null"));
  DEBUG_MTCP_LOG (SEQNUM,  "argc: %d, %s[%d]: %s",         //MTCP/SEQNUM is enabled.
            argc, "seqnum: argv", 2, (argc > 2 ? argv[2] : "null"));

  DEBUG_OUTPUT_FILE_UNSET ();                              //disable and cleanup file output.
}
```

Although it is not exactly from the above example,
the debug log messages like below can be easily
output to (any combinations of) stdout/stderr/syslog/file.

```
2024/10/10 18:23:17 debuglog[59320]: test test, argc: 1
2024/10/10 18:23:17 debuglog[59320]: main.c[60] main(): argc: 1, seqnum: argv[2]: null
2024/10/10 18:23:17 debuglog[59320]: main.c[14] a(): begin.
2024/10/10 18:23:17 debuglog[59320]: main.c[22] b(): begin.
2024/10/10 18:23:17 debuglog[59320]: main.c[31] c(): begin.
2024/10/10 18:23:17 debuglog[59320]: main.c[32] c(): end.
2024/10/10 18:23:17 debuglog[59320]: main.c[38] d(): begin.
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[129] backtrace_log(): backtrace frames: 7
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): ./debuglog(+0x159e) [0x55865ab1759e]
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): ./debuglog(+0x146e) [0x55865ab1746e]
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): ./debuglog(+0x13d7) [0x55865ab173d7]
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): ./debuglog(+0x18b6) [0x55865ab178b6]
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): /lib/x86_64-linux-gnu/libc.so.6(+0x29d90) [0x7f5707e77d90]
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0x80) [0x7f5707e77e40]
2024/10/10 18:23:17 debuglog[59320]: debug_log.h[140] backtrace_log(): ./debuglog(+0x12c5) [0x55865ab172c5]
2024/10/10 18:23:17 debuglog[59320]: main.c[40] d(): end.
2024/10/10 18:23:17 debuglog[59320]: main.c[25] b(): end.
2024/10/10 18:23:17 debuglog[59320]: main.c[16] a(): end.
```
