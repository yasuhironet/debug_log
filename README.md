# debug_log: A Portable debug logging module with user-defined category and type switches.

```
#include <stdio.h>

#include "debug_log.h"

int
main (int argc, char **argv)
{
  debug_log_init (argv[0]);                      //prepare progname, etc.

  DEBUG_OUTPUT_SET (STDOUT);                     //enable stdout output.
  DEBUG_OUTPUT_FILE_SET ("debug_log.log");       //enable file output.

  DEBUG_SET(MTCP, SEQNUM);                       //enable MTCP/SEQNUM debug.
  DEBUG_UNSET(MTCP, RECV);                       //disable MTCP/RECV debug.

  debug_log ("test test, argc: %d", argc);                //this is always printed.
  DEBUG_LOG(MTCP, RECV, "argc: %d, %s: %s",               //MTCP/RECV is disabled.
            argc, "argv", (argc > 1 ? argv[1] : "null"));
  DEBUG_MTCP_LOG (SEQNUM,  "argc: %d, %s[%d]: %s",        //MTCP/SEQNUM is enabled.
            argc, "seqnum: argv", 2, (argc > 2 ? argv[2] : "null"));

  DEBUG_OUTPUT_FILE_UNSET ();                    //disable and cleanup file output.
}
```
